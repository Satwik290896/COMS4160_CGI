// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       image_texture.cc
//! \brief      ImageTexture class
//! \author     Hadi Fadaifard, 2022

#include "core/texture/image_texture.h"
#include <spdlog/spdlog.h>

namespace olio {
namespace core {

using namespace std;
namespace fs = boost::filesystem;

ImageTexture::ImageTexture(const fs::path &image_path, bool flipx, bool flipy,
                           const std::string &name) :
  Texture{name}
{
  name_ = name.size() ? name : "ImageTexture";
  SetImagePath(image_path, flipx, flipy);
}


ImageTexture::ImageTexture(const ImageTexture &other) :
  Texture{other}
{
  SetImagePath(other.image_path_, other.flipx_, other.flipy_);
}


void
ImageTexture::SetImagePath(const fs::path &image_path, bool flipx, bool flipy)
{
  const std::lock_guard<std::mutex> lock(image_mutex_);
  image_path_ = image_path;
  flipx_ = flipx;
  flipy_ = flipy;
  needs_reload_.store(true);
}


cv::Mat
ImageTexture::FlipImage(const cv::Mat &in_image, bool flipx, bool flipy)
{
  // return original image if not flipping
  if (!flipx && !flipy)
    return in_image;

  cv::Mat out_image;
  int flip_code = 1;            // flip horizontally
  if (flipx && flipy)
    flip_code = -1;             // flip horizontally and vertically
  else if (flipy)
    flip_code = 0;              // flip vertically
  cv::flip(in_image, out_image, flip_code);
  return out_image;
}


bool
ImageTexture::LoadImageIfNeeded()
{
  const std::lock_guard<std::mutex> lock(image_mutex_);
  bool success = true;
  if (needs_reload_.load()) {
    // drops alpha channel (if exists)
    image_ = cv::imread(image_path_.string(), cv::IMREAD_ANYDEPTH |
                        cv::IMREAD_COLOR);
    if (image_.empty()) {
      spdlog::error("ImageTexture: failed to read image {}",
                    image_path_.string());
      success = false;
    } else {
      // flip image
      image_ = FlipImage(image_, flipx_, flipy_);

      // process image (determine depth and ensure correct channels)
      static const vector<string> depth_names{"CV_8U", "CV_8S", "CV_16U",
                                              "CV_16S", "CV_32S", "CV_32F",
                                              "CV_32F"};
      uint depth = static_cast<uint>(image_.depth());
      string depth_name = depth < depth_names.size() ? depth_names[depth] :
        string("<INVALID DEPTH>");
      spdlog::info("ImageTexture: read image {} ({}x{}) with {} channels "
                   "and depth of {}",
                   image_path_.string(), image_.cols, image_.rows,
                   image_.channels(), depth_name);
      if (image_.depth() != CV_8U) {
        spdlog::error("ImageTexture: unsupported depth");
        success = false;
      }
      if (image_.channels() != 3) {
        spdlog::error("ImageTexture: unsupported number of channels");
        success = false;
      }
    }
    needs_reload_.store(false);
  }
  return success;
}


Vec3r
ImageTexture::Value(const Vec2r &uv, const Vec3r &/*position*/)
{
  // check if we need to (re)read image
  if (needs_reload_.load())
    LoadImageIfNeeded();

  // check we have valid image dimensions
  if (image_.empty() || image_.cols < 2 || image_.rows < 2) {
    spdlog::warn("ImageTexture: bad image dimensions ({})",
                 image_path_.string());
    return Vec3r{0, 0, 0} + bias_;
  }

  // ======================================================================
  // *** Homework: Compute pixel coordiantes from u,v and return pixel
  // color at location: make sure the pixel value is computed using
  // bilinear interpolation and that gain_ and bias_ have been applied
  // to the color that's returned by the function. See implementation
  // of SolidTexture::Value() in core/texture/texture.h for example of
  // how to apply gain and bias to the pixel color before returning
  // it.
  //
  // NOTE: the image pixel values will be in [0, 255] but the returned
  // RGB values by the function should be mapped to [0, 1] for each
  // channel.
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

  if (uv[0] == -1 || uv[1] == -1)
    return Vec3r{0,0,0} + bias_;
  
  Real u = CLAMP(uv[0], 0.0f, 1.0f);
  Real v = CLAMP(uv[1], 0.0f, 1.0f);
  //std::cout << "color: " << u << " " << v << "\n" << std::endl;

  auto x = static_cast<float>(u * (static_cast<float>(image_.cols) - 1));
  auto y = static_cast<float>(v * (static_cast<float>(image_.rows) - 1));

  cv::Mat patch;
  cv::getRectSubPix(image_, cv::Size(1,1), cv::Point2f{x, y}, patch);
  auto &color = patch.at<cv::Vec3b>(0, 0);

  Vec3r color_r{0,0,0};
  color_r[2] = (Real)color.val[0]/255.0f;
  color_r[1] = (Real)color.val[1]/255.0f;
  color_r[0] = (Real)color.val[2]/255.0f;

  
  return color_r + bias_;
  //return Vec3r{0, 0, 0};  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


}  // namespace core
}  // namespace olio
