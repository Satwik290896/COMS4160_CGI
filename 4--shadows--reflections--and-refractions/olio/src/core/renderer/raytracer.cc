// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       raytracer.cc
//! \brief      Raytracer class
//! \author     Hadi Fadaifard, 2022

#include "core/renderer/raytracer.h"
#include <algorithm>
#include <chrono>
#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include "core/geometry/sphere.h"
#include "core/material/phong_material.h"
#include "core/material/phong_dielectric.h"
#include <iostream>
#include <cstddef>

namespace olio {
namespace core {

using namespace std;

bool
RayTracer::RayColor(const Ray &ray, Surface::Ptr scene,
                    const std::vector<Light::Ptr> &lights,
                    uint ray_depth, uint max_ray_depth,
                    Vec3r &ray_color)
{
  // check whether ray hits any scene object
  ray_color = Vec3r{0, 0, 0};

  if (ray_depth > max_ray_depth)
    return false;
  
  HitRecord hit_record;
  if (!scene->Hit(ray, kEpsilon, kInfinity, hit_record))
    return false;

  // get surface material
  auto hit_surface = hit_record.GetSurface();
  if (!hit_surface)
    return false;
  auto material = hit_surface->GetMaterial();
  auto normal = hit_record.GetNormal();
  auto hitpoint = hit_record.GetPoint();

  if (!material) {
    spdlog::error("RayColor: surface has no material -- returning black.");
    return true;
  }

  // compute the Phong shading contribution from each light and add
  // them to get the final ray color
  auto phong_material = dynamic_pointer_cast<PhongMaterial>(material);
  if (phong_material) {
    // compute direct light shading


    auto phong_dielectric = dynamic_pointer_cast<PhongDielectric>(phong_material);

    if (phong_dielectric) 
    {
      std::shared_ptr<Ray> reflect_ray;
      std::shared_ptr<Ray> refract_ray;
      Real schlick_reflectance;
      Vec3r attenuate = phong_dielectric->Scatter(hit_record, ray, 
                                          reflect_ray, refract_ray, schlick_reflectance);
      /*if(reflect_ray != NULL)
        std::cout << "Reflect Ray1234: "<< reflect_ray <<"\n" << std::endl;
      */
      if (refract_ray!=NULL) {
        Vec3r refract_color = Vec3r{0,0,0};
        
        if (RayColor(*refract_ray.get(), scene, lights, ray_depth+1, max_ray_depth, refract_color))
          ray_color += attenuate.cwiseProduct(refract_color*(1-schlick_reflectance));
      }

      if (reflect_ray!=NULL) {

        Vec3r reflect_color = Vec3r{0,0,0};
        
        if (RayColor(*reflect_ray.get(), scene, lights, ray_depth+1, max_ray_depth, reflect_color)) {
          //std::cout << "I am here\n" << std::endl;
          ray_color += attenuate.cwiseProduct(reflect_color*schlick_reflectance);
        }
      }

    }
    else 
    {
      
      Vec3r view_vec = -ray.GetDirection().normalized();
      for (auto light : lights)
        ray_color += light->Illuminate(hit_record, view_vec, scene);
    
      if (hit_record.IsFrontFace() == false)
        return true;
      
      Vec3r mirror_reflection_factor = phong_material->GetMirror();

      if (mirror_reflection_factor != Vec3r{0,0,0}) 
      {
        Vec3r d = - view_vec;
        Vec3r r_dir = d - 2*(d.dot(normal))*normal;
        Ray reflect_ray(hitpoint, r_dir);
        Vec3r reflect_color = Vec3r{0,0,0};
        
        if (RayColor(reflect_ray, scene, lights, ray_depth+1, max_ray_depth, reflect_color))
          ray_color += reflect_color.cwiseProduct(mirror_reflection_factor);
      }

    }
  }


  return true;
}


bool
RayTracer::Render(Surface::Ptr scene, const std::vector<Light::Ptr> &lights,
                  Camera::Ptr camera)
{
  // error checking
  if (!scene || !camera)
    return false;

  // start timer
  auto start_time = chrono::system_clock::now();

  // compute output image dimensions
  auto aspect = camera->GetAspectRatio();
  auto height = static_cast<int>(image_height_);
  auto width = static_cast<int>(aspect * static_cast<Real>(height) + 0.5f);
  if (height <= 0 || width <= 0) {
    spdlog::error("RayTracer: invalid image dimensions");
    return false;
  }

  // initialize image
  rendered_image_ = cv::Mat(cv::Size(width, height), CV_64FC3,
                            cv::Scalar(0, 0, 0, 0));

  // start progress bar
  spdlog::info("Rendering...");
  auto total_pixels = static_cast<size_t>(width * height);
  RenderProgressStart(total_pixels);

  // send rays
  Real xscale = 1.0 / width;
  Real yscale = 1.0 / height;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      auto ray = camera->GetRay((x + .5) * xscale, (y + .5) * yscale);
      Vec3r ray_color;
      uint ray_depth = 0;
      uint max_ray_depth = 10;
      RayColor(ray, scene, lights, ray_depth, max_ray_depth, ray_color);
      rendered_image_.at<cv::Vec3d>((height - y -1), x) =
        cv::Vec3d{ray_color[0], ray_color[1], ray_color[2]};
      RenderProgressIncDonePixels();
    }
  }

  // stop progress bar
  RenderProgressEnd();

  // stop timer
  auto end_time = std::chrono::system_clock::now();
  auto total_time = chrono::duration_cast<chrono::duration<double>>
    (end_time - start_time).count();
  spdlog::info("Total render time: {}", total_time);

  return true;
}


cv::Mat
RayTracer::GammaCorrectImage(const cv::Mat &in_image, Real gamma) const
{
  Real gamma_inv = 1.0f / gamma;
  cv::Mat out_image(in_image.rows, in_image.cols, CV_64FC3);
  for (int y = 0; y < out_image.rows; ++y) {
    for (int x = 0; x < out_image.cols; ++x) {
      auto pixel = in_image.at<cv::Vec3d>(y, x);
      auto r = pow(pixel[0], gamma_inv);
      auto g = pow(pixel[1], gamma_inv);
      auto b = pow(pixel[2], gamma_inv);
      out_image.at<cv::Vec3d>(y, x) = cv::Vec3d{r, g, b};
    }
  }
  return out_image;
}


cv::Mat
RayTracer::RGBToBGRUChar(const cv::Mat &in_image) const
{
  // double RGB to uchar BGR:
  cv::Mat out_image(in_image.rows, in_image.cols, CV_8UC3);
  for (int y = 0; y < out_image.rows; ++y) {
    for (int x = 0; x < out_image.cols; ++x) {
      auto colord = in_image.at<cv::Vec3d>(y, x);
      out_image.at<cv::Vec3b>(y, x) =
        cv::Vec3b{static_cast<uchar>(CLAMP(colord[2] * 255 + 0.5f, 0, 255)),
                  static_cast<uchar>(CLAMP(colord[1] * 255 + 0.5f, 0, 255)),
                  static_cast<uchar>(CLAMP(colord[0] * 255 + 0.5f, 0, 255))};

    }
  }
  return out_image;
}


cv::Mat
RayTracer::RGBToBGRFloat32(const cv::Mat &in_image) const
{
  // double RGB to float BGR:
  cv::Mat out_image(in_image.rows, in_image.cols, CV_32FC3);
  for (int y = 0; y < out_image.rows; ++y) {
    for (int x = 0; x < out_image.cols; ++x) {
      auto colord = in_image.at<cv::Vec3d>(y, x);
      out_image.at<cv::Vec3f>(y, x) =
        cv::Vec3f{static_cast<float>(colord[2]),
                  static_cast<float>(colord[1]),
                  static_cast<float>(colord[0])};
    }
  }
  return out_image;
}


bool
RayTracer::WriteImage(const std::string &image_name, Real gamma) const
{
  namespace fs = boost::filesystem;

  // check we have a rendered image
  if (rendered_image_.empty())
    return false;

  // decide the output format
  bool is_exr = fs::path(image_name).extension().string() == ".exr";
  cv::Mat out_image;
  if (is_exr) {
    // HDR output does not need gamma correction
    out_image = RGBToBGRFloat32(rendered_image_);
  } else {
    // gamma correct and convert to uchar
    cv::Mat image_real;
    if (gamma == 1)
      image_real = rendered_image_;
    else
      image_real = GammaCorrectImage(rendered_image_, gamma);
    out_image = RGBToBGRUChar(image_real);
  }

  // write image
  cv::imwrite(image_name, out_image);
  return true;
}


void
RayTracer::RenderProgressStart(size_t total_pixels)
{
  const std::lock_guard<std::mutex> lock(progress_bar_mutex_);
  progress_bar_ = make_shared<tqdm>();
  progress_bar_done_pixels_  = 0;
  progress_bar_total_pixels_ = total_pixels;
}


void
RayTracer::RenderProgressIncDonePixels()
{
  const std::lock_guard<std::mutex> lock(progress_bar_mutex_);
  progress_bar_done_pixels_ = std::min(progress_bar_done_pixels_ + 1,
                                       progress_bar_total_pixels_);
  if (!progress_bar_)
    return;
  progress_bar_->progress(static_cast<int>(progress_bar_done_pixels_),
                          static_cast<int>(progress_bar_total_pixels_));
}


void
RayTracer::RenderProgressEnd()
{
  const std::lock_guard<std::mutex> lock(progress_bar_mutex_);
  if (progress_bar_)
    progress_bar_->finish();
  progress_bar_.reset();
}

}  // namespace core
}  // namespace olio
