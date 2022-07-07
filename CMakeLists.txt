cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fmodules-ts -Wall")

# set the project name
project(VoxelVolumeTracer VERSION 0.11)

include_directories(include)
link_directories(include) 

# add the executable
add_executable(VoxelVolumeTracer)
target_sources(VoxelVolumeTracer PRIVATE src/test.cpp)
target_sources(VoxelVolumeTracer PRIVATE src/vec3.cpp)
target_sources(VoxelVolumeTracer PRIVATE src/main.cpp)
