#include <iostream>
#include "core/material/phong_dielectric.h"
#include <spdlog/spdlog.h>
#include <cstddef>
#include <cmath>

namespace olio {
namespace core {

using namespace std;

PhongDielectric::PhongDielectric(const std::string &name) :
  PhongMaterial{}
{
  name_ = name.size() ? name : "PhongDielectric";
}

PhongDielectric::PhongDielectric(Real ior, const Vec3r &attenuation, const std::string &name) :
  PhongMaterial{} 
{
    name_ = name.size() ? name : "PhongDielectric";
    ior_ = ior;
    diffuse_ = attenuation;
}

Vec3r PhongDielectric::Scatter(const HitRecord &hit_record, const Ray &ray_in, 
                                std::shared_ptr<Ray> &reflect_ray, 
                                std::shared_ptr<Ray> &refract_ray, 
                                Real &schlick_reflectance) const
{
    Real n1 = 1;
    Real n2 = ior_;

    if (hit_record.IsFrontFace() == false) {
        n1 = ior_;
        n2 = 1;
    }

    Real ior_ratio = n1/n2;
    Vec3r N = hit_record.GetNormal();
    Vec3r d = (ray_in.GetDirection()).normalized();
    Real d_N = N.dot(d);

    Real comp1 = pow(ior_ratio, 2) * (1 - pow(d_N, 2));

    if (comp1 > 1) {
        refract_ray = NULL;
        schlick_reflectance = 1;
    }
    else {
        Vec3r R_dir = (ior_ratio * (d - (d_N)*N)) - (N * (sqrt(1 - comp1)));
        refract_ray = std::make_shared<Ray>(hit_record.GetPoint(), R_dir);
    }

    Vec3r reflect_dir = d - 2*(d_N)*N;
    std::shared_ptr<Ray> reflect_ray_temp;
    reflect_ray_temp = std::make_shared<Ray>(hit_record.GetPoint(), reflect_dir);
    reflect_ray = reflect_ray_temp;
    //std::cout << reflect_ray->GetOrigin() << "\n" << std::endl;
    schlick_reflectance = SchlicksReflectance(-d_N, ior_ratio);

    return diffuse_;

}

Real PhongDielectric::SchlicksReflectance(Real cos_theta, Real ior_ratio)
{
    Real R0 = pow((ior_ratio-1)/(ior_ratio+1), 2);
    Real ret = R0 + (1-R0)*pow((1-cos_theta),5);

    return ret;
}


}
}