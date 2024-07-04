// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       bvh_node.h
//! \brief      BVHNode class
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
class Material;

//! \class BVHNode
//! \brief BVHNode class
class BVHNode : public Surface {
public:
  OLIO_NODE(BVHNode)

  explicit BVHNode(const std::string &name=std::string());

  //! \brief Check if ray intersects with surface
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t
  //! \param[in] tmax Maximum value for acceptable t
  //! \param[in] hit_record Resulting hit record if ray intersected with surface
  //! \return True if ray intersected with surface
  bool Hit(const Ray &ray, Real tmin, Real tmax,HitRecord &hit_record) override;
  AABB GetBoundingBox(bool force_recompute=false) override;
  static BVHNode::Ptr BuildBVH(std::vector<Surface::Ptr> surfaces,
                               const std::string &name=std::string());
protected:
  //! \brief Build a BVH (sub)tree from the input list of surface in
  //!        the specified range.
  //! \details Only surfaces with indices in the range [start, end)
  //!        are used to build the tree. The surfaces will form the
  //!        leaves of the tree
  //! \param[in] name Tree name
  //! \param[in] surfaces List of all surfaces
  //! \param[in] start Index of the first surface in the list
  //! \param[in] end Index of the last surface in the list
  //! \param[in] split_axis Axis (dimension) to use to sort and split
  //! the surfaces by (0: x-axis, 1: y-axis, 2: z-axis).
  //! \return Built (sub)tree
  static BVHNode::Ptr BuildBVH(std::vector<Surface::Ptr> &surfaces,
                               size_t start, size_t end, uint split_axis,
                               const std::string &name=std::string());
  Surface::Ptr left_;
  Surface::Ptr right_;
private:
};

}  // namespace core
}  // namespace olio
