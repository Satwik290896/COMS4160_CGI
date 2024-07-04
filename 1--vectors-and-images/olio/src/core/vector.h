//! \file       vector.h
//! \brief      Vector3 class
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <iostream>
#include "core/types.h"

namespace olio {

//! \class Vector3
//! \brief Vector3 class
class Vector3 {
public:
  //! \brief Default constructor
  Vector3();

  //! \brief Parameterized constructor for setting initial x, y, z values
  //! \param[in] x Vector's x coordinates
  //! \param[in] y Vector's y coordinates
  //! \param[in] z Vector's z coordinates
  Vector3(double x, double y, double z);

  //! \brief Get X coordinate
  //! \return X coordinate
  double X() const;

  //! \brief Get Y coordinate
  //! \return Y coordinate
  double Y() const;

  //! \brief Get Z coordinate
  //! \return Z coordinate
  double Z() const;

  //! \brief Set X coordinate
  //! \param[in] x X coordinate value to set
  void SetX(double x);

  //! \brief Set Y coordinate
  //! \param[in] y Y coordinate value to set
  void SetY(double y);

  //! \brief Set Z coordinate
  //! \param[in] z Z coordinate value to set
  void SetZ(double z);

  //! \brief Get i'th coordinate value; 0: x, 1: y, 2: z.
  //! \return i'th coordinate value
  double operator[](uint i) const;

  //! \brief Get i'th coordinate value by reference; 0: x, 1: y, 2: z.
  //! \return i'th coordinate value
  double& operator[](uint i);

  //! \brief Compute and return the square of vector's norm (length)
  //! \return Square of vector's norm
  double SquaredNorm() const;

  //! \brief Compute and return vector's norm (length)
  //! \return Vector norm
  double Norm() const;

  //! \brief Compute the dot product of this vector with input vector
  //! \param[in] vec The other vector to compute the dot product with
  //! \return Dot product of this vector with vec
  double Dot(const Vector3 &vec) const;

  //! \brief Compute the cross product of this vector with input vector
  //! \param[in] vec The other vector to compute the cross product with
  //! \return Cross product of this vector with vec (*this x vec)
  Vector3 Cross(const Vector3 &vec) const;

  //! \brief Multiply the vector by a scalar value and return result
  //! \param[in] scale Scalar value to multiply vector by
  //! \return The result of multiplyig the vector by input scalar
  Vector3 operator*(double scale) const;

  //! \brief Divide the vector by a scalar value and return result
  //! \param[in] scale Scalar value to divide vector by
  //! \return The result of dividing the vector by input scalar
  Vector3 operator/(double scale) const;

  //! \brief Negation operator
  //! \return Negated version of the vector
  Vector3 operator-() const;

  //! \brief Addition operator, which will add the input vector to this vector
  //! \param[in] vec The other vector
  //! \return Resulting vector
  Vector3 operator+(const Vector3 &other) const;

  //! \brief Subtraction operator, which will add the negated version
  //! of the input vector to this vector.
  //! \param[in] vec The other vector
  //! \return Resulting vector
  Vector3 operator-(const Vector3 &other) const;

  //! \brief Check if this vector is approximately equal to input
  //! vector within the given precision
  //! \details Two vectors v and w are considered to be approximately
  //! equal within precision prec if:
  //!       Norm(v - w) <= prec * min(Norm(v), Norm(w))
  //! \param[in] vec Input vector to compare to
  //! \param[in] precision Precision
  //! \return true if the vector is approximately equal to input vector
  bool IsApprox(const Vector3 &vec, double precision) const;

  //! \brief Declare this scaling binary operator as friend, so that
  //! the function can access this class' protected and private data
  //! members
  friend Vector3 operator*(double scale, const Vector3 &vec);

  //! \brief Declare this << (insertion) operator as a friend
  //! function, so that the function can access this class' protected
  //! and private data members
  friend std::ostream& operator<<(std::ostream &stream, const Vector3 &v);
protected:
  double data_[3];
};

//! \param[in] scale Scale factor
//! \param[in] vec Vector to scale
//! \return The resulting vector
Vector3 operator*(double scale, const Vector3 &vec);

//! \brief Insertion operator allowing Vector3 to be written to an
//! output stream; e.g., std::cout.
//! \param[in] stream Output stream to write to
//! \param[in] v vector to write
//! \return Output stream that was written to
std::ostream& operator<<(std::ostream &stream, const Vector3 &v);

}  // namespace olio
