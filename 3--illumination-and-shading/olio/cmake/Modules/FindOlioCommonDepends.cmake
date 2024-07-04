# boost
find_package(Boost 1.55 COMPONENTS program_options system filesystem thread regex REQUIRED)

# eigen
find_package(Eigen3 REQUIRED)
# Ubuntu Xenial fix
if (NOT DEFINED Eigen3_INCLUDE_DIR)
  set (Eigen3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIRS})
else()
  set (Eigen3_LIBRARIES Eigen3::Eigen)
endif()

# TBB
include(FindTBB)
find_package(TBB REQUIRED)
if (UNIX AND NOT APPLE)
  if (NOT DEFINED TBB_tbb_LIBRARY)
    set(TBB_tbb_LIBRARY tbb)
  endif()
endif()

# OpenMesh
include(FindOpenMesh)
find_package(OpenMesh 9.0 REQUIRED)

# opencv
# find_package(OpenCV 4.1 REQUIRED PATHS "$ENV{OPENCV_DIR}")
find_package(OpenCV REQUIRED)

# jemalloc
if(UNIX AND NOT APPLE)
find_package(jemalloc)
endif()

# spdlog
if (${CODIO_BUILD})
set(SPDLOG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/spdlog/include)
else()
find_package(spdlog REQUIRED)
set(SPDLOG_LIBRARIES spdlog::spdlog)
endif()

# catch2
set(CATCH2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Catch2)
set(CATCH2_INCLUDE_DIRS ${CATCH2_DIR}/include)

# tqdm
set(TQDM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/tqdm/include)

# OpenGL
find_package(OpenGL REQUIRED)

# glm
find_package(glm REQUIRED)
if (APPLE)
set(GLM_LIBRARIES glm::glm)
endif()

# glew
find_package(GLEW REQUIRED)

set (olio_COMMON_SYSTEM_INCLUDE_DIRS
  SYSTEM ${Boost_INCLUDE_DIR}
  SYSTEM ${Eigen3_INCLUDE_DIR}
  SYSTEM ${OPENMESH_INCLUDE_DIRS}
  SYSTEM ${TBB_INCLUDE_DIRS}
  SYSTEM ${OpenCV_INCLUDE_DIRS}
  SYSTEM ${TQDM_INCLUDE_DIRS}
  SYSTEM ${OPENGL_INCLUDE_DIR}
  SYSTEM ${GLM_INCLUDE_DIRS}
  SYSTEM ${GLEW_INCLUDE_DIRS}
  SYSTEM ${SPDLOG_INCLUDE_DIRS}
)

set (olio_COMMON_EXTERNAL_LIBRARIES
  ${OPENMESH_LIBRARIES}
  ${Boost_LIBRARIES}
  ${TBB_tbb_LIBRARY}
  ${OpenCV_LIBS}
  ${Eigen3_LIBRARIES}
  ${JEMALLOC_SHARED_LIB}
  ${SPDLOG_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLM_LIBRARIES}
  GLEW::GLEW
)

# add libdl to non-windows builds
if (NOT WIN32)
set (olio_COMMON_EXTERNAL_LIBRARIES
  ${olio_COMMON_EXTERNAL_LIBRARIES}
  dl
)
endif()
