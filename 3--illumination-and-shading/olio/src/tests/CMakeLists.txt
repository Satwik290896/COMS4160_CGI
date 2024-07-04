cmake_minimum_required(VERSION 3.1.0)
project (olio_tests)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

# headers
set (HEADERS
)

set (SOURCES
  main.cc
)

set (SYSTEM_INCLUDES
)

set (EXTERNAL_LIBS
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
  PRIVATE ./
  PRIVATE ${olio_core_INCLUDE_DIRS}
  PRIVATE ${CATCH2_INCLUDE_DIRS}
  PRIVATE ${SYSTEM_INCLUDES})
target_link_libraries(${PROJECT_NAME}
  PRIVATE ${olio_core_LIBRARIES}
  PRIVATE ${EXTERNAL_LIBS}
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
