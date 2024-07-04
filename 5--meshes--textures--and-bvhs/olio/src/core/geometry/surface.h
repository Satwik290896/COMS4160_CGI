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
#include "core/aabb.h"
#include "core/face_geouv.h"

namespace olio {
namespace core {

class Ray;
class HitRecord;
class Material;

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

  //! \brief Set surface's material
  //! \param[in] material Material to set
  virtual void SetMaterial(std::shared_ptr<Material> material);

  //! \brief Get surface's material
  //! \return Node's material
  virtual std::shared_ptr<Material> GetMaterial();

  //! \brief Set surfaces's bounding box
  //! \param[in] bbox Surface bbox
  virtual void SetBoundingBox(const AABB &bbox) {bbox_ = bbox;}

  //! \brief Set/unset dirty bound flag
  //! \param[in] dirty Whether to dirty bound
  virtual void SetBoundDirty(bool dirty) {bound_dirty_ = dirty;}

  //! \brief Get/compute surface's AABB
  //! \return Surface's AABB
  virtual AABB GetBoundingBox(bool force_recompute=false);

  //! \brief Check if bounds are dirty and need to be recomputed
  //! \return Whether node bounds are dirty
  virtual bool IsBoundDirty() const {return bound_dirty_;}
protected:
  std::shared_ptr<Material> material_; //!< node material
  AABB bbox_;  //!< surface's axis-aligned bounding box
  bool bound_dirty_{true};    //!< whether bounds need to be recomputed
private:
};

}  // namespace core
}  // namespace olio
