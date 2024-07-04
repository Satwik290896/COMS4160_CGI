#include "raytra_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <spdlog/spdlog.h>
#include "core/geometry/surface.h"
#include "core/geometry/sphere.h"
#include "core/camera/camera.h"
#include "core/geometry/triangle.h"
#include "core/geometry/surface_list.h"

namespace olio {
namespace core {

using namespace std;
using boost::algorithm::trim;
namespace fs = boost::filesystem;

bool RaytraParser::ParseFile(const std::string &filename, Surface::Ptr &scene, std::vector<Light::Ptr> &lights,
                             Camera::Ptr &camera, Vec2i &image_size)
{
  // get absoulte file path
  fs::path filepath(filename);
  if (!filepath.is_absolute())
    filepath = boost::filesystem::absolute(filepath,
      boost::filesystem::current_path());
  fs::path path_prefix(filepath.parent_path());

  // check if file exists
  if (!fs::exists(filepath)) {
    spdlog::error("RaytraParser::ParseFile: file {} does not exist", filename);
    return false;
  }

  // open file
  ifstream in(filename);
  if (!in) {
    spdlog::error("RaytraParser::ParseFile: could not open file {} "
                  "for reading", filename);
    return false;
  }
  int camera_count = 0;
  int surface_count = 0;
  int ambilights_count = 0;
  std::vector<Surface::Ptr> surfaces;
  PhongMaterial::Ptr mat;
  bool mat_occured = false;
  bool error_set = false;

  // parse file
  for (string line; getline(in, line);) {
    // skip comments and empty lines
    trim(line);
    if (line.size() && line.back() == '\n')
      line.pop_back();
    if (!line.size() || line[0] == '/')
      continue;

    // parse file
    char cmd {'\0'};
    istringstream iss(line);
    iss >> cmd;
    //cout << "Did we reach here?2:  " + cmd + "\n"<< endl;
    switch(cmd) {
    case '/': continue;
    case 's':
      {
        // sphere
        Real x, y, z, r;
        iss >> x >> y >> z >> r;
        Sphere::Ptr sphere = Sphere::Create(Vec3r{x, y, z}, r);
        if(mat_occured == true)
        {
          sphere->SetMaterial(mat);
        }
        else{
          spdlog::error("Invalid scene file: cannot find matching material for surface: " + line);
          error_set = true;
        }
        surfaces.push_back(sphere);
        //scene = Sphere::Create(Vec3r{x, y, z}, r);
        ++surface_count;
        break;
      }
    case 'c':
      {
        // camera
        Real x, y, z, vx, vy, vz, focal_length, viewport_width, viewport_height,
          pixels_width, pixels_height;
        iss >> x >> y >> z >> vx >> vy >> vz >> focal_length >> viewport_width
            >> viewport_height >> pixels_width >> pixels_height;
        Vec3r eye{x, y, z};
        Vec3r view_vec{vx, vy, vz};
        view_vec.normalize();
        Vec3r target = eye + view_vec;
        Vec3r up_vec{0, 1, 0};
        if (view_vec.isApprox(up_vec))
          up_vec = Vec3r{0, 0, 1};
        Real fovy = 2 * atan2(viewport_height * 0.5f, focal_length) * kRADtoDEG;

        // check viewport/image aspect ratios
        Real viewport_aspect = viewport_width / viewport_height;
        if (!isfinite(viewport_aspect) || viewport_aspect <= 0) {
          spdlog::error("Camera has bad viewport_aspect ratio: {}",
                        viewport_aspect);
          return false;
        }
        if (viewport_aspect > 20000) {
          spdlog::warn("Camera has very large viewport_aspect ratio: {}",
                       viewport_aspect);
        }
        Real image_aspect = pixels_width / pixels_height;
        if (fabs(viewport_aspect - image_aspect) > kEpsilon) {
          spdlog::warn("Camera viewport has a different aspect ratio than "
                       "output image (viewport_aspect: {} vs "
                       "image_aspect: {})", viewport_aspect, image_aspect);
          spdlog::warn("Output image width will be adjusted to match the "
                       "viewport aspect ratio");
        }

        // create camera
        camera = Camera::Create(eye, target, up_vec, fovy,viewport_aspect);
        image_size = Vec2i{static_cast<int>(pixels_width),
                           static_cast<int>(pixels_height)};
        ++camera_count;
        break;
      }
    case 't':
      {
        // triangle
        Real ax, ay, az, bx, by, bz, cx, cy, cz;
        iss >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz;
        vector<Vec3r> points;
        points.reserve(3);
        points.push_back(Vec3r{ax, ay, az});
        points.push_back(Vec3r{bx, by, bz});
        points.push_back(Vec3r{cx, cy, cz});
        Triangle::Ptr triangle = Triangle::Create(points);
        if(mat_occured == true)
        {
          triangle->SetMaterial(mat);
        }
        else{
          spdlog::error("Invalid scene file: cannot find matching material for surface: " + line);
          error_set = true;
        }
        surfaces.push_back(triangle);
        //scene = Triangle::Create(points);
        ++surface_count;
        break;
      }
    case 'l':
      {
        char cmd2 {'\0'};
        iss >> cmd2;

        switch (cmd2) {
          case 'p':
            {
              Real x, y, z, c_r, c_g, c_b;
              iss >> x >> y >> z >> c_r >> c_g >> c_b;
              Vec3r pos{x,y,z};
              Vec3r intens{c_r,c_g,c_b};
              PointLight::Ptr pl =  PointLight::Create(pos, intens);
              lights.push_back(pl);
              break;
            }
          case 'a':
            {
              Real c_ar, c_ag, c_ab;
              iss >> c_ar >> c_ag >> c_ab;
              Vec3r ambi_intens{c_ar, c_ag, c_ab};
              AmbientLight::Ptr al =  AmbientLight::Create(ambi_intens);
              lights.push_back(al);
              ambilights_count++;
              break;
            }
        }
        break;
      }
    case 'm':
      {
        Real dr, dg, db, sr, sg, sb, r, ir, ig, ib;
        iss >> dr >> dg >> db >> sr >> sg >> sb >> r >> ir >> ig >> ib;
        Vec3r diffus{dr, dg, db};
        Vec3r specul{sr, sg, sb};
        Vec3r mirr{ir, ig, ib};
        Real p = r;

        mat = PhongMaterial::Create(diffus, diffus, specul, p, mirr);
        //cout << "Did we reach here?\n" << endl;
        mat_occured = true;
        break;
      }
    default:
      continue;
    }

    if(error_set)
      break;
      
  }

  // close input file
  in.close();

  if(error_set)
    return false;

  if (camera_count != 1) {
    spdlog::error("Parse error: scene file should contain only one camera");
    return false;
  }

  if (ambilights_count > 1) {
    spdlog::error("Parse error: scene file should contain a maximum of one ambient light");
    return false;    
  }

  scene = SurfaceList::Create(surfaces);

  /*if (surface_count > 1) {
    spdlog::error("Parse error: scene file currently can only contain one "
                  "surface");
    return false;
  }*/
  return true;
}

}  // namespace core
}  // namespace raytra
