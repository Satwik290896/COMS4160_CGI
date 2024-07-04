cmake_minimum_required(VERSION 3.1.0)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")
    message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

if (WIN32)
  add_definitions(-D_USE_MATH_DEFINES -DNOMINMAX)
endif()

# use clang on Mac and Linux
if (NOT WIN32)
  set (CMAKE_C_COMPILER             "clang")
  set (CMAKE_CXX_COMPILER           "clang++")
  if (APPLE)
    add_definitions(-arch x86_64 -mmacosx-version-min=10.9 -Wall -Wno-unused-parameter -Wno-overloaded-virtual -fPIC -DNOMINMAX)
  endif()
endif()

set (CMAKE_CXX_STANDARD 11)

project (olio)

# set Codio build flag
set(CODIO_BUILD "$ENV{IS_CODIO}")
if (${CODIO_BUILD})
add_definitions(-DCODIO_BUILD)
endif()

# find Olio dependencies
include(FindOlioCommonDepends)

add_subdirectory(src)
