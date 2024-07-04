cmake_minimum_required(VERSION 3.1.0)
project (olio_mesh_view)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

# headers
set (HEADERS
  types.h
  sphere.h
  trimesh.h

  # utils
  utils/gldrawdata.h
  utils/glshader.h
  utils/light.h
  utils/material.h
  utils/segfault_handler.h
  utils/utils.h
)

# cc sources
set (SOURCES
  main.cc
  sphere.cc
  trimesh.cc

  # utils
  utils/glshader.cc
  utils/segfault_handler.cc
  utils/utils.cc
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
  PRIVATE ./
  PRIVATE ${olio_COMMON_SYSTEM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
  PRIVATE ${olio_COMMON_EXTERNAL_LIBRARIES}
)

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
