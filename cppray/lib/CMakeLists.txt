# CMakeList.txt : CMake project for cppray, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library(
    cppraylib
    "PixelArray.cpp"
    "RayTracer.cpp"
    "RayTraceRenderData.cpp"
    "SceneRenderer.cpp"
    "Camera.cpp"
    "Ray.cpp"
    "Scene.cpp"
    "Background.cpp"
    "Shape.cpp"
    "SceneFactory.cpp"
    "LogTimer.cpp")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4576")
    target_link_libraries(cppraylib png_static zlibstatic ${Boost_LIBRARIES})
endif()
if (UNIX)
    target_link_libraries(cppraylib ${PNG_LIBRARY} ${ZLIB_LIBRARY} ${Boost_LIBRARIES})
endif()
