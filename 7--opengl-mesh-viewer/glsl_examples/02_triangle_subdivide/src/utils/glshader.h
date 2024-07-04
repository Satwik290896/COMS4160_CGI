// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       glshader.h
//! \brief      GLShader class for loading vertex and fragment shaders and
//!             setting uniforms
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "types.h"

namespace olio {

class GLShader : public std::enable_shared_from_this<GLShader> {
public:
  using Ptr = std::shared_ptr<GLShader>;
  using WeakPtr = std::weak_ptr<GLShader>;

  GLShader() = default;
  GLShader(const GLShader &) = delete;
  GLShader(GLShader &&) = delete;
  GLShader& operator=(const GLShader &) = delete;
  GLShader& operator=(GLShader &&) = delete;
  virtual ~GLShader();

  static bool ReadTextFile(const boost::filesystem::path &file_path,
			   std::string &content);
  static void PrintShaderLog(GLuint shader);
  static void PrintProgramLog(GLuint prog);
  virtual GLuint LoadShaders(const boost::filesystem::path &vertex_shader_path,
			   const boost::filesystem::path &fragment_shader_path);
  virtual bool Use() const;
  virtual inline void SetProgramID(GLuint id) {program_id_ = id;}
  virtual inline GLuint GetProgramID() const {return program_id_;}

  virtual bool SetMVPMatrices(const Mat4r &model_matrix,
			      const Mat4r &view_matrix,
			      const Mat4r &proj_matrix) const;
  virtual bool SetMVPMatrices(const glm::mat4 &model_matrix,
			      const glm::mat4 &view_matrix,
			      const glm::mat4 &proj_matrix) const;

  virtual bool SetUniformFloat(const std::string &name, GLfloat value) const;
  virtual bool SetUniformInt(const std::string &name, GLint value) const;
  virtual bool SetUniformUInt(const std::string &name, GLuint value) const;
  virtual bool SetUniformVec2(const std::string &name, const glm::vec2 &vec) const;
  virtual bool SetUniformVec3(const std::string &name, const glm::vec3 &vec) const;
  virtual bool SetUniformVec4(const std::string &name, const glm::vec4 &vec) const;
  virtual bool SetUniformMat4(const std::string &name, const glm::mat4 &mat) const;
  virtual bool SetUniformVec2(const std::string &name, const Vec2r &vec) const;
  virtual bool SetUniformVec3(const std::string &name, const Vec3r &vec) const;
  virtual bool SetUniformVec4(const std::string &name, const Vec4r &vec) const;
  virtual bool SetUniformMat4(const std::string &name, const Mat4r &mat) const;
protected:
  GLuint program_id_{0};
};

}  // namespace olio
