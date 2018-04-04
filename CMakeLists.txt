# Copyright 2018 Andrea Mantovani

PROJECT(vtkUniformRandomSampling)

cmake_minimum_required(VERSION 3.3)

if (NOT ParaView_BINARY_DIR)
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})
endif()

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(vcglib)

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

ADD_PARAVIEW_PLUGIN(
  UniformRandomSampling
  "1.0"
  SERVER_MANAGER_XML UniformRandomSampling.xml
  SERVER_MANAGER_SOURCES vtkUniformRandomSamplingFilter.cxx
)
