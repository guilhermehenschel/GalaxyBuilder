cmake_minimum_required(VERSION 3.24)

# Unified Tests for Galaxy Builder
project(GalaxyFactoriesTests VERSION 1.0.0 LANGUAGES CXX)

include(googletest)

# Enable testing
enable_testing()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create unified test executable for all GTest-based tests
add_executable(GalaxyFactoriesTests
    test_GalaxyGenerator.cpp
    test_AbstractGalaxyFactory.cpp
    test_GalaxyGenerator_Full.cpp
)

target_link_libraries(GalaxyFactoriesTests PRIVATE
    gtest_main
    GGH::GalaxyFactories
    Qt6::Core
    Qt6::Xml
)

gtest_discover_tests(GalaxyFactoriesTests)
