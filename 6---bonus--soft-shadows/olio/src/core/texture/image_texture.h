// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       image_texture.h
//! \brief      ImageTexture class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include <mutex>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include "core/types.h"
#include "core/node.h"
#include "core/texture/texture.h"

namespace olio {
namespace core {

//! \class ImageTexture
//! \brief ImageTexture class
class ImageTexture : public Texture {
public:
  OLIO_NODE(ImageTexture)

  //! \brief Constructor.
  //! \param[in] image_path Texture image path
  //! \param[in[ flipx Flip horizontally (around the y-axis)
  //! \param[in[ flipy Flip vertically (around the x-axis)
  //! \param[in] name Node name
  ImageTexture(const boost::filesystem::path &image_path,
               bool flipx=false, bool flipy=false,
               const std::string &name=std::string());
  ImageTexture(ImageTexture &&other) = delete;
  ImageTexture& operator=(const ImageTexture &) = delete;
  ImageTexture& operator=(ImageTexture &&) = delete;

  //! \brief Set texture image path
  //! \param[in] image_path Texture image path
  //! \param[in[ flipx Flip horizontally (around the y-axis)
  //! \param[in[ flipy Flip vertically (around the x-axis)
  void SetImagePath(const boost::filesystem::path &image_path,
                    bool flipx=false, bool flipy=false);
  cv::Mat GetImage() const {return image_;}
  boost::filesystem::path GetImagePath() const {return image_path_;}
  Vec3r Value(const Vec2r &uv, const Vec3r &position) override;

  //! \brief Flip image horizontally, vertically, or both
  //! \param[in] in_image Input image
  //! \param[in[ flipx Flip horizontally (around the y-axis)
  //! \param[in[ flipy Flip vertically (around the x-axis)
  //! \return Flipped image
  static cv::Mat FlipImage(const cv::Mat &in_image, bool flipx, bool flipy);
protected:
  ImageTexture(const ImageTexture &other);
  bool LoadImageIfNeeded();

  boost::filesystem::path image_path_;
  bool flipx_{false};
  bool flipy_{false};
  cv::Mat image_;
  std::mutex image_mutex_;
  std::atomic<bool> needs_reload_{false};
};

}  // namespace core
}  // namespace olio
