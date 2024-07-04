// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       sphere.h
//! \brief      Sphere class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>

#include "core/geometry/surface.h"

namespace olio {
namespace core {

//! \class Sphere
//! \brief Sphere class
class Sphere : public Surface {
public:
  OLIO_NODE(Sphere)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit Sphere(const std::string &name=std::string());

  //! \brief Constructor
  //! \param[in] center Sphere position
  //! \param[in] radius Sphere radius
  //! \param[in] name Node name
  Sphere(const Vec3r &center, Real radius,
	 const std::string &name=std::string());

  //! \brief Check if ray intersects with surface
  //! \details If the ray intersections the surface, the function
  //!          should fill in the 'hit_record' (i.e., information
  //!          about the hit point, normal, etc.)
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t (ray fractional distance)
  //! \param[in] tmax Maximum value for acceptable t (ray fractional distance)
  //! \param[out] hit_record Resulting hit record if ray intersected with surface
  //! \return True if ray intersected with surface
  bool Hit(const Ray &ray, Real tmin, Real tmax,
	   HitRecord &hit_record) override;

  //! \brief Set sphere position
  //! \param[in] center Sphere center/position
  void SetCenter(const Vec3r &center);

  //! \brief Set sphere radius
  //! \param[in] radius Sphere radius
  void SetRadius(Real radius);

  //! \brief Get sphere position
  //! \return Sphere position
  Vec3r GetCenter() const {return center_;}

  //! \brief Get sphere radius
  //! \return Sphere radius
  Real GetRadius() const {return radius_;}
protected:
  Vec3r center_{0, 0, 0};  //!< sphere position
  Real radius_{0};	   //!< sphere radius
private:
};

}  // namespace core
}  // namespace olio
