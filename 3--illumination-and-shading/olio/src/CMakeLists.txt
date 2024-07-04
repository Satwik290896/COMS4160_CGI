cmake_minimum_required(VERSION 3.1.0)

# core lib
add_subdirectory(core)

# command line executable
add_subdirectory(rtbasic)
add_dependencies(olio_rtbasic olio_core)

# tests
add_subdirectory(tests)
add_dependencies(olio_tests olio_core)
