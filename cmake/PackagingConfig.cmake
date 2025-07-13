# Galaxy Builder - Installer Configuration
# This file contains installer and packaging configuration

# Function to create Windows installer using NSIS
function(galaxy_builder_create_installer target_name)
    if(NOT WIN32)
        return()
    endif()
    
    # Find NSIS
    find_program(NSIS_MAKENSIS makensis
        PATHS
            "C:/Program Files (x86)/NSIS"
            "C:/Program Files/NSIS"
        DOC "NSIS makensis executable"
    )
    
    if(NSIS_MAKENSIS)
        message(STATUS "Found NSIS: ${NSIS_MAKENSIS}")
        
        # Configure installer script
        set(INSTALLER_SOURCE_DIR "${CMAKE_SOURCE_DIR}")
        set(INSTALLER_BUILD_DIR "${CMAKE_BINARY_DIR}")
        set(INSTALLER_BINARY_DIR "${CMAKE_BINARY_DIR}/bin")
        set(INSTALLER_VERSION "${PROJECT_VERSION}")
        set(INSTALLER_NAME "GalaxyBuilder")
        
        # Configure NSIS script
        configure_file(
            "${CMAKE_SOURCE_DIR}/installer/windows/installer.nsi.in"
            "${CMAKE_BINARY_DIR}/installer.nsi"
            @ONLY
        )
        
        # Create installer target
        add_custom_target(installer
            COMMAND ${NSIS_MAKENSIS} "${CMAKE_BINARY_DIR}/installer.nsi"
            DEPENDS ${target_name}
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating Windows installer"
        )
        
        message(STATUS "Windows installer target created. Run 'cmake --build . --target installer' to create installer.")
    else()
        message(STATUS "NSIS not found, skipping Windows installer creation")
    endif()
endfunction()

# Function to create Windows portable package
function(galaxy_builder_create_portable_package target_name)
    if(NOT WIN32)
        return()
    endif()
    
    # Create portable package target
    add_custom_target(portable
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/portable"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/portable"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${target_name}>" "${CMAKE_BINARY_DIR}/portable/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/resources" "${CMAKE_BINARY_DIR}/portable/resources"
        DEPENDS ${target_name}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Creating portable package"
    )
    
    # Copy Qt6 DLLs for portable version
    get_target_property(Qt6_CORE_LOCATION Qt6::Core LOCATION_DEBUG)
    if(NOT Qt6_CORE_LOCATION)
        get_target_property(Qt6_CORE_LOCATION Qt6::Core LOCATION_RELEASE)
    endif()
    
    if(Qt6_CORE_LOCATION)
        get_filename_component(Qt6_BIN_DIR "${Qt6_CORE_LOCATION}" DIRECTORY)
        get_filename_component(Qt6_INSTALL_DIR "${Qt6_BIN_DIR}" DIRECTORY)
        
        # Add Qt6 DLL copying to portable target
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(QT_DLLS "Qt6Cored.dll" "Qt6Guid.dll" "Qt6Widgetsd.dll")
        else()
            set(QT_DLLS "Qt6Core.dll" "Qt6Gui.dll" "Qt6Widgets.dll")
        endif()
        
        foreach(DLL ${QT_DLLS})
            add_custom_command(TARGET portable POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${Qt6_BIN_DIR}/${DLL}"
                    "${CMAKE_BINARY_DIR}/portable/"
                COMMENT "Copying ${DLL} to portable package"
            )
        endforeach()
        
        # Copy Qt6 platform plugins
        set(PLATFORM_PLUGINS_DIR "${Qt6_INSTALL_DIR}/plugins/platforms")
        if(EXISTS "${PLATFORM_PLUGINS_DIR}")
            add_custom_command(TARGET portable POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/portable/platforms"
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PLATFORM_PLUGINS_DIR}"
                    "${CMAKE_BINARY_DIR}/portable/platforms"
                COMMENT "Copying Qt6 platform plugins to portable package"
            )
        endif()
    endif()
    
    message(STATUS "Portable package target created. Run 'cmake --build . --target portable' to create portable package.")
endfunction()

# Function to create Linux AppImage
function(galaxy_builder_create_appimage target_name)
    if(NOT UNIX OR APPLE)
        return()
    endif()
    
    # Find linuxdeploy
    find_program(LINUXDEPLOY linuxdeploy
        PATHS
            "/usr/local/bin"
            "/usr/bin"
            "${CMAKE_SOURCE_DIR}/tools"
        DOC "linuxdeploy executable"
    )
    
    if(LINUXDEPLOY)
        message(STATUS "Found linuxdeploy: ${LINUXDEPLOY}")
        
        # Create AppImage target
        add_custom_target(appimage
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/AppDir"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/AppDir"
            COMMAND ${CMAKE_COMMAND} --install . --prefix "${CMAKE_BINARY_DIR}/AppDir/usr"
            COMMAND ${LINUXDEPLOY} --appdir "${CMAKE_BINARY_DIR}/AppDir" --plugin qt --output appimage
            DEPENDS ${target_name}
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating Linux AppImage"
        )
        
        message(STATUS "AppImage target created. Run 'cmake --build . --target appimage' to create AppImage.")
    else()
        message(STATUS "linuxdeploy not found, skipping AppImage creation")
    endif()
endfunction()

# Function to setup packaging
function(galaxy_builder_setup_packaging target_name)
    # Create installer based on platform
    if(WIN32)
        galaxy_builder_create_installer(${target_name})
        galaxy_builder_create_portable_package(${target_name})
    elseif(UNIX AND NOT APPLE)
        galaxy_builder_create_appimage(${target_name})
    endif()
    
    # Setup CPack for additional packaging options
    set(CPACK_PACKAGE_NAME "GalaxyBuilder")
    set(CPACK_PACKAGE_VENDOR "Galaxy Builder Team")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Stellaris-style procedural galaxy map generator")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "GalaxyBuilder")
    
    # Platform-specific CPack generators
    if(WIN32)
        set(CPACK_GENERATOR "ZIP;NSIS")
        set(CPACK_NSIS_DISPLAY_NAME "Galaxy Builder")
        set(CPACK_NSIS_PACKAGE_NAME "Galaxy Builder")
        set(CPACK_NSIS_CONTACT "galaxybuilder@example.com")
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    elseif(UNIX AND NOT APPLE)
        set(CPACK_GENERATOR "TGZ;DEB;RPM")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Galaxy Builder Team")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6")
        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
        set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase")
    elseif(APPLE)
        set(CPACK_GENERATOR "TGZ;DragNDrop")
        set(CPACK_DMG_VOLUME_NAME "Galaxy Builder")
    endif()
    
    # Include CPack
    include(CPack)
    
    message(STATUS "Packaging configured. Available targets:")
    if(WIN32)
        message(STATUS "  - installer: Create NSIS installer")
        message(STATUS "  - portable: Create portable ZIP package")
    elseif(UNIX AND NOT APPLE)
        message(STATUS "  - appimage: Create Linux AppImage")
    endif()
    message(STATUS "  - package: Create CPack package")
endfunction()
