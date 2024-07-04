// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       triangle.h
//! \brief      Triangle class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>

#include "core/geometry/surface.h"

namespace olio {
namespace core {

//! \class Triangle
//! \brief Triangle class
class Triangle : public Surface {
public:
  OLIO_NODE(Triangle)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit Triangle(const std::string &name=std::string());

  //! \brief Constructor
  //! \details The constructor should also compute the face/triangle
  //! normal by calling ComputeNormal(). Points should be ordered
  //! counterclockwise wrt triangle normal.
  //! \param[in] point0 first triangle point
  //! \param[in] point1 second triangle point
  //! \param[in] point2 third triangle point
  //! \param[in] name Node name
  Triangle(const Vec3r &point0, const Vec3r &point1, const Vec3r &point2,
	   const std::string &name=std::string());

  //! \brief Static function to compute ray-triangle intersection
  //!        between input ray and triangle defined by the three input
  //!        points.
  //! \param[in] p0 first triangle point
  //! \param[in] p1 second triangle point
  //! \param[in] p2 third triangle point
  //! \param[in] ray Input ray
  //! \param[in] tmin Minimum acceptable value for ray_t
  //! \param[in] tmax Maximum acceptable value for ray_t
  //! \param[out] In case of intersection, value of t for hit point p:
  //!             p = ray_origin + t * ray_dir
  //! \param[out] uv UV coordinates of the hit point inside the
  //!             triangle. Can be used to compute the barycentric
  //!             coordinates as: (1 - uv[0] - uv[1], uv[0], uv[1])
  //! \return True on success
  static bool RayTriangleHit(const Vec3r &p0, const Vec3r &p1, const Vec3r &p2,
			     const Ray &ray, Real tmin, Real tmax,
			     Real &ray_t, Vec2r &uv);

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

  //! \brief Set triangle points
  //! \details The function should also compute/update the triangle
  //!          normal by calling 'ComputeNormal()'
  //! \param[in] point0 first triangle point
  //! \param[in] point1 second triangle point
  //! \param[in] point2 third triangle point
  void SetPoints(const Vec3r &point0, const Vec3r &point1, const Vec3r &point2);

  //! \brief Get triangle points
  //! \param[out] point0 first triangle point
  //! \param[out] point1 second triangle point
  //! \param[out] point2 third triangle point
  void GetPoints(Vec3r &point0, Vec3r &point1, Vec3r &point2) const;

  //! \brief Get triangle normal
  //! \return Triangle normal
  Vec3r GetNormal() const;
protected:
  //! \brief Compute/update triangle normal
  void ComputeNormal();

  Vec3r point0_{0, 0, 0};  //!< first triangle point
  Vec3r point1_{0, 0, 0};  //!< second triangle point
  Vec3r point2_{0, 0, 0};  //!< third triangle point
  Vec3r normal_{0, 0, 0};  //!< triangle normal
private:
};

}  // namespace core
}  // namespace olio
