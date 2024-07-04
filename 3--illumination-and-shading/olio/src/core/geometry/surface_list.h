#pragma once

#include <memory>
#include <string>
#include <list>
#include <core/geometry/surface.h>

namespace olio {
namespace core {


class SurfaceList : public Surface {
public:
    OLIO_NODE(SurfaceList)

    SurfaceList(const std::vector<Surface::Ptr> &surfaces, const std::string &name=std::string());

    bool Hit(const Ray &ray, Real tmin, Real tmax, HitRecord &hit_record) override;

private:
    std::vector<Surface::Ptr> surface_;
};

}
}