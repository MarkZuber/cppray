# CMakeList.txt : CMake project for cppray, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (
    cppray 
    "cppray.cpp" 
    "cppray.h"
    "PixelArray.cpp"
    "RayTracer.cpp"
    "RayTraceRenderData.cpp"
    "SceneRenderer.cpp"
    "Vector3.h")

# TODO: Add tests and install targets if needed.
