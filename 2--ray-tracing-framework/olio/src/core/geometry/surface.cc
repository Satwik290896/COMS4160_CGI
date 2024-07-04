// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       surface.cc
//! \brief      Surface class
//! \author     Hadi Fadaifard, 2022

#include "core/geometry/surface.h"
#include "core/ray.h"

namespace olio {
namespace core {

Surface::Surface(const std::string &name) :
  Node{name}
{
  name_ = name.size() ? name : "Surface";
}


bool
Surface::Hit(const Ray &, Real, Real, HitRecord &)
{
  return false;
}

}  // namespace core
}  // namespace olio
