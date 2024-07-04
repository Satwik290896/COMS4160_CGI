// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       sphere.cc
//! \brief      Sphere class
//! \author     Hadi Fadaifard, 2022

#include "core/geometry/sphere.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"
#include <cmath>

namespace olio {
namespace core {

Sphere::Sphere(const std::string &name) :
  Surface{}
{
  name_ = name.size() ? name : "Sphere";
}


Sphere::Sphere(const Vec3r &center, Real radius, const std::string &name) :
  Surface{},
  center_{center},
  radius_{radius}
{
  name_ = name.size() ? name : "Sphere";
}


void
Sphere::SetCenter(const Vec3r &center)
{
  center_ = center;
  bound_dirty_ = true;
}


void
Sphere::SetRadius(Real radius)
{
  radius_ = radius;
  bound_dirty_ = true;
}


AABB
Sphere::GetBoundingBox(bool force_recompute)
{
  // if bound is clean, just return existing bbox_
  if (!force_recompute && !IsBoundDirty())
    return bbox_;

  // compute sphere bbox
  bbox_.Reset();
  Vec3r radius3{radius_, radius_, radius_};
  bbox_.ExpandBy(AABB{center_ - radius3, center_ + radius3});
  bound_dirty_ = false;
  return bbox_;
}


bool
Sphere::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
  auto p0 = ray.GetOrigin() - center_;
  auto v = ray.GetDirection();
  auto a = v.squaredNorm();
  auto b = 2 * p0.dot(v);
  auto c = p0.squaredNorm() - radius_ * radius_;

  // solve quadratic equation to find t
  auto a2 = 2 * a;
  auto discriminant = b * b - 2 * a2 * c;
  if (discriminant < 0)
    return false;
  auto s = static_cast<Real>(sqrt(discriminant));
  auto t = (-b - s) / a2;
  if (t < tmin)
    t = (-b + s) / a2;
  if (t < tmin || t > tmax)
    return false;

  // fill hit record
  const Vec3r &hit_point = ray.At(t);
  hit_record.SetRayT(t);
  hit_record.SetPoint(hit_point);
  hit_record.SetNormal(ray, (hit_point - center_).normalized());
  hit_record.SetSurface(GetPtr());


  const Vec3r hitt = hit_point - center_;
  double ratio = hitt[1]/hitt[0];
  double phi;

  if (hitt[0] >= 0 && hitt[1] >= 0) {
      phi = atan(ratio);
  }
  else if (hitt[0] < 0 && hitt[1] >= 0) {
      phi = kPi/2 + atan(-ratio);
  }
  else if (hitt[0] < 0 && hitt[1] < 0) {
    phi = kPi + atan(ratio);
  }
  else if (hitt[0] >= 0 && hitt[1] < 0) {
    phi = 3*kPi/2 + atan(-ratio);
  }

  double theta = acos(hitt[2]/radius_);

  if (theta > kPi) {
    theta = k2Pi - theta;
  }
  else if (theta < 0) {
    theta = -theta;
  }





  FaceGeoUV face_geouv;

  face_geouv.set_face_id(-1);
  face_geouv.set_uv(Vec2r{-1, -1});
  face_geouv.set_global_uv(Vec2r{phi/k2Pi, theta/kPi});

  hit_record.SetFaceGeouv(face_geouv);

  return true;
}

}  // namespace core
}  // namespace olio
