cmake_minimum_required(VERSION 3.1.0)
project (olio_core)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

# ui headers
set (HEADERS
  node.h
  ray.h
  types.h

  # camera
  camera/camera.h

  # geometry
  geometry/sphere.h
  geometry/surface.h
  geometry/triangle.h
  geometry/surface_list.h

  # light
  light/light.h

  # material
  material/material.h
  material/phong_material.h

  # parser
  parser/raytra_parser.h

  # renderer
  renderer/raytracer.h

  # utils
  utils/segfault_handler.h
)

set (SOURCES
  node.cc
  ray.cc

  # camera
  camera/camera.cc

  # geometry
  geometry/sphere.cc
  geometry/surface.cc
  geometry/triangle.cc
  geometry/surface_list.cc

  # light
  light/light.cc

  # material
  material/material.cc
  material/phong_material.cc

  # parser
  parser/raytra_parser.cc

  # renderer
  renderer/raytracer.cc

  # utils
  utils/segfault_handler.cc
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
  PRIVATE ./
  PRIVATE ../
  PUBLIC ${olio_COMMON_SYSTEM_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  ${olio_COMMON_EXTERNAL_LIBRARIES}
)

# expose olio_core's includes and libs to other subprojects
set (${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/..
  ${olio_COMMON_SYSTEM_INCLUDE_DIRS}
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
set (${PROJECT_NAME}_LIBRARIES ${PROJECT_NAME}
  ${olio_COMMON_EXTERNAL_LIBRARIES}
  CACHE INTERNAL "${PROJECT_NAME}: External Libraries" FORCE)

# set warning/error level
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wconversion -Wsign-conversion)
endif()

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# copy over header files
install(DIRECTORY ./ # source directory
        DESTINATION include/olio/core # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
        PATTERN "*.hpp" # select header files
)
