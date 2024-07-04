// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       light.cc
//! \brief      Node class
//! \author     Hadi Fadaifard, 2022

#include "core/light/light.h"
#include "core/geometry/surface.h"
#include "core/ray.h"
#include "core/material/phong_material.h"
#include <iostream>

namespace olio {
namespace core {

using namespace std;

size_t grid_size_ = 0;
Real *random_pu_t;
Real *random_pv_t;

Light::Light(const std::string &name) :
  Node{name}
{
  name_ = name.size() ? name : "Light";
}


Vec3r
Light::Illuminate(const HitRecord &/*hit_record*/, const Vec3r &/*view_vec*/,
                  Surface::Ptr /*scene*/) const
{
  return Vec3r{0, 0, 0};
}


AmbientLight::AmbientLight(const std::string &name) :
  Light{name}
{
  name_ = name.size() ? name : "AmbientLight";
}


AmbientLight::AmbientLight(const Vec3r &ambient, const std::string &name) :
  Light{name},
  ambient_{ambient}
{
  name_ = name.size() ? name : "AmbientLight";
}


Vec3r
AmbientLight::Illuminate(const HitRecord &hit_record, const Vec3r &/*view_vec*/,
                         Surface::Ptr /*scene*/) const
{
  // only process phong materials
  auto surface = hit_record.GetSurface();
  if (!surface)
    return Vec3r{0, 0, 0};
  auto phong_material = dynamic_pointer_cast<PhongMaterial>(surface->
                                                            GetMaterial());
  if (!phong_material)
    return Vec3r{0, 0, 0};
  return ambient_.cwiseProduct(phong_material->GetAmbient());
}


PointLight::PointLight(const std::string &name) :
  Light{name}
{
  name_ = name.size() ? name : "PointLight";
}


PointLight::PointLight(const Vec3r &position, const Vec3r &intensity,
                       const std::string &name) :
  Light{name},
  position_{position},
  intensity_{intensity}
{
  name_ = name.size() ? name : "PointLight";
}


Vec3r
PointLight::Illuminate(const HitRecord &hit_record, const Vec3r &view_vec,
                       Surface::Ptr scene) const
{
  // evaluate hit points material
  Vec3r black{0, 0, 0};

  // create a shadow ray to the point light and check for occlusion
  const auto &hit_position = hit_record.GetPoint();
  Ray shadow_ray{hit_position, GetPosition() - hit_position};
  HitRecord shadow_record;
  if (scene->Hit(shadow_ray, kEpsilon, 1, shadow_record)) {
    return black;
  }

  // only process phong materials
  auto surface = hit_record.GetSurface();
  if (!surface)
    return black;
  auto phong_material = dynamic_pointer_cast<PhongMaterial>(surface->
                                                            GetMaterial());
  if (!phong_material)
    return black;

  // compute irradiance at hit point
  const Vec3r &normal = hit_record.GetNormal();
  Vec3r light_vec = position_ - hit_position;
  auto distance2 = light_vec.squaredNorm();
  light_vec.normalize();
  auto denominator = std::max(kEpsilon2, distance2);
  Vec3r irradiance = intensity_ * fmax(0.0f, normal.dot(light_vec))/denominator;

  // compute how much the material absorts light
  const Vec3r &attenuation = phong_material->Evaluate(hit_record, light_vec,
                                                      view_vec);
  return irradiance.cwiseProduct(attenuation);
}



void update_grid_size(size_t &grid_size)
{
  grid_size_ = grid_size;
  random_pu_t = new Real[grid_size*grid_size];
  random_pv_t = new Real[grid_size*grid_size];

  int count = 0;
  for (int i = 0; i < grid_size_; i++) {
    for (int j = 0; j < grid_size_; j++) {
      random_pu_t[count] = (((Real)rand())/((Real)RAND_MAX));
      random_pv_t[count] = (((Real)rand())/((Real)RAND_MAX));
      count++;
    }
  }
}

Vec3r give_coordinates(Vec3r center, Vec3r u, Vec3r v, Vec2r rC, Real len) {
  Vec3r CD = center + ((rC[0] - 0.5) * len * u) + ((rC[1] - 0.5) * len * v);
  return CD;
}


AreaLight::AreaLight(const Vec3r &center, const Vec3r &normal, const Vec3r &u_dir, const Vec3r &color, const Real &len,
             const std::string &name):
  Light{name},
  center_{center},
  normal_{normal/normal.norm()},
  u_dir_{u_dir/u_dir.norm()},
  color_{color},
  len_{len}
{
  name_ = name.size() ? name : "AreaLight";
}

Vec3r AreaLight::Illuminate(const HitRecord &hit_record, const Vec3r &view_vec,
                   std::shared_ptr<Surface> scene) const
{
  Vec3r black{0, 0, 0};
  
  auto surface = hit_record.GetSurface();
  if (!surface)
    return black;
  auto phong_material = dynamic_pointer_cast<PhongMaterial>(surface->
                                                            GetMaterial());
  if (!phong_material)
    return black;



  Vec3r v_dir = normal_.cross(u_dir_);
  Real sum_c = color_[0] + color_[1] + color_[2];
  //Real max_value = max({color_[0], color_[1], color_[2]});

  Real I = sum_c;

  Vec3r color_scaled = color_/I;



  // create a shadow ray to the point light and check for occlusion
  Vec3r hit_position = hit_record.GetPoint();
  Vec3r hit_normal = hit_record.GetNormal();

  Real grid_slen = len_/grid_size_;
  Vec3r ret{0, 0, 0};

  int n_rays = 0;
  int count = 0;
  for (int i = 0; i < grid_size_; i++) {
    for (int j = 0; j < grid_size_; j++) {
      (n_rays)++;
      Vec2r rc = Vec2r{(i*grid_slen)/len_, (j*grid_slen)/len_};
      Vec3r point_scorner = give_coordinates(center_, u_dir_, v_dir, rc, len_);

      Real random_pu = (((Real)rand())/((Real)RAND_MAX))*grid_slen;
      Real random_pv = (((Real)rand())/((Real)RAND_MAX))*grid_slen;

      Vec3r random_p = point_scorner + random_pu*u_dir_ + random_pv*v_dir;

      Vec3r dir = random_p - hit_position;


      Real cos_theta = (hit_normal.dot(dir))/((dir.norm())*(hit_normal.norm()));
      Real cos_alpha = (normal_.dot(-dir))/((dir.norm())*(normal_.norm()));
      Real r = dir.norm();

      if ((cos_theta >= 0) && (cos_alpha >= 0)) {

      Ray ray_temp(hit_position, dir);

      HitRecord shadow_record;

      update_shadow_hit();
      if (scene->Hit(ray_temp, kEpsilon, 1, shadow_record)) {
        update_shadow_hit();
        continue;
      }
      update_shadow_hit();

      Vec3r light_vec = dir/dir.norm();
     // compute how much the material absorts light
      Vec3r attenuation = phong_material->Evaluate(hit_record, light_vec,
                                                      view_vec);
      Vec3r ret_add = (color_*((cos_theta*cos_alpha)/(r*r)));
      Vec3r ret_add_v2 = ret_add.cwiseProduct(attenuation);

      ret = ret + ret_add_v2;

      }
    }
  }
  if (n_rays > 0){
    Vec3r final_ret = ret * (len_*len_/n_rays);
  
    return final_ret;
  }
  else
    return ret;
  
}


}  // namespace core
}  // namespace olio
