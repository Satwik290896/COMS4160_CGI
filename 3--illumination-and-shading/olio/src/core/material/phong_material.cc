// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       phong_material.cc
//! \brief      PhongMaterial class
//! \author     Hadi Fadaifard, 2022

#include "core/material/phong_material.h"
#include <spdlog/spdlog.h>
#include "core/light/light.h"
#include <iostream>
#include <algorithm>

namespace olio {
namespace core {

using namespace std;

PhongMaterial::PhongMaterial(const std::string &name) :
  Material{}
{
  name_ = name.size() ? name : "PhongMaterial";
  SetDiffuse(Vec3r{0, 0, 0});
}


PhongMaterial::PhongMaterial(const Vec3r &ambient, const Vec3r &diffuse,
			     const Vec3r &specular, Real shininess,
			     const Vec3r &mirror, const std::string &name) :
  Material{},
  ambient_{ambient},
  specular_{specular},
  shininess_{shininess},
  mirror_{mirror}
{
  name_ = name.size() ? name : "PhongMaterial";
  SetDiffuse(diffuse);
}


void
PhongMaterial::SetDiffuse(const Vec3r &diffuse)
{
  diffuse_ = diffuse;
}


Vec3r
PhongMaterial::Evaluate(const HitRecord &hit_record, const Vec3r &light_vec,
			const Vec3r &view_vec) const
{
// ======================================================================
// *** Homework: Your code for PhongMaterial::Evaluate() goes here.
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

  Vec3r h_cap = (light_vec + view_vec)/(light_vec + view_vec).norm();
  Vec3r n_cap = hit_record.GetNormal();

  Vec3r attenuation{0,0,0};

  double Max2 = h_cap.dot(n_cap);

  if (Max2 > 0) { 
    attenuation = ( diffuse_ + ( specular_*pow(Max2, shininess_) ) );
  }
  else {
    attenuation = diffuse_;
  }

  return attenuation;
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}

}  // namespace core
}  // namespace olio
