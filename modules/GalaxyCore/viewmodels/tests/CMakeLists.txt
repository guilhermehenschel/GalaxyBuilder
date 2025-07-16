cmake_minimum_required(VERSION 3.24)

# Unified Tests for Galaxy Builder
project(GalaxyCoreViewModelsTests VERSION 1.0.0 LANGUAGES CXX)

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
add_executable(GalaxyCoreViewModelsTests
    test_planet_viewmodel.cpp
    test_starsystem_viewmodel.cpp
    test_galaxy_viewmodel.cpp
    test_travellane_viewmodel.cpp
)

target_link_libraries(GalaxyCoreViewModelsTests PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Qml
    Qt6::Test
    Qt6::Xml
    Qt6::Widgets
    GGH::GalaxyCore::ViewModels
    GGH::GalaxyCore::Models
    GGH::GalaxyCore::Utilities
)

gtest_discover_tests(GalaxyCoreViewModelsTests)
