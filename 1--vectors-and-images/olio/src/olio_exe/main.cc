//! \file       main.cc
//! \brief      Main file containing the executable entry point, main()

#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include "core/segfault_handler.h"
#include "core/vector.h"
#include "core/image_utils.h"

using namespace std;
using namespace olio;
namespace po = boost::program_options;

bool ParseArguments(int argc, char **argv, std::string *input_name,
		    std::string *output_name, Real *gamma) {
  po::options_description desc("options");
  try {
    desc.add_options()
      ("help,h", "print usage")
      ("input,i",
       po::value             (input_name)->default_value(""),
       "Input name")
      ("output,o",
       po::value             (output_name)->default_value(""),
       "Output name")
      ("gamma,g",
       po::value             (gamma)->default_value(0),
       "Gamma value");

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


int main(int argc, char **argv)
{
  olio::InstallSegfaultHandler();

  cout << "Hello, World!\n";

  // parse command line arguments
  Real gamma;
  string input_name, output_name;
  if (!ParseArguments(argc, argv, &input_name, &output_name, &gamma))
    return -1;

  // check we have an output image name, if an input name was provided
  if (input_name.size() && !output_name.size()) {
    spdlog::error("Must provide output image name");
    return -1;
  }

  // gamma correct image
  if (gamma > 0 && input_name.size()) {
    cv::Mat out_image;
    spdlog::info("Gamma correcting image {} with gamma of {}...", input_name,
		 gamma);
    if (!utils::GammaCorrectImage(input_name, output_name, gamma)) {
      spdlog::error("Failed to gamma correct image");
      return -1;
    }
    spdlog::info("done");
  }

  return 0;
}
