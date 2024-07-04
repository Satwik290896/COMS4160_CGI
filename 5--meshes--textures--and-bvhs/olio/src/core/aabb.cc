// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       aabb.cc
//! \brief      AABB class
//! \author     Hadi Fadaifard, 2022

#include "core/aabb.h"
#include "core/ray.h"

namespace olio {
namespace core {

using namespace std;

AABB::AABB(const Vec3r &bmin, const Vec3r &bmax) :
  min_{bmin},
  max_{bmax}
{
}


void
AABB::Reset()
{
  min_ = Vec3r{kInfinity, kInfinity, kInfinity};
  max_ = Vec3r{-kInfinity, -kInfinity, -kInfinity};
}


bool
AABB::IsValid() const
{
  return (min_[0] != kInfinity && min_[1] != kInfinity &&
          min_[2] != kInfinity && max_[0] != -kInfinity &&
          max_[1] != -kInfinity && max_[2] != -kInfinity);
}


void
AABB::ExpandBy(const Vec3r &point)
{
  min_[0] = std::min(min_[0], point[0]);
  min_[1] = std::min(min_[1], point[1]);
  min_[2] = std::min(min_[2], point[2]);
  max_[0] = std::max(max_[0], point[0]);
  max_[1] = std::max(max_[1], point[1]);
  max_[2] = std::max(max_[2], point[2]);
}


void
AABB::ExpandBy(const AABB &other)
{
  if (!other.IsValid())
    return;
  ExpandBy(other.min_);
  ExpandBy(other.max_);
}


AABB
AABB::IntersectWith(const AABB &other) const
{
  AABB out_box;
  if (!IsValid() || !other.IsValid())
    return out_box;

  // check if the two boxes intersect
  if (max_[0] < other.min_[0] || min_[0] > other.max_[0] ||
      max_[1] < other.min_[1] || min_[1] > other.max_[1] ||
      max_[2] < other.min_[2] || min_[2] > other.max_[2])
    return out_box;

  out_box.min_[0] = std::max(min_[0], other.min_[0]);
  out_box.min_[1] = std::max(min_[1], other.min_[1]);
  out_box.min_[2] = std::max(min_[2], other.min_[2]);
  out_box.max_[0] = std::min(max_[0], other.max_[0]);
  out_box.max_[1] = std::min(max_[1], other.max_[1]);
  out_box.max_[2] = std::min(max_[2], other.max_[2]);
  return out_box;
}


bool
AABB::IsPointInside(const Vec3r &point) const
{
  return (point[0] >= min_[0] && point[1] >= min_[1] && point[2] >= min_[2] &&
          point[0] <= max_[0] && point[1] <= max_[1] && point[2] <= max_[2]);
}


bool
AABB::Hit(const Ray &ray, Real tmin, Real tmax) const
{
  if (!IsValid())
    return false;

  // refactored code from Andrew Kensler
  const Vec3r &origin = ray.GetOrigin();
  const Vec3r &dir = ray.GetDirection();
  for (int i = 0; i < 3; ++i) {
    Real dir_inv = 1.0f / dir[i];
    Real t0 = (min_[i] - origin[i]) * dir_inv;
    Real t1 = (max_[i] - origin[i]) * dir_inv;
    if (dir_inv < 0.0f)
      std::swap(t0, t1);
    tmin = t0 > tmin ? t0 : tmin;
    tmax = t1 < tmax ? t1 : tmax;
    if (tmax < tmin)
      return false;
    // if (tmax <= tmin)
    //   return false;
  }
  return true;
}


AABB
operator*(const Mat4r &xform, const AABB &bbox)
{
  Vec3r delta = bbox.max_ - bbox.min_;
  vector<Vec3r> corners;
  corners.reserve(8);
  corners.push_back(bbox.min_);
  corners.push_back(bbox.min_ + Vec3r{delta[0], 0, 0});
  corners.push_back(bbox.min_ + Vec3r{delta[0], delta[1], 0});
  corners.push_back(bbox.min_ + Vec3r{0, delta[1], 0});
  corners.push_back(bbox.max_);
  corners.push_back(bbox.max_ - Vec3r{delta[0], 0, 0});
  corners.push_back(bbox.max_ - Vec3r{delta[0], delta[1], 0});
  corners.push_back(bbox.max_ - Vec3r{0, delta[1], 0});
  AABB out_bbox;
  for (const auto &corner : corners)
    out_bbox.ExpandBy(XformPoint(xform, corner));
  return out_bbox;
}

}  // namespace core
}  // namespace olio
