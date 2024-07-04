//! \file       vector.cc
//! \brief      Vector3 class
//! \author     Hadi Fadaifard, 2022

#include "core/vector.h"
#include <cassert>
#include <cmath>

namespace olio {

using namespace std;

Vector3::Vector3() :
  data_{0, 0, 0}
{
  //cout << "Vector3 is constructed" << endl;
}


Vector3::Vector3(double x, double y, double z) :
  data_{x, y, z}
{
  //cout << "Vector3 data is stored" << endl;
}


double Vector3::X() const
{
  // ======================================================================
  // *** Homework: Function should return first element of data_
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return data_[0];  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


double Vector3::Y() const
{
  // ======================================================================
  // *** Homework: Function should return second element of data_
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return data_[1];  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}

double Vector3::Z() const
{
  // ======================================================================
  // *** Homework: Function should return third element of data_
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return data_[2];  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


void Vector3::SetX(double x)
{
  // ======================================================================
  // *** Homework: Function should set value of first element of data_ to x
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  data_[0] = x;
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


void Vector3::SetY(double y)
{
  // ======================================================================
  // *** Homework: Function should set value of first element of data_ to y
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  data_[1] = y;
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


void Vector3::SetZ(double z)
{
  // ======================================================================
  // *** Homework: Function should set value of first element of data_ to z
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  data_[2] = z;
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


double Vector3::operator[](uint i) const
{
  assert(i < 3);

  // ======================================================================
  // *** Homework: Function should return i'th element of data_
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return data_[i];  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


double& Vector3::operator[](uint i)
{
  assert(i < 3);

  // ======================================================================
  // *** Homework: Function should return i'th element of data_
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  double *r = &(data_[i]);
  return *r;  //<! delete this line before adding your own code!
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


double Vector3::SquaredNorm() const
{
  // ======================================================================
  // *** Homework: Function should compute and return the squared norm
  // *** of the vector
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return pow(data_[0],2) + pow(data_[1],2) + pow(data_[2],2);  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


double Vector3::Norm() const
{
  // ======================================================================
  // *** Homework: Function should compute and return the norm (length)
  // *** of the vector
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return sqrt(pow(data_[0],2) + pow(data_[1],2) + pow(data_[2],2));  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


double Vector3::Dot(const Vector3 &vec) const
{
  // ======================================================================
  // *** Homework: Function should compute and return the dot product
  // *** of this vector with the input vector (vec)
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return (data_[0]*vec[0] + data_[1]*vec[1] + data_[2]*vec[2]);  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


Vector3 Vector3::Cross(const Vector3 &vec) const
{
  // ======================================================================
  // *** Homework: Function should compute and return the cross product
  // *** of this vector with the input vector (vec)
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{(data_[1]*vec[2] - data_[2]*vec[1]), (data_[2]*vec[0] - data_[0]*vec[2]), (data_[0]*vec[1]-data_[1]*vec[0])};  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


Vector3 Vector3::operator-() const
{
  // ======================================================================
  // *** Homework: Function should compute and return the negated
  // version of the vector
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{-data_[0], -data_[1], -data_[2]};
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


Vector3 Vector3::operator*(double scale) const
{
  // ======================================================================
  // *** Homework: Function should compute and return the product of
  // *** the vector with the input scalar (scale)
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{data_[0]*scale, data_[1]*scale, data_[2]*scale};  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


Vector3 Vector3::operator/(double scale) const
{
  // ======================================================================
  // *** Homework: Function should return the result of dividing the
  // *** vector by the input scalar
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{data_[0]/scale, data_[1]/scale, data_[2]/scale};  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


Vector3 Vector3::operator+(const Vector3 &other) const
{
  // ======================================================================
  // *** Homework: Function should return the result of adding the
  // *** vector to the input vector (other)
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{data_[0]+other[0], data_[1]+other[1], data_[2]+other[2]};  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


Vector3 Vector3::operator-(const Vector3 &other) const
{
  // ======================================================================
  // *** Homework: Function should return the result of subtracting
  // *** the input vector (other) from this vector
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{data_[0]-other[0], data_[1]-other[1], data_[2]-other[2]};  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


bool Vector3::IsApprox(const Vector3 &vec, double precision) const
{
  // ======================================================================
  // *** Homework: Function should return true if the input vector
  // *** (vec) is approximately equal to this vector. See the
  // *** documentation of the function in the header file for what it
  // *** means for two vectors to be approximately equal.
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  //! equal within precision prec if:
  //!       Norm(v - w) <= prec * min(Norm(v), Norm(w))

  Vector3 subtr(data_[0]-vec[0], data_[1]-vec[1], data_[2]-vec[2]);
  
  if(subtr.Norm() <= precision * min(Norm(),vec.Norm()))
  {
    return true;
  }
  else
  {
    return false;  //!< remove this line and add your own code
  }
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


// ----------------------------------------------------------------------
// friend functions
Vector3 operator*(double scale, const Vector3 &vec)
{
  // ======================================================================
  // *** Homework: Function should compute and return the product of
  // *** the vector with the input scalar (scale)
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  return Vector3{vec[0]*scale, vec[1]*scale, vec[2]*scale};  
  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


ostream& operator<<(ostream &stream, const Vector3 &v)
{
  stream << "[" << v.data_[0] << ", " << v.data_[1] << ", "
         << v.data_[2] << "]";
  return stream;
}

}  // namespace olio
