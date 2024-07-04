// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       texture.h
//! \brief      Texture class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "core/types.h"
#include "core/node.h"

namespace olio {
namespace core {

//! \class Texture
//! \brief Texture class
class Texture : public Node {
public:
  OLIO_NODE(Texture)

  // Default constructor and assignment oprators
  explicit Texture(const std::string &name=std::string());

  virtual void SetGain(const Vec3r &gain) {gain_ = gain;}
  virtual void SetBias(const Vec3r &bias) {bias_ = bias;}
  virtual Vec3r GetGain() const {return gain_;}
  virtual Vec3r GetBias() const {return bias_;}
  virtual Vec3r Value(const Vec2r &/*uv*/, const Vec3r &/*position*/) {
    return Vec3r{0, 0, 0} + bias_;
  }
protected:
  Vec3r gain_{1, 1, 1};
  Vec3r bias_{0, 0, 0};
};


//! \class SolidTexture
//! \brief SolidTexture class
class SolidTexture : public Texture {
public:
  OLIO_NODE(SolidTexture)

  // Default constructor and assignment oprators
  explicit SolidTexture(const Vec3r &color, const std::string &name=std::string());

  void SetColor(const Vec3r &color) {color_ = color;}
  Vec3r GetColor() const {return color_;}
  Vec3r Value(const Vec2r &/*uv*/, const Vec3r &/*position*/) override {
    return color_.cwiseProduct(gain_) + bias_;
  }
protected:
  Vec3r color_{0, 0, 0};
};

}  // namespace core
}  // namespace olio
