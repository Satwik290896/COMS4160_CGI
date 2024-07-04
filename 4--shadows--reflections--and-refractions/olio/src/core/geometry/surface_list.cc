// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       surface_list.cc
//! \brief      SurfaceList class
//! \author     Hadi Fadaifard, 2022

#include "core/geometry/surface_list.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"

namespace olio {
namespace core {

using namespace std;

SurfaceList::SurfaceList(const vector<Surface::Ptr> &surfaces,
                         const std::string &name) :
  Surface{},
  surfaces_{surfaces}
{
  name_ = name.size() ? name : "SurfaceList";
}


bool
SurfaceList::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
  bool hit_something = false;
  for (size_t i = 0; i < surfaces_.size(); ++i) {
    auto surface = surfaces_[i];
    if (!surface)
      continue;

    // check for hit
    HitRecord current_hit;
    if (!surface->Hit(ray, tmin, tmax, current_hit))
      continue;

    // update closest hit record
    hit_record = current_hit;
    tmax = current_hit.GetRayT();
    hit_something = true;
  }

  return hit_something;
}

}  // namespace core
}  // namespace olio
