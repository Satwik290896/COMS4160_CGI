// ======================================================================
// Olio: Simple renderer
//
// ======================================================================

//! \file       camera.cc
//! \brief      Camera class

#include "core/camera/camera.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <cmath>

namespace olio {
namespace core {

using namespace std;

// ======================================================================
// *** Homework: implement Camera class' member functions here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

  Camera::Camera(const std::string &name) :
    Node{name}
  {
    name_ = !name.size() ? "Camera" : name;
  }

  //! \brief Constructor
  //! \details Set up the camera using the input (LookAt) parameters.
  //! \param[in] eye Eye (center of projection) position
  //! \param[in] target Target/reference position for camera to look at
  //! \param[in] up The up vector
  //! \param[in] fovy y field of view in degrees
  //! \param[in] aspect viewport aspect ratio
  //! \param[in] name Node name
  Camera::Camera(const Vec3r &eye, const Vec3r &target, const Vec3r &up,
	 Real fovy, Real aspect, const std::string &name) :
         Node{name}, eye_{eye}, target_{target}, up_{up}, fovy_{fovy}, aspect_{aspect}
    {
        name_ = !name.size() ? "Camera" : name;
        LookAt(eye, target, up, true);
    }

  //! \brief Set up camera using the provided LookAt parameters
  //! \param[in] eye Eye (center of projection) position
  //! \param[in] target Target/reference position for camera to look at
  //! \param[in] up The up vector
  //! \param[in] update_viewport Whether to also compute/update the
  //!            viewport parameters
  void Camera::LookAt(const Vec3r &eye, const Vec3r &target, const Vec3r &up, bool update_viewport)
    {
        eye_ = eye;
        target_ = target;
        up_ = up; 

        Vec3r ec = (eye_-target_);
        Vec3r w = ec/(ec.norm());
        Vec3r upCw = up_.cross(w);
        Vec3r u = upCw/upCw.norm();
        Vec3r wCu = w.cross(u);
        Vec3r v = wCu;


        /*upCw[0] = up_[1] * w[2] - up_[2] * w[1];
        upCw[1] = -(up_[0] * w[2] - up_[2] * w[0]);
        upCw[2] = up_[0] * w[1] - up_[1] * w[0];

        u = upCw/upCw.norm();

        wCu[0] = w[1] * u[2] - w[2] * u[1];
        wCu[1] = -(w[0] * u[2] - w[2] * u[0]);
        wCu[2] = w[0] * u[1] - w[1] * u[0];

        v = wCu;*/

        camera_xform_.col(0).head<3>() = u;
        camera_xform_.col(1).head<3>() = v;
        camera_xform_.col(2).head<3>() = w;
        camera_xform_.col(3).head<3>() = eye;

        if(update_viewport)
        {
          UpdateViewport(); 
        }
    }

  //! \brief Set vertical field of view
  //! \param[in] fovy Vertical field of view in degrees
  //! \param[in] update_viewport Whether to update viewport parameters
  void Camera::SetFovy(Real fovy, bool update_viewport)
  {
        fovy_ = fovy;
        if(update_viewport)
        {
          UpdateViewport(); 
        }
  }

  //! \brief Set viewport aspect ratio
  //! \param[in] aspect viewport aspect ratio
  //! \param[in] update_viewport Whether to update viewport parameters
  void Camera::SetAspect(Real aspect, bool update_viewport)
  {
        aspect_ = aspect;
        if(update_viewport)
        {
          UpdateViewport(); 
        }
  }

  //! \brief Get eye (cop) position
  //! \return Eye position
  Vec3r Camera::GetEye() const
  {
      return eye_;
  }

  //! \brief Get target/reference position
  //! \return Target position
  Vec3r Camera::GetTarget() const
  {
      return target_;
  }

  //! \brief Get up vector
  //! \return Up vector
  Vec3r Camera::GetUpVector() const
  {
      return up_;
  }

  //! \brief Get camera xform, which describes the eye position and
  //!        the u, v, w axes
  //! \return 4x4 camera matrix
  Mat4r Camera::GetCameraXform() const
  {
      return camera_xform_;
  }

  //! \brief Get the vertical field of view
  //! \return Vertical field of view in degrees
  Real Camera::GetFovy() const
  {
      return fovy_;
  }

  //! \brief Get the viewport aspect ratio
  //! \return Viewport aspect ratio
  Real Camera::GetAspectRatio() const
  {
      return aspect_;
  }

  //! \brief Update viewport parameters using current values of
  //!        camera_xform_, fovy_, and aspect_
  //! \details Parameters that will be updated by the function:
  //!    * horizontal_: viewport's horizontal axis
  //!    * vertical_: viewport's vertical axis
  //!    * lower_left_corner_: lower left corner of the viewport
  void Camera::UpdateViewport()
  {
  //! \brief viewport horizontal axis (with length equal to viewport width)

  //! \brief viewport vertical axis (with length equal to viewport height)
        Vec3r u = camera_xform_.col(0).head<3>();
        Vec3r v = camera_xform_.col(1).head<3>();
        Vec3r w = camera_xform_.col(2).head<3>();
        Vec3r eye = camera_xform_.col(3).head<3>();
        vertical_ = 2 * tan((fovy_/2)*kDEGtoRAD) * v;
        horizontal_ = 2 * tan((fovy_/2)*kDEGtoRAD) * aspect_ * u;
        lower_left_corner_ = (eye - w) - 0.5f*(vertical_ + horizontal_);
  }

}  // namespace core
}  // namespace olio
