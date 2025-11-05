cmake_minimum_required(VERSION 3.20)

project(kuukan
  VERSION 0.1.0
  DESCRIPTION "kuukan: abstract functional analysis interfaces in C++ (header-only)"
  LANGUAGES CXX)

# Header-only interface target
add_library(kuukan INTERFACE)
add_library(kuukan::kuukan ALIAS kuukan)

target_include_directories(kuukan
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# C++20 for concepts/templates
target_compile_features(kuukan INTERFACE cxx_std_20)

# Examples (optional)
option(KUUKAN_BUILD_EXAMPLES "Build kuukan examples" ON)
if(KUUKAN_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# (Optional) Simple installation rules
include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS kuukan EXPORT kuukanTargets)
install(EXPORT kuukanTargets
  NAMESPACE kuukan::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/kuukan)
