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
#include <algorithm>

namespace olio {
namespace core {

using namespace std;

Light::Light(const std::string &name) :
  Node{name}
{
  name_ = name.size() ? name : "Light";
}


Vec3r
Light::Illuminate(const HitRecord &/*hit_record*/, const Vec3r &/*view_vec*/) const
{
  return Vec3r{0, 0, 0};
}


// ======================================================================
// *** Homework: Implement PointLight constructors and member
// *** functions here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

PointLight::PointLight(const Vec3r &position, const Vec3r &intensity, const std::string &name) :
  Light{}
{
  name_ = name.size() ? name : "PointLight";
  position_ = position;
  intensity_ = intensity;
}

Vec3r PointLight::Illuminate(const HitRecord &hit_record, const Vec3r &view_vec) const
{
  Vec3r hit_point = hit_record.GetPoint();
  Vec3r l = position_ - hit_point;
  double distance = l.norm();
  Vec3r n = hit_record.GetNormal();

  Vec3r l_cap = l/l.norm();
  Vec3r n_cap = n/n.norm();
  double Max2 = l_cap.dot(n_cap);
  Vec3r Irradiance{0,0,0};
  
  if (Max2 > 0) { 
    Irradiance = (intensity_) * (Max2/pow(l.norm(),2));
  }

  Vec3r reached_Ir{0,0,0};
  
  if (hit_record.GetSurface())
  {
  PhongMaterial::Ptr mat = dynamic_pointer_cast <PhongMaterial> (hit_record.GetSurface()->GetMaterial());

  if (mat != NULL)
  {
  Vec3r attenuation = mat->Evaluate(hit_record, l_cap, view_vec);

  reached_Ir[0] =  Irradiance[0] * attenuation[0];
  reached_Ir[1] =  Irradiance[1] * attenuation[1];
  reached_Ir[2] =  Irradiance[2] * attenuation[2];
  }
  }
  return reached_Ir;
}

// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

// ======================================================================
// *** Homework: Implement AmbientLight constructors and member
// *** functions here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

AmbientLight::AmbientLight(const Vec3r &ambient, const std::string &name) :
  Light{}
{
  name_ = name.size() ? name : "AmbientLight";
  ambient_ = ambient;
}

Vec3r AmbientLight::Illuminate(const HitRecord &hit_record, const Vec3r &view_vec) const
{
  Vec3r reached_Ir{0,0,0};

  if (hit_record.GetSurface())
  {
  PhongMaterial::Ptr mat = dynamic_pointer_cast <PhongMaterial> (hit_record.GetSurface()->GetMaterial());
    if (mat != NULL)
    {
    Vec3r ambi_coeff = mat->GetAmbient();
    reached_Ir[0] =  ambient_[0] * ambi_coeff[0];
    reached_Ir[1] =  ambient_[1] * ambi_coeff[1];
    reached_Ir[2] =  ambient_[2] * ambi_coeff[2];
    }
  }
  return reached_Ir;
}

// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

}  // namespace core
}  // namespace olio
