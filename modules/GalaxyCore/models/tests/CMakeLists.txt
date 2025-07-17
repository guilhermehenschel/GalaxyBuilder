cmake_minimum_required(VERSION 3.24)

# Unified Tests for Galaxy Builder
project(GalaxyCoreModelsTests VERSION 1.0.0 LANGUAGES CXX)

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
add_executable(GalaxyCoreModelsTests
    test_GalaxyModel.cpp
    test_PlanetModel.cpp
    test_StarSystemModel.cpp
    test_TravelLaneModel.cpp
)

target_link_libraries(GalaxyCoreModelsTests PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Qml
    Qt6::Xml
    Qt6::Widgets
    GGH::GalaxyCore::Models
)
if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/WindowsInstall.cmake)
    galaxy_builder_deploy_qt_windows(GalaxyCoreModelsTests)
    # Refactored: Copy additional project DLLs to the test executable directory only if they exist
    set(_dll_targets GalaxyCoreUtilitiesd GalaxyCoreViewModelsd)
    foreach(_dll ${_dll_targets})
        if(TARGET ${_dll})
            add_custom_command(TARGET GalaxyCoreModelsTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${_dll}>
                    $<TARGET_FILE_DIR:GalaxyCoreModelsTests>
                COMMENT "Copying ${_dll}.dll to test executable directory"
            )
        endif()
    endforeach()
endif()

gtest_discover_tests(GalaxyCoreModelsTests)
