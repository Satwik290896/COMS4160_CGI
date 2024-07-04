#pragma once

#include <vector>
#include "core/node.h"
#include "core/geometry/surface.h"
#include "core/camera/camera.h"

namespace olio {
namespace core {

class RaytraParser {
public:
  static bool ParseFile (const std::string &filename, Surface::Ptr &scene,
			 Camera::Ptr &camera, Vec2i &image_size);
};

}  // namespace core
}  // namespace olio
