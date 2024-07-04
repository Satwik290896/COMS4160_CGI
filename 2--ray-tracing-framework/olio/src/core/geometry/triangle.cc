// ======================================================================
// Olio: Simple renderer
//
// ======================================================================

//! \file       triangle.cc
//! \brief      Triangle class

#include "core/geometry/triangle.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"
#include <iostream>

namespace olio {
namespace core {

using namespace std;
// ======================================================================
// *** Homework: implement all of Triangle class' member functions here
// ======================================================================
// ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
// ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  Triangle::Triangle(const std::string &name) :
    Surface{name}
  {
      name_ = !name.size() ? "Triangle" : name;
  }

  //! \brief Constructor
  //! \details The constructor should also compute the face/triangle
  //! normal by calling ComputeNormal(). Points should be ordered
  //! counterclockwise wrt triangle normal.
  //! \param[in] point0 first triangle point
  //! \param[in] point1 second triangle point
  //! \param[in] point2 third triangle point
  //! \param[in] name Node name
  Triangle::Triangle(const Vec3r &point0, const Vec3r &point1, const Vec3r &point2,
	   const std::string &name) :
        Surface{name}, point0_{point0}, point1_{point1}, point2_{point2}
    {
        name_ = !name.size() ? "Triangle" : name;
        ComputeNormal();
    }

  //! \brief Static function to compute ray-triangle intersection
  //!        between input ray and triangle defined by the three input
  //!        points.
  //! \param[in] p0 first triangle point
  //! \param[in] p1 second triangle point
  //! \param[in] p2 third triangle point
  //! \param[in] ray Input ray
  //! \param[in] tmin Minimum acceptable value for ray_t
  //! \param[in] tmax Maximum acceptable value for ray_t
  //! \param[out] In case of intersection, value of t for hit point p:
  //!             p = ray_origin + t * ray_dir
  //! \param[out] uv UV coordinates of the hit point inside the
  //!             triangle. Can be used to compute the barycentric
  //!             coordinates as: (1 - uv[0] - uv[1], uv[0], uv[1])
  //! \return True on success
  bool Triangle::RayTriangleHit(const Vec3r &p0, const Vec3r &p1, const Vec3r &p2,
			     const Ray &ray, Real tmin, Real tmax, Real &ray_t, Vec2r &uv)
    {
        Vec3r a = p0;
        Vec3r b = p1;
        Vec3r c = p2;

        Vec3r u = p1-p0;
        Vec3r v = p2-p0;
        Vec3r n = u.cross(v);
        Vec3r normal = n/n.norm();

        Vec3r dr = ray.GetDirection();
        Vec3r Pr = ray.GetOrigin();

        Vec3r Pra = Pr - p0;
        Mat31r Vm;
        Mat31r Rm;
        Vec3r result;

        Mat3r M = Mat3r::Identity();

        M.col(0).head<3>() = u;
        M.col(1).head<3>() = v;
        M.col(2).head<3>() = -dr;  
        Vm.col(0).head<3>() = Pra;

        if (M.determinant() != 0)
        {
            Rm = M.inverse() * Vm;
            //cout << "Rm: " << Rm[0] <<", " << Rm[1] << ", " << Rm[2] << endl; 
            result = Rm.col(0).head<3>();
            if(result[2] >= tmin && result[2] <= tmax)
            {
                Vec3r x = Pr + result[2] * dr;

                Vec3r ba = b - a;
                Vec3r xa = x - a;
                Vec3r cb = c - b;
                Vec3r xb = x - b;
                Vec3r ac = a - c;
                Vec3r xc = x - c;

                Vec3r baCxa = ba.cross(xa);
                Vec3r cbCxb = cb.cross(xb);
                Vec3r acCxc = ac.cross(xc);

                if((baCxa.dot(normal) <= 0) || (cbCxb.dot(normal) <= 0) || (acCxc.dot(normal) <= 0))
                {
                    /*Point doesn't lie inside the triangle*/
                    return false;
                }
                
                uv[0] = result[0];
                uv[1] = result[1];
                //ray.SetRayT(result[2]);
                ray_t = result[2];
            
            }
            else
            {
                return false;
            }
            return true;
        }
        else{
            return false;
        }
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
  bool Triangle::Hit(const Ray &ray, Real tmin, Real tmax,
	   HitRecord &hit_record)
  {
    Vec3r Pr = ray.GetOrigin();
    Vec3r dr = ray.GetDirection();
    Vec3r a = point0_;
    double nDotaPr = normal_.dot((a-Pr));
    double DrDotn = dr.dot(normal_);

    if (DrDotn == 0)
    {
        return false;
    }

    double Maint = nDotaPr/DrDotn;
    if(Maint < tmin || Maint > tmax)
    {
        return false;
    }

    Vec3r x = Pr + Maint * dr;

    a = point0_;
    Vec3r b = point1_;
    Vec3r c = point2_;

    Vec3r ba = b - a;
    Vec3r xa = x - a;
    Vec3r cb = c - b;
    Vec3r xb = x - b;
    Vec3r ac = a - c;
    Vec3r xc = x - c;

    Vec3r baCxa = ba.cross(xa);
    Vec3r cbCxb = cb.cross(xb);
    Vec3r acCxc = ac.cross(xc);

    /*baCxa[0] = ba[1] * xa[2] - ba[2] * xa[1];
    baCxa[1] = -(ba[0] * xa[2] - ba[2] * xa[0]);
    baCxa[2] = ba[0] * xa[1] - ba[1] * xa[0];

    cbCxb[0] = cb[1] * xb[2] - cb[2] * xb[1];
    cbCxb[1] = -(cb[0] * xb[2] - cb[2] * xb[0]);
    cbCxb[2] = cb[0] * xb[1] - cb[1] * xb[0];

    acCxc[0] = ac[1] * xc[2] - ac[2] * xc[1];
    acCxc[1] = -(ac[0] * xc[2] - ac[2] * xc[0]);
    acCxc[2] = ac[0] * xc[1] - ac[1] * xc[0];*/

    if((baCxa.dot(normal_) <= 0) || (cbCxb.dot(normal_) <= 0) || (acCxc.dot(normal_) <= 0))
    {
        /*Point doesn't lie inside the triangle*/
        return false;
    }
    hit_record.SetRayT(Maint);
    hit_record.SetPoint(ray.At(Maint));
    hit_record.SetNormal(ray, normal_);
    hit_record.SetSurface(GetPtr());
    //hit_record = HitRecord(ray, Maint, x, normal_);

    return true;
  }

  //! \brief Set triangle points
  //! \details The function should also compute/update the triangle
  //!          normal by calling 'ComputeNormal()'
  //! \param[in] point0 first triangle point
  //! \param[in] point1 second triangle point
  //! \param[in] point2 third triangle point
  void Triangle::SetPoints(const Vec3r &point0, const Vec3r &point1, const Vec3r &point2)
  {
      point0_ = point0;
      point1_ = point1;
      point2_ = point2;
      ComputeNormal();
  }

  //! \brief Get triangle points
  //! \param[out] point0 first triangle point
  //! \param[out] point1 second triangle point
  //! \param[out] point2 third triangle point
  void Triangle::GetPoints(Vec3r &point0, Vec3r &point1, Vec3r &point2) const
  {
      point0 = point0_;
      point1 = point1_;
      point2 = point2_;
  }

  //! \brief Get triangle normal
  //! \return Triangle normal
  Vec3r Triangle::GetNormal() const
  {
      return normal_;
  }

  void Triangle::ComputeNormal()
  {
      Vec3r v_A = (point1_ - point0_);
      Vec3r v_B = (point2_ - point0_);

      normal_ = v_A.cross(v_B);
      /*normal_[0] = v_A[1] * v_B[2] - v_A[2] * v_B[1];
      normal_[1] = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
      normal_[2] = v_A[0] * v_B[1] - v_A[1] * v_B[0];*/
  }


}  // namespace core
}  // namespace olio
