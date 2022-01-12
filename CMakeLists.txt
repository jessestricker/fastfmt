cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(
  fastfmt
  VERSION 0.1.0
  LANGUAGES CXX)

# disallow in-source builds
if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(FATAL_ERROR "in-source builds not allowed")
endif()

# determine if this project is not included via `add_subdirectory`
set(MASTER_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(MASTER_PROJECT ON)
endif()

# set target defaults
set(CMAKE_CXX_EXTENSION OFF)

# include targets
add_subdirectory(src/fastfmt)
