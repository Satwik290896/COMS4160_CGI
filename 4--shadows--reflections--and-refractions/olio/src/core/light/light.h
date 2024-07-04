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
#include "core/types.h"
#include "core/node.h"
#include "core/ray.h"
#include "core/geometry/surface.h"

namespace olio {
namespace core {

/*class Ray;
class HitRecord;
class Surface;
*/
//! \class Light
//! \brief Light class
class Light : public Node {
public:
  OLIO_NODE(Light)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit Light(const std::string &name=std::string());

  //! \brief Illuminate a hit point by retrieving its Phong material
  //! and evaluating it with a call to `Material::Eval()`
  //! \param[in] hit_record Hit record for the point
  //! \param[in] view_vec View vector (points away from the surface)
  //! \return Total radiance leaving the point in the direction of
  //!         view_vec
  virtual Vec3r Illuminate(const HitRecord &hit_record, const Vec3r &view_vec, Surface::Ptr scene) const;
protected:
};


//! \class AmbientLight
//! \brief AmbientLight class
class AmbientLight : public Light {
public:
  OLIO_NODE(AmbientLight)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit AmbientLight(const std::string &name=std::string());

  //! \brief Constructor
  //! \param[in] ambient Light's ambient intensity
  //! \param[in] name Node name
  AmbientLight(const Vec3r &ambient, const std::string &name=std::string());

  //! \brief Illuminate a hit point by retrieving its Phong material
  //! and evaluating it with a call to `Material::Eval()`
  //! \param[in] hit_record Hit record for the point
  //! \param[in] view_vec View vector (points away from the surface)
  //! \return Total radiance leaving the point in the direction of
  //!         view_vec
  Vec3r Illuminate(const HitRecord &hit_record, const Vec3r &view_vec, Surface::Ptr scene) const override;

  //! \brief Set ambient intensity
  //! \param[in] ambient Ambient intensity
  void SetAmbient(const Vec3r &ambient) {ambient_ = ambient;}

  //! \brief Get ambient intensity
  //! \return Ambient intensity
  Vec3r GetAmbient() const {return ambient_;}
protected:
  Vec3r ambient_{0, 0, 0};  //!< ambient intensity
};


//! \class PointLight
//! \brief PointLight class
class PointLight : public Light {
public:
  OLIO_NODE(PointLight)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit PointLight(const std::string &name=std::string());

  //! \brief Constructor
  //! \param[in] position Point light's position
  //! \param[in] intensity Point light's intensity
  //! \param[in] name Node name
  PointLight(const Vec3r &position, const Vec3r &intensity,
             const std::string &name=std::string());

  //! \brief Illuminate a hit point by retrieving its Phong material
  //! and evaluating it with a call to `Material::Eval()`
  //! \param[in] hit_record Hit record for the point
  //! \param[in] view_vec View vector (points away from the surface)
  //! \return Total radiance leaving the point in the direction of
  //!         view_vec
  Vec3r Illuminate(const HitRecord &hit_record, const Vec3r &view_vec, Surface::Ptr scene) const override;

  //! \brief Set light's position
  //! \param[in] position Light position
  void SetPosition(const Vec3r &position) {position_=position;}

  //! \brief Set light's intensity
  //! \param[in] intensity Light's intensity
  void SetIntensity(const Vec3r &intensity) {intensity_ = intensity;}

  //! \brief Get light's position
  //! \return Light position
  Vec3r GetPosition() const {return position_;}

  //! \brief Get light's intensity
  //! \return Light's intensity
  Vec3r GetIntensity() const  {return intensity_;}
protected:
  Vec3r position_{0, 0, 0};   //!< light position
  Vec3r intensity_{0, 0, 0};  //!< light intensity
};

}  // namespace core
}  // namespace olio
