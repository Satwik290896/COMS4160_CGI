// ======================================================================
// Olio: Simple renderer
//
// ======================================================================

//! \file       ray.cc
//! \brief      Ray class

#include "core/ray.h"
#include <spdlog/spdlog.h>

namespace olio {
namespace core {

using namespace std;

// ======================================================================
// *** Homework: implement Ray class' parameterized constructor here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

  //! \brief Constructor
  //! \param[in] origin Ray origin
  //! \param[in] dir Ray direction
  Ray::Ray(const Vec3r &origin, const Vec3r &dir) : 
    origin_{origin[0],origin[1],origin[2]}, dir_{dir[0],dir[1],dir[2]}
  {

  }

// ======================================================================
// *** Homework: implement HitRecord class' parameterized constructor here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  HitRecord::HitRecord(const Ray &ray, Real ray_t, const Vec3r &point, const Vec3r &face_normal) :
    ray_t_{ray_t}, point_{point[0], point[1], point[2]}, normal_{face_normal[0], face_normal[1], face_normal[2]}
  {
    Vec3r dir = ray.GetDirection();

    if((dir).dot(normal_) < 0){
      front_face_ = true;
    }
    else{
      normal_ = -face_normal;
      front_face_ = false;
    }
  }

}  // namespace core
}  // namespace olio
