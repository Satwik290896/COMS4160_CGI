// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       main.cc
//! \brief      rtbasic cli main.cc file
//! \author     Hadi Fadaifard, 2022

#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

#include "core/types.h"
#include "core/node.h"
#include "core/camera/camera.h"
#include "core/geometry/surface.h"
#include "core/parser/raytra_parser.h"
#include "core/renderer/raytracer.h"
#include "core/utils/segfault_handler.h"

using namespace olio::core;
using namespace std;
namespace po = boost::program_options;

bool ParseArguments(int argc, char **argv, std::string *input_scene_name,
		    std::string *output_name) {
  po::options_description desc("options");
  try {
    desc.add_options()
      ("help,h", "print usage")
      ("input_scene,s",
       po::value             (input_scene_name)->required(),
       "Input scene file")
      ("output,o",
       po::value             (output_name)->required(),
       "Output name");

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


int
main(int argc, char **argv)
{
  utils::InstallSegfaultHandler();
  srand(123543);

  // parse command line arguments
  string input_scene_name, output_name;
  if (!ParseArguments(argc, argv, &input_scene_name, &output_name))
    return -1;

  // ======================================================================
  // *** Homework: After parsing the command line arguments above, the
  // main function should:
  //   1. Parse the input scene file by calling
  //      RaytraParser::ParseFile(...) and obtaining the scene and
  //      main camera. Note: for this assignment the 'scene' will be
  //      either a sphere or a triangle.
  //   2. Create a RayTracer object and set its output image height
  //      based on the value obtained from ParseFile().
  //   3. Render the scene by calling the RayTracer object's Render()
  //      function.
  //   4. Write output (rendered) image by calling RayTracer object's
  //      WriteImage() function.
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  RaytraParser rtParser;
  Surface::Ptr scene;
  Camera::Ptr camera;
  Vec2i image_size;
  //const std::string &filename, Surface::Ptr &scene, Camera::Ptr &camera, Vec2i &image_size
  rtParser.ParseFile(input_scene_name, scene, camera, image_size);

  int image_height = image_size[1];
  cout << "image height: " << image_height << "\n" << endl;
  RayTracer rt;
  rt.SetImageHeight((uint)image_height);
  bool is_success = rt.Render(scene, camera);
  if(is_success){
    cout << "\nIt is a success\n" << endl;
    string output_image = "output_image.exr";
    rt.WriteImage(output_name);
  }
  else{
    cout << "\nIt is a failure\n" << endl;
  }

  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****

  return 0;
}
