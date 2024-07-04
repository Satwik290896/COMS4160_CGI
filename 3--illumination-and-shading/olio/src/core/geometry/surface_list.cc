#include "core/geometry/surface_list.h"
#include <spdlog/spdlog.h>
#include "core/ray.h"

namespace olio {
namespace core {

SurfaceList::SurfaceList(const std::vector<Surface::Ptr> &surfaces, const std::string &name) :
  Surface{}
{
    name_ = name.size() ? name : "SurfaceList";

    surface_ = surfaces;
}

bool SurfaceList::Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record)
{
    bool r = false;
    Vec3r ey = ray.GetOrigin();
    Real mini_d = kInfinity;
    Real min_ray_t = kInfinity;

    for(auto SuF = surface_.begin(); SuF != surface_.end(); SuF++)
    {
        HitRecord local_hit_record;
        bool result;

        result = (*SuF)->Hit(ray, tmin, tmax, local_hit_record);
        if (result == true){
            Real ray_t = local_hit_record.GetRayT();
            Real d = (ray.At(ray_t) - ey).norm();
            if (r == false) {
                r = true;
                min_ray_t = ray_t;
                mini_d = (ray.At(ray_t) - ey).norm();
                hit_record = local_hit_record;
            }
            else if (mini_d > d)
            {
                min_ray_t = ray_t;
                mini_d = d;
                hit_record = local_hit_record;
            }
            
        }
        else
            continue;
    }
    return r;
}

}
}