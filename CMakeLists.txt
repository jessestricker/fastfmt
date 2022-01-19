cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(
  fastfmt
  VERSION 0.4.0
  LANGUAGES CXX)

# disallow in-source builds
if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(FATAL_ERROR "in-source builds not allowed")
endif()

# determine if this is the root project
set(ROOT_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(ROOT_PROJECT ON)
endif()

# include targets
add_subdirectory(src/fastfmt)
