// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       trimesh.h
//! \brief      TriMesh class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Geometry/EigenVectorT.hh>
#include "core/geometry/surface.h"

namespace olio {
namespace core {

class BVHTriMeshFace : public Surface {
public:
  OLIO_NODE(BVHTriMeshFace)

  explicit BVHTriMeshFace(const std::string &name=std::string());

  BVHTriMeshFace(const std::vector<Vec3r> &points, int face_id, std::vector<Vec3r> normal, std::vector<Vec2r> texture);
  //! \brief Check if ray intersects with surface
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t
  //! \param[in] tmax Maximum value for acceptable t
  //! \param[out] hit_record Resulting hit record if ray intersected with surface
  //! \return True if ray intersected with surface
  bool Hit(const Ray &ray, Real tmin, Real tmax,
           HitRecord &hit_record) override;

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

  //! \brief Get/compute surface's AABB
  //! \return Surface's AABB
  AABB GetBoundingBox(bool force_recompute=false) override;

protected:
  bool ComputeNormal();

  int face_id_{-1};  //!< triangle id in mesh (-1 if not a triangle or TriMesh)
  std::vector<Vec3r> points_;  //!< triangle points
  std::vector<Vec3r> normal_;      //!< triangle normal
  std::vector<Vec2r> texture_; 

};
}
}