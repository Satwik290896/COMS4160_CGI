// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       utils.h
//! \brief      Misc utility functions
//! \author     Hadi Fadaifard, 2022

#include "utils/utils.h"
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

namespace olio {

using namespace std;

bool
CheckOpenGLError()
{
  bool found_error = false;
  auto error_code = glGetError();
  while (error_code != GL_NO_ERROR) {
    spdlog::error("GL error code: {}", error_code);
    found_error = true;
    error_code = glGetError();
  }
  return found_error;
}

}  // namespace olio
