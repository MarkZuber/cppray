# CMakeList.txt : CMake project for cppray, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (cppray "cppray.cpp")
target_link_libraries(cppray cppraylib)

# if (MSVC)
#     target_link_libraries(cppray png_static zlibstatic ${Boost_LIBRARIES})
# endif()
# if (UNIX)
#     target_link_libraries(cppray ${PNG_LIBRARY} ${ZLIB_LIBRARY} ${Boost_LIBRARIES})
# endif()
