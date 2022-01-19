# define library target
add_library(fastfmt STATIC fastfmt.cpp)
target_include_directories(fastfmt PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_features(fastfmt PUBLIC cxx_std_20)
add_library(fastfmt::fastfmt ALIAS fastfmt)

if(ROOT_PROJECT)
  # import testing framework
  include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
  CPMAddPackage(
    GITHUB_REPOSITORY "doctest/doctest"
    VERSION 2.4.8
    OPTIONS "DOCTEST_USE_STD_HEADERS ON")

  # define testing target
  add_executable(fastfmt_test detail/checked_arith.test.cpp fastfmt.test.cpp)
  target_link_libraries(fastfmt_test PRIVATE fastfmt doctest_with_main)
endif()
