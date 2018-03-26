# Copyright 2018 Andrea Mantovani
#
# This file is part of UniformRandomSampling.
#
# UniformRandomSampling is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# UniformRandomSampling is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar. If not, see <http://www.gnu.org/licenses/>.

PROJECT(vtkUniformRandomSampling)

cmake_minimum_required(VERSION 3.3)

if (NOT ParaView_BINARY_DIR)
  find_package(ParaView REQUIRED)
  include(${PARAVIEW_USE_FILE})
endif()

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

include_directories(vcglib)

ADD_PARAVIEW_PLUGIN(
  UniformRandomSampling
  "1.0"
  SERVER_MANAGER_XML UniformRandomSampling.xml
  SERVER_MANAGER_SOURCES vtkUniformRandomSamplingFilter.cxx
)
