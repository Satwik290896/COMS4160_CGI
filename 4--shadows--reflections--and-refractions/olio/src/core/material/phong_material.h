// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       phong_material.h
//! \brief      PhongMaterial class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "core/types.h"
#include "core/node.h"
#include "core/material/material.h"
#include "core/ray.h"
#include "core/light/light.h"

namespace olio {
namespace core {

/*class Light;
class AmbientLight;
class PointLight;*/

//! \class PhongMaterial
//! \brief PhongMaterial class
class PhongMaterial : public Material {
public:
  OLIO_NODE(PhongMaterial)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit PhongMaterial(const std::string &name=std::string());

  //! \brief Constructor
  //! \param[in] ambient Ambient coefficients
  //! \param[in] diffuse Diffuse coefficients
  //! \param[in] specular Specular coefficients
  //! \param[in] shininess Shininess coefficient
  //! \param[in] mirror Mirror coefficients
  //! \param[in] name Node name
  PhongMaterial(const Vec3r &ambient, const Vec3r &diffuse, const Vec3r &specular,
                Real shininess, const Vec3r &mirror=Vec3r{0, 0, 0},
                const std::string &name=std::string());

  //! \brief Evaluate material using point light, surface position,
  //!        etc. View and normal vectors must be unit length.
  //! \param[in] light Input light
  //! \param[in] hit_record Hit record at hit point
  //! \param[in] light_vec light vector (unit length)
  //! \param[in] view_vec view vector (unit length)
  //! \return Evaluated color at hit point
  Vec3r Evaluate(const HitRecord &hit_record, const Vec3r &light_vec,
                 const Vec3r &view_vec) const;

  //! \brief Set ambient coefficients
  //! \param[in] ambient Ambient coefficients
  void SetAmbient(const Vec3r &ambient) {ambient_ = ambient;}

  //! \brief Set diffuse coefficients
  //! \param[in] diffuse Diffuse coefficients
  void SetDiffuse(const Vec3r &diffuse);

  //! \brief Set specular coefficients
  //! \param[in] specular Specular coefficients
  void SetSpecular(const Vec3r &specular) {specular_ = specular;}

  //! \brief Set shininess coefficient (Phong exponent)
  //! \param[in] shininess Shininess coefficient
  void SetShininess(Real shininess) {shininess_ = shininess;}

  //! \brief Set mirror coefficients
  //! \param[in] mirror Mirror coefficients
  void SetMirror(const Vec3r &mirror) {mirror_ = mirror;}

  //! \brief Get ambient coefficients
  //! \return Ambient coefficients
  Vec3r GetAmbient() const  {return ambient_;}

  //! \brief Get diffuse coefficients
  //! \return Diffuse coefficients
  Vec3r GetDiffuse() const  {return diffuse_;}

  //! \brief Get specular coefficients
  //! \return Specular coefficients
  Vec3r GetSpecular() const {return specular_;}

  //! \brief Get shininess coefficient (Phong exponent)
  //! \return Shininess coefficient
  Real GetShininess() const {return shininess_;}

  //! \brief Get mirror coefficients
  //! \return Mirror coefficients
  Vec3r GetMirror() const {return mirror_;}
protected:
  Vec3r ambient_{0, 0, 0};      //!< ambient coefficients
  Vec3r diffuse_{0, 0, 0};      //!< diffuse coefficients
  Vec3r specular_{0, 0, 0};     //!< specular coefficients
  Real shininess_{1};           //!< shininess coefficient
  Vec3r mirror_{0, 0, 0};       //!< mirror coefficients
};

}  // namespace core
}  // namespace olio
