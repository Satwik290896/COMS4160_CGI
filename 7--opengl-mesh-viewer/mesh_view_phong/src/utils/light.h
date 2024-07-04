// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       light.h
//! \brief      Light classes
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "types.h"

namespace olio {

//! \class Light
//! \brief Light class
class Light {
public:
  using Ptr = std::shared_ptr<Light>;

  //! \brief Constructor
  Light() = default;
  virtual ~Light() = default;
protected:
};


//! \class PointLight
//! \brief PointLight class
class PointLight : public Light {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Ptr = std::shared_ptr<PointLight>;

  //! \brief Constructor
  PointLight() = default;

  //! \brief Constructor
  //! \param[in] position Point light's position
  //! \param[in] intensity Point light's intensity
  PointLight(const Vec3r &position, const Vec3r &intensity,
             const Vec3r &ambient=Vec3r{0, 0, 0}) :
    Light{},
    position_{position},
    intensity_{intensity},
    ambient_{ambient} {}

  //! \brief Set light's position
  //! \param[in] position Light position
  void SetPosition(const Vec3r &position) {position_=position;}

  //! \brief Set light's intensity
  //! \param[in] intensity Light's intensity
  void SetIntensity(const Vec3r &intensity) {intensity_ = intensity;}

  //! \brief Set light's ambient value
  //! \param[in] ambient
  void SetAmbient(const Vec3r &ambient) {ambient_ = ambient;}

  //! \brief Get light's position
  //! \return Light position
  Vec3r GetPosition() const {return position_;}

  //! \brief Get light's intensity
  //! \return Light's intensity
  Vec3r GetIntensity() const  {return intensity_;}

  //! \brief Get light's ambient value
  //! \return Light's ambient value
  Vec3r GetAmbient() const  {return ambient_;}
protected:
  Vec3r position_{0, 0, 0};   //!< light position
  Vec3r intensity_{0, 0, 0};  //!< light intensity
  Vec3r ambient_{0, 0, 0};    //!< light ambient value
};

}  // namespace olio
