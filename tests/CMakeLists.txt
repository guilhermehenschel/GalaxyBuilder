cmake_minimum_required(VERSION 3.24)

# Tests for Galaxy Builder
project(GalaxyBuilderTests VERSION 1.0.0 LANGUAGES CXX)

# Enable testing
enable_testing()

# Find Qt6 for test framework
find_package(Qt6 REQUIRED COMPONENTS Test Core Widgets Qml)

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

add_executable(test_planetlistmodel
    test_planetlistmodel_simple.cpp
)

add_executable(test_planet_persistence
    test_planet_persistence.cpp
)

add_executable(test_simple_planet
    test_simple_planet.cpp
)

add_executable(test_systemproperties_viewmodel
    test_systemproperties_viewmodel.cpp
)

add_executable(test_importexport_viewmodel
    test_importexport_viewmodel.cpp
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

target_link_libraries(test_planetlistmodel PRIVATE
    Qt6::Core
    GalaxyCore
)

target_link_libraries(test_planet_persistence PRIVATE
    Qt6::Test
    Qt6::Core
    Qt6::Xml
    GalaxyCore
)

target_link_libraries(test_simple_planet PRIVATE
    Qt6::Test
    Qt6::Core
    Qt6::Xml
    GalaxyCore
)

target_link_libraries(test_systemproperties_viewmodel PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Test
    Qt6::Qml
    GalaxyCore
)

target_link_libraries(test_importexport_viewmodel PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Test
    Qt6::Qml
    GalaxyCore
)

# Register tests with CTest using GTest discovery
include(GoogleTest)
gtest_discover_tests(test_simple)
gtest_discover_tests(test_galaxy)
gtest_discover_tests(test_systemdatamanager)
gtest_discover_tests(test_xmlvalidator)
gtest_discover_tests(test_integration)
# Temporarily disable ViewModel test discovery due to DLL dependency issues
# gtest_discover_tests(test_systemproperties_viewmodel)
# gtest_discover_tests(test_importexport_viewmodel)

# Register Qt test
add_test(NAME test_planetlistmodel COMMAND test_planetlistmodel)
add_test(NAME test_planet_persistence COMMAND test_planet_persistence)
add_test(NAME test_simple_planet COMMAND test_simple_planet)

# Manual test registration for ViewModel tests to avoid discovery issues
add_test(NAME test_systemproperties_viewmodel_manual COMMAND test_systemproperties_viewmodel)
add_test(NAME test_importexport_viewmodel_manual COMMAND test_importexport_viewmodel)

# Set target properties for all test executables
foreach(target test_simple test_galaxy test_systemdatamanager test_xmlvalidator test_integration test_planetlistmodel test_planet_persistence test_simple_planet test_systemproperties_viewmodel test_importexport_viewmodel)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()

# Enable automatic MOC for ViewModel tests
set_target_properties(test_systemproperties_viewmodel PROPERTIES
    AUTOMOC ON
)
set_target_properties(test_importexport_viewmodel PROPERTIES
    AUTOMOC ON
)

# Enable testing
enable_testing()
