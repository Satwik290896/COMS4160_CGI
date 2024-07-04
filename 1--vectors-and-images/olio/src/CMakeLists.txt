cmake_minimum_required(VERSION 3.1.0)

# core
add_subdirectory(core)

# olio executable
add_subdirectory(olio_exe)
add_dependencies(olio_exe olio_core)

# tests
add_subdirectory(tests)
add_dependencies(olio_tests olio_core)
