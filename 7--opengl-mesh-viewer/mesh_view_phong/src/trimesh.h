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

#include "types.h"
#include "utils/utils.h"
#include "utils/material.h"

namespace olio {

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
class GLDrawData;

class TriMesh : public OMTriMesh {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Ptr = std::shared_ptr<TriMesh>;

  explicit TriMesh();


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
  void GetBoundingBox(Vec3r &bmin, Vec3r &bmax);

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

  // opengl
  void DeleteGLBuffers();
  void UpdateGLBuffers(bool force_update=false);
  void DrawGL(const GLDrawData &draw_data);

protected:
  boost::filesystem::path filepath_;
  size_t vertex_count_ = 0;
  size_t face_indices_count_ = 0;
  GLuint positions_normals_vbo_{0};
  GLuint faces_ebo_{0};
};

}  // namespace olio
