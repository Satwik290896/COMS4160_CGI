// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       material.cc
//! \brief      Material class
//! \author     Hadi Fadaifard, 2022

#include "core/material/material.h"
#include "core/ray.h"

namespace olio {
namespace core {

Material::Material(const std::string &name) :
  Node{}
{
  name_ = name.size() ? name : "Material";
}

}  // namespace core
}  // namespace olio
