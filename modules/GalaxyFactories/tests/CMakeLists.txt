cmake_minimum_required(VERSION 3.24)

# Unified Tests for Galaxy Builder
# project(GalaxyFactoriesTests VERSION 1.0.0 LANGUAGES CXX)

include(googletest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create unified test executable for all GTest-based tests
add_executable(GalaxyFactoriesTests
    test_GalaxyGenerator.cpp
    test_AbstractGalaxyFactory.cpp
    test_GalaxyGenerator_Full.cpp
    test_GalaxyParameterRespect.cpp
    test_XmlGalaxyImporter.cpp
    test_planet_generation.cpp
)

target_link_libraries(GalaxyFactoriesTests PRIVATE
    gtest_main
    GGH::GalaxyFactories
    Qt6::Core
    Qt6::Xml
)
if(WIN32)
    # Manually copy essential Qt DLLs for test executable (avoiding windeployqt issues)
    set(_qt_dlls Qt6Cored Qt6Cored Qt6Xml Qt6Xmld)
    foreach(_qt_dll ${_qt_dlls})
        if(TARGET Qt6::Core)
            get_target_property(_qt_location Qt6::Core IMPORTED_LOCATION_DEBUG)
            if(NOT _qt_location)
                get_target_property(_qt_location Qt6::Core IMPORTED_LOCATION)
            endif()
            if(_qt_location)
                get_filename_component(_qt_bin_dir "${_qt_location}" DIRECTORY)
                if(EXISTS "${_qt_bin_dir}/${_qt_dll}.dll")
                    add_custom_command(TARGET GalaxyFactoriesTests POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${_qt_bin_dir}/${_qt_dll}.dll"
                            $<TARGET_FILE_DIR:GalaxyFactoriesTests>
                        COMMENT "Copying ${_qt_dll}.dll"
                    )
                endif()
            endif()
        endif()
    endforeach()
    
    # Copy additional project DLLs to the test executable directory only if they exist
    set(_dll_targets GalaxyFactoriesd GalaxyCoreModelsd GalaxyCoreUtilitiesd GalaxyCoreViewModelsd)
    foreach(_dll ${_dll_targets})
        if(TARGET ${_dll})
            add_custom_command(TARGET GalaxyFactoriesTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${_dll}>
                    $<TARGET_FILE_DIR:GalaxyFactoriesTests>
                COMMENT "Copying ${_dll}.dll to test executable directory"
            )
        endif()
    endforeach()
endif()

gtest_discover_tests(GalaxyFactoriesTests)
