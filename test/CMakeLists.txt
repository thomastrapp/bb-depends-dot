cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(bb-depends-dot-test)

# Add the top-level cmake module directory to CMAKE_MODULE_PATH
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/../cmake)

include(EnableWarnings)

find_package(Catch2 REQUIRED)
find_package(Boost COMPONENTS graph REQUIRED)

add_executable(
  bb-depends-dot-test
  "${PROJECT_SOURCE_DIR}/../bbrd/bbrd/DependencyGraph.cpp"
  "${PROJECT_SOURCE_DIR}/../ragel/Dependencies.cpp"
  "${PROJECT_SOURCE_DIR}/test.cpp")
enable_warnings(bb-depends-dot-test PUBLIC)
target_link_libraries(
  bb-depends-dot-test
  Boost::graph
  Catch2::Catch2
  "-fsanitize=address")
target_include_directories(
  bb-depends-dot-test PUBLIC
  "${PROJECT_SOURCE_DIR}/../bbrd")
target_compile_features(bb-depends-dot-test PUBLIC cxx_std_17)
target_compile_options(bb-depends-dot-test PUBLIC "-fsanitize=address")
