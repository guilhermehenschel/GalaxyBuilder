# Galaxy Builder - Windows Installation Configuration
# This file handles Windows-specific installation and deployment

function(galaxy_builder_install_qt_windows target_name)
    if(NOT WIN32)
        return()
    endif()
    
    message(STATUS "Configuring Windows Qt6 deployment for ${target_name}")
    
    # Get Qt6 installation directory
    get_target_property(Qt6_CORE_LOCATION Qt6::Core LOCATION_DEBUG)
    if(NOT Qt6_CORE_LOCATION)
        get_target_property(Qt6_CORE_LOCATION Qt6::Core LOCATION_RELEASE)
    endif()
    
    if(Qt6_CORE_LOCATION)
        get_filename_component(Qt6_BIN_DIR "${Qt6_CORE_LOCATION}" DIRECTORY)
        get_filename_component(Qt6_INSTALL_DIR "${Qt6_BIN_DIR}" DIRECTORY)
        
        message(STATUS "Qt6 installation directory: ${Qt6_INSTALL_DIR}")
        
        # Try to use windeployqt for comprehensive deployment
        find_program(WINDEPLOYQT_EXECUTABLE 
            NAMES windeployqt
            HINTS ${Qt6_BIN_DIR}
            DOC "Path to windeployqt executable"
        )
        
        if(WINDEPLOYQT_EXECUTABLE)
            message(STATUS "Using windeployqt for comprehensive Qt deployment")
            
            # Install using windeployqt (this is more comprehensive than manual DLL copying)
            install(CODE "
                execute_process(
                    COMMAND ${WINDEPLOYQT_EXECUTABLE} 
                        --dir \${CMAKE_INSTALL_PREFIX}/bin
                        --qmldir ${CMAKE_CURRENT_SOURCE_DIR}/qml
                        --compiler-runtime
                        \$<TARGET_FILE:${target_name}>
                    RESULT_VARIABLE WINDEPLOYQT_RESULT
                )
                if(NOT WINDEPLOYQT_RESULT EQUAL 0)
                    message(WARNING \"windeployqt failed with result: \${WINDEPLOYQT_RESULT}\")
                endif()
            ")
        else()
            message(STATUS "windeployqt not found, falling back to manual Qt deployment")
            
            # Fallback: manual DLL installation (simplified version)
            set(QT6_DLLS
                "Qt6Core.dll" "Qt6Cored.dll"
                "Qt6Gui.dll" "Qt6Guid.dll" 
                "Qt6Widgets.dll" "Qt6Widgetsd.dll"
                "Qt6Qml.dll" "Qt6Qmld.dll"
                "Qt6Quick.dll" "Qt6Quickd.dll"
                "Qt6QuickControls2.dll" "Qt6QuickControls2d.dll"
            )
            
            # Install Qt DLLs
            foreach(DLL ${QT6_DLLS})
                install(FILES "${Qt6_BIN_DIR}/${DLL}"
                    DESTINATION bin
                    OPTIONAL
                )
            endforeach()
            
            # Install essential plugins
            set(PLATFORM_PLUGINS_DIR "${Qt6_INSTALL_DIR}/plugins/platforms")
            if(EXISTS "${PLATFORM_PLUGINS_DIR}")
                install(FILES
                    "${PLATFORM_PLUGINS_DIR}/qwindows.dll"
                    "${PLATFORM_PLUGINS_DIR}/qwindowsd.dll"
                    DESTINATION bin/platforms
                    OPTIONAL
                )
            endif()
        endif()
        
        # Install Visual C++ redistributables
        include(InstallRequiredSystemLibraries)
        
    else()
        message(WARNING "Could not determine Qt6 installation directory for Windows deployment")
    endif()
endfunction()

# Function to create Windows installer package
function(galaxy_builder_create_windows_package target_name)
    if(NOT WIN32)
        return()
    endif()
    
    # Find NSIS or WiX for creating installers
    find_program(NSIS_MAKE makensis)
    
    if(NSIS_MAKE)
        message(STATUS "Found NSIS: ${NSIS_MAKE}")
        
        # Configure CPack for NSIS
        set(CPACK_GENERATOR "NSIS")
        set(CPACK_PACKAGE_NAME "Galaxy Builder")
        set(CPACK_PACKAGE_VENDOR "Galaxy Builder Team")
        set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Stellaris-style procedural galaxy map generator")
        set(CPACK_PACKAGE_EXECUTABLES "${target_name};Galaxy Builder")
        set(CPACK_CREATE_DESKTOP_LINKS "${target_name}")
        
        # NSIS specific settings
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        set(CPACK_NSIS_PACKAGE_NAME "Galaxy Builder")
        set(CPACK_NSIS_DISPLAY_NAME "Galaxy Builder")
        
        include(CPack)
        
    else()
        message(STATUS "NSIS not found, skipping Windows installer creation")
    endif()
endfunction()

# Function to run windeployqt during build
function(galaxy_builder_deploy_qt_windows target_name)
    if(NOT WIN32)
        return()
    endif()
    
    message(STATUS "Configuring windeployqt for ${target_name}")
    
    # Find windeployqt executable
    find_program(WINDEPLOYQT_EXECUTABLE 
        NAMES windeployqt
        HINTS 
            ${Qt6_DIR}/../../../bin
            ${QT_INSTALL_PREFIX}/bin
            $ENV{QT_ROOT}/bin
            $ENV{QTDIR}/bin
        DOC "Path to windeployqt executable"
    )
    
    if(WINDEPLOYQT_EXECUTABLE)
        message(STATUS "Found windeployqt: ${WINDEPLOYQT_EXECUTABLE}")
        
        # Get the target output directory
        get_target_property(TARGET_RUNTIME_OUTPUT_DIRECTORY ${target_name} RUNTIME_OUTPUT_DIRECTORY)
        if(NOT TARGET_RUNTIME_OUTPUT_DIRECTORY)
            set(TARGET_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        endif()
        
        # Create custom command to run windeployqt after build
        add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
                --qmldir ${CMAKE_CURRENT_SOURCE_DIR}/qml
                --dir ${TARGET_RUNTIME_OUTPUT_DIRECTORY}
                --compiler-runtime
                $<TARGET_FILE:${target_name}>
            COMMENT "Running windeployqt for ${target_name}"
            VERBATIM
        )
        
        message(STATUS "windeployqt will run automatically after building ${target_name}")
    else()
        message(WARNING "windeployqt not found. Qt runtime dependencies will not be automatically deployed.")
        message(STATUS "Make sure Qt bin directory is in your PATH or set Qt6_DIR correctly.")
    endif()
endfunction()
