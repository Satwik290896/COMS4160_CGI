// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       utils.h
//! \brief      Misc utility functions
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "types.h"

namespace olio {

bool CheckOpenGLError();

inline void
GLMToEigen(const glm::mat4 &glm_mat, Mat4r &m)
{
  // glm::mat4 is column-major (and single precision)
  m << glm_mat[0][0], glm_mat[1][0], glm_mat[2][0], glm_mat[3][0],
    glm_mat[0][1], glm_mat[1][1], glm_mat[2][1], glm_mat[3][1],
    glm_mat[0][2], glm_mat[1][2], glm_mat[2][2], glm_mat[3][2],
    glm_mat[0][3], glm_mat[1][3], glm_mat[2][3], glm_mat[3][3];
}


inline Mat4r
GLMToEigen(const glm::mat4 &glm_mat)
{
  Mat4r eigen_mat;
  GLMToEigen(glm_mat, eigen_mat);
  return eigen_mat;
}


inline void
EigenToGLM(const Mat4r &matd, glm::mat4 &glm_mat)
{
  // glm::mat4 is column-major (and single precision)
  Mat4f matf = matd.cast<float>();
  float m4_values[16] = {matf(0, 0), matf(1, 0), matf(2, 0), matf(3, 0),
			 matf(0, 1), matf(1, 1), matf(2, 1), matf(3, 1),
			 matf(0, 2), matf(1, 2), matf(2, 2), matf(3, 2),
			 matf(0, 3), matf(1, 3), matf(2, 3), matf(3, 3)};
  glm_mat = glm::make_mat4(m4_values);
}


inline glm::mat4
EigenToGLM(const Mat4r &matd)
{
  glm::mat4 glm_mat;
  EigenToGLM(matd, glm_mat);
  return glm_mat;
}


inline glm::vec3
EigenToGLM(const Vec3r &vec3d)
{
  Vec3f vec3f{vec3d.cast<float>()};
  return glm::vec3{vec3f[0], vec3f[1], vec3f[2]};
}


inline Vec3r
GLMToEigen(const glm::vec3 &vec3f)
{
  return Vec3r{vec3f[0], vec3f[1], vec3f[2]};
}

}  // namespace olio
