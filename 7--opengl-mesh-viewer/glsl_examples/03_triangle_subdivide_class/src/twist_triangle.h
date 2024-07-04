// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file   twist_triangle.h
//! \brief  TwistTriangle class
//! \author Hadi Fadaifard, 2022

#pragma once

#include <string>
#include <memory>
#include "types.h"
#include "utils/utils.h"
#include "utils/glshader.h"

namespace olio {

class GLDrawData;

class TwistTriangle {
public:
  using Ptr = std::shared_ptr<TwistTriangle>;

  explicit TwistTriangle(int subdivisions=0);
  TwistTriangle(const TwistTriangle &) = delete;
  TwistTriangle(TwistTriangle &&) = delete;
  TwistTriangle& operator=(const TwistTriangle &) = delete;
  TwistTriangle& operator=(TwistTriangle &&) = delete;
  ~TwistTriangle();

  void SetSubdivisions(int subdivisions);
  void SetTwistAngle(Real twist_angle);
  int GetSubdivisions() const {return subdivisions_;}
  Real GetTwistAngle() const {return twist_angle_;}

  //! \brief recursive function that subdivides and draws all the
  //!        triangles that are displayed by the application
  //! \param[in] p0 first triangle vertex
  //! \param[in] p1 second triangle vertex
  //! \param[in] p2 third triangle vertex
  //! \param[in] level subdivision level
  //! \param[in] color1 color of triangles pointing up
  //! \param[in] color2 color of triangles pointing down
  //! \param[in] twist_angle amount by which to rotate triangle vertices
  //!                        (in degrees)
  //! \param[in] center center of rotation
  void DivideTriangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2,
                      int level, const Vec3f &color1,
                      const Vec3f &color2, Real twist_angle, const Vec3f &center,
                      std::vector<GLfloat> &positions_and_colors);

  // opengl
  void SetGLShader(GLShader::Ptr shader) { glshader_ = shader; }
  GLShader::Ptr GetGLShader() {return glshader_;}
  void DeleteGLBuffers();
  void UpdateGLBuffers(GLShader::Ptr glshader, bool force_update=false);
  void DrawGL(const GLDrawData &draw_data);
protected:
  GLShader::Ptr glshader_;
  int subdivisions_ = 0;  //!< number of subdivision levels
  Real twist_angle_ = 0;   //!< degrees
  size_t vertex_count_ = 0;
  int max_subdivisions_ = 10;

  // opengl
  bool gl_buffers_dirty_ = false;
  GLuint vao_{0};
  GLuint positions_colors_vbo_{0};
};

}  // namespace olio
