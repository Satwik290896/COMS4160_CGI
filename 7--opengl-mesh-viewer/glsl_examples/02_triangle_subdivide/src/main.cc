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

using namespace std;
using namespace olio;

// global variable for rotation angle of the triangle vertices
Real twist_angle_g = 0.0f;  // angle in degrees

// global variable for the number of triangle subdivisions
int triangle_subdivisions_g = 0;

// vertex array object
GLuint vao_g = 0;

// vertex buffer object (VBO) for triangle vertex positions and colors
// (vertex positions and colors are interleaved)
GLuint positions_and_colors_vbo_g = 0;

// total number of triangle vertices
size_t total_triangle_vertex_count_g = 0;

// gl shader object containing vertex and fragment shaders and gl
// render program
GLShader::Ptr glshader_g;

// displayed window dimensions
Vec2i window_size_g{1, 1};

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
               const Vec3f &color2, Real twist_angle, const Vec3f &center,
               std::vector<GLfloat> &positions_and_colors)
{
  if (level <= 0) {
    // rotate and draw triangle vertices
    vector<Vec3f> points{p0, p1, p2};
    for (const auto &point : points) {
      Mat3f R{Mat3f::Identity()};
      float r = (point - center).norm();
      float c = static_cast<float>(cos(twist_angle * r * kDEGtoRAD));
      float s = static_cast<float>(sin(twist_angle * r * kDEGtoRAD));
      R(0,0) = c; R(0,1) = -s;
      R(1,0) = s; R(1,1) = c;
      Vec3f pt = R * (point - center) + center;

      // add vertex position
      positions_and_colors.push_back(pt[0]);
      positions_and_colors.push_back(pt[1]);
      positions_and_colors.push_back(pt[2]);

      // add vertex color
      positions_and_colors.push_back(color1[0]);
      positions_and_colors.push_back(color1[1]);
      positions_and_colors.push_back(color1[2]);
    }
    return;
  }

  // compute edge midpoints
  Vec3f p01 = 0.5f * (p0 + p1);
  Vec3f p02 = 0.5f * (p0 + p2);
  Vec3f p12 = 0.5f * (p1 + p2);

  // draw subdivided triangles
  DivideTriangle(p0, p01, p02, level - 1, color1, color2, twist_angle, center,
                 positions_and_colors);
  DivideTriangle(p01, p1, p12, level - 1, color1, color2, twist_angle, center,
                 positions_and_colors);
  DivideTriangle(p02, p12, p2, level - 1, color1, color2, twist_angle, center,
                 positions_and_colors);
  DivideTriangle(p01, p12, p02, level - 1, color2, color1, twist_angle, center,
                 positions_and_colors);
}


//! \brief delete all existing VBOs
void
DeleteGLBuffers()
{
  if (vao_g) {
    // delete VBOs
    glBindVertexArray(vao_g);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (positions_and_colors_vbo_g) {
      glDeleteBuffers(1, &positions_and_colors_vbo_g);
      positions_and_colors_vbo_g = 0;
    }
  }
}


//! \brief update contents of the vertex buffers (positions and
//! colors) with the latest version of the subdivided triangle
void
UpdateGLBuffers()
{
  // make sure we have a valid glshader object
  if (!glshader_g || !glshader_g->GetProgramID()) {
    spdlog::error("invalid GL shader");
    return;
  }

  // delete existing VBOs
  DeleteGLBuffers();

  // generate a VAO, if it doesn't already exist
  if (!vao_g)
    glGenVertexArrays(1, &vao_g);

  // bind VAO
  glBindVertexArray(vao_g);

  // generate triangles
  Vec3f p0{0, 0.75f, 0};
  Vec3f p1{0.65f, -0.375, 0};
  Vec3f p2{-0.65f, -0.375, 0};
  Vec3f center = (p0 + p1 + p2) / 3;
  Vec3f color1{1, 0, 0};
  Vec3f color2{0, 1, 0};
  vector<GLfloat> positions_and_colors;
  DivideTriangle(p0, p1, p2, triangle_subdivisions_g, color1, color2,
                 twist_angle_g, center, positions_and_colors);
  total_triangle_vertex_count_g = positions_and_colors.size() / 6;

  // generate position and color VBOs
  GLint positions_attr_index = 0;
  GLint colors_attr_index = 1;
  positions_attr_index = glGetAttribLocation(glshader_g->GetProgramID(), "position");
  colors_attr_index = glGetAttribLocation(glshader_g->GetProgramID(), "color");

  // create VBO for positions and colors
  glGenBuffers(1, &positions_and_colors_vbo_g);
  glBindBuffer(GL_ARRAY_BUFFER, positions_and_colors_vbo_g);
  glBufferData(GL_ARRAY_BUFFER, positions_and_colors.size() * sizeof(GLfloat),
               &positions_and_colors[0], GL_STATIC_DRAW);

  // enable positions attribute and set pointer
  glVertexAttribPointer(positions_attr_index, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(positions_attr_index);

  // enable colors attribute and set pointer
  glVertexAttribPointer(colors_attr_index, 3, GL_FLOAT, GL_FALSE,
                        6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(colors_attr_index);

  // unbind VAO
  glBindVertexArray(0);
}


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

  // check we have a valid vao and glshader object
  if (!vao_g || !total_triangle_vertex_count_g || !glshader_g ||
      !glshader_g->Use())
    return;

  // bind VAO and draw triangles
  glBindVertexArray(vao_g);

  // set model/view and projection matrices
  glm::mat4 model_matrix{1.0f};
  glm::mat4 view_matrix, proj_matrix;
  GetViewAndProjectionMatrices(view_matrix, proj_matrix);
  glshader_g->SetMVPMatrices(model_matrix, view_matrix, proj_matrix);

  // draw triangles
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(total_triangle_vertex_count_g));

  // check for gl errors
  CheckOpenGLError();

  // unbind VAO
  glBindVertexArray(0);
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
  bool needs_update = false;
  Real twist_delta = 5;  // degrees
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_0:            // rotate clockwise
      twist_angle_g -= twist_delta;
      needs_update = true;
      break;
    case GLFW_KEY_9:            // rotate counter clockwise
      twist_angle_g += twist_delta;
      needs_update = true;
      break;
    case GLFW_KEY_1:            // decrease number of subdivisions
      triangle_subdivisions_g = max(triangle_subdivisions_g - 1, 0);
      needs_update = true;
      break;
    case GLFW_KEY_2:            // increase number of subdivisions
      triangle_subdivisions_g += 1;
      needs_update = true;
      break;
    default:
      break;
    }
  }

  if (needs_update)
    UpdateGLBuffers();
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
  auto window = CreateGLFWWindow(1280, 720, "Olio - Triangles");
  if (!window)
    return -1;

  // create gl shader object and load vertex and fragment shaders
  glshader_g = make_shared<GLShader>();
  if (!glshader_g->LoadShaders("../shaders/simple_vert.glsl",
                               "../shaders/simple_frag.glsl")) {
    spdlog::error("Failed to load shaders.");
    return -1;
  }

  // we will only be using this shader to draw everything, so set it
  // to use here
  glshader_g->Use();

  // create and update VAOs and VBOs for triangle vertices
  UpdateGLBuffers();

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
