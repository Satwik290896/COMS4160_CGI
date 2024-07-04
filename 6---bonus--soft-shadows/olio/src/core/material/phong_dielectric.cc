// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       phong_dielectric.cc
//! \brief      PhongDielectric class
//! \author     Hadi Fadaifard, 2022

#include "core/material/phong_dielectric.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"

namespace olio {
namespace core {

using namespace std;

PhongDielectric::PhongDielectric(const std::string &name) :
  PhongMaterial{}
{
  name_ = name.size() ? name : "PhongDielectric";
  //SetDiffuse(Vec3r{1, 1, 1});
  SetDiffuse(nullptr);
}


PhongDielectric::PhongDielectric(Real ior, Texture::Ptr &attenuation,
                                 const std::string &name) :
  PhongMaterial{},
  ior_{ior}
{
  name_ = name.size() ? name : "PhongDielectric";
  SetDiffuse(attenuation);
}


Vec3r
PhongDielectric::Scatter(const HitRecord &hit_record,
                         const Ray &ray_in,
                         std::shared_ptr<Ray> &reflect_ray,
                         std::shared_ptr<Ray> &refract_ray,
                         Real &schlick_reflectance) const
{
  // attenuation is stored in diffuse_
  //Vec3r attenuation = diffuse_;

  Vec3r attenuation{0,0,0};
  Vec3r position{0,0,0};
  if (diffuse_) {
    const auto &uv = hit_record.GetFaceGeouv().get_global_uv();
    attenuation = diffuse_->Value(uv, position);
  }

  // compute incoming angle's cos/sin
  const Vec3r &normal = hit_record.GetNormal();
  const Vec3r &v = -ray_in.GetDirection().normalized();
  auto cos_theta = fmin(v.dot(normal), 1.0f);
  auto sin_theta = sqrt(1.f - cos_theta * cos_theta);

  // the below comparison checks for total internal reflection
  Real ior_in = 1.0f;
  Real ior_out = ior_;
  if (!hit_record.IsFrontFace())
    swap(ior_in, ior_out);
  bool reflect_only = (sin_theta * ior_in / ior_out) > 1;

  // Schlick’s approximation: estimate probability of reflection
  if (reflect_only)
    schlick_reflectance = 1.0f;
  else
    schlick_reflectance = SchlicksReflectance(cos_theta, ior_in, ior_out);

  // create reflection ray
  reflect_ray = make_shared<Ray>();
  *reflect_ray = Ray::Reflect(ray_in, hit_record.GetPoint(), normal);

  // create refraction ray
  if (!reflect_only) {
    refract_ray = make_shared<Ray>();
    *refract_ray = Ray::Refract(ray_in, hit_record.GetPoint(), normal,
                                ior_in, ior_out);
  }
  return attenuation;
}

}  // namespace core
}  // namespace olio
