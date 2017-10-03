cmake_minimum_required (VERSION 3.6)
project(cube)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set (SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set (INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")

find_package (SDL2 REQUIRED)
include_directories (${INCLUDE_PATH} ${SDL2_INCLUDE_DIR})

file (GLOB HEADER_FILES "${INCLUDE_PATH}/*.h")
file (GLOB SOURCE_FILES "${SRC_PATH}/*.cpp")

add_executable (cube ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries (cube ${SDL2_LIBRARY})
