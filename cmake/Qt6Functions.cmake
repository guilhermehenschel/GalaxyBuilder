# Galaxy Builder - Qt6 Build Configuration and Macros
# This file contains Qt6-specific build functions and macros

# Function to setup Qt6 for a target using modern Qt6 CMake API
function(galaxy_builder_setup_qt6 target_name)
    # Link Qt6 libraries
    target_link_libraries(${target_name} PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
    )
    
    # Set Qt6-specific properties
    set_target_properties(${target_name} PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )
    
    # Platform-specific Qt6 setup
    if(WIN32)
        # Windows-specific Qt6 setup
        if(Qt6_VERSION VERSION_GREATER_EQUAL "6.2.0")
            qt_import_plugins(${target_name}
                INCLUDE Qt6::QWindowsIntegrationPlugin
            )
        endif()
        
    elseif(UNIX AND NOT APPLE)
        # Linux-specific Qt6 setup
        if(Qt6_VERSION VERSION_GREATER_EQUAL "6.2.0")
            qt_import_plugins(${target_name}
                INCLUDE Qt6::QXcbIntegrationPlugin
            )
        endif()
        
    elseif(APPLE)
        # macOS-specific Qt6 setup
        if(Qt6_VERSION VERSION_GREATER_EQUAL "6.2.0")
            qt_import_plugins(${target_name}
                INCLUDE Qt6::QCocoaIntegrationPlugin
            )
        endif()
    endif()
    
    # Add Qt6 compile features - use C++23 to match project standard
    target_compile_features(${target_name} PRIVATE cxx_std_23)
    
    # Qt6 compile definitions for the target
    target_compile_definitions(${target_name} PRIVATE
        QT_DEPRECATED_WARNINGS
        QT_DISABLE_DEPRECATED_BEFORE=0x060300
    )
    
    # Set Qt6 properties for better debugging
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${target_name} PRIVATE
            QT_QML_DEBUG
            QT_DECLARATIVE_DEBUG
        )
    endif()
endfunction()

# Function to add Qt6 resources to a target
function(galaxy_builder_add_qt_resources target_name)
    # Check if resources directory exists
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources")
        # Add .qrc files if they exist using the modern Qt6 approach
        file(GLOB QRC_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "resources/*.qrc")
        if(QRC_FILES)
            # Create a simple resource target
            set(RESOURCE_TARGET "${target_name}_resources")
            
            # Add resources to the target directly
            target_sources(${target_name} PRIVATE ${QRC_FILES})
            
            message(STATUS "Added Qt6 resources: ${QRC_FILES}")
        else()
            message(STATUS "No Qt6 resource files (.qrc) found in resources/ directory")
        endif()
        
        # Also check for individual resource files
        file(GLOB RESOURCE_FILES 
            "${CMAKE_SOURCE_DIR}/resources/*.png"
            "${CMAKE_SOURCE_DIR}/resources/*.jpg"
            "${CMAKE_SOURCE_DIR}/resources/*.svg"
            "${CMAKE_SOURCE_DIR}/resources/*.ico"
        )
        if(RESOURCE_FILES)
            message(STATUS "Found resource files: ${RESOURCE_FILES}")
        endif()
    else()
        message(STATUS "Resources directory not found, skipping resource setup")
    endif()
endfunction()

# Function to setup Qt6 application properties
function(galaxy_builder_setup_qt_app target_name)
    # Set application properties
    set_target_properties(${target_name} PROPERTIES
        # Application metadata
        OUTPUT_NAME "GalaxyBuilder"
        VERSION ${PROJECT_VERSION}
        
        # Qt-specific properties
        QT_TARGET_TYPE "Application"
        QT_QML_ROOT_PATH "${CMAKE_SOURCE_DIR}"
    )
    
    # Platform-specific application setup
    if(WIN32)
        set_target_properties(${target_name} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
        
        # Set Windows-specific properties
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(${target_name} PROPERTIES
                LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
            )
        endif()
        
    elseif(APPLE)
        set_target_properties(${target_name} PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/Info.plist.in"
        )
    endif()
endfunction()
