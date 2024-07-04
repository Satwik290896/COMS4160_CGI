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
#include "core/light/light.h"
#include "core/geometry/surface.h"
#include "core/ray.h"
#include "core/material/phong_material.h"

namespace olio {
namespace core {

/*class Ray;
class HitRecord;
class Surface;*/

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
  virtual Vec3r Illuminate(const HitRecord &hit_record, const Vec3r &view_vec) const;
protected:
};


// ======================================================================
// *** Homework: Define the PointLight class here. The definition of
// *** the member functions should go in light.cc. Similar to the
// *** definition of the Light class, above, make sure the class
// *** includes the OLIO_NODE macro: the first three lines of the
// *** class definition should be this:
// class PointLight : public Light {
// public:
//   OLIO_NODE(PointLight)
// ...
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

class PointLight : public Light {
public:
  OLIO_NODE(PointLight)

  PointLight(const Vec3r &position, const Vec3r &intensity, const std::string &name=std::string());

  Vec3r Illuminate(const HitRecord &hit_record, const Vec3r &view_vec) const override;

  inline void SetPosition(const Vec3r &position) {position_ = position;}

  inline void SetIntensity(const Vec3r &intensity)  {intensity_ = intensity;}

  inline Vec3r GetPosition() const {return position_;}

  inline Vec3r GetIntensity() const {return intensity_;}

private:
  Vec3r position_{0,0,0};
  Vec3r intensity_{0,0,0};
};

// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****


// ======================================================================
// *** Homework: Define the AmbientLight class here. The definition of
// *** the member functions should go in light.cc. Similar to the
// *** definition of the Light class, above, make sure the class
// *** includes the OLIO_NODE macro: the first three lines of the
// *** class definition should be this:
// class AmbientLight : public Light {
// public:
//   OLIO_NODE(AmbientLight)
// ...
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

class AmbientLight : public Light {
public:
  OLIO_NODE(AmbientLight)

  AmbientLight(const Vec3r &ambient, const std::string &name=std::string());
  
  Vec3r Illuminate(const HitRecord &hit_record, const Vec3r &view_vec) const override;

  inline void SetAmbient(const Vec3r &ambient) {ambient_ = ambient;}
  
  inline Vec3r GetAmbient() const {return ambient_;}

private:
  Vec3r ambient_{0,0,0};
};

// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****


}  // namespace core
}  // namespace olio
