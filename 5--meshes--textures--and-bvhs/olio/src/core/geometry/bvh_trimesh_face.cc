#include <iostream>
#include "core/geometry/bvh_trimesh_face.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"

namespace olio {
namespace core {


BVHTriMeshFace::BVHTriMeshFace(const std::vector<Vec3r> &points, int face_id, std::vector<Vec3r> normal, std::vector<Vec2r> texture) :
    points_{points},
    face_id_{face_id},
    normal_{normal},
    texture_{texture}
{
      
}


bool
BVHTriMeshFace::ComputeNormal()
{
  /*normal_ = Vec3r{0, 0, 0};
  if (points_.size() < 3)
    return false;
  normal_ = (points_[1] - points_[0]).cross(points_[2] - points_[0]);
  normal_.normalize();*/
  return true;
}


bool
BVHTriMeshFace::RayTriangleHit(const Vec3r &p0, const Vec3r &p1, const Vec3r &p2,
                         const Ray &ray, Real tmin, Real tmax,
                         Real &ray_t, Vec2r &uv)
{
  const Vec3r &ray_dir = ray.GetDirection();
  const Vec3r &ray_origin = ray.GetOrigin();
  Real a = p0[0] - p1[0];
  Real b = p0[1] - p1[1];
  Real c = p0[2] - p1[2];
  Real d = p0[0] - p2[0];
  Real e = p0[1] - p2[1];
  Real f = p0[2] - p2[2];
  Real g = ray_dir[0];
  Real h = ray_dir[1];
  Real i = ray_dir[2];
  Real j = p0[0] - ray_origin[0];
  Real k = p0[1] - ray_origin[1];
  Real l = p0[2] - ray_origin[2];
  Real ei_minus_hf = e * i - h * f;
  Real gf_minus_di = g * f - d * i;
  Real dh_minus_eg = d * h - e * g;
  Real ak_minus_jb = a * k - j * b;
  Real jc_minus_al = j * c - a * l;
  Real bl_minus_kc = b * l - k * c;
  Real M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;
  if (fabs(M) < kEpsilon) {
    //std::cout << "Return 1 \n" << std::endl;
    return false;
  }
  // compute t
  ray_t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
  if (ray_t < tmin || ray_t > tmax) {
    //std::cout << "Return 2 \n" << std::endl;
    return false;
  }
  // compute gamma
  Real gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;
  if (gamma < 0 || gamma > 1) {
    //std::cout << "Return 3 \n" << std::endl;
    return false;
  }

  // compute beta
  Real beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;
  if (beta < 0 || beta > 1 - gamma) {
    //std::cout << "Return 4 \n" << std::endl;
    return false;
  }

  // set uv
  uv[0] = beta;
  uv[1] = gamma;
  return true;
}


bool
BVHTriMeshFace::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
  if (points_.size() < 3)
    return false;

  Real ray_t{0};
  Vec2r uv;
  if (!RayTriangleHit(points_[0], points_[1], points_[2], ray,
                      tmin, tmax, ray_t, uv))
    return false;

  // fill hit_record
    Real alpha, beta, gamma;

    alpha = 1-uv[0]-uv[1];
    beta = uv[0];
    gamma = uv[1];

    Vec3r normal_tt = alpha * normal_[0] + beta * normal_[1] + gamma * normal_[2];
    Vec3r norm_normal = normal_tt/normal_tt.norm();

    const Vec3r &hit_point = ray.At(ray_t);
    hit_record.SetRayT(ray_t);
    hit_record.SetPoint(hit_point);
    hit_record.SetNormal(ray, norm_normal);
    hit_record.SetSurface(GetPtr());

    FaceGeoUV face_geouv;
    //if (has_vertex_texcoords2D())
    //{
      Vec2r avg_Tcoord = alpha * texture_[0] + beta * texture_[1] + gamma * texture_[2];
      face_geouv.set_global_uv(avg_Tcoord);
    //}
    /*else
    {
      face_geouv.set_global_uv(Vec2r{-1, -1});
    }*/

    face_geouv.set_face_id(face_id_);
    face_geouv.set_uv(uv);
    hit_record.SetFaceGeouv(face_geouv);

    return true;
}

AABB
BVHTriMeshFace::GetBoundingBox(bool force_recompute)
{
  // if bound is clean, just return existing bbox_
  if (!force_recompute && !IsBoundDirty())
    return bbox_;

  // compute triangle bbox
  bbox_.Reset();
  for (const auto &point : points_)
    bbox_.ExpandBy(point);
  bound_dirty_ = false;
  return bbox_;
}



}
}