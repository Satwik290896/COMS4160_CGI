// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file   twist_triangle.h
//! \brief  TwistTriangle class
//! \author Hadi Fadaifard, 2022

#include "twist_triangle.h"
#include <vector>
#include <algorithm>
#include <spdlog/spdlog.h>
#include "utils/gldrawdata.h"

namespace olio {

using namespace std;

TwistTriangle::TwistTriangle(int subdivisions)
{
  SetSubdivisions(subdivisions);
}


TwistTriangle::~TwistTriangle()
{
  DeleteGLBuffers();
}


void
TwistTriangle::SetSubdivisions(int subdivisions)
{
  subdivisions_ = CLAMP(subdivisions, 0, max_subdivisions_);
  gl_buffers_dirty_ = true;
}

void
TwistTriangle::SetTwistAngle(Real twist_angle)
{
  twist_angle_ = twist_angle;
  gl_buffers_dirty_ = true;
}


void
TwistTriangle::DivideTriangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2,
                              int level, const Vec3f &color1,
                              const Vec3f &color2, Real twist_angle,
                              const Vec3f &center,
                              std::vector<GLfloat> &positions_and_colors)
{
  if (level <= 0) {
    // rotate and draw triangle vertices
    vector<Vec3f> points{p0, p1, p2};
    for (const auto &point : points) {
      Mat3f R{Mat3f::Identity()};
      float r = (point - center).norm();
      float c = static_cast<float>(cos(twist_angle * r * kDEGtoRAD));
      float s = static_cast<float>(sin(twist_angle * r * kDEGtoRAD));
      R(0,0) = c; R(0,1) = -s;
      R(1,0) = s; R(1,1) = c;
      Vec3f pt = R * (point - center) + center;

      // add vertex position
      positions_and_colors.push_back(pt[0]);
      positions_and_colors.push_back(pt[1]);
      positions_and_colors.push_back(pt[2]);

      // add vertex color
      positions_and_colors.push_back(color1[0]);
      positions_and_colors.push_back(color1[1]);
      positions_and_colors.push_back(color1[2]);
    }
    return;
  }

  // compute edge midpoints
  Vec3f p01 = 0.5f * (p0 + p1);
  Vec3f p02 = 0.5f * (p0 + p2);
  Vec3f p12 = 0.5f * (p1 + p2);

  // draw subdivided triangles
  DivideTriangle(p0, p01, p02, level - 1, color1, color2, twist_angle, center,
                 positions_and_colors);
  DivideTriangle(p01, p1, p12, level - 1, color1, color2, twist_angle, center,
                 positions_and_colors);
  DivideTriangle(p02, p12, p2, level - 1, color1, color2, twist_angle, center,
                 positions_and_colors);
  DivideTriangle(p01, p12, p02, level - 1, color2, color1, twist_angle, center,
                 positions_and_colors);
}


void
TwistTriangle::DeleteGLBuffers()
{
  if (!vao_)
    return;

  // delete vbos
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (positions_colors_vbo_) {
    glDeleteBuffers(1, &positions_colors_vbo_);
    positions_colors_vbo_ = 0;
  }

  // delete vao
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao_);
  vao_ = 0;
}


void
TwistTriangle::UpdateGLBuffers(GLShader::Ptr glshader, bool force_update)
{
  if (!gl_buffers_dirty_ && !force_update)
    return;

  // make sure we have a valid glshader object
  if (!glshader || !glshader->Use()) {
    spdlog::error("TwistTriangle::UpdateGLBuffers: invalid GL shader");
    return;
  }

  // delete existing VBOs
  DeleteGLBuffers();

  // generate a VAO, if it doesn't already exist
  if (!vao_)
    glGenVertexArrays(1, &vao_);

  // bind VAO
  glBindVertexArray(vao_);

  // generate triangles
  Vec3f p0{0, 0.75f, 0};
  Vec3f p1{0.65f, -0.375, 0};
  Vec3f p2{-0.65f, -0.375, 0};
  Vec3f center = (p0 + p1 + p2) / 3;
  Vec3f color1{1, 0, 0};
  Vec3f color2{0, 1, 0};
  vector<GLfloat> positions_and_colors;
  DivideTriangle(p0, p1, p2, subdivisions_, color1, color2,
                 twist_angle_, center, positions_and_colors);
  vertex_count_ = positions_and_colors.size() / 6;

  // generate position and color VBOs
  GLint positions_attr_index = 0;
  GLint colors_attr_index = 1;
  positions_attr_index = glGetAttribLocation(glshader->GetProgramID(), "position");
  colors_attr_index = glGetAttribLocation(glshader->GetProgramID(), "color");

  // create VBO for positions and colors
  glGenBuffers(1, &positions_colors_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, positions_colors_vbo_);
  glBufferData(GL_ARRAY_BUFFER, positions_and_colors.size() * sizeof(GLfloat),
               &positions_and_colors[0], GL_STATIC_DRAW);

  // enable positions attribute and set pointer
  glVertexAttribPointer(positions_attr_index, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(positions_attr_index);

  // enable colors attribute and set pointer
  glVertexAttribPointer(colors_attr_index, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(colors_attr_index);

  // unbind VAO
  glBindVertexArray(0);
  gl_buffers_dirty_ = false;
}


void
TwistTriangle::DrawGL(const GLDrawData &draw_data)
{
  // check we have a valid glshader object
  if (!glshader_ || !glshader_->Use())
    return;

  if (gl_buffers_dirty_ || !vao_ || !positions_colors_vbo_)
    UpdateGLBuffers(glshader_, false);

  if (!vao_ || !positions_colors_vbo_ || !vertex_count_)
    return;

  // bind VAO and draw triangles
  glBindVertexArray(vao_);

  // set model, view, and projection matrices
  glshader_->SetMVPMatrices(draw_data.GetModelMatrix(),
                            draw_data.GetViewMatrix(),
                            draw_data.GetProjectionMatrix());

  // draw triangles
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex_count_));

  // check for gl errors
  CheckOpenGLError();

  // unbind VAO
  glBindVertexArray(0);
}

}  // namespace olio
