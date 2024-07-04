// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       surface_list.h
//! \brief      SurfaceList class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include <set>
#include "core/geometry/surface.h"

namespace olio {
namespace core {

class Ray;
class HitRecord;

//! \class SurfaceList
//! \brief SurfaceList class
class SurfaceList : public Surface {
public:
  OLIO_NODE(SurfaceList)

  SurfaceList(std::vector<Surface::Ptr> surfaces,
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

  inline std::vector<Surface::Ptr> GetListSurfaces() const {return surfaces_;} 
  //! \brief Get/compute surface's AABB
  //! \return Surface's AABB
  AABB GetBoundingBox(bool force_recompute=false) override;
protected:
  std::vector<Surface::Ptr> surfaces_;
private:
};

}  // namespace core
}  // namespace olio
