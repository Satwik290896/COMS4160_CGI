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
#include <cmath>
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

#include "core/types.h"
#include "core/node.h"
#include "core/camera/camera.h"
#include "core/geometry/surface.h"
#include "core/geometry/surface_list.h"
#include "core/parser/raytra_parser.h"
#include "core/renderer/raytracer.h"
#include "core/utils/segfault_handler.h"
#include "core/light/light.h"
#include "core/geometry/bvh_node.h"

using namespace olio::core;
using namespace std;
namespace po = boost::program_options;

bool ParseArguments(int argc, char **argv, std::string *input_scene_name,
		    std::string *output_name, std::string *samples_per_pixel, std::string *shadow_samples) {
  po::options_description desc("options");
  try {
    desc.add_options()
      ("help,h", "print usage")
      ("input_scene,s",
       po::value             (input_scene_name)->required(),
       "Input scene file")
      ("output,o",
       po::value             (output_name)->required(),
       "Output name")
      ("samples per pixel,a",
       po::value             (samples_per_pixel)->default_value("1"),
       "Number of Samples per pixel")
      ("shadow_samples,d",
       po::value             (shadow_samples)->required(),
       "shadow samples");

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
  string samples_per_pixel;
  string shadow_samples;
  uint num_samples;
  uint int_shadow_samples;
  size_t int_sqrt_shadow_samples;

  if (!ParseArguments(argc, argv, &input_scene_name, &output_name, &samples_per_pixel, &shadow_samples))
    return -1;

  num_samples = (uint) stoi(samples_per_pixel);
  int_shadow_samples = (size_t) stoi(shadow_samples);
  int_sqrt_shadow_samples = (size_t) round(sqrt(int_shadow_samples));
  std::cout << "int_sqrt_shadow_samples: " << int_sqrt_shadow_samples << "\n" << std::endl;
  // parse and render raytra scene
  Vec2i image_size;
  Surface::Ptr scene;
  vector<Light::Ptr> lights;
  Camera::Ptr camera;
  if (!RaytraParser::ParseFile(input_scene_name, scene, lights, camera,
      image_size) || !scene || !camera || image_size[0] <= 0 ||
      image_size[1] <= 0) {
    spdlog::error("Failed to parse scene file.");
    return -1;
  }

  update_grid_size(int_sqrt_shadow_samples);

  BVHNode bvh;
  auto scene_vec = dynamic_pointer_cast<SurfaceList>(scene);
  BVHNode::Ptr BVH_pass = bvh.BuildBVH(scene_vec->GetListSurfaces());

  // render scene
  RayTracer rt;
  rt.SetImageHeight(static_cast<uint>(image_size[1]));
  rt.SetNumSamplesPerPixel(num_samples);
  rt.Render(BVH_pass, lights, camera);

  // save rendered image to file
  rt.WriteImage(output_name, 2);
  return 0;
}
