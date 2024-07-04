cmake_minimum_required(VERSION 3.1.0)

# core lib
add_subdirectory(core)

# rtbasic executable
add_subdirectory(rtbasic)
add_dependencies(olio_rtbasic olio_core)

# mesh_gen executable
add_subdirectory(mesh_gen)
add_dependencies(olio_mesh_gen olio_core)

# tests
add_subdirectory(tests)
add_dependencies(olio_tests olio_core)
