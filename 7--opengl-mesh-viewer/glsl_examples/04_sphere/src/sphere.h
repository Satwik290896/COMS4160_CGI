// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file   sphere.h
//! \brief  Sphere class
//! \author Hadi Fadaifard, 2022

#pragma once

#include <string>
#include <memory>
#include "types.h"
#include "utils/utils.h"
#include "utils/material.h"

namespace olio {

class GLDrawData;

class Sphere {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  using Ptr = std::shared_ptr<Sphere>;

  explicit Sphere(const Vec3r &center=Vec3r{0,0,0}, Real radius=1.0f,
                  uint grid_nx=10, uint grid_ny=10);
  Sphere(const Sphere &) = delete;
  Sphere(Sphere &&) = delete;
  Sphere& operator=(const Sphere &) = delete;
  Sphere& operator=(Sphere &&) = delete;
  ~Sphere();

  void SetCenter(const Vec3r &center);
  void SetRadius(Real radius);
  void SetGridSize(uint grid_nx, uint grid_ny);
  Vec3r GetCenter() const {return center_;}
  Real  GetRadius() const {return radius_;}
  void  GetGridSize(uint &grid_nx, uint &grid_ny) const {
    grid_nx = grid_nx_;
    grid_ny = grid_ny_;
  }

  // opengl
  void DeleteGLBuffers();
  void UpdateGLBuffers(bool force_update=false);
  void DrawGL(const GLDrawData &draw_data);
protected:
  Vec3r center_{0, 0, 0};
  Real radius_ = 1.0f;
  uint grid_nx_ = 10;
  uint grid_ny_ = 10;
  size_t vertex_count_ = 0;
  size_t face_indices_count_ = 0;

  // opengl
  bool gl_buffers_dirty_ = false;
  GLuint positions_normals_vbo_{0};
  GLuint faces_ebo_{0};
};

}  // namespace olio
