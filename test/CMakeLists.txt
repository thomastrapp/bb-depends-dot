cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(bitbake-recipe-depend-test)

# Add the top-level cmake module directory to CMAKE_MODULE_PATH
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/../cmake)

include(EnableWarnings)

find_package(Catch2 REQUIRED)
find_package(Boost COMPONENTS graph REQUIRED)

add_executable(
  bitbake-recipe-depend-test
  "${PROJECT_SOURCE_DIR}/../bbrd/bbrd/DependencyGraph.cpp"
  "${PROJECT_SOURCE_DIR}/../ragel/Dependencies.cpp"
  "${PROJECT_SOURCE_DIR}/test.cpp")
enable_warnings(bitbake-recipe-depend-test PUBLIC)
target_link_libraries(
  bitbake-recipe-depend-test
  Boost::graph
  Catch2::Catch2
  "-fsanitize=address")
target_include_directories(
  bitbake-recipe-depend-test PUBLIC
  "${PROJECT_SOURCE_DIR}/../bbrd")
target_compile_features(bitbake-recipe-depend-test PUBLIC cxx_std_17)
target_compile_options(bitbake-recipe-depend-test PUBLIC "-fsanitize=address")
