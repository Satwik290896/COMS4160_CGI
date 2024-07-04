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
#include "core/geometry/bvh_node.h"
#include "core/geometry/bvh_trimesh_face.h"

namespace olio {
namespace core {

// class TriMeshBVHNode;
//class BVHNode;

// use Eigen instead of OpenMesh's default structures for Point and
// Normal
struct EigenMeshTraits : OpenMesh::DefaultTraits {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Point  = Vec3r;
  using Normal = Vec3r;
  using TexCoord2D = Vec2r;
};
using OMTriMesh = OpenMesh::TriMesh_ArrayKernelT<EigenMeshTraits>;

//! \class TriMesh
//! \brief TriMesh class
class TriMesh : public OMTriMesh, public Surface {
public:
  OLIO_NODE(TriMesh)

  explicit TriMesh(const std::string &name=std::string());

  //! \brief Check if ray intersects with surface
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t
  //! \param[in] tmax Maximum value for acceptable t
  //! \param[out] hit_record Resulting hit record if ray intersected with surface
  //! \return True if ray intersected with surface
  bool Hit(const Ray &ray, Real tmin, Real tmax,
           HitRecord &hit_record) override;

  //! \brief Check if input ray intersects with input face in the mesh
  //! \param[in] fh Handle of face to check for intersection
  //! \param[in] ray Input ray to check for intersection
  //! \param[in] tmin Minimum value for acceptable t (ray fractional distance)
  //! \param[in] tmax Maximum value for acceptable t (ray fractional distance)
  //! \param[out] hit_record Resulting hit record if ray intersected with face
  //! \return True if ray intersected with surface
  bool RayFaceHit(TriMesh::FaceHandle fh, const Ray &ray, Real tmin,
                  Real tmax, HitRecord &hit_record);

  //! \brief Load mesh from file
  //! \param[in] filepath Path of mesh file to read
  //! \return True on success
  bool Load(const boost::filesystem::path &filepath);

  //! \brief Save mesh to file
  //! \param[in] filepath Path of mesh file to write
  //! \return True on success
  bool Save(const boost::filesystem::path &filepath,
            OpenMesh::IO::Options opts=OpenMesh::IO::Options::Default);

  //! \brief Get/compute mesh's AABB
  //! \return Mesh AABB
  AABB GetBoundingBox(bool force_recompute=false) override;

  void Fill_bbox_vec();

  //! \brief Compute face normals
  //! \return true on success
  bool ComputeFaceNormals();

  //! \brief Compute vertex normals
  //! \return true on success
  bool ComputeVertexNormals();

  //! \brief Set filename associated with mesh
  //! \param[in] filepath Mesh file path
  void SetFilePath(const boost::filesystem::path &filepath)
      {filepath_ = filepath;}

  //! \brief Get mesh filename
  //! \return mesh filename
  boost::filesystem::path GetFilePath() const {return filepath_;}

  void UpdateBVHTriMeshFaces();

  void UpdateBVHNodeArr();

  void UpdateBVHNode();
protected:
  boost::filesystem::path filepath_;
  std::vector<Surface::Ptr> bvh_faces_;
  BVHNode::Ptr bvh_{nullptr};
  std::vector<BVHNode::Ptr> bvh2_;
  std::vector<AABB> bbox_vec_;
  std::vector<std::vector<Surface::Ptr>> bvh_faces_arr_;
};


void update_shadow_hit();

class bvh_class_face {
public:
  BVHNode::Ptr bvh;
  Real ray_t;
};

}  // namespace core
}  // namespace olio
