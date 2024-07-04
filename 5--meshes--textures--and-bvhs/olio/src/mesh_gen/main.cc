// ======================================================================
// Olio: Simple renderer
// Copyright (C) 2022 by Hadi Fadaifard
//
// Author: Hadi Fadaifard, 2022
// ======================================================================

//! \file       main.cc
//! \brief      main.cc file for olio mesh_gen
//! \author     Hadi Fadaifard, 2022

#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

#include "core/types.h"
#include "core/node.h"
#include "core/utils/segfault_handler.h"
#include "core/geometry/trimesh.h"

using namespace olio::core;
using namespace std;
namespace po = boost::program_options;

bool ParseArguments(int argc, char **argv, uint *sphere_type, uint *grid_nx,
                    uint *grid_ny, std::string *output_name) {
  po::options_description desc("options");
  try {
    desc.add_options()
      ("help,h", "print usage")
      ("sphere_type,s",
       po::value             (sphere_type)->required(),
       "Sphere type: 0: with texture coordinates (not watertight), "
       "1: watertight (without texture coordinates)")
      ("grid_nx,m",
       po::value             (grid_nx)->default_value(10),
       "grid_nx: number of grid's horizontal cells")
      ("grid_ny,n",
       po::value             (grid_ny)->default_value(10),
       "grid_ny: number of grid's vertical cells")
      ("output,o",
       po::value             (output_name)->required(),
       "Output mesh name");

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


//! \brief Build a sphere mesh without seam
//! \param[in] center Sphere center
//! \param[in] radius Sphere radius
//! \param[in] grid_nx number of longitudinal (horizontal) division for grid defining sphere
//! \param[in] grid_ny number of latitudinal (vertical) division for grid defining sphere
//! \return Smart pointer to create mesh
TriMesh::Ptr
BuildWatertightSphere(const Vec3r &center, Real radius, uint grid_nx,
                      uint grid_ny)
{
  // ======================================================================
  // *** Homework: Implement function
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  if (grid_nx < 1 || grid_ny < 1) return nullptr;
  TriMesh::Ptr mesh = TriMesh::Create();

  mesh->request_face_normals();
  if (!mesh->has_face_normals()) {
    spdlog::error("failed to allocate face normals for mesh");
    return nullptr;
  }

  mesh->request_vertex_normals();
  if (!mesh->has_vertex_normals()) {
    spdlog::error("failed to allocate vertex normals for mesh");
    return nullptr;
  }

  MatXi vertex_indices{grid_ny + 1, grid_nx + 1};
  for (uint j = 0; j <= grid_ny; j++) {
    for (uint i = 0; i <= grid_nx; i++) {
        Real t1 = ((Real)i)/((Real)grid_nx);
        Real t2 = ((Real)j)/((Real)grid_ny);

        Vec3r point{radius * cos(k2Pi * (t1)) * sin(kPi * (t2)),
                  radius * sin(k2Pi * (t1)) * sin(kPi * (t2)),
                  radius * cos(kPi * (t2))};

        auto vh = mesh->add_vertex(center - point);
        vertex_indices(j, i) = vh.idx();
    }
  }

  for (uint i = 0; i< grid_nx; ++i) {
      vector<TriMesh::VertexHandle> face{3};
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(0, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(1, (i+1)%grid_nx)));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(1, i)));
      mesh->add_face(face);
  }

   for (uint j = 1; j < grid_ny-1; ++j) {
    for (uint i = 0; i < grid_nx; ++i) {
      // add face 1
      vector<TriMesh::VertexHandle> face{3};
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j, (i+1)%grid_nx)));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j+1, (i+1)%grid_nx)));
      mesh->add_face(face);
      // add face 2
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j+1, (i+1)%grid_nx)));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j+1, i)));
      mesh->add_face(face);
    }
  }

  for (uint i = 0; i< grid_nx; ++i) {
      vector<TriMesh::VertexHandle> face{3};
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(grid_ny-1, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(grid_ny-1, (i+1)%grid_nx)));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(grid_ny, (i+1)%grid_nx)));
      mesh->add_face(face);
  }

  mesh->ComputeFaceNormals();
  mesh->ComputeVertexNormals();

  return mesh;  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


//! \brief Build a sphere mesh with texture coordinates
//! \param[in] center Sphere center
//! \param[in] radius Sphere radius
//! \param[in] grid_nx number of longitudinal (horizontal) division for grid defining sphere
//! \param[in] grid_ny number of latitudinal (vertical) division for grid defining sphere
//! \return Smart pointer to create mesh
TriMesh::Ptr
BuildSphereWithTexCoords(const Vec3r &center, Real radius, uint grid_nx,
                         uint grid_ny)
{
  // ======================================================================
  // *** Homework: Implement function
  // ======================================================================
  // ***** START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
  if (grid_nx < 1 || grid_ny < 1) return nullptr;
  TriMesh::Ptr mesh = TriMesh::Create();

  // allocate 2D texture coordiantes
  mesh->request_vertex_texcoords2D();
  if (!mesh->has_vertex_texcoords2D()) {
    spdlog::error("failed to allocate vertex texcoords2d for mesh");
    return nullptr;
  }

  mesh->request_face_normals();
  if (!mesh->has_face_normals()) {
    spdlog::error("failed to allocate face normals for mesh");
    return nullptr;
  }

  mesh->request_vertex_normals();
  if (!mesh->has_vertex_normals()) {
    spdlog::error("failed to allocate vertex normals for mesh");
    return nullptr;
  }

  Real du = 1.0 / (Real)grid_nx;
  Real dv = 1.0 / (Real)grid_ny;
  MatXi vertex_indices{grid_ny + 1, grid_nx + 1};

  cout << " Retu Save cos: " << cos(kPi) << "\n" << endl;
  for (uint j = 0; j <= grid_ny; j++) {
    for (uint i = 0; i <= grid_nx; i++) {
        Real t1 = ((Real)i)/((Real)grid_nx);
        Real t2 = ((Real)j)/((Real)grid_ny);

        Vec3r point{radius * cos(k2Pi * (t1)) * sin(kPi * (t2)),
                  radius * sin(k2Pi * (t1)) * sin(kPi * (t2)),
                  -radius * cos(kPi * (t2))};

        auto vh = mesh->add_vertex(center + point);
        vertex_indices(j, i) = vh.idx();

        // set vertex texcoord2d
        mesh->set_texcoord2D(vh, Vec2r{i * du, 1 - j * dv});
    }
  }

  for (uint i = 0; i< grid_nx; ++i) {
      vector<TriMesh::VertexHandle> face{3};
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(0, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(1, (i+1))));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(1, i)));
      TriMesh::FaceHandle fh = mesh->add_face(face);
      Vec3r n = (mesh->point(face[1])-mesh->point(face[0])).cross(mesh->point(face[2])-mesh->point(face[0]));
      mesh->set_normal(fh, n/n.norm());
  }

   for (uint j = 1; j < grid_ny-1; ++j) {
    for (uint i = 0; i < grid_nx; ++i) {
      // add face 1
      vector<TriMesh::VertexHandle> face{3};
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j, (i+1))));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j+1, (i+1))));
      TriMesh::FaceHandle fh1 = mesh->add_face(face);
      Vec3r n1 = (mesh->point(face[1])-mesh->point(face[0])).cross(mesh->point(face[2])-mesh->point(face[0]));
      mesh->set_normal(fh1, n1/n1.norm());
      // add face 2
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j+1, (i+1))));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(j+1, i)));
      TriMesh::FaceHandle fh2 = mesh->add_face(face);
      Vec3r n2 = (mesh->point(face[1])-mesh->point(face[0])).cross(mesh->point(face[2])-mesh->point(face[0]));
      mesh->set_normal(fh2, n2/n2.norm());
    }
  }

  for (uint i = 0; i< grid_nx; ++i) {
      vector<TriMesh::VertexHandle> face{3};
      face[0] = mesh->vertex_handle(static_cast<uint>(vertex_indices(grid_ny-1, i)));
      face[1] = mesh->vertex_handle(static_cast<uint>(vertex_indices(grid_ny-1, (i+1))));
      face[2] = mesh->vertex_handle(static_cast<uint>(vertex_indices(grid_ny, (i+1))));
      TriMesh::FaceHandle fh = mesh->add_face(face);
      Vec3r n = (mesh->point(face[1])-mesh->point(face[0])).cross(mesh->point(face[2])-mesh->point(face[0]));
      mesh->set_normal(fh, n/n.norm());
  }

  //mesh->ComputeFaceNormals();
  mesh->ComputeVertexNormals();


  return mesh;  //!< remove this line and add your own code
  // ***** END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE) *****
}


int
main(int argc, char **argv)
{
  utils::InstallSegfaultHandler();
  srand(123543);

  // parse command line arguments
  uint sphere_type, grid_nx, grid_ny;
  string output_name;
  if (!ParseArguments(argc, argv, &sphere_type, &grid_nx, &grid_ny,
                      &output_name))
    return -1;

  // Depending on the sphere_type, generate either a watertight
  // sphere or a sphere with texture coordinates. Both spheres should
  // have a radius of 2 and be centered at (1, 2, 3). See hw5 for more
  // details.
  TriMesh::Ptr sphere;
  Real radius = 2.0f;
  Vec3r center{1, 2, 3};
  switch (sphere_type) {
  case 0:
    sphere = BuildSphereWithTexCoords(center, radius, grid_nx, grid_ny);
    break;
  case 1:
    sphere = BuildWatertightSphere(center, radius, grid_nx, grid_ny);
    break;
  default:
    spdlog::error("invalid sphere type.");
    return -1;
  }

  // Save the generated sphere mesh in the file specified by the
  // command line argument
  OpenMesh::IO::Options options{OpenMesh::IO::Options::VertexNormal};
  bool retu;
  if (sphere) {
    if (sphere->has_vertex_texcoords2D())
      options = options | OpenMesh::IO::Options::VertexTexCoord;
    retu = sphere->Save(output_name, options);
    cout << " Retu Save: " << retu << "\n" << endl;
  }

  /*TriMesh::Ptr mesh2 = BuildWatertightSphere(center, radius, 10, 10);
  mesh2->request_vertex_colors();
  for (auto vit = mesh2->vertices_begin(); vit != mesh2->vertices_end(); ++vit) {
    TriMesh::Color vert_color{255, 255, 255};
    if (mesh2->is_boundary(*vit))
      vert_color = TriMesh::Color{255, 0, 0};
    mesh2->set_color(*vit, vert_color);
  }
  OpenMesh::IO::write_mesh(*mesh2, "sphere110.ply", OpenMesh::IO::Options::VertexColor);*/
  
  return 0;
}
