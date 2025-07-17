cmake_minimum_required(VERSION 3.24)

# Unified Tests for Galaxy Builder
# project(GalaxyExporterTests VERSION 1.0.0 LANGUAGES CXX)

include(googletest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create unified test executable for all GTest-based tests
add_executable(GalaxyExporterTests
    test_exporter.cpp
    test_GalaxyXMLExporter.cpp
    test_StarStystemXMLExporter.cpp
)

add_dependencies(GalaxyExporterTests GalaxyExporter)

target_link_libraries(GalaxyExporterTests PRIVATE
    gtest_main
    GalaxyExporter
    GGH::GalaxyCore::ViewModels
    GGH::GalaxyCore::Models
    Qt6::Core
    Qt6::Test
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
                    add_custom_command(TARGET GalaxyExporterTests POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                            "${_qt_bin_dir}/${_qt_dll}.dll"
                            $<TARGET_FILE_DIR:GalaxyExporterTests>
                        COMMENT "Copying ${_qt_dll}.dll"
                    )
                endif()
            endif()
        endif()
    endforeach()
    
    # Copy additional project DLLs to the test executable directory only if they exist
    set(_dll_targets GalaxyExporterd GalaxyCoreModelsd GalaxyCoreUtilitiesd GalaxyCoreViewModelsd)
    foreach(_dll ${_dll_targets})
        if(TARGET ${_dll})
            add_custom_command(TARGET GalaxyExporterTests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${_dll}>
                    $<TARGET_FILE_DIR:GalaxyExporterTests>
                COMMENT "Copying ${_dll}.dll to test executable directory"
            )
        endif()
    endforeach()
endif()

gtest_discover_tests(GalaxyExporterTests)
