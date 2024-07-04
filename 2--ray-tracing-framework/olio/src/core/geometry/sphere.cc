// ======================================================================
// Olio: Simple renderer
//
// ======================================================================

//! \file       sphere.cc
//! \brief      Sphere class
#include <iostream>
#include "core/geometry/sphere.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"

namespace olio {
namespace core {

using namespace std;
// ======================================================================
// *** Homework: implement all of Sphere class' member functions here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  Sphere::Sphere(const std::string &name) :
    Surface{name}
  {
      name_ = !name.size() ? "Sphere" : name;
  }

  //! \brief Constructor
  //! \param[in] center Sphere position
  //! \param[in] radius Sphere radius
  //! \param[in] name Node name
  Sphere::Sphere(const Vec3r &center, Real radius, const std::string &name) :
    Surface{name}, center_{center}, radius_{radius}
  {
    name_ = !name.size() ? "Sphere" : name;
  }

  //! \brief Check if ray intersects with surface
  //! \details If the ray intersections the surface, the function
  //!          should fill in the 'hit_record' (i.e., information
  //!          about the hit point, normal, etc.)
  //! \param[in] ray Ray to check intersection against
  //! \param[in] tmin Minimum value for acceptable t (ray fractional distance)
  //! \param[in] tmax Maximum value for acceptable t (ray fractional distance)
  //! \param[out] hit_record Resulting hit record if ray intersected with surface
  //! \return True if ray intersected with surface
  bool Sphere::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
  {
    Vec3r Pr = ray.GetOrigin();
    Vec3r dr = ray.GetDirection();
    Vec3r O = center_;
    double DrDotPrO = dr.dot((Pr-O));
    double DrDotDr = dr.dot(dr);
    double PrODotPrO = (Pr-O).dot((Pr-O));

    double Det = pow(DrDotPrO,2) - DrDotDr*(PrODotPrO - pow(radius_,2));
    bool toCheck = false;
    double t1 = 0;
    double t2 = 0;
    double Maint = 0;
    bool t1btwn = false;
    bool t2btwn = false;

    if(Det >= 0)
    {
      toCheck = true;
    }
    else {
      //cout << "Returning Sphere_Hit Det" << endl;
      //cout << "\nIt is a success\n" << endl;
      return false;
    }

    if(toCheck == true)
    {
      t1 = (-DrDotPrO + Det)/DrDotDr;
      if(t1 >= tmin && t1 <= tmax){
        t1btwn = true;
      }

      t2 = (-DrDotPrO - Det)/DrDotDr;
      if(t2 >= tmin && t2 <= tmax){
        t2btwn = true;
      }
    }

    if(t1btwn == true){
      Maint = t1;
    }
    else if(t2btwn == true){
      Maint = t2;
    }
    else{
      return false;
    }
    /*if(t1btwn == true && t2btwn == true)
    {
      if (t1 >= 0 && t2 >= 0) {
        if(t1 <= t2){
          Maint = t1;
        }
        else {
          Maint = t2;
        }
      }
      else if (t1 >= 0 && t2 <= 0) {
        
      }  
      else if (t1 <= 0 && t2 >= 0) {
        
      }  
      else if (t1 <= 0 && t2 <= 0) {
        if(t1 <= t2){
          Maint = t2;
        }
        else {
          Maint = t1;
        }
      }
    }
    & t2btwn == false) {
      Maint else if(t1btwn == true &= t1;
    }
    else if(t1btwn == false && t2btwn == true) {
      Maint = t2;
    }
    else if(t1btwn == false && t2btwn == false) {
      //cout << "Returning Sphere_Hit both false" << endl;
      //cout << "\nIt is a success\n" << endl;
      return false;
    }*/

    /*Update HitRecord*/
    Vec3r P = Pr + Maint * dr;
    Vec3r normal = (P - O)/(P-O).norm();
    hit_record.SetRayT(Maint);
    hit_record.SetPoint(ray.At(Maint));
    hit_record.SetNormal(ray, normal);
    hit_record.SetSurface(GetPtr());
    //hit_record = HitRecord(ray, Maint, P, normal);

    //cout << "Returning true Sphere_Hit" << endl;
    //cout << "\nIt is a success\n" << endl;
    return true;
  }

  //! \brief Set sphere position
  //! \param[in] center Sphere center/position
  void Sphere::SetCenter(const Vec3r &center)
  {
    center_ = center;
  }

  //! \brief Set sphere radius
  //! \param[in] radius Sphere radius
  void Sphere::SetRadius(Real radius)
  {
    radius_ = radius;
  }

  //! \brief Get sphere position
  //! \return Sphere position
  Vec3r Sphere::GetCenter() const
  {
    return center_;
  }

  //! \brief Get sphere radius
  //! \return Sphere radius
  Real Sphere::GetRadius() const
  {
    return radius_;
  }

}  // namespace core
}  // namespace olio
