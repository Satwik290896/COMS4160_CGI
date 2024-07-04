// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       phong_dielectric.h
//! \brief      PhongDielectric class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "core/types.h"
#include "core/node.h"
#include "core/material/phong_material.h"
#include "core/ray.h"

namespace olio {
namespace core {

class Light;
class AmbientLight;
class PointLight;

//! \class PhongDielectric
//! \brief PhongDielectric class
class PhongDielectric : public PhongMaterial {
public:
  OLIO_NODE(PhongDielectric)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit PhongDielectric(const std::string &name=std::string());

  //! \brief Constructor
  //! \param[in] ior Index of refraction
  //! \param[in] attenuation Attenuation factor (how much the glass attenuates
  //!            reflected/refracted rays)
  //! \param[in] name Node name
  PhongDielectric(Real ior, Texture::Ptr &attenuation,
                  const std::string &name=std::string());

  //! \brief Scatter incoming ray ray_in
  //! \details The function will generate a reflection and a refraction
  //!    ray. The refraction ray will be null (not generated) if there is total
  //!    internal reflection.
  //! \param[in] hit_record Hit record at hit point
  //! \param[in] ray_in Incoming ray that hit the point
  //! \param[out] reflect_ray Reflected ray
  //! \param[out] refract_ray Refracted ray (null if not refracted)
  //! \param[out] schlick_reflectance Schlick's reflectance
  //! \return Attenuation factor (how much color of reflected/refracted
  //! rays should be attenuated)
  Vec3r Scatter(const HitRecord &hit_record, const Ray &ray_in,
                std::shared_ptr<Ray> &reflect_ray,
                std::shared_ptr<Ray> &refract_ray,
                Real &schlick_reflectance) const;

  //! \brief Set index of refraction
  //! \param[in] ior Index of refraction
  void SetIOR(Real ior) {ior_ = ior;}

  //! \brief Get index of refraction
  //! \return Index of refraction
  Real GetIOR() const {return ior_;}

  //! \brief Compute Schlick's reflectance
  //! \param[in] cos_theta cosine of angle between view vector and
  //!            surface normal
  //! \param[in] ior_in incoming material IOR
  //! \param[in] ior_out outgoing material IOR
  //! \return Reflectance (probability of reflection)
  static Real SchlicksReflectance(Real cos_theta, Real ior_in, Real ior_out);
protected:
  Real ior_{1};  //!< index of refraction
};


inline Real
PhongDielectric::SchlicksReflectance(Real cos_theta, Real ior_in, Real ior_out)
{
  auto ior_ratio = ior_in / ior_out;
  auto r0 = (1.0f - ior_ratio) / (1.0f + ior_ratio);
  r0 = r0 * r0;
  return r0 + (1.0f - r0) * static_cast<Real>(pow(1 - cos_theta, 5));
}

}  // namespace core
}  // namespace olio
