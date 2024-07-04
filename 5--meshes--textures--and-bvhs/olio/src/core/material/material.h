// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       material.h
//! \brief      Material class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "core/types.h"
#include "core/node.h"
#include "core/texture/image_texture.h"

namespace olio {
namespace core {

class Ray;
class HitRecord;

//! \class Material
//! \brief Material class
class Material : public Node {
public:
  OLIO_NODE(Material)
  explicit Material(const std::string &name=std::string());
protected:
};

}  // namespace core
}  // namespace olio
