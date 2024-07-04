// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       gldrawdata.h
//! \brief      Draw data that's passed to DrawGL function of shapes when
//!             they need to be drawn during the GL render pass
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <string>
#include <memory>

namespace olio {

class Light;
class Material;

class GLDrawData {
public:
  inline void SetModelMatrix(const glm::mat4 &model_matrix)
      {model_matrix_=model_matrix;}
  inline void SetViewMatrix(const glm::mat4 &view_matrix)
      {view_matrix_ = view_matrix;}
  inline void SetProjectionMatrix(const glm::mat4 &projection_matrix)
      {projection_matrix_ = projection_matrix;}
  inline void SetLights(const std::vector<std::shared_ptr<Light>> &lights)
      {lights_ = lights;}
  inline void SetMaterial(std::shared_ptr<Material> material)
      {material_ = material;}
  inline glm::mat4 GetModelMatrix() const {return model_matrix_;}
  inline glm::mat4 GetViewMatrix() const {return view_matrix_;}
  inline glm::mat4 GetProjectionMatrix() const {return projection_matrix_;}
  inline void GetLights(std::vector<std::shared_ptr<Light>> &lights) const
      {lights=lights_;}
  inline std::shared_ptr<Material> GetMaterial() const {return material_;}
protected:
  glm::mat4 model_matrix_{1.0f};
  glm::mat4 view_matrix_{1.0f};
  glm::mat4 projection_matrix_{1.0f};
  std::vector<std::shared_ptr<Light>> lights_;
  std::shared_ptr<Material> material_;
};


// class MeshGLDrawInfo {
// public:
//   inline void SetHasVertexColors(bool has_colors)
//       {has_vertex_colors_ = has_colors;}
//   inline bool GetHasVertexColors() const {return has_vertex_colors_;}
// protected:
//   bool has_vertex_colors_{false};
// };

}  // namespace olio
