// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       trimesh.cc
//! \brief      TriMesh class
//! \author     Hadi Fadaifard, 2022

#include "core/geometry/trimesh.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"
#include "core/material/material.h"
#include "core/geometry/triangle.h"

namespace olio {
namespace core {

using namespace std;
namespace fs=boost::filesystem;

TriMesh::TriMesh(const std::string &name) :
  OMTriMesh{},
  Surface{name}
{
  name_ = name.size() ? name : "TriMesh";
}


// ======================================================================
// *** Homework: Implement unimplemented TriMesh functions here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

bool TriMesh::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record) 
{
    bool had_hit = false;
    //std::vector<TriMesh::Face>::iterator face;
    //TriMesh::VertexIter  v_it
    if (!bvh_) {
      std::cout << "Print here n! \n" << std::endl;
      for (auto f_it = faces_begin(); f_it != faces_end(); ++f_it) {
        TriMesh::FaceHandle fh = *f_it;
        if (RayFaceHit(fh, ray, tmin, tmax, hit_record)) {
          tmax = hit_record.GetRayT();
          had_hit = true;
        }
      }
    }
    else {
      
      if (bvh_->Hit(ray, tmin, tmax, hit_record))
        had_hit = true;
    }

  return had_hit;
}

bool TriMesh::RayFaceHit(TriMesh::FaceHandle fh, const Ray &ray, Real tmin,
                Real tmax, HitRecord &hit_record)
{
    int i = 0;
    int f_id = fh.idx();
    Vec3r p[3];
    Vec3r n[3];
    Vec2r Tcoord[3];

    for (auto fv_it=fv_iter(fh); fv_it; ++fv_it)
    {

        p[i] = point( fv_it );
        n[i] = normal( fv_it );
        Tcoord[i] = texcoord2D( fv_it );

        i++;
    }


    Vec2r uv{0,0};
    Real t = 0;

    if (!Triangle::RayTriangleHit(p[0], p[1], p[2], ray, tmin, tmax, t, uv)) {
      
      return false;
    }
    
    //std::cout << "RayTriangleHit: Hit \n" << std::endl;
    Real alpha, beta, gamma;

    alpha = 1-uv[0]-uv[1];
    beta = uv[0];
    gamma = uv[1];

    Vec3r normal_tt = alpha * n[0] + beta * n[1] + gamma * n[2];
    Vec3r norm_normal = normal_tt/normal_tt.norm();

    const Vec3r &hit_point = ray.At(t);
    hit_record.SetRayT(t);
    hit_record.SetPoint(hit_point);
    hit_record.SetNormal(ray, norm_normal);
    hit_record.SetSurface(GetPtr());

    FaceGeoUV face_geouv;
    if (has_vertex_texcoords2D())
    {
      Vec2r avg_Tcoord = alpha * Tcoord[0] + beta * Tcoord[1] + gamma * Tcoord[2];
      face_geouv.set_global_uv(avg_Tcoord);
    }
    else
    {
      face_geouv.set_global_uv(Vec2r{-1, -1});
    }

    face_geouv.set_face_id(fh.idx());
    face_geouv.set_uv(uv);
    hit_record.SetFaceGeouv(face_geouv);

    return true;

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
    if (!opts.check(OpenMesh::IO::Options::FaceNormal)) {
      //update_face_normals();
      ComputeFaceNormals();
    }

    if (!opts.check(OpenMesh::IO::Options::VertexNormal)) {
      //update_vertex_normals();
      ComputeVertexNormals();
    }
  bvh_ = nullptr;
  //ComputeVertexNormals();
  //ComputeFaceNormals();
  
  if (ret == true)
    UpdateBVHTriMeshFaces();

  return ret;
}

bool TriMesh::Save(const boost::filesystem::path &filepath,
          OpenMesh::IO::Options opts)
{
  bool ret;
  ret = OpenMesh::IO::write_mesh(*this, filepath.string(), opts);
  return ret;

}

AABB TriMesh::GetBoundingBox(bool force_recompute)
{
   // if bound is clean, just return existing bbox_
  if (!force_recompute && !IsBoundDirty())
    return bbox_;

  // compute triangle bbox
  bbox_.Reset();

  for (auto v_it=vertices_begin(); v_it!=vertices_end(); ++v_it)
  {
    Vec3r p = point(v_it);
    bbox_.ExpandBy(p);
  }

  bound_dirty_ = false;
  return bbox_;
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


void TriMesh::UpdateBVHTriMeshFaces() {

  std::vector<Vec3r> points;  //!< triangle points
  std::vector<Vec3r> normals;      //!< triangle normal
  std::vector<Vec2r> textures;

  for (auto f_it = faces_begin(); f_it != faces_end(); ++f_it) {

    TriMesh::FaceHandle fh = *f_it;
    points.clear();
    normals.clear();
    textures.clear();

    for (auto fv_it=fv_iter(fh); fv_it; ++fv_it)
    {

        points.push_back(point( fv_it ));
        normals.push_back(normal( fv_it ));
        textures.push_back(texcoord2D( fv_it ));
    }    
    BVHTriMeshFace::Ptr bvh_face = BVHTriMeshFace::Create(points, fh.idx(), normals, textures);
    bvh_face->SetMaterial(GetMaterial());
    bvh_faces_.push_back(bvh_face);
  }

  UpdateBVHNode();
}


void TriMesh::UpdateBVHNode()
{
  BVHNode bvhnode;
  bvh_ = bvhnode.BuildBVH(bvh_faces_);
}

}  // namespace core
}  // namespace olio
