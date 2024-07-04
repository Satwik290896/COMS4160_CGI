// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       trimesh.cc
//! \brief      TriMesh class
//! \author     Hadi Fadaifard, 2022

#include "trimesh.h"
#include <vector>
#include <algorithm>
#include <spdlog/spdlog.h>
#include "utils/gldrawdata.h"
#include "utils/glshader.h"


namespace olio {

using namespace std;

TriMesh::TriMesh()
{

}

bool TriMesh::Load(const boost::filesystem::path &filepath)
{
  bool ret;
  std::cout << "Here is the string3: " << filepath << "\n" << std::endl;
  request_face_normals();
  request_vertex_normals();
  request_vertex_texcoords2D();

    OpenMesh::IO::Options opts{OpenMesh::IO::Options::FaceNormal | 
                             OpenMesh::IO::Options::VertexNormal |
                             OpenMesh::IO::Options::VertexTexCoord};

  ret = OpenMesh::IO::read_mesh(*this, filepath.string(), opts);
  //ComputeVertexNormals();
  //ComputeFaceNormals();

  Vec3r bmin{0, 0, 0};
  Vec3r bmax{0, 0, 0};

  GetBoundingBox(bmin, bmax);

  Real x_len = bmax[0] - bmin[0];
  Real y_len = bmax[1] - bmin[1];
  Real z_len = bmax[2] - bmin[2];

  Real MAX = std::max(x_len, std::max(y_len,z_len));
  std::cout << " " << x_len << " " << y_len << " " << z_len << "\n" << std::endl;
    /*for (auto vit = vertices_begin(); vit != vertices_end(); ++vit) {
      Vec3r p = point(vit);
      p[0] = p[0] * (1.0f/MAX);
      p[1] = p[1] * (1.0f/MAX);
      p[2] = p[2] * (1.0f/MAX);

      set_point(*vit, p);
    }*/

  GetBoundingBox(bmin, bmax);
  ComputeFaceNormals();
  ComputeVertexNormals();
    if (!opts.check(OpenMesh::IO::Options::FaceNormal)) {
      //update_face_normals();
      ComputeFaceNormals();
      //update_face_normals();
    }

    if (!opts.check(OpenMesh::IO::Options::VertexNormal)) {
      //update_vertex_normals();
      ComputeVertexNormals();
      //update_vertex_normals();
    }

  return ret;
}

bool TriMesh::Save(const boost::filesystem::path &filepath,
          OpenMesh::IO::Options opts)
{
  bool ret;
  ret = OpenMesh::IO::write_mesh(*this, filepath.string(), opts);
  return ret;

}

void TriMesh::GetBoundingBox(Vec3r &bmin, Vec3r &bmax)
{
    int i = 0;
    for (auto vit = vertices_begin(); vit != vertices_end(); ++vit, ++i) {
      if (i == 0) {
        bmin = point(vit);
        bmax = point(vit);
      }
      else {
        Vec3r p = point(vit);
        if (p[0] < bmin[0])
          bmin[0] = p[0];
        if (p[1] < bmin[1])
          bmin[1] = p[1];
        if (p[2] < bmin[2])
          bmin[2] = p[2];        

        if (p[0] > bmax[0])
          bmax[0] = p[0];
        if (p[1] > bmax[1])
          bmax[1] = p[1];
        if (p[2] > bmax[2])
          bmax[2] = p[2]; 
      }
    }
}


bool TriMesh::ComputeFaceNormals()
{
  for (auto fit = faces_begin(); fit != faces_end(); ++fit) {
    /*const Vec3r &face_normal = calc_normal(fit);*/
    Vec3r face_normal;
    TriMesh::FaceHandle fh = *fit;
    Vec3r p[3];
    p[0] = Vec3r{0, 0, 0};
    p[1] = Vec3r{0, 0, 0};
    p[2] = Vec3r{0, 0, 0};

    int i = 0;
    for (auto fv_it=fv_iter(fh); fv_it; ++fv_it)
    {
        p[i] = point( fv_it );
        //std::cout << "Points: " << p[i] << "\n" << std::endl;
        i++;
    }

    Vec3r n = (p[1]-p[0]).cross(p[2]-p[0]);
    face_normal = n/n.norm();
    set_normal(*fit, face_normal);
  } 
  return 1;
}

bool TriMesh::ComputeVertexNormals()
{
  for (auto vit = vertices_begin(); vit != vertices_end(); ++vit) {
    Vec3r vertex_normal{0,0,0};
    TriMesh::VertexHandle vh = *vit;
    for (auto vf_it=vf_iter(vh); vf_it; ++vf_it)
    {
        vertex_normal += normal(vf_it);
        //std::cout << "Points: " << p[i] << "\n" << std::endl;
    }    
    if (vertex_normal.norm() != 0) {
      set_normal(*vit, vertex_normal/vertex_normal.norm());
    }
    else {
      set_normal(*vit, Vec3r{0,0,-1});
    }
  } 
  return 1;
}



void TriMesh::DeleteGLBuffers() {
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

void TriMesh::UpdateGLBuffers(bool force_update) {
  // delete existing VBOs
  DeleteGLBuffers();

  vector<Vec3r> positions, normals;
  vector<int> face_indices;

  // interleave positions and normals
  vector<GLfloat> positions_normals(n_vertices() * 6, 0);
  int vertex_count = 0;
  for (auto vit = vertices_begin(); vit != vertices_end(); ++vit) {
    // position
    vertex_count = vertex_count + 1;
    int vid = vit->idx();
    Vec3r p = point( vit );
    Vec3r n = normal(vit)/(normal(vit).norm());

    positions_normals[6*vid] = static_cast<GLfloat>(p[0]);
    positions_normals[6*vid + 1] = static_cast<GLfloat>(p[1]);
    positions_normals[6*vid + 2] = static_cast<GLfloat>(p[2]);

    positions_normals[6*vid + 3] = static_cast<GLfloat>(n[0]);
    positions_normals[6*vid + 4] = static_cast<GLfloat>(n[1]);
    positions_normals[6*vid + 5] = static_cast<GLfloat>(n[2]);


    /*positions_normals.push_back(static_cast<GLfloat>(p[0]));
    positions_normals.push_back(static_cast<GLfloat>(p[1]));
    positions_normals.push_back(static_cast<GLfloat>(p[2]));
    // normal
    positions_normals.push_back(static_cast<GLfloat>(n[0]));
    positions_normals.push_back(static_cast<GLfloat>(n[1]));
    positions_normals.push_back(static_cast<GLfloat>(n[2]));*/
  }

  vertex_count_ = vertex_count;
  // create VBO for positions and normals
  glGenBuffers(1, &positions_normals_vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, positions_normals_vbo_);
  glBufferData(GL_ARRAY_BUFFER, positions_normals.size() * sizeof(GLfloat),
               &positions_normals[0], GL_STATIC_DRAW);



  int face_count = 0;
  vector<GLuint> faces(n_faces()*3, 0);
  for (auto fit = faces_begin(); fit != faces_end(); ++fit) {
    face_count = face_count + 1;
    TriMesh::FaceHandle fh = *fit;
    size_t fid = static_cast<size_t>(fit->idx());

    size_t i = 0;
    for (auto fv_it = fv_iter(*fit); fv_it.is_valid(); fv_it++, i++){
        faces[3 * fid + i] = static_cast<GLuint>(fv_it->idx());
    }

    //faces.push_back(static_cast<GLuint>(fh.idx()));
  }
  
    
  face_indices_count_ = face_count;

  //faces.reserve(face_count);

  // create EBO for faces
  glGenBuffers(1, &faces_ebo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint),
               &faces[0], GL_STATIC_DRAW);

}

void TriMesh::DrawGL(const GLDrawData &draw_data) {
    // check we have a valid material and shader
  auto material = draw_data.GetMaterial();
  if (!material)
    return;
  auto shader = material->GetGLShader();
  if (!shader || !shader->Use())
    return;

  if (!positions_normals_vbo_)
    UpdateGLBuffers(false);

  if (!positions_normals_vbo_ || !faces_ebo_)
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
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(face_indices_count_*3),
                 GL_UNSIGNED_INT, nullptr);

  // check for gl errors
  CheckOpenGLError();
}



}  // namespace olio
