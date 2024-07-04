#pragma once

#include <memory>
#include <string>
#include "core/types.h"
#include "core/node.h"
#include "core/material/material.h"
#include "core/material/phong_material.h"
#include "core/ray.h"
#include "core/light/light.h"

namespace olio {
namespace core {

class PhongDielectric : public PhongMaterial {

public:
    OLIO_NODE(PhongDielectric)

    //! \brief Constructor
    //! \param[in] name Node name
    explicit PhongDielectric(const std::string &name=std::string());

    PhongDielectric(Real ior, const Vec3r &attenuation=Vec3r{1, 1, 1},
                  const std::string &name=std::string());

    //! \brief Scatter incoming ray ray_in
    //! \details The function will generate a reflection and a refraction
    //!    ray. The refraction ray will be null (not generated) if there is total
    //!    internal reflection.
    //! \param[in] hit_record Hit record at hit point
    //! \param[in] ray_in Incoming ray that hit the point
    //! \param[out] reflect_ray Reflected ray
    //! \param[out] refract_ray Refracted ray (null if not refracted because of total internal reflection)
    //! \param[out] schlick_reflectance Schlick's reflectance
    //! \return Attenuation factor (how much color of reflected/refracted
    //! rays should be attenuated)
    Vec3r Scatter(const HitRecord &hit_record, const Ray &ray_in, 
                                std::shared_ptr<Ray> &reflect_ray, 
                                std::shared_ptr<Ray> &refract_ray, 
                                Real &schlick_reflectance) const;

    static Real SchlicksReflectance(Real cos_theta, Real ior_ratio);  

protected:
    Real ior_{1};  //!< index of refraction

};

}
} 
