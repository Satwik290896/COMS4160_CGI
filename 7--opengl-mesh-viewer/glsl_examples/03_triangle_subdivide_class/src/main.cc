// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file  main.cc
//! \brief Simple GL viewer. This example shows how to use OpenGL
//!        shaders to draw a subdivided triangle
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
#include "twist_triangle.h"

using namespace std;
using namespace olio;

// displayed window dimensions
Vec2i window_size_g{1, 1};

TwistTriangle::Ptr twist_triangle_g;

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

  // set xmax, ymax
  float xmax, ymax;
  if(aspect > 1.0f) {  // wide screen
    xmax = aspect;
    ymax = 1.0f;
  } else {             // tall screen
    xmax = 1.0f;
    ymax = 1.0f / aspect;
  }
  view_matrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  proj_matrix = glm::ortho(-xmax, xmax, -ymax, ymax, -1.0f, 1.0f);
}


//! \brief main display function that's called to update the content
//! of the OpenGL (GLFW) window
void
Display()
{
  // clear window
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // make sure we have a valid twist_triangle_g object
  if (!twist_triangle_g)
    return;

  // get view and projection matrices
  glm::mat4 view_matrix, proj_matrix;
  GetViewAndProjectionMatrices(view_matrix, proj_matrix);

  // fill GLDraw data for TwistTriangle
  GLDrawData draw_data;
  glm::mat4 model_matrix{1.0f};
  draw_data.SetModelMatrix(model_matrix);
  draw_data.SetViewMatrix(view_matrix);
  draw_data.SetProjectionMatrix(proj_matrix);

  // draw twist triangle
  twist_triangle_g->DrawGL(draw_data);
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
  if (!twist_triangle_g)
    return;
  Real twist_delta = 5;  // degrees
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_0:            // rotate clockwise
      twist_triangle_g->SetTwistAngle(twist_triangle_g->GetTwistAngle() - twist_delta);
      break;
    case GLFW_KEY_9:            // rotate counter clockwise
      twist_triangle_g->SetTwistAngle(twist_triangle_g->GetTwistAngle() + twist_delta);
      break;
    case GLFW_KEY_1:            // decrease number of subdivisions
      twist_triangle_g->SetSubdivisions(std::max(twist_triangle_g->
                                                 GetSubdivisions() - 1, 0));
      break;
    case GLFW_KEY_2:            // increase number of subdivisions
      twist_triangle_g->SetSubdivisions(twist_triangle_g->GetSubdivisions() + 1);
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

  // enable antialiasing (with 5 samples per pixel)
  glfwWindowHint(GLFW_SAMPLES, 5);

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
  auto window = CreateGLFWWindow(1280, 720, "Olio - Triangles (Class)");
  if (!window)
    return -1;

  // create gl shader object and load vertex and fragment shaders
  auto glshader = make_shared<GLShader>();
  if (!glshader->LoadShaders("../shaders/simple_vert.glsl",
                             "../shaders/simple_frag.glsl")) {
    spdlog::error("Failed to load shaders.");
    return -1;
  }

  // create a TwistTriangle instance
  twist_triangle_g = std::make_shared<TwistTriangle>();
  twist_triangle_g->SetSubdivisions(0);
  twist_triangle_g->SetGLShader(glshader);

  // main draw loop
  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      break;
    Display();
    glfwSwapBuffers(window);
    // glfwPollEvents();
    glfwWaitEvents();
  }

  // clean up stuff
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
