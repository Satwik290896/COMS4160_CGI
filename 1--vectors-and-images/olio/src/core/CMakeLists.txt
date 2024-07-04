cmake_minimum_required(VERSION 3.1.0)
project (olio_core)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

# ui headers
set (HEADERS
  image_utils.h
  types.h
  vector.h
  circle.h

  # segfault handler
  segfault_handler.h
)

set (SOURCES
  image_utils.cc
  vector.cc
  circle.cc

  # segfault handler
  segfault_handler.cc
)

set (SYSTEM_INCLUDES
)

set (EXTERNAL_LIBS
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
