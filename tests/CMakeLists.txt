cmake_minimum_required(VERSION 3.24)

# Tests for Galaxy Builder
project(GalaxyBuilderTests VERSION 1.0.0 LANGUAGES CXX)

# Enable testing
enable_testing()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include GalaxyCore module
include_directories(${CMAKE_SOURCE_DIR}/modules/GalaxyCore)

add_executable(test_simple
    test_simple.cpp
)

# Create test executables
add_executable(test_galaxy
    test_galaxy.cpp
)

add_executable(test_systemdatamanager
    test_systemdatamanager_new.cpp
)

add_executable(test_xmlvalidator
    test_xmlvalidator_new.cpp
)

add_executable(test_integration
    test_integration_new.cpp
)

# Link with GTest and GalaxyCore
target_link_libraries(test_simple PRIVATE
    gtest_main
)

target_link_libraries(test_galaxy PRIVATE
    gtest_main
    GalaxyCore
)

target_link_libraries(test_systemdatamanager PRIVATE
    gtest_main
    GalaxyCore
)

target_link_libraries(test_xmlvalidator PRIVATE
    gtest_main
    GalaxyCore
)

target_link_libraries(test_integration PRIVATE
    gtest_main
    GalaxyCore
)

# Register tests with CTest using GTest discovery
include(GoogleTest)
gtest_discover_tests(test_simple)
gtest_discover_tests(test_galaxy)
gtest_discover_tests(test_systemdatamanager)
gtest_discover_tests(test_xmlvalidator)
gtest_discover_tests(test_integration)

# Set target properties for all test executables
foreach(target test_simple test_galaxy test_systemdatamanager test_xmlvalidator test_integration)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()

# Enable testing
enable_testing()
