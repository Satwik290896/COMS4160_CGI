// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       camera.cc
//! \brief      Camera class
//! \author     Hadi Fadaifard, 2022

#include "core/camera/camera.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace olio {
namespace core {

using namespace std;

Camera::Camera(const std::string &name) :
  Node{name}
{
  name_ = name.size() ? name : "Camera";
}


Camera::Camera(const Vec3r &eye, const Vec3r &target, const Vec3r &up,
               Real fovy, Real aspect, const std::string &name) :
  Node{name},
  fovy_{fovy},
  aspect_{aspect}
{
  name_ = name.size() ? name : "Camera";
  LookAt(eye, target, up, true);
}


void
Camera::LookAt(const Vec3r &eye, const Vec3r &target, const Vec3r &up,
               bool update_viewport)
{
  eye_ = eye;
  target_ = target;
  up_ = up;
  Vec3r w = Vec3r{eye - target}.normalized();
  Vec3r u = up.cross(w).normalized();
  Vec3r v = w.cross(u);
  camera_xform_ = Mat4r::Identity();
  camera_xform_.col(0).head<3>() = u;
  camera_xform_.col(1).head<3>() = v;
  camera_xform_.col(2).head<3>() = w;
  camera_xform_.col(3).head<3>() = eye;

  if (update_viewport)
    UpdateViewport();
}


void
Camera::SetFovy(Real fovy, bool update_viewport)
{
  fovy_ = fovy;
  if (update_viewport)
    UpdateViewport();
}


void
Camera::SetAspect(Real aspect, bool update_viewport)
{
  aspect_ = aspect;
  if (update_viewport)
    UpdateViewport();
}


void
Camera::UpdateViewport()
{
  Mat4r l2w_xform{camera_xform_};

  // compute viewport dimensions from fovy and aspect ratio;
  // focal length is always 1
  auto fovy2 = fovy_ * kDEGtoRAD * 0.5;
  auto height = 2.0 * tan(fovy2);

  // get camera's u, v, w axes
  const Vec3r &u = l2w_xform.col(0).head<3>();
  const Vec3r &v = l2w_xform.col(1).head<3>();
  const Vec3r &w = l2w_xform.col(2).head<3>();

  // update viewport parameters: center of projection, lower left
  // corner and horizontal/vertical axes
  cop_ = l2w_xform.col(3).head<3>();
  vertical_ = height * v;
  horizontal_ = height * aspect_ * u;
  lower_left_corner_ = cop_ - w - 0.5 * (horizontal_ + vertical_);
}

}  // namespace core
}  // namespace olio
