# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

set(CMAKE_CXX_STANDARD 11)

project ("cppray")

enable_testing()
set(BUILD_TESTING, ON)

# Set options used by "FindBoost.cmake".
# set (BOOST_ROOT "${CMAKE_SOURCE_DIR}/deps/boost/")
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

find_package (Boost)
if (Boost_FOUND)
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})
endif ()


# Download and unpack googletest at configure time
configure_file(CMakeLists.gtest.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build")

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

if (MSVC)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/deps)
    set(PNG_SHARED OFF CACHE BOOL "Disable PNG_SHARED")
    set(PNG_TESTS OFF CACHE BOOL "Disable PNG_TESTS")

    add_subdirectory(deps/zlib)
    set(ZLIB_ROOT "${CMAKE_SOURCE_DIR}/deps/zlib/")
    set(ZLIB_LIBRARY "${CMAKE_BINARY_DIR}/deps/zlib/${CMAKE_CFG_INTDIR}/zlibstatic${CMAKE_DEBUG_POSTFIX}.lib")
    include(FindZLIB)

    # zconf.h gets generated into cmake_current_binary_dir/deps/zlib
    include_directories(${ZLIB_INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/deps/zlib")

    add_subdirectory(deps/libpng)
    set(PNG_LIBRARY "${CMAKE_BINARY_DIR}/deps/libpng/${CMAKE_CFG_INTDIR}/libpng16_static${CMAKE_DEBUG_POSTFIX}.lib")
    set(PNG_PNG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/deps/libpng/")
    include(FindPNG)
    # pnglibconf.h gets generated into cmake_current_binary_dir/deps/libpnng
    include_directories(${PNG_INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/deps/libpng")
endif()
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
endif()
if (UNIX)
    include(FindZLIB)
    include(FindPNG)
endif()

# Include sub-projects.
add_subdirectory("cppray")
add_subdirectory("test")
