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

class GLDrawData {
public:
  inline void SetModelMatrix(const glm::mat4 &model_matrix)
      {model_matrix_=model_matrix;}
  inline void SetViewMatrix(const glm::mat4 &view_matrix)
      {view_matrix_ = view_matrix;}
  inline void SetProjectionMatrix(const glm::mat4 &projection_matrix)
      {projection_matrix_ = projection_matrix;}
  inline glm::mat4 GetModelMatrix() const {return model_matrix_;}
  inline glm::mat4 GetViewMatrix() const {return view_matrix_;}
  inline glm::mat4 GetProjectionMatrix() const {return projection_matrix_;}
protected:
  glm::mat4 model_matrix_{1.0f};
  glm::mat4 view_matrix_{1.0f};
  glm::mat4 projection_matrix_{1.0f};
};

}  // namespace olio
