// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file   sphere.h
//! \brief  Sphere class
//! \author Hadi Fadaifard, 2022

#include "sphere.h"
#include <vector>
#include <algorithm>
#include <spdlog/spdlog.h>
#include "utils/gldrawdata.h"
#include "utils/glshader.h"

namespace olio {

using namespace std;

Sphere::Sphere(const Vec3r &center, Real radius, uint grid_nx, uint grid_ny) :
  center_{center},
  radius_{radius},
  grid_nx_{grid_nx},
  grid_ny_{grid_ny}
{
  gl_buffers_dirty_ = true;
}


void
Sphere::SetCenter(const Vec3r &center)
{
  center_ = center;
  gl_buffers_dirty_ = true;
}


void
Sphere::SetRadius(Real radius)
{
  radius_ = radius;
  gl_buffers_dirty_ = true;
}


void
Sphere::SetGridSize(uint grid_nx, uint grid_ny)
{
  grid_nx_ = std::max(grid_nx, 3u);
  grid_ny_ = std::max(grid_ny, 3u);
  gl_buffers_dirty_ = true;
}


Sphere::~Sphere()
{
  DeleteGLBuffers();
}


void
Sphere::DeleteGLBuffers()
{
  // delete vbos
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (positions_normals_vbo_) {
    glDeleteBuffers(1, &positions_normals_vbo_);
    positions_normals_vbo_ = 0;
  }

  // delete ebos
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  if (faces_ebo_) {
    glDeleteBuffers(1, &faces_ebo_);
    faces_ebo_ = 0;
  }
}


// sphere without seam
bool
BuildWatertightSphere(const Vec3r &center, Real radius, uint grid_nx,
                      uint grid_ny, vector<Vec3r> &positions,
                      vector<Vec3r> &normals,
                      vector<int> &face_indices)
{
  if (grid_nx < 1 || grid_ny < 1)
    return false;

  // add mesh vertices
  MatXi vertex_indices{grid_ny + 1, grid_nx};
  Real phi_start = 0;
  Real theta_start = 0;
  Real dphi = k2Pi / grid_nx;
  Real dtheta = kPi / grid_ny;
  int vertex_id = 0;
  positions.resize(0);
  normals.resize(0);
  positions.reserve((grid_ny +1) * grid_nx + 2);
  normals.reserve((grid_ny +1) * grid_nx + 2);
  for (uint j = 1; j < grid_ny; ++j) {
    for (uint i = 0; i < grid_nx; ++i) {
      Real cos_phi = cos(phi_start + dphi * i);
      Real sin_phi = sin(phi_start + dphi * i);
      Real cos_theta = cos(theta_start + j * dtheta);
      Real sin_theta = sin(theta_start + j * dtheta);
      Vec3r point{radius * cos_phi * sin_theta,
                  radius * sin_phi * sin_theta,
                  -radius * cos_theta};
      positions.push_back(center + point);
      normals.push_back(point.normalized());
      vertex_indices(j, i) = vertex_id;
      ++vertex_id;
    }
  }

  // add mesh faces
  face_indices.resize(0);
  for (uint j = 1; j < grid_ny-1; ++j) {
    for (uint i = 0; i < grid_nx; ++i) {
      // add face 1
      face_indices.push_back(vertex_indices(j, i));
      face_indices.push_back(vertex_indices(j, (i+1)%grid_nx));
      face_indices.push_back(vertex_indices(j+1, (i+1)%grid_nx));

      // add face 2
      face_indices.push_back(vertex_indices(j, i));
      face_indices.push_back(vertex_indices(j+1, (i+1)%grid_nx));
      face_indices.push_back(vertex_indices(j+1, i));
    }
  }

  // add bottom cap (triangle fan)
  Vec3r bottom{0, 0, -radius};
  int bottom_index = vertex_id;
  positions.push_back(center + bottom);
  normals.push_back(bottom.normalized());
  ++vertex_id;
  for (uint i = 0; i < grid_nx; ++i) {
    face_indices.push_back(bottom_index);
    face_indices.push_back(vertex_indices(1, (i+1)%grid_nx));
    face_indices.push_back(vertex_indices(1, i));
  }

  // add top cap (triangle fan)
  Vec3r top{0, 0, radius};
  int top_index = vertex_id;
  positions.push_back(center + top);
  normals.push_back(top.normalized());
  ++vertex_id;
  for (uint i = 0; i < grid_nx; ++i) {
    face_indices.push_back(top_index);
    face_indices.push_back(vertex_indices(grid_ny - 1, i));
    face_indices.push_back(vertex_indices(grid_ny - 1, (i+1)%grid_nx));
  }
  return true;
}


void
Sphere::UpdateGLBuffers(bool force_update)
{
  if (!gl_buffers_dirty_ && !force_update)
    return;

  // delete existing VBOs
  DeleteGLBuffers();

  vector<Vec3r> positions, normals;
  vector<int> face_indices;
  BuildWatertightSphere(center_, radius_, grid_nx_, grid_ny_, positions, normals,
                        face_indices);

  // interleave positions and normals
  vector<GLfloat> positions_normals;
  for (size_t i = 0; i < positions.size(); ++i) {
    // position
    positions_normals.push_back(static_cast<GLfloat>(positions[i][0]));
    positions_normals.push_back(static_cast<GLfloat>(positions[i][1]));
    positions_normals.push_back(static_cast<GLfloat>(positions[i][2]));
    // normal
    positions_normals.push_back(static_cast<GLfloat>(normals[i][0]));
    positions_normals.push_back(static_cast<GLfloat>(normals[i][1]));
    positions_normals.push_back(static_cast<GLfloat>(normals[i][2]));
  }
  vertex_count_ = positions.size();
  face_indices_count_ = face_indices.size();

  // create VBO for positions and normals
  glGenBuffers(1, &positions_normals_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, positions_normals_vbo_);
  glBufferData(GL_ARRAY_BUFFER, positions_normals.size() * sizeof(GLfloat),
               &positions_normals[0], GL_STATIC_DRAW);

  // create elements array
  vector<GLuint> faces;
  faces.reserve(face_indices.size());
  for (auto index : face_indices)
    faces.push_back(static_cast<GLuint>(index));

  // create EBO for faces
  glGenBuffers(1, &faces_ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint),
               &faces[0], GL_STATIC_DRAW);

  gl_buffers_dirty_ = false;
}


void
Sphere::DrawGL(const GLDrawData &draw_data)
{
  // check we have a valid material and shader
  auto material = draw_data.GetMaterial();
  if (!material)
    return;
  auto shader = material->GetGLShader();
  if (!shader || !shader->Use())
    return;

  if (gl_buffers_dirty_ || !positions_normals_vbo_)
    UpdateGLBuffers(false);

  if (!vertex_count_ || !face_indices_count_ ||
      !positions_normals_vbo_ || !faces_ebo_)
    return;

  // enable depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // set up uniforms: MVP matrices, lights, material
  shader->SetupUniforms(draw_data);

  // enable positions attribute and set pointer
  glBindBuffer(GL_ARRAY_BUFFER, positions_normals_vbo_);
  auto positions_attr_index = glGetAttribLocation(shader->GetProgramID(), "position");
  glVertexAttribPointer(positions_attr_index, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(positions_attr_index);

  // enable normals attribute and set pointer
  auto normals_attr_index = glGetAttribLocation(shader->GetProgramID(), "normal");
  glVertexAttribPointer(normals_attr_index, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(normals_attr_index);

  // draw mesh
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_ebo_);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(face_indices_count_),
                 GL_UNSIGNED_INT, nullptr);

  // check for gl errors
  CheckOpenGLError();
}

}  // namespace olio
