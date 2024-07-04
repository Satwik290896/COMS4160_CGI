// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       glshader.cc
//! \brief      GLShader class for loading vertex and fragment shaders and
//!             setting uniforms
//! \author     Hadi Fadaifard, 2022

#include "utils/glshader.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "utils/utils.h"
#include "utils/gldrawdata.h"
#include "utils/light.h"
#include "utils/material.h"

namespace olio {

using namespace std;
namespace fs=boost::filesystem;

bool
GLShader::ReadTextFile(const fs::path &file_path, std::string &content)
{
  if (!fs::exists(file_path)) {
    spdlog::error("ReadTextFile: {} does not exist", file_path.string());
    return false;
  }
  ifstream infile(file_path.string());
  if (!infile) {
    spdlog::error("ReadTextFile: failed to open {} for reading",
                  file_path.string());
    return false;
  }

  // read file
  stringstream buffer;
  buffer << infile.rdbuf();
  content = buffer.str();
  return true;
}


GLShader::~GLShader()
{
  glUseProgram(0);
  if (program_id_)
    glDeleteProgram(program_id_);
}


void
GLShader::PrintShaderLog(GLuint shader)
{
  int len = 0;
  int written = 0;
  char *log = nullptr;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = new char[len];
    glGetShaderInfoLog(shader, len, &written, log);
    spdlog::info("Shader log:\n{}", log);
    delete []log;
  }
}


void
GLShader::PrintProgramLog(GLuint prog)
{
  int len = 0;
  int written = 0;
  char *log = nullptr;
  glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
  if (len > 0) {
    log = new char[len];
    glGetProgramInfoLog(prog, len, &written, log);
    spdlog::info("Program log:\n{}", log);
    delete []log;
  }
}


GLuint
GLShader::LoadShaders(const fs::path &vertex_shader_path,
                      const fs::path &fragment_shader_path)
{
  // read vertex shader
  string vert_shader_src;
  if (!ReadTextFile(vertex_shader_path, vert_shader_src)) {
    spdlog::error("ReadTextFile failed to read {}",
                  vertex_shader_path.string());
    return 0;
  }

  // read fragment shader
  string frag_shader_src;
  if (!ReadTextFile(fragment_shader_path, frag_shader_src)) {
    spdlog::error("ReadTextFile failed to read {}",
                  fragment_shader_path.string());
    return 0;
  }

  // create shaders
  auto vert_shader = glCreateShader(GL_VERTEX_SHADER);
  auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *vert_shader_src_ptr = vert_shader_src.c_str();
  const char *frag_shader_src_ptr = frag_shader_src.c_str();
  glShaderSource(vert_shader, 1, &vert_shader_src_ptr, nullptr);
  glShaderSource(frag_shader, 1, &frag_shader_src_ptr, nullptr);
  glCompileShader(vert_shader);
  CheckOpenGLError();
  GLint compiled{0};
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled);
  if (compiled != 1)
    PrintShaderLog(vert_shader);
  glCompileShader(frag_shader);
  CheckOpenGLError();
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compiled);
  if (compiled != 1)
    PrintShaderLog(frag_shader);

  // delete existing program
  glUseProgram(0);
  if (program_id_)
    glDeleteProgram(program_id_);

  // create rendering program
  program_id_ = glCreateProgram();
  glAttachShader(program_id_, vert_shader);
  glAttachShader(program_id_, frag_shader);

  glLinkProgram(program_id_);
  CheckOpenGLError();
  GLint linked{0};
  glGetProgramiv(program_id_, GL_LINK_STATUS, &linked);
  if (linked != 1)
    PrintProgramLog(program_id_);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  return program_id_;
}


bool
GLShader::Use() const
{
  if (!program_id_)
    return false;
  glUseProgram(program_id_);
  return true;
}


bool
GLShader::SetUniformFloat(const std::string &name, GLfloat value) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniform1f(location, value);
  return true;
}


bool
GLShader::SetUniformInt(const std::string &name, GLint value) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniform1i(location, value);
  return true;
}


bool
GLShader::SetUniformUInt(const std::string &name, GLuint value) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniform1ui(location, value);
  return true;
}


bool
GLShader::SetUniformVec2(const std::string &name, const glm::vec2 &vec) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniform2fv(location, 1, glm::value_ptr(vec));
  return true;
}


bool
GLShader::SetUniformVec3(const std::string &name, const glm::vec3 &vec) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniform3fv(location, 1, glm::value_ptr(vec));
  return true;
}


bool
GLShader::SetUniformVec4(const std::string &name, const glm::vec4 &vec) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniform4fv(location, 1, glm::value_ptr(vec));
  return true;
}


bool
GLShader::SetUniformMat4(const std::string &name, const glm::mat4 &mat) const
{
  if (!program_id_)
    return false;
  auto location = glGetUniformLocation(program_id_, name.c_str());
  if (location < 0)
    return false;
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
  return true;
}


bool
GLShader::SetUniformVec2(const std::string &name, const Vec2r &vec) const
{
  return SetUniformVec2(name, glm::vec2{vec[0], vec[1]});
}


bool
GLShader::SetUniformVec3(const std::string &name, const Vec3r &vec) const
{
  return SetUniformVec3(name, glm::vec3{vec[0], vec[1], vec[2]});
}


bool
GLShader::SetUniformVec4(const std::string &name, const Vec4r &vec) const
{
  return SetUniformVec4(name, glm::vec4{vec[0], vec[1], vec[2], vec[3]});
}


bool
GLShader::SetUniformMat4(const std::string &name, const Mat4r &eigen_mat) const
{
  glm::mat4 glm_mat;
  EigenToGLM(eigen_mat, glm_mat);
  return SetUniformMat4(name, glm_mat);
}


bool
GLShader::SetMVPMatrices(const Mat4r &model_matrix, const Mat4r &view_matrix,
                         const Mat4r &proj_matrix) const
{
  if (!program_id_)
    return false;

  // convert eigen matrices to glm
  glm::mat4 model_glm_matrix;
  EigenToGLM(model_matrix, model_glm_matrix);
  glm::mat4 view_glm_matrix;
  EigenToGLM(view_matrix, view_glm_matrix);
  glm::mat4 proj_glm_matrix;
  EigenToGLM(proj_matrix, proj_glm_matrix);

  // set matrices
  return SetMVPMatrices(model_glm_matrix, view_glm_matrix, proj_glm_matrix);
}


bool
GLShader::SetMVPMatrices(const glm::mat4 &model_matrix,
                         const glm::mat4 &view_matrix,
                         const glm::mat4 &proj_matrix) const
{
  if (!program_id_)
    return false;

  glm::mat4 mv_matrix = view_matrix * model_matrix;
  glm::mat4 norm_matrix = glm::transpose(glm::inverse(mv_matrix));
  SetUniformMat4("mv_matrix", mv_matrix);
  SetUniformMat4("norm_matrix", norm_matrix);  // for transforming normals
  SetUniformMat4("proj_matrix", proj_matrix);
  return true;
}


bool
GLShader::SetupUniforms(const GLDrawData &draw_data) const
{
  if (!program_id_)
    return false;

  // set MVP matrices
  if (!SetMVPMatrices(draw_data.GetModelMatrix(), draw_data.GetViewMatrix(),
                      draw_data.GetProjectionMatrix()) || CheckOpenGLError())
    return false;

  // set lights
  vector<Light::Ptr> lights;
  draw_data.GetLights(lights);
  if (!SetLights(draw_data.GetViewMatrix(), lights) || CheckOpenGLError())
    return false;

  // set material
  if (!SetMaterial(draw_data.GetMaterial()) || CheckOpenGLError())
    return false;
  // if (!SetUniformInt("has_vertex_colors", false))
  //   return false;
  return true;
}


bool
GLShader::SetLights(const glm::mat4 &/*view_matrix*/,
                    const std::vector<std::shared_ptr<Light>> &/*lights*/) const
{
  return true;
}


bool
GLShader::SetMaterial(std::shared_ptr<Material> /*material*/) const
{
  return true;
}


// ======================================================================
// GLPhongShader class
bool
GLPhongShader::SetLights(const glm::mat4 &view_matrix,
                         const std::vector<Light::Ptr> &lights) const
{
  if (!program_id_)
    return false;

  vector<PointLight::Ptr> point_lights;
  point_lights.reserve(lights.size());
  for (auto light : lights) {
    auto point_light = dynamic_pointer_cast<PointLight>(light);
    if (!point_light) {
      spdlog::error("GLPhongShader::SetLights: only PointLights supported -- "
                    " ignoring light");
      continue;
    }
    point_lights.push_back(point_light);
  }

  // set point light parameters in the gl shader
  auto light_count = static_cast<GLuint>(point_lights.size());
  SetUniformUInt("point_light_count", light_count);
  for (size_t i = 0; i < point_lights.size(); ++i) {
    // bring light position into camera space
    const auto &light = point_lights[i];
    glm::vec3 light_position = view_matrix * glm::vec4(EigenToGLM(light->GetPosition()), 1);

    // set light
    SetUniformVec3(fmt::format("point_lights[{}].position", i),
                   light_position);
    SetUniformVec3(fmt::format("point_lights[{}].intensity", i),
                   light->GetIntensity());
    SetUniformVec3(fmt::format("point_lights[{}].ambient", i),
                   light->GetAmbient());
  }
  return true;
}


bool
GLPhongShader::SetMaterial(Material::Ptr material) const
{
  // error checking
  if (!program_id_)
    return false;
  auto phong_material = dynamic_pointer_cast<PhongMaterial>(material);
  if (!phong_material) {
    spdlog::error("GLPhongShader::SetMaterial: "
                  "only PhongMaterials are supported.");
    return false;
  }
  SetUniformVec3("material.ambient", phong_material->GetAmbient());
  SetUniformVec3("material.diffuse", phong_material->GetDiffuse());
  SetUniformVec3("material.specular", phong_material->GetSpecular());
  SetUniformFloat("material.shininess",
                  static_cast<GLfloat>(phong_material->GetShininess()));
  return true;
}

}  // namespace olio
