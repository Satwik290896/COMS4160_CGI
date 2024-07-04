// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file  main.cc
//! \brief Simple GL viewer. This example shows how to use OpenGL
//!        shaders to draw a gouraud-shaded rotating sphere
//! \author Hadi Fadaifard, 2022

#include <iostream>
#include <sstream>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <spdlog/spdlog.h>
#include "types.h"
#include "utils/utils.h"
#include "utils/glshader.h"
#include "utils/gldrawdata.h"
#include "utils/material.h"
#include "utils/light.h"
#include "sphere.h"

using namespace std;
using namespace olio;

// application-wide vao
GLuint vao_;

// window dimensions
Vec2i window_size_g{1, 1};

// the sphere model and its material and xform
Sphere::Ptr sphere_g;
Material::Ptr sphere_material_g;
Mat4r sphere_xform_g{Mat4r::Identity()};

// scene lights
vector<Light::Ptr> lights_g;

//! \brief compute view and projection matrices based on current
//! window dimensions
//! \param[out] view_matrix view matrix
//! \param[out] proj_matrix projection matrix
void
GetViewAndProjectionMatrices(glm::mat4 &view_matrix, glm::mat4 &proj_matrix)
{
  // compute aspect ratio
  float aspect = static_cast<float>(window_size_g[0]) /
    static_cast<float>(window_size_g[1]);
  view_matrix = glm::lookAt(glm::vec3(0, 0, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  proj_matrix = glm::perspective(static_cast<float>(60.0 * kDEGtoRAD), aspect,
                                 0.01f, 50.0f);
}


//! \brief Update sphere and its transformation matrix based on
//! current time
//! \param[in] glfw_time current time
void
UpdateSphere(Real glfw_time)
{
  // scale 2x along x-axis
  Mat4r scale_xform{Mat4r::Identity()};
  scale_xform.diagonal() = Vec4r{2, 1, 1, 1};

  // rotate around y-axis based on current time
  Real rotation_speed = 90;  // 90 degrees per second
  Mat4r rotate_y_xform{Mat4r::Identity()};
  Real rotate_y_angle = glfw_time * rotation_speed;
  Real c = cos(rotate_y_angle * kDEGtoRAD);
  Real s = sin(rotate_y_angle * kDEGtoRAD);
  rotate_y_xform(0, 0) = c;
  rotate_y_xform(0, 2) = s;
  rotate_y_xform(2, 0) = -s;
  rotate_y_xform(2, 2) = c;

  // rotate around z-axis based on current time
  rotation_speed = 30;  // 30 degrees per second
  Mat4r rotate_z_xform{Mat4r::Identity()};
  Real rotate_z_angle = glfw_time * rotation_speed;
  c = cos(rotate_z_angle * kDEGtoRAD);
  s = sin(rotate_z_angle * kDEGtoRAD);
  rotate_z_xform(0, 0) = c;
  rotate_z_xform(0, 1) = -s;
  rotate_z_xform(1, 0) = s;
  rotate_z_xform(1, 1) = c;

  // compose sphere's transformation matrix
  sphere_xform_g = rotate_z_xform * rotate_y_xform * scale_xform;
}


//! \brief main display function that's called to update the content
//! of the OpenGL (GLFW) window
void
Display(Real glfw_time)
{
  // clear window
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // make sure we have a valid sphere object
  if (!sphere_g)
    return;

  // update sphere and its transform based on current time
  UpdateSphere(glfw_time);

  // get view and projection matrices
  glm::mat4 view_matrix, proj_matrix;
  GetViewAndProjectionMatrices(view_matrix, proj_matrix);

  // fill GLDraw data for the sphere
  GLDrawData draw_data;
  draw_data.SetModelMatrix(EigenToGLM(sphere_xform_g));
  draw_data.SetViewMatrix(view_matrix);
  draw_data.SetProjectionMatrix(proj_matrix);
  draw_data.SetMaterial(sphere_material_g);
  draw_data.SetLights(lights_g);

  // draw the sphere
  sphere_g->DrawGL(draw_data);
}


//! \brief Resize callback function, which is called everytime the
//!        window is resize
//! \param[in] window pointer to glfw window (unused)
//! \param[in] width window width
//! \param[in] height window height
void
WindowResizeCallback(GLFWwindow */*window*/, int width, int height)
{
  // set viewport to occupy full canvas
  window_size_g = Vec2i{width, height};
  glViewport(0, 0, width, height);
}


//! \brief Keyboard callback function, which is called everytime a key
//! on the keyboard is pressed.
//! \param[in] window pointer to glfw window (unused)
//! \param[in] key key that was pressed/released/repeated etc.
//! \param[in] scancode scancode for the keyboard
//! \param[in] action one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
//! \param[in] mods integer values specifying which modifier keys
//!                 (Shift, Alt, Ctrl, etc.) are currently pressed
void
KeyboardCallback(GLFWwindow */*window*/, int key, int /*scancode*/, int action,
                 int /*mods*/)
{
  if (!sphere_g)
    return;
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    uint grid_nx, grid_ny;
    sphere_g->GetGridSize(grid_nx, grid_ny);
    switch (key) {
    case GLFW_KEY_0:            // increase number of subdivisions (grid_nx)
      sphere_g->SetGridSize(grid_nx + 1, grid_ny);
      break;
    case GLFW_KEY_9:            // decrease number of subdivisions (grid_nx)
      sphere_g->SetGridSize(grid_nx - 1, grid_ny);
      break;
    case GLFW_KEY_2:            // increase number of subdivisions (grid_ny)
      sphere_g->SetGridSize(grid_nx, grid_ny + 1);
      break;
    case GLFW_KEY_1:            // decrease number of subdivisions (grid_ny)
      sphere_g->SetGridSize(grid_nx, grid_ny - 1);
      break;
    default:
      break;
    }
  }
}


//! \brief Create the main OpenGL (GLFW) window
//! \param[in] width window width
//! \param[in] height window height
//! \param[in] window_name window name/title
//! \return pointer to created window
GLFWwindow*
CreateGLFWWindow(int width, int height, const std::string &window_name)
{
  // init glfw
  if (!glfwInit()) {
    spdlog::error("glfwInit failed");
    return nullptr;
  }

  // on the Codio box, we can only use 3.1 (GLSL 1.4). On macos, we
  // can use OpenGL 4.1 and only core profile
#if !defined(__APPLE__)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // // enable antialiasing (with 5 samples per pixel)
  // glfwWindowHint(GLFW_SAMPLES, 5);

  // create main glfw window
  auto *window = glfwCreateWindow(width, height, window_name.c_str(),
                                  nullptr, nullptr);
  if (!window) {
    spdlog::error("glfwCreatewindow failed");
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  // init glew. should be called after a window has been created and
  // we have a gl context
  if (glewInit() != GLEW_OK) {
    spdlog::error("glewInit failed");
    return nullptr;
  }

  // enable vsync
  glfwSwapInterval(1);

  // handle window resize events
  glfwSetFramebufferSizeCallback(window, WindowResizeCallback);

  // set keyboard callback function
  glfwSetKeyCallback(window, KeyboardCallback);

  // get the current window size and call the resize callback to
  // update the viewport with the correct aspect ratio
  int window_width, window_height;
  glfwGetWindowSize(window, &window_width, &window_height);
  WindowResizeCallback(window, window_width, window_height);

  return window;
}


//! \brief Main executable function
int
main()
{
  // create the main GLFW window
  auto window = CreateGLFWWindow(1280, 720, "Olio - Sphere");
  if (!window)
    return -1;

  // create VAO
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // create a Sphere instance
  sphere_g = std::make_shared<Sphere>();

  // create phong material for the sphere
  Vec3r ambient{0, 0, 0}, diffuse{.8, .8, 0}, specular{.5, .5, .5};
  Real shininess{50};
  ambient = diffuse;
  auto material = std::make_shared<PhongMaterial>(ambient, diffuse, specular, shininess);

  // create gl shader object and load vertex and fragment shaders
  auto glshader = make_shared<GLPhongShader>();
  if (!glshader->LoadShaders("../shaders/test_v.glsl",
                             "../shaders/test_f.glsl")) {
    spdlog::error("Failed to load shaders.");
    return -1;
  }

  // set the gl shader for the material
  material->SetGLShader(glshader);

  // set sphere's material
  sphere_material_g = material;

  // add point light 1
  auto point_light1 = make_shared<PointLight>(Vec3r{2, 2, 4}, Vec3r{10, 10, 10},
                                              Vec3r{0.01f, 0.01f, 0.01f});
  lights_g.push_back(point_light1);

  // add point light 2
  auto point_light2 = make_shared<PointLight>(Vec3r{-1, -4, 1}, Vec3r{7, 2, 2},
                                              Vec3r{0.01f, 0.01f, 0.01f});
  lights_g.push_back(point_light2);

  // add point light 3
  auto point_light3 = make_shared<PointLight>(Vec3r{-2, 4, 1}, Vec3r{0, 5, 2},
                                              Vec3r{0.01f, 0.01f, 0.01f});
  lights_g.push_back(point_light3);

  // main draw loop
  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      break;
    Display(glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
    // glfwWaitEvents();
  }

  // clean up stuff
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
