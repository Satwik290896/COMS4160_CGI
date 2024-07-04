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
#include <boost/program_options.hpp>
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
#include "trimesh.h"

using namespace std;
using namespace olio;

// application-wide vao
GLuint vao_;


bool update_happened = true;
Real y_input_move = 270.0f/1280.0f; /*Degrees*/
Real x_input_move = 135.0f/720.0f;  /*Degrees*/

Real y_input_move2 = 270.0f/50.0f; /*Degrees*/
Real x_input_move2 = 135.0f/50.0f;  /*Degrees*/
bool temp_mouse_button_press = false;

Real x_delta_deg = 0;
Real y_delta_deg = 0;

Real current_camera = 2.0f;
// window dimensions
Vec2i window_size_g{1, 1};

// the sphere model and its material and xform
Sphere::Ptr sphere_g;
Material::Ptr sphere_material_g;
Mat4r sphere_xform_g{Mat4r::Identity()};
std::vector<TriMesh::Ptr> trimesh_g;
int mesh_count;
bool mouse_button_press = false;

// scene lights
vector<Light::Ptr> lights_g;

Real x_prev = 0;
Real y_prev = 0;

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
  view_matrix = glm::lookAt(glm::vec3(0, 0, current_camera), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
  scale_xform.diagonal() = Vec4r{1, 1, 1, 1};

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
  sphere_xform_g = rotate_z_xform * rotate_y_xform * sphere_xform_g;
}


void
UpdateMeshes(Real glfw_time)
{
  // scale 2x along x-axis
  Mat4r scale_xform{Mat4r::Identity()};
  scale_xform.diagonal() = Vec4r{2, 1, 1, 1};

  // rotate around y-axis based on current time
  Real rotation_speed = 90;  // 90 degrees per second
  Mat4r rotate_y_xform{Mat4r::Identity()};
  Real rotate_y_angle = glfw_time * 0.2f;
  Real c = cos(x_delta_deg * y_input_move * kDEGtoRAD);
  Real s = sin(x_delta_deg * y_input_move * kDEGtoRAD);
  rotate_y_xform(0, 0) = c;
  rotate_y_xform(0, 2) = s;
  rotate_y_xform(2, 0) = -s;
  rotate_y_xform(2, 2) = c;

  // rotate around z-axis based on current time
  rotation_speed = 30;  // 30 degrees per second
  Mat4r rotate_z_xform{Mat4r::Identity()};
  Real rotate_x_angle = glfw_time * 0.2f;
  c = cos(y_delta_deg * x_input_move * kDEGtoRAD);
  s = sin(y_delta_deg * x_input_move * kDEGtoRAD);
  rotate_z_xform(1, 1) = c;
  rotate_z_xform(1, 2) = -s;
  rotate_z_xform(2, 1) = s;
  rotate_z_xform(2, 2) = c;

  // compose sphere's transformation matrix
  sphere_xform_g = rotate_z_xform * rotate_y_xform * sphere_xform_g;
  update_happened = true;
}


//! \brief main display function that's called to update the content
//! of the OpenGL (GLFW) window
void
Display(Real glfw_time)
{

  if (!update_happened)
    return;
  // clear window
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // make sure we have a valid sphere object

  // update sphere and its transform based on current time
  //UpdateSphere(glfw_time);
  //UpdateMeshes(glfw_time);  
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
  Real si = trimesh_g.size();
  Vec3r init{-1.0f + (1.0f/si),0,0};
  Vec3r addi{2.0f/si,0,0};
  Real multiplier = 2.0f/si;
  for (int i = 0; i < trimesh_g.size(); i++){
    Vec3r bmax{0,0,0};
    Vec3r bmin{0,0,0};
    trimesh_g[i]->GetBoundingBox(bmin, bmax);

    Real x_len = bmax[0] - bmin[0];
    Real y_len = bmax[1] - bmin[1];
    Real z_len = bmax[2] - bmin[2];
    

    Vec3r center = 0.5f * (bmin + bmax);
    Real MAXI = std::max(x_len, std::max(y_len, z_len));
    //if (MAXI < 1)
    //  MAXI = 1;
    
    //MAXI = 1;
    Real mul = (multiplier/MAXI);

    if (abs(multiplier-MAXI) < kEpsilon)
      mul = 1;

    //std::cout << "MAXI: " << MAXI << "\n" << std::endl;
    Vec3r trans = ((init+(i*addi))) - center;
    for(auto vit = trimesh_g[i]->vertices_begin(); vit != trimesh_g[i]->vertices_end(); ++vit) {
      Vec3r p = (((trimesh_g[i]->point(vit)) - center)*mul) + ((init+(i*addi)));
      trimesh_g[i]->set_point(*vit, p);
    }
    trimesh_g[i]->DrawGL(draw_data);
  }

  update_happened = false;
  //trimesh_g[2]->DrawGL(draw_data);
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
  /*if (!sphere_g)
    return;*/
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    uint grid_nx, grid_ny;
    //sphere_g->GetGridSize(grid_nx, grid_ny);
    switch (key) {
    case GLFW_KEY_X:            // increase number of subdivisions (grid_nx)
    {
      Real temp_current_camera = 0.9 * current_camera;

      if (temp_current_camera > 0.01f) {
        current_camera = temp_current_camera;
        update_happened = true;
      }
    }
      break;
    case GLFW_KEY_Z:            // decrease number of subdivisions (grid_nx)
    {
      Real temp_current_camera = 1.1 * current_camera;
      current_camera = temp_current_camera;
      update_happened = true;
    
    }
      break;
    default:
      break;
    }
  }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (mouse_button_press == true) {
    if (temp_mouse_button_press == true) {
      x_prev = xpos;
      y_prev = ypos;
      temp_mouse_button_press = false;
    }
    //std::cout << "Now, Cursor: [Prev] " << x_prev << " : " << y_prev << "  [Now] "<< xpos << " : " << ypos << " [Delta] " << (xpos - x_prev) << " : " << (ypos - y_prev) << " [Degrees] " << (xpos - x_prev)*x_input_move << " : " << (ypos - y_prev)*y_input_move << "\n" << std::endl;
    x_delta_deg = (xpos - x_prev);
    y_delta_deg = (ypos - y_prev); 
    UpdateMeshes(glfwGetTime());
    x_prev = xpos;
    y_prev = ypos;
  }
}


static void cursor_enter_callback(GLFWwindow* window, int Entered) {
  if (Entered) {
    //x_prev = 0;
    //y_prev = 0;
    //std::cout << "Entered the Window \n " << std::endl;
  }
  else {
    mouse_button_press = false;
    //std::cout << "Left the Window \n " << std::endl;
  }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    x_prev = 0;
    y_prev = 0;
    x_delta_deg = 0;
    y_delta_deg = 0;
    mouse_button_press = true;
    temp_mouse_button_press = true;
    //std::cout << "Pressed Left Mouse Button \n " << std::endl;
  }
  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    x_prev = 0;
    y_prev = 0;
    x_delta_deg = 0;
    y_delta_deg = 0;
    mouse_button_press = false;
    temp_mouse_button_press = false;
    //std::cout << "Released Left Mouse Button \n " << std::endl;
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

  glfwSetCursorPosCallback(window, cursor_position_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  glfwSetCursorEnterCallback(window, cursor_enter_callback);

  glfwSetMouseButtonCallback(window, mouse_button_callback);
  /*if (glfwRawMouseMotionSupported()) {
      std::cout << "glfwRawMouseMotionSupported() Enabled: " << "Yes" << "\n" << std::endl;
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }*/
  // get the current window size and call the resize callback to
  // update the viewport with the correct aspect ratio
  int window_width, window_height;
  glfwGetWindowSize(window, &window_width, &window_height);
  WindowResizeCallback(window, window_width, window_height);

  return window;
}


bool
ParseArguments(int argc, char **argv, std::vector<std::string> *mesh_names)
{
  namespace po = boost::program_options;
  po::options_description desc("options");
  try {
    desc.add_options()
      ("help,h", "print usage")
      ("mesh_name,m",
       po::value<vector<std::string>>(mesh_names)->multitoken(),
       "Mesh filenames");

    // parse arguments
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    if (vm.count("help")) {
      cout << desc << endl;
      return false;
    }
    po::notify(vm);
  } catch(std::exception &e) {
    cout << desc << endl;
    spdlog::error("{}", e.what());
    return false;
  } catch(...) {
    cout << desc << endl;
    spdlog::error("Invalid arguments");
    return false;
  }
  return true;
}


//! \brief Main executable function
int
main(int argc, char **argv)
{
  std::vector<string> mesh_names;
  if (!ParseArguments(argc, argv, &mesh_names))
    return -1;

  for (int i = 0; i < mesh_names.size(); i++) {
    std::cout << "Mesh Names: " << mesh_names[i] << "\n" << std::endl;

    std::string str = mesh_names[i];
    boost::filesystem::path concat_str;
          //str.erase(0,2);
    concat_str = "" + str;
    std::cout << "Here is the string: " << concat_str << "\n" << std::endl;

    TriMesh::Ptr mesh = std::make_shared<TriMesh>();
    bool retu;
          //mesh->SetMaterial(current_material);
    retu = mesh->Load(concat_str);
    std::cout << "Here is the string2: " << concat_str << "\n" << std::endl;
    std::cout << "Return: " << retu << "\n" << std::endl;

    trimesh_g.push_back(mesh);
  }
  
  std::cout << "Number of Trimeshes: " << trimesh_g.size() << "\n" << std::endl;
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

  /*if (!glshader->LoadShaders("../shaders/gouraud_vert.glsl",
                             "../shaders/gouraud_frag.glsl")) {
    spdlog::error("Failed to load shaders.");
    return -1;
  }*/

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
