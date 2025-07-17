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

if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/WindowsInstall.cmake)
    galaxy_builder_deploy_qt_windows(GalaxyCoreViewModelsTests)
    # Refactored: Copy additional project DLLs to the test executable directory only if they exist
    set(_dll_targets GalaxyCoreModelsd GalaxyCoreUtilitiesd GalaxyCoreViewModelsd)
    foreach(_dll ${_dll_targets})
        if(TARGET ${_dll})
            add_custom_command(TARGET GalaxyCoreViewModelsTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${_dll}>
                    $<TARGET_FILE_DIR:GalaxyCoreViewModelsTests>
                COMMENT "Copying ${_dll}.dll to test executable directory"
            )
        endif()
    endforeach()
endif()

gtest_discover_tests(GalaxyCoreViewModelsTests)
