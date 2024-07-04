// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file  main.cc
//! \brief  Simple GL viewer. This example shows how to use legacy
//!         OpenGL functions to draw a subdivided triangle
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

using namespace std;
using namespace olio;

// global variable for rotation angle of the triangle vertices
Real twist_angle_g = 0.0f;  // angle in degrees

// global variable for the number of triangle subdivisions
int triangle_subdivisions_g = 0;

//! \brief recursive function that subdivides and draws all the
//!        triangles that are displayed by the application
//! \param[in] p0 first triangle vertex
//! \param[in] p1 second triangle vertex
//! \param[in] p2 third triangle vertex
//! \param[in] level subdivision level
//! \param[in] color1 color of triangles pointing up
//! \param[in] color2 color of triangles pointing down
//! \param[in] twist_angle amount by which to rotate triangle vertices
//!                        (in degrees)
//! \param[in] center center of rotation
void
DivideTriangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2,
              int level, const Vec3f &color1,
              const Vec3f &color2, Real twist_angle, const Vec3f &center)
{
  if (level <= 0) {
    // set triangle color
    glColor3f(color1[0], color1[1], color1[2]);
    vector<Vec3f> points{p0, p1, p2};

    // rotate and draw triangle vertices
    for (const auto &point : points) {
      Mat3f R{Mat3f::Identity()};
      float r = (point - center).norm();
      float c = static_cast<float>(cos(twist_angle * r * kDEGtoRAD));
      float s = static_cast<float>(sin(twist_angle * r * kDEGtoRAD));
      R(0,0) = c; R(0,1) = -s;
      R(1,0) = s; R(1,1) = c;
      Vec3f pt = R * (point - center) + center;
      glVertex3f(pt[0], pt[1], pt[2]);
    }
    return;
  }

  // compute edge midpoints
  Vec3f p01 = 0.5f * (p0 + p1);
  Vec3f p02 = 0.5f * (p0 + p2);
  Vec3f p12 = 0.5f * (p1 + p2);

  // draw subdivided triangles
  DivideTriangle(p0, p01, p02, level - 1, color1, color2, twist_angle, center);
  DivideTriangle(p01, p1, p12, level - 1, color1, color2, twist_angle, center);
  DivideTriangle(p02, p12, p2, level - 1, color1, color2, twist_angle, center);
  DivideTriangle(p01, p12, p02, level - 1, color2, color1, twist_angle, center);
}


//! \brief main display function that's called to update the content
//! of the OpenGL (GLFW) window
void
Display()
{
  // clear window
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw triangles
  Vec3f p0{0, 0.75f, 0};
  Vec3f p1{0.65f, -0.375, 0};
  Vec3f p2{-0.65f, -0.375, 0};
  Vec3f center = (p0 + p1 + p2) / 3;
  Vec3f color1{1, 0, 0};
  Vec3f color2{0, 1, 0};
  glBegin(GL_TRIANGLES);
  DivideTriangle(p0, p1, p2, triangle_subdivisions_g, color1, color2,
                 twist_angle_g, center);
  glEnd();
}


//! \brief Resize callback function, which is called everytime the
//!        window is resize
//! \param[in] window pointer to glfw window (unused)
//! \param[in] width window width
//! \param[in] height window height
void
WindowResizeCallback(GLFWwindow */*window*/, int width, int height)
{
  // compute aspect ratio
  float aspect = (float) width / (float) height;

  // set xmax, ymax
  float xmax, ymax;
  if(aspect > 1.0f) {  // wide screen
    xmax = aspect;
    ymax = 1.0f;
  } else {             // tall screen
    xmax = 1.0f;
    ymax = 1.0f / aspect;
  }
  // set viewport to occupy full canvas
  glViewport(0, 0, width, height);

  // init viewing coordinates for orthographic projection
  glLoadIdentity();
  glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
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
  Real twist_delta = 5;  // degrees
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_0:            // rotate clockwise
      twist_angle_g -= twist_delta;
      break;
    case GLFW_KEY_9:            // rotate counter clockwise
      twist_angle_g += twist_delta;
      break;
    case GLFW_KEY_1:            // decrease number of subdivisions
      triangle_subdivisions_g = max(triangle_subdivisions_g - 1, 0);
      break;
    case GLFW_KEY_2:            // increase number of subdivisions
      triangle_subdivisions_g += 1;
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
  auto window = CreateGLFWWindow(1280, 720, "Olio - Triangles (Legacy OpenGL)");
  if (!window)
    return -1;

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
