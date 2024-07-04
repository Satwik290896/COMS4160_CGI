#pragma once

#include <memory>
#include <string>
#include "core/types.h"

namespace olio {
namespace core {

class FaceGeoUV {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  FaceGeoUV() = default;

  inline void set_face_id(const int &face_id) {face_id_ = face_id;}

  inline void set_uv(const Vec2r &uv) {uv_ = uv;}

  inline void set_global_uv(const Vec2r &global_uv) {global_uv_ = global_uv;}

  inline int get_face_id() const {return face_id_;}

  inline Vec2r get_uv() const {return uv_;}

  inline Vec2r get_global_uv() const {return global_uv_;}

protected:
  int face_id_{-1};  //!< triangle id in mesh (-1 if not a triangle or TriMesh)
  Vec2r uv_{-1,-1};     //!< barycentric coordinates ( ) of the point inside triangle; (-1, -1) if not triangle
  Vec2r global_uv_{-1,-1};  //!< uv coordinates of point, if surface has uv coordinates; (-1,-1) otherwise
};

}
}