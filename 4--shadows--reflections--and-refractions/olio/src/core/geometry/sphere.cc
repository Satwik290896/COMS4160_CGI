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
}


void
Sphere::SetRadius(Real radius)
{
  radius_ = radius;
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

  return true;
}

}  // namespace core
}  // namespace olio
