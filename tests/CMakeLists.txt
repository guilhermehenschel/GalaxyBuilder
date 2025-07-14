cmake_minimum_required(VERSION 3.24)

# Unified Tests for Galaxy Builder
project(GalaxyBuilderUnifiedTests VERSION 1.0.0 LANGUAGES CXX)

include(googletest)

# Enable testing
enable_testing()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include GalaxyCore module
include_directories(${CMAKE_SOURCE_DIR}/modules/GalaxyCore)

# Create unified test executable for all GTest-based tests
add_executable(GalaxyCoreTests
    test_galaxy.cpp
    test_systemdatamanager_new.cpp
    test_xmlvalidator_new.cpp
    test_simple_planet.cpp
)

target_link_libraries(GalaxyCoreTests PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Qml
    Qt6::Xml
    Qt6::Widgets
    GalaxyCore
)

gtest_discover_tests(GalaxyCoreTests)
