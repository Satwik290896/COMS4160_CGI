// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       phong_material.cc
//! \brief      PhongMaterial class
//! \author     Hadi Fadaifard, 2022

#include <iostream>
#include "core/material/phong_material.h"
#include <spdlog/spdlog.h>
#include "core/light/light.h"

namespace olio {
namespace core {

using namespace std;

PhongMaterial::PhongMaterial(const std::string &name) :
  Material{}
{
  name_ = name.size() ? name : "PhongMaterial";
  SetDiffuse(Vec3r{0, 0, 0});
}


PhongMaterial::PhongMaterial(const Vec3r &ambient, const Vec3r &diffuse,
                             const Vec3r &specular, Real shininess,
                             const Vec3r &mirror, const std::string &name) :
  Material{},
  ambient_{ambient},
  specular_{specular},
  shininess_{shininess},
  mirror_{mirror}
{
  name_ = name.size() ? name : "PhongMaterial";
  SetDiffuse(diffuse);
}


void
PhongMaterial::SetDiffuse(const Vec3r &diffuse)
{
  diffuse_ = diffuse;
}


Vec3r Reflect(const Vec3r &v, const Vec3r &normal)
{
  return v - 2 * v.dot(normal) * normal;
}


Vec3r
PhongMaterial::Evaluate(const HitRecord &hit_record, const Vec3r &light_vec,
                        const Vec3r &view_vec) const
{
  // Blinn-Phong halfway vector formulation

  if (hit_record.IsFrontFace() == false)
    return Vec3r{1,1,0};

  const Vec3r &normal = hit_record.GetNormal();
  Vec3r half = (view_vec + light_vec).normalized();
  auto half_dot = fmax(0, half.dot(normal));
  Real specular_falloff = pow(half_dot, shininess_);

  // // phong
  // Vec3r reflect = Reflect(-light_vec, normal);
  // auto reflect_dot = fmax(0, reflect.dot(view_vec));
  // specular_falloff = pow(reflect_dot, shininess_);

  Vec3r specular_color = specular_falloff * specular_;
  return specular_color + diffuse_;
}

}  // namespace core
}  // namespace olio
