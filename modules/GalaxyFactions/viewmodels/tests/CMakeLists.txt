cmake_minimum_required(VERSION 3.24)

project(GalaxyFactionsViewModelsTests)

# Find required packages
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Test Qml Quick)

# Enable Qt's MOC for this test project
set(CMAKE_AUTOMOC ON)

# Set C++ standard to match project requirements
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Test source files
set(TEST_SOURCES
    test_faction_viewmodel.cpp
    test_faction_list_model.cpp
    test_galaxy_factions_viewmodel.cpp
    test_system_viewmodel.cpp
    test_system_list_model.cpp
    test_system_resource_bonus_viewmodel.cpp
    test_system_resource_bonus_list_model.cpp
)

# Create test executables
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    target_link_libraries(${TEST_NAME}
        PRIVATE
            GalaxyFactionsViewModelsLib
            GGH::Galaxy::Factions::Models
            GGH::GalaxyCore::Models
            GGH::GalaxyCore::Utilities
            Qt6::Core
            Qt6::Test
            Qt6::Qml
            Qt6::Quick
    )
    
    target_include_directories(${TEST_NAME}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    
    # Set properties for test executable
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )
    
    # Add the test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# Enable testing
enable_testing()
