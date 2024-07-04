// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       types.h
//! \brief      misc type definitions
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>
#include <limits>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

// define uchar, uint, etc
using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;

namespace olio {
namespace core {

// define our floating point precision: float or double
#if defined(OLIO_USE_SINGLE_PRECISION)
// single precision
typedef float Real;
// misc constants
static constexpr Real kEpsilon = 1e-4f;
static constexpr Real kEpsilon2 = 1e-6f;
static constexpr Real kPi = 3.14159265359f;
static constexpr Real k2Pi = 6.28318530718f;
static constexpr Real kPi2 = 9.86960440108935861906f;
static constexpr Real kDEGtoRAD = 0.017453292519944f;
static constexpr Real kRADtoDEG = 57.29577951307855f;
#else
// double precision
typedef double Real;
// misc constants
static constexpr Real kEpsilon = 1e-8;
static constexpr Real kEpsilon2 = 1e-14;
static constexpr Real kPi = 3.14159265359;
static constexpr Real k2Pi = 6.28318530718;
static constexpr Real kPi2 = 9.86960440108935861906;
static constexpr Real kDEGtoRAD = 0.017453292519944;
static constexpr Real kRADtoDEG = 57.29577951307855;
#endif
static constexpr Real kInfinity = std::numeric_limits<Real>::max();

// misc macros
#define CLAMP(A, L, H) (A > H ? H : (A < L ? L : A))

// vector types
using VecXr = Eigen::Matrix<Real, Eigen::Dynamic, 1>;
using Vec4r = Eigen::Matrix<Real, 4, 1>;
using Vec3r = Eigen::Matrix<Real, 3, 1>;
using Vec2r = Eigen::Matrix<Real, 2, 1>;
using VecXd = Eigen::VectorXd;
using VecXf = Eigen::VectorXf;
using VecXi = Eigen::VectorXi;
using Vec4d = Eigen::Vector4d;
using Vec3d = Eigen::Vector3d;
using Vec2d = Eigen::Vector2d;
using Vec4f = Eigen::Vector4f;
using Vec3f = Eigen::Vector3f;
using Vec2f = Eigen::Vector2f;
using Vec4i = Eigen::Vector4i;
using Vec3i = Eigen::Vector3i;
using Vec2i = Eigen::Vector2i;

// matrix types
using SpMatXr = Eigen::SparseMatrix<Real>;
using SpMatXi = Eigen::SparseMatrix<int>;
using MatXr = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
using Mat4r = Eigen::Matrix<Real, 4, 4>;
using Mat3r = Eigen::Matrix<Real, 3, 3>;
using Mat2r = Eigen::Matrix<Real, 2, 2>;
using MatXd = Eigen::MatrixXd;
using MatXf = Eigen::MatrixXf;
using MatXi = Eigen::MatrixXi;
using Mat4d = Eigen::Matrix4d;
using Mat3d = Eigen::Matrix3d;
using Mat2d = Eigen::Matrix2d;
using Mat4f = Eigen::Matrix4f;
using Mat3f = Eigen::Matrix3f;
using Mat2f = Eigen::Matrix2f;
using Mat4i = Eigen::Matrix4i;
using Mat3i = Eigen::Matrix3i;
using Mat2i = Eigen::Matrix2i;

//! \brief Transform a 3D point by 4x4 matrix (in homogeneous coordinates)
//! \param[in] xform transformation matrix
//! \param[in] point input point
//! \return Transformed point (3D)
inline Vec3r XformPoint(const Mat4r &xform, const Vec3r &point) {
  return (xform * point.homogeneous()).hnormalized();
}

//! \brief Transform a 3D vector by 4x4 matrix (in homogeneous coordinates)
//! \param[in] xform transformation matrix
//! \param[in] vector input vector
//! \return Transformed vector (3D)
inline Vec3r XformVector(const Mat4r &xform, const Vec3r &vector) {
  return (xform * Vec4r(vector[0], vector[1], vector[2], 0)).head<3>();
}

}  // namespace core
}  // namespace olio
