// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       triangle.cc
//! \brief      Triangle class
//! \author     Hadi Fadaifard, 2022

#include <iostream>
#include "core/geometry/triangle.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"

namespace olio {
namespace core {

Triangle::Triangle(const std::string &name) :
  Surface{}
{
  name_ = name.size() ? name : "Triangle";
}


Triangle::Triangle(const std::vector<Vec3r> &points, const std::string &name) :
  Surface{},
  points_{points}
{
  name_ = name.size() ? name : "Triangle";
  ComputeNormal();
}


bool
Triangle::ComputeNormal()
{
  normal_ = Vec3r{0, 0, 0};
  if (points_.size() < 3)
    return false;
  normal_ = (points_[1] - points_[0]).cross(points_[2] - points_[0]);
  normal_.normalize();
  return true;
}


bool
Triangle::RayTriangleHit(const Vec3r &p0, const Vec3r &p1, const Vec3r &p2,
                         const Ray &ray, Real tmin, Real tmax,
                         Real &ray_t, Vec2r &uv)
{
  const Vec3r &ray_dir = ray.GetDirection();
  const Vec3r &ray_origin = ray.GetOrigin();
  Real a = p0[0] - p1[0];
  Real b = p0[1] - p1[1];
  Real c = p0[2] - p1[2];
  Real d = p0[0] - p2[0];
  Real e = p0[1] - p2[1];
  Real f = p0[2] - p2[2];
  Real g = ray_dir[0];
  Real h = ray_dir[1];
  Real i = ray_dir[2];
  Real j = p0[0] - ray_origin[0];
  Real k = p0[1] - ray_origin[1];
  Real l = p0[2] - ray_origin[2];
  Real ei_minus_hf = e * i - h * f;
  Real gf_minus_di = g * f - d * i;
  Real dh_minus_eg = d * h - e * g;
  Real ak_minus_jb = a * k - j * b;
  Real jc_minus_al = j * c - a * l;
  Real bl_minus_kc = b * l - k * c;
  Real M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;
  if (fabs(M) < kEpsilon) {
    //std::cout << "Return 1 \n" << std::endl;
    return false;
  }
  // compute t
  ray_t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
  if (ray_t < tmin || ray_t > tmax) {
    //std::cout << "Return 2 \n" << std::endl;
    return false;
  }
  // compute gamma
  Real gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;
  if (gamma < 0 || gamma > 1) {
    //std::cout << "Return 3 \n" << std::endl;
    return false;
  }

  // compute beta
  Real beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;
  if (beta < 0 || beta > 1 - gamma) {
    //std::cout << "Return 4 \n" << std::endl;
    return false;
  }

  // set uv
  uv[0] = beta;
  uv[1] = gamma;
  return true;
}


bool
Triangle::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
  if (points_.size() < 3)
    return false;

  Real ray_t{0};
  Vec2r uv;
  if (!RayTriangleHit(points_[0], points_[1], points_[2], ray,
                      tmin, tmax, ray_t, uv))
    return false;

  // fill hit_record
  const Vec3r &hit_point = ray.At(ray_t);
  hit_record.SetRayT(ray_t);
  hit_record.SetPoint(hit_point);
  hit_record.SetNormal(ray, normal_);
  hit_record.SetSurface(GetPtr());

  FaceGeoUV face_geouv;

  face_geouv.set_face_id(0);
  face_geouv.set_uv(uv);
  face_geouv.set_global_uv(Vec2r{-1, -1});

  hit_record.SetFaceGeouv(face_geouv);

  return true;
}


AABB
Triangle::GetBoundingBox(bool force_recompute)
{
  // if bound is clean, just return existing bbox_
  if (!force_recompute && !IsBoundDirty())
    return bbox_;

  // compute triangle bbox
  bbox_.Reset();
  for (const auto &point : points_)
    bbox_.ExpandBy(point);
  bound_dirty_ = false;
  return bbox_;
}


bool
Triangle::SetPoints(const std::vector<Vec3r> &points)
{
  if (points.size() < 3) {
    spdlog::error("Triangle::SetPoints: number of points < 3");
    return false;
  }

  if (points.size() > 3) {
    spdlog::warn("Triangle::SetPoints: number of points > 3 -- "
                 "using first three points");
  }
  points_.reserve(3);
  points_.push_back(points[0]);
  points_.push_back(points[1]);
  points_.push_back(points[2]);
  ComputeNormal();
  return true;
}


bool
Triangle::GetPoints(std::vector<Vec3r> &points) const
{
  points = points_;
  return points_.size() == 3;
}

}  // namespace core
}  // namespace olio
