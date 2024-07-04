// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       material.h
//! \brief      Material and PhongMaterial classes
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "types.h"

namespace olio {

class GLShader;

//! \class Material
//! \brief Material class
class Material {
public:
  using Ptr = std::shared_ptr<Material>;

  //! \brief Constructor
  Material() = default;
  virtual ~Material() = default;

  virtual void SetGLShader(std::shared_ptr<GLShader> shader) {glshader_=shader;}
  virtual std::shared_ptr<GLShader> GetGLShader() {return glshader_;}
protected:
  std::shared_ptr<GLShader> glshader_;
};


//! \class PhongMaterial
//! \brief PhongMaterial class
class PhongMaterial : public Material {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Ptr = std::shared_ptr<PhongMaterial>;

  //! \brief Constructor
  PhongMaterial() = default;

  //! \brief Constructor
  //! \param[in] ambient Ambient coefficients
  //! \param[in] diffuse Diffuse coefficients
  //! \param[in] specular Specular coefficients
  //! \param[in] shininess Shininess coefficient
  PhongMaterial(const Vec3r &ambient, const Vec3r &diffuse, const Vec3r &specular,
                Real shininess) :
    Material{},
    ambient_{ambient},
    diffuse_{diffuse},
    specular_{specular},
    shininess_{shininess} {}

  //! \brief Set ambient coefficients
  //! \param[in] ambient Ambient coefficients
  void SetAmbient(const Vec3r &ambient) {ambient_ = ambient;}

  //! \brief Set diffuse coefficients
  //! \param[in] diffuse Diffuse coefficients
  void SetDiffuse(const Vec3r &diffuse) {diffuse_ = diffuse;}

  //! \brief Set specular coefficients
  //! \param[in] specular Specular coefficients
  void SetSpecular(const Vec3r &specular) {specular_ = specular;}

  //! \brief Set shininess coefficient (Phong exponent)
  //! \param[in] shininess Shininess coefficient
  void SetShininess(Real shininess) {shininess_ = shininess;}

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
protected:
  Vec3r ambient_{0, 0, 0};      //!< ambient coefficients
  Vec3r diffuse_{0, 0, 0};      //!< diffuse coefficients
  Vec3r specular_{0, 0, 0};     //!< specular coefficients
  Real shininess_{1};           //!< shininess coefficient
};

}  // namespace olio
