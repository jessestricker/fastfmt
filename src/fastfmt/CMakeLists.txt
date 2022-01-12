# library target
add_library(fastfmt STATIC detail.cpp fastfmt.cpp)
add_library(fastfmt::fastfmt ALIAS fastfmt)
target_include_directories(fastfmt PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_compile_features(fastfmt PUBLIC cxx_std_20)

# testing target
if(MASTER_PROJECT)
  include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)
  CPMAddPackage("gh:catchorg/Catch2@3.0.0-preview4")
  add_executable(fastfmt_test fastfmt.test.cpp)
  target_link_libraries(fastfmt_test PRIVATE fastfmt Catch2::Catch2WithMain)
endif()
