// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       aabb.h
//! \brief      AABB class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <limits>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/ostream.h>
#include "core/types.h"

namespace olio {
namespace core {

class Ray;

//! \class AABB
//! \brief AABB class
class AABB {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  AABB() = default;
  AABB(const Vec3r &bmin, const Vec3r &bmax);

  //! \brief Get min coordinates
  //! \return min coordinates
  inline Vec3r GetMin() const {return min_;}

  //! \brief Get max coordinates
  //! \return max coordinates
  inline Vec3r GetMax() const {return max_;}

  //! \brief Reset bounding box to uninitialized coordinates
  void Reset();

  //! \brief Return whether the bbox is valid (ie min/max coordinates are valid)
  //! \return True if valid
  bool IsValid() const;

  //! \brief Expand bounding box by point
  //! \param[in] point point to expand by
  void ExpandBy(const Vec3r &point);

  //! \brief Expand bounding box by another box
  //! \param[in] other box to expand by
  void ExpandBy(const AABB &other);

  //! \brief Compute intersection of this box another one
  //! \param[in] other other bbox to intersec with
  //! \return Intersecting bbox
  AABB IntersectWith(const AABB &other) const;

  //! \brief Check if point is inside bbox (inclusive)
  //! \param[in] point point to check
  //! \return True if point is inside
  bool IsPointInside(const Vec3r &point) const;

  //! \brief Check if ray intersects with aabb
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t
  //! \param[in] tmax Maximum value for acceptable t
  //! \return True if ray intersected with aabb
  bool Hit(const Ray &ray, Real tmin, Real tmax) const;

  //! \brief Use * operator to transform min and max coordinates by a matrix
  //! \param[in] xform 4x4 matrix to xform by
  //! \param[in] bbox input bbox
  //! \return Transformed bbox
  friend AABB operator*(const Mat4r &xform, const AABB &bbox);
protected:
  Vec3r min_{kInfinity, kInfinity, kInfinity};    //!< min coordinates
  Vec3r max_{-kInfinity, -kInfinity, -kInfinity}; //!< max coordinates
};

//! \brief Use * operator to transform min and max coordinates by a matrix
//! \param[in] xform 4x4 matrix to xform by
//! \param[in] bbox input bbox
//! \return Transformed bbox
AABB operator*(const Mat4r &xform, const AABB &bbox);

}  // namespace core
}  // namespace olio


namespace fmt {
template<>
struct formatter<olio::core::AABB>
{
  template<typename ParseContext>
  inline auto parse(ParseContext &ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto format(olio::core::AABB const &bbox, FormatContext &ctx) ->
    decltype(ctx.out()) {
    if (!bbox.IsValid())
      return format_to(ctx.out(), "*** INVALID BBOX ***");
    const auto &bmin = bbox.GetMin();
    const auto &bmax = bbox.GetMax();
    return format_to(ctx.out(), "\nbmin: {:.6f}, {:.6f}, {:.6f}\n"
                     "bmax: {:.6f}, {:.6f}, {:.6f}",
                     bmin[0], bmin[1], bmin[2],
                     bmax[0], bmax[1], bmax[2]);
  }
};
}  // namespace fmt
