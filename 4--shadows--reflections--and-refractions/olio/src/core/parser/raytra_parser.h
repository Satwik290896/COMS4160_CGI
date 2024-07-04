#pragma once

#include <vector>
#include "core/node.h"
#include "core/geometry/surface.h"
#include "core/camera/camera.h"
#include "core/light/light.h"

namespace olio {
namespace core {

class RaytraParser {
public:
  static bool ParseFile (const std::string &filename, Surface::Ptr &scene,
                         std::vector<Light::Ptr> &lights, Camera::Ptr &camera,
                         Vec2i &image_size);
};

}  // namespace core
}  // namespace olio
