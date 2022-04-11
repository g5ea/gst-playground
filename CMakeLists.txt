cmake_minimum_required(VERSION "3.16")

project(save-frames)

option(ENABLE_TEST "Enable unit tests" OFF)

set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set (CORE_LIBRARY "core")

set(CMAKE_CXX_STANDARD 17) #setting C++ 17 standard
find_package(PkgConfig) #finding pkg-config is a helper tool

#using pkg-config to getting Gstreamer
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

#using pkg-config to getting GLIB
pkg_check_modules(GLIB REQUIRED glib-2.0)

#including GStreamer header files directory
include_directories(
  ${GLIB_INCLUDE_DIRS}
  ${GSTREAMER_INCLUDE_DIRS}
)

#linking GStreamer library directory
link_directories(
  ${GLIB_LIBRARY_DIRS}
  ${GSTREAMER_LIBRARY_DIRS}
)

# Build the playground
add_subdirectory(src)

if (ENABLE_TEST)
  add_subdirectory (test)
endif ()
