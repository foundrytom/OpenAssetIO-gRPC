cmake_minimum_required(VERSION 3.21)

# Define default install directories, e.g. CMAKE_INSTALL_LIBDIR

include(GNUInstallDirs)

project(OpenAssetIO-gRPC VERSION 1.0.0)


# Additional CMake includes
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Default install directory
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND PROJECT_IS_TOP_LEVEL)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/dist" CACHE PATH "Installation location" FORCE)
endif ()

# Options

option(OPENASSETIO_GRPC_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# Disable C++11 ABI by default, as per current VFX reference
# platform(s), but allow optionally enabling.
# For reference, note that the default varies by platform, e.g.
# * CentOS 7.9.2009's GCC 6.3, `_GLIBCXX_USE_CXX11_ABI` is
#   always `0` and cannot be overridden.
# * Ubuntu 18.04's GCC 6.5, `_GLIBCXX_USE_CXX11_ABI` defaults
#   to `1`, but can be overridden.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 5.0)
option(OPENASSETIO_GRPC_GLIBCXX_USE_CXX11_ABI "For gcc, use the new C++11 library ABI" OFF)
endif ()

set(OPENASSETIO_GRPC_GLIBCXX_USE_CXX12_ABI OFF)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(DefaultTargetProperties)

add_subdirectory(src)
