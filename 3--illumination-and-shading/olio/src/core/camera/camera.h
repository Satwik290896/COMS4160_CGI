// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       camera.h
//! \brief      Camera class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <string>
#include "core/types.h"
#include "core/ray.h"
#include "core/node.h"

namespace olio {
namespace core {

//! \class Camera
//! \brief Camera node
class Camera : public Node {
public:
  OLIO_NODE(Camera)

  //! \brief Constructor
  //! \param[in] name Node name
  explicit Camera(const std::string &name=std::string());

  //! \brief Constructor
  //! \details Set up the camera using the input (LookAt) parameters.
  //! \param[in] eye Eye (center of projection) position
  //! \param[in] target Target/reference position for camera to look at
  //! \param[in] up The up vector
  //! \param[in] fovy y field of view in degrees
  //! \param[in] aspect viewport aspect ratio
  //! \param[in] name Node name
  Camera(const Vec3r &eye, const Vec3r &target, const Vec3r &up,
         Real fovy, Real aspect, const std::string &name=std::string());

  //! \brief Set up camera using the provided LookAt parameters
  //! \param[in] eye Eye (center of projection) position
  //! \param[in] target Target/reference position for camera to look at
  //! \param[in] up The up vector
  //! \param[in] update_viewport Whether to also compute/update the
  //!            viewport parameters
  void LookAt(const Vec3r &eye, const Vec3r &target, const Vec3r &up,
              bool update_viewport=true);

  //! \brief Generate ray passing through point (s, t) in the viewport. s
  //!        and t should be in [0, 1].
  //! \param[in] s horizontal coordinate in [0, 1]
  //! \param[in] t vertical coordinate in [0, 1]
  //! \return Created ray for point (s, t)
  inline Ray GetRay(Real s, Real t) const {
    return Ray{cop_,
               lower_left_corner_ + s * horizontal_ + t * vertical_ - cop_};
  }

  //! \brief Set vertical field of view
  //! \param[in] fovy Vertical field of view in degrees
  //! \param[in] update_viewport Whether to update viewport parameters
  void SetFovy(Real fovy, bool update_viewport=true);

  //! \brief Set viewport aspect ratio
  //! \param[in] aspect viewport aspect ratio
  //! \param[in] update_viewport Whether to update viewport parameters
  void SetAspect(Real aspect, bool update_viewport=true);

  //! \brief Get eye (cop) position
  //! \return Eye position
  Vec3r GetEye() const {return eye_;}

  //! \brief Get target/reference position
  //! \return Target position
  Vec3r GetTarget() const {return target_;}

  //! \brief Get up vector
  //! \return Up vector
  Vec3r GetUpVector() const {return up_;}

  //! \brief Get camera xform, which describes the eye position and
  //!        the u, v, w axes
  //! \return 4x4 camera matrix
  Mat4r GetCameraXform() const {return camera_xform_;}

  //! \brief Get the vertical field of view
  //! \return Vertical field of view in degrees
  Real GetFovy() const {return fovy_;}

  //! \brief Get the viewport aspect ratio
  //! \return Viewport aspect ratio
  Real GetAspectRatio() const {return aspect_;}

  //! \brief Update viewport parameters using current values of
  //!        camera_xform_, fovy_, and aspect_
  //! \details Parameters that will be updated by the function:
  //!    * cop_: center of projection (which should be the same as eye_)
  //!    * horizontal_: viewport's horizontal axis
  //!    * vertical_: viewport's vertical axis
  //!    * lower_left_corner_: lower left corner of the viewport
  void UpdateViewport();
protected:
  Vec3r eye_{0, 0, 0};    //!< eye location
  Vec3r target_{0, 0, 0}; //!< target location
  Vec3r up_{0, 1, 0};     //!< up vector

  //! \brief Camera axes representing local to to world transformation
  //! for the camera. The first three columns represent camera's x, y,
  //! and z unit axes (u, v, w); the top-left 3x3 sub-matrix
  //! represents the camera rotation. The last column represents the
  //! position of the center of projection (cop).
  Mat4r camera_xform_{Mat4r::Identity()};
  Real fovy_{60};         //!< vertical field of view in degrees
  Real aspect_{1.77778f};  //!< viewport aspect ratio

  // viewport related params
  Vec3r cop_{0, 0, 0};  //!< center of projection (eye position)

  //! \brief 3D coordinates of lower left point in viewport
  Vec3r lower_left_corner_{-1.0264f, -0.5774f, -1.0f};

  //! \brief viewport horizontal axis (with length equal to viewport width)
  Vec3r horizontal_{2.0528f, 0.0f, 0.0f};

  //! \brief viewport vertical axis (with length equal to viewport height)
  Vec3r vertical_{0.0f, 1.1547f, 0.0f};
private:
};

}  // namespace core
}  // namespace olio
