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
  //! normal by calling ComputeNormal()
  //! \param[in] points Triangle points.
  //! \param[in] name Node name
  Triangle(const std::vector<Vec3r> &points,
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
  //! \details The function returns false if the number of input
  //! points is fewer than 3. The function should also compute/update
  //! the triangle normal by calling 'ComputeNormal()'
  //! \param[in] point Triangle points
  //! \return True if the points were set; false otherwise
  bool SetPoints(const std::vector<Vec3r> &points);

  //! \brief Get triangle points
  //! \param[out] point Triangle points
  //! \return true if the triangle has three points
  bool GetPoints(std::vector<Vec3r> &points) const;

  //! \brief Get triangle normal
  //! \return Triangle normal
  Vec3r GetNormal() const {return normal_;}
protected:
  //! \brief Compute/update triangle normal
  //! \return True if triangle has three points
  bool ComputeNormal();

  std::vector<Vec3r> points_;  //!< triangle points
  Vec3r normal_{0, 0, 0};      //!< triangle normal
private:
};

}  // namespace core
}  // namespace olio
