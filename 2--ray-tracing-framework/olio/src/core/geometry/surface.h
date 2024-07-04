// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       surface.h
//! \brief      Surface class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>

#include "core/node.h"

namespace olio {
namespace core {

class Ray;
class HitRecord;

//! \class Surface
//! \brief Surface class
class Surface : public Node {
public:
  OLIO_NODE(Surface)

  explicit Surface(const std::string &name=std::string());

  //! \brief Check if ray intersects with surface
  //! \details If the ray intersections the surface, the function
  //!          should fill in the 'hit_record' (i.e., information
  //!          about the hit point, normal, etc.)
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t (ray fractional distance)
  //! \param[in] tmax Maximum value for acceptable t (ray fractional distance)
  //! \param[out] hit_record Resulting hit record if ray intersected with surface
  //! \return True if ray intersected with surface
  virtual bool Hit(const Ray &ray, Real tmin, Real tmax,
		   HitRecord &hit_record);
protected:
private:
};

}  // namespace core
}  // namespace olio
