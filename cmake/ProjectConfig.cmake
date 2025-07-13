# Galaxy Builder - CMake Configuration
# This file contains project configuration settings

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Global compiler settings
if(MSVC)
    # Visual Studio specific settings
    add_compile_options(/W4 /permissive- /utf-8)
    # Enable multi-processor compilation
    add_compile_options(/MP)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od /Zi /RTC1)
        add_compile_definitions(_DEBUG)
    else()
        add_compile_options(/O2 /DNDEBUG)
    endif()
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC and Clang specific settings
    add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fno-omit-frame-pointer)
        add_compile_definitions(_DEBUG)
    else()
        add_compile_options(-O3 -DNDEBUG -fomit-frame-pointer)
    endif()
endif()

# Platform-specific definitions
if(WIN32)
    add_compile_definitions(
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        _CRT_SECURE_NO_WARNINGS
    )
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(
        _GNU_SOURCE
        _DEFAULT_SOURCE
    )
endif()

# Project options
option(GALAXY_BUILDER_BUILD_TESTS "Build unit tests" OFF)
option(GALAXY_BUILDER_BUILD_DOCS "Build documentation" OFF)
option(GALAXY_BUILDER_ENABLE_LTO "Enable Link Time Optimization" ON)
option(GALAXY_BUILDER_ENABLE_SANITIZERS "Enable sanitizers (Debug only)" OFF)

# Debug postfix for libraries and executables
set(CMAKE_DEBUG_POSTFIX "d")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Per-configuration output directories
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/lib")
endforeach()

# Include build configuration
include(cmake/BuildConfig.cmake)

# NOTE: Qt6 detection is now handled in third_party/third_party.cmake
# This will be included from the main CMakeLists.txt

# Print build information
message(STATUS "")
message(STATUS "=== Galaxy Builder Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==========================================")
message(STATUS "")
