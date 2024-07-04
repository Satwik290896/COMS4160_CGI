// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       ray.h
//! \brief      Ray class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include "core/types.h"
#include "face_geouv.h"

namespace olio {
namespace core {

class Surface;

//! \class Ray
//! \brief Ray class used during path tracing
class Ray {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  //! \brief Constructor
  //! \param[in] origin Ray origin
  //! \param[in] dir Ray direction
  Ray(const Vec3r &origin, const Vec3r &dir);

  //! \brief Default constructor
  Ray() = default;

  //! \brief Set ray origin
  //! \param[in] origin Ray origin
  inline void SetOrigin(const Vec3r &origin) {origin_ = origin;}

  //! \brief Set ray direction
  //! \param[in] dir Ray direction
  inline void SetDirection(const Vec3r &dir) {dir_ = dir;}

  //! \brief Get ray origin
  //! \return Ray origin
  inline Vec3r GetOrigin() const {return origin_;}

  //! \brief Get ray direction
  //! \return Ray direction
  inline Vec3r GetDirection() const {return dir_;}

  //! \brief Evaluate ray at fractional distance t
  //! \param[in] t Fractional distance t to evaluate ray at
  //! \return Point along ray at t
  inline Vec3r At(Real t) const {return origin_ + t * dir_;}

  //! \brief Reflect incoming ray
  //! \details Assumes ray dir is pointing towards surface. The normal
  //!          vector is assumed to be unit length.
  //! \param[in] ray_in Incoming ray
  //! \param[in] point Point to reflect around
  //! \param[in] normal Unit normal direction
  //! \return Reflected ray
  static Ray Reflect(const Ray &ray_in, const Vec3r &point,const Vec3r &normal);

  //! \brief Refract incoming ray
  //! \details Assumes ray dir is pointing towards surface. The normal
  //!          vector is assumed to be unit length.
  //! \param[in] ray_in Incoming ray
  //! \param[in] point Point to refract around
  //! \param[in] normal Unit normal direction
  //! \param[in] ior_in incoming material IOR
  //! \param[in] ior_out outgoing material IOR
  //! \return Refracted ray
  static Ray Refract(const Ray& ray_in, const Vec3r &point, const Vec3r& normal,
                     Real ior_in, Real ior_out);

  //! \brief Refract incoming ray
  //! \details Assumes ray dir is pointing towards surface. The normal
  //!          vector is assumed to be unit length.
  //! \param[in] ray_in Incoming ray
  //! \param[in] point Point to refract around
  //! \param[in] normal Unit normal direction
  //! \param[in] ior_ratio Ratio of incoming material IOR to outgoing IOR
  //! \return Refracted ray
  static Ray Refract(const Ray& ray_in, const Vec3r &point, const Vec3r& normal,
                     Real ior_ratio);
protected:
  Vec3r origin_{0, 0, 0};  //!< Ray origin
  Vec3r dir_{0, 0, 0};     //!< Ray direction
};


//! \class HitRecord
//! \brief HitRecord class is used during path tracing to keep track
//! of information about the point on a surface that was hit by a ray.
//! \details Information such as hit poisition, surface normal at
//! that position, whether the normal was facing towards or away
//! from the ray, the pointer to the surface that was hit, etc.
class HitRecord {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  //! \brief Constructor
  //! \param[in] ray Ray that intersected the surface and generated
  //!            the hit record
  //! \param[in] ray_t Ray's hit t
  //! \param[in] point Hit point on the surface
  //! \param[in] point Surface normal at hit point (should always be
  //!            facing towards fron ray)
  HitRecord(const Ray &ray, Real ray_t, const Vec3r &point,
            const Vec3r &face_normal);

  //! \brief Default constructor
  HitRecord() = default;

  //! \brief Set ray's t (fractional distance) at hit time
  //! \param[in] ray_t Ray's t
  inline void SetRayT(Real ray_t) {ray_t_ = ray_t;}

  //! \brief Set hit point position
  //! \param[in] point Hit position
  inline void SetPoint(const Vec3r &point) {point_ = point;}

  //! \brief Set surface normal at hit position
  //! \details The function should store the face_normal in its
  //! 'normal_' data member. However, it should also adjust the
  //! orientation of the stored normal to guarantee that the normal is
  //! pointing towards the ray (the dot product is negative). If the
  //! original normal was facing towards the ray, 'front_face_' should
  //! be set to true. If the original normal was point away (the dot
  //! product is positive), 'front_face_' should be set to false.
  //! \param[in] ray Ray that hit the surface
  //! \param[in] face_normal Surface normal at hit position
  inline void SetNormal(const Ray &ray, const Vec3r &face_normal) {
    front_face_ = ray.GetDirection().dot(face_normal) < 0;
    if (front_face_)
      normal_ = face_normal;
    else
      normal_ = -face_normal;
  }

  //! \brief Set surface normal at hit position.
  //! \details See the details in the other (overloaded) SetNormals.
  //! \param[in] face_normal Surface normal at hit point
  //! \param[in] front_face whether the hit point was front or back facing
  inline void SetNormal(const Vec3r &face_normal, bool front_face) {
    normal_ = face_normal;
    front_face_ = front_face;
  }

  //! \brief Set surface that was hit
  //! \param[in] surface Pointer to surface that was hit
  inline void SetSurface(std::shared_ptr<Surface> surface) {surface_ = surface;}

  //! \brief Get ray's fractional distance
  //! \return Ray's fractional distance
  inline Real GetRayT() const {return ray_t_;}

  //! \brief Get hit position
  //! \return Hit position
  inline Vec3r GetPoint() const {return point_;}

  //! \brief Get surface normal at hit point
  //! \return surface normal
  inline Vec3r GetNormal() const {return normal_;}

  //! \brief Return whether the hit point was front or back facing
  //! \return Whether the hit point was front or back facing
  inline bool IsFrontFace() const {return front_face_;}

  //! \brief Get hit surface
  //! \return Hit surface
  inline std::shared_ptr<Surface> GetSurface() const {return surface_;}


  inline void SetFaceGeouv(FaceGeoUV &face_geouv) {face_geouv_ = face_geouv;}

  inline FaceGeoUV GetFaceGeouv() const {return face_geouv_;}

protected:
  Real ray_t_{0}; //!< fractional distance along ray (t) that intersects surface
  Vec3r point_{0, 0, 0};   //!< hit point
  Vec3r normal_{0, 0, 0};  //!< surface normal at hit point
  bool front_face_{true};  //!< whether hit point was front or back facing
  std::shared_ptr<Surface> surface_;  //!< pointer to the hit surface
  FaceGeoUV face_geouv_;  //!< track uv coordinates of intersected point
};


inline Ray
Ray::Reflect(const Ray &ray_in, const Vec3r &point, const Vec3r &normal)
{
  const Vec3r &dir = ray_in.GetDirection();
  return Ray{point, dir - 2 * normal.dot(dir) * normal};
}


inline Ray
Ray::Refract(const Ray& ray_in, const Vec3r &point, const Vec3r& normal,
             Real ior_in, Real ior_out)
{
  const Vec3r &v = ray_in.GetDirection().normalized();
  auto cos_theta = fmin(normal.dot(-v), 1.0f);
  Vec3r r_out_perp =  (ior_in / ior_out) * (v + cos_theta * normal);
  Vec3r r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.squaredNorm())) * normal;
  return Ray{point, r_out_perp + r_out_parallel};
}


inline Ray
Ray::Refract(const Ray& ray_in, const Vec3r &point, const Vec3r& normal,
             Real ior_ratio)
{
  const Vec3r &v = ray_in.GetDirection().normalized();
  auto cos_theta = fmin(normal.dot(-v), 1.0f);
  Vec3r r_out_perp =  ior_ratio * (v + cos_theta * normal);
  Vec3r r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.squaredNorm())) * normal;
  return Ray{point, r_out_perp + r_out_parallel};
}

}  // namespace core
}  // namespace olio
