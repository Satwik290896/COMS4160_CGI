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

namespace olio {
namespace core {

using namespace std;

bool
RayTracer::RayColor(const Ray &ray, Surface::Ptr scene, Vec3r &ray_color)
{
  // ======================================================================
  // *** Homework: implement RayColor (See description of function in
  // *** the header file raytracer.h)
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  HitRecord hit_record;
  if(scene->Hit(ray,0,kInfinity,hit_record) == true)
  {
      ray_color[0] = 1;
      ray_color[1] = 0; 
      ray_color[2] = 0; 

      return true;
  }
  else
  {
      ray_color[0] = 0;
      ray_color[1] = 0; 
      ray_color[2] = 0; 

      return false;
  }

}


bool
RayTracer::Render(Surface::Ptr scene, Camera::Ptr camera)
{
  // error checking
  if (!scene || !camera)
    return false;

  // start timer
  auto start_time = chrono::system_clock::now();

  // compute output image dimensions
  auto aspect = camera->GetAspectRatio();
  cout<< "Aspect: " << aspect << endl;
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

  // ======================================================================
  // *** Homework: iterate through every pixel in rendered_image_, and
  // *** for each pixel:
  //   1. Use the camera class to generate a ray that passes through the pixel
  //   2. Call RayColor() compute the color of the pixel
  //   3. Set pixel's color in rendered_image_
  //   4. Update the progress bar by calling RenderProgressIncDonePixels()
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  for (int i = 0; i < height; i++)
  {
      for (int j = 0; j < width; j++)
      {
          //cout << "index: "<< (height-i-0.5f)/height << "\n" << endl;
          Real ho = (j+0.5f)/width;
          Real lo = (height-(i+0.5f))/height;
          Ray ray = camera->GetRay(ho,lo);
          Vec3r ray_color{0,0,0};
          RayColor(ray, scene, ray_color);
          rendered_image_.at<Vec3r>(i, j) = ray_color;
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
