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

SurfaceList::SurfaceList(vector<Surface::Ptr> surfaces,
                         const std::string &name) :
  Surface{},
  surfaces_{surfaces}
{
  name_ = name.size() ? name : "SurfaceList";
}


AABB
SurfaceList::GetBoundingBox(bool force_recompute)
{
  // if bound is clean, just return existing bbox_
  if (!force_recompute && !IsBoundDirty())
    return bbox_;

  // compute bounding box of child surfaces given in the surfaces_
  // list
  bbox_.Reset();
  for (size_t i = 0; i < surfaces_.size(); ++i) {
    if (surfaces_[i])
      bbox_.ExpandBy(surfaces_[i]->GetBoundingBox());
  }
  bound_dirty_ = false;
  return bbox_;
}

bool
SurfaceList::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
  bool first_hit = true;
  for (size_t i = 0; i < surfaces_.size(); ++i) {
    auto surface = surfaces_[i];
    if (!surface)
      continue;

    // check for hit
    HitRecord current_hit;
    if (!surface->Hit(ray, tmin, tmax, current_hit))
      continue;

    // update closest hit record
    if (first_hit) {
      hit_record = current_hit;
      first_hit = false;
      tmax = current_hit.GetRayT();
      continue;
    }
    hit_record = current_hit;
    tmax = current_hit.GetRayT();
  }

  return !first_hit;
}

}  // namespace core
}  // namespace olio
