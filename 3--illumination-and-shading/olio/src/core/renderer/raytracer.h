// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       raytracer.h
//! \brief      RayTracer class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <set>
#include <tbb/tbb.h>
#include <opencv2/opencv.hpp>
#include <tqdm/tqdm.h>
#include "core/types.h"
#include "core/node.h"
#include "core/geometry/surface.h"
#include "core/camera/camera.h"
#include "core/light/light.h"
#include "core/material/phong_material.h"

namespace olio {
namespace core {

//! \class RayTracer
//! \brief Main rendering class responsible for generating rays, path
//! tracing, computing ray colors, and generating a rendered image of
//! the scene
class RayTracer {
public:
  //! \brief Default constructor
  RayTracer() = default;

  //! \brief The main render function responsible for generating a
  //! rendered image for the input scene as seen by the input camera
  //! \details The function is responsible to generating primary rays
  //!    for each pixel in the output image and determining each pixel
  //!    color. Each pixel/ray color will be determined by a call to
  //!    RayColor(). The final color for each pixel will be stored in
  //!    'rendered_image_'. This function is also responsible for
  //!    allocating an initial black image for 'rendered_image_'
  //!    before the start of the ray tracing process.
  //! \param[in] scene Input scene to render
  //! \param[in] camera Camera used for generating rays and rendering
  //!            the scene from its point of view
  //! \return True on success
  bool Render(Surface::Ptr scene, const std::vector<Light::Ptr> &lights, Camera::Ptr camera);

  //! \brief Set output image height. The image width will be
  //! determined based on the aspect ratio of the camera's viewport.
  //! \param[in] image_height Output image height
  inline void SetImageHeight(uint image_height) {image_height_ = image_height;}

  //! \brief Get output image height
  //! \return Output image height
  inline uint GetImageHeight() const {return image_height_;}

  //! \brief Write rendered image to file. If the image extension is
  //!        exr, the image won't be gamma corrected before it's saved
  //!        (gamma is ignored).
  //! \param[in] image_name Output image path
  //! \param[in] gamma Gamma value
  //! \return True on success
  bool WriteImage(const std::string &image_name, Real gamma=1) const;
protected:
  //! \brief Determine ray color by intersecting it with the scene
  //! \details The main function responsible for checking for
  //!    intersections of the rays with scene objects (surfaces) and
  //!    determining the color of each ray. For this homework
  //!    assignment, if a ray hits a surface, its color will be
  //!    red (1, 0, 0) and black, otherwise.
  //! \param[in] ray Input ray
  //! \param[in] scene Input scene
  //! \param[out] ray_color Output ray color
  //! \return True if ray intersects a surface in the scene
  bool RayColor(const Ray &ray, Surface::Ptr scene, const std::vector<Light::Ptr> &lights, 
    Vec3r &ray_color);

  //! \brief Gamma correct input image
  //! \details Input image is assumed to be of type CV_64FC3
  //! \param[in] in_image Input image; must be of type: CV_64FC3
  //! \param[in] gamma Gamma
  //! \return Gamma corrected image
  cv::Mat GammaCorrectImage(const cv::Mat &in_image, Real gamma) const;

  //! \brief Convert an input image with double-precision floating
  //! point RGB channels to an output image with uchar BGR channels.
  //! \param[in] in_image Input image; must be of type CV_64FC3
  //! \return Output image of type CV_8UC3 with BGR channel ordering
  cv::Mat RGBToBGRUChar(const cv::Mat &in_image) const;

  //! \brief Convert an input image with double-precision floating
  //! point RGB channels to an output image with single-precision BGR
  //! channels.
  //! \param[in] in_image Input image
  //! \return Output image of type CV_32FC3 with BGR channel ordering
  cv::Mat RGBToBGRFloat32(const cv::Mat &in_image) const;

  //! \brief Start the render progress bar
  //! \param[in] total_pixels Total number of pixels that will be rendered
  void RenderProgressStart(size_t total_pixels);

  //! \brief Incremenet the number of rendered pixels in the progress bar
  void RenderProgressIncDonePixels();

  //! \brief Stop/end the render progress bar
  void RenderProgressEnd();

  uint image_height_{180};  //!< output image height
  cv::Mat rendered_image_;  //!< output rendered image

  // progress bar related data members
  std::mutex progress_bar_mutex_;        //!< progress bar mutex
  std::shared_ptr<tqdm> progress_bar_;   //!< render progress bar
  size_t progress_bar_total_pixels_ = 0; //!< total number pixels to render
  size_t progress_bar_done_pixels_ = 0;  //!< number of pixels that have been rendered
};

}  // namespace core
}  // namespace olio
