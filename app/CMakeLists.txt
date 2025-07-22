cmake_minimum_required(VERSION 3.24)

# Galaxy Builder App
project(GalaxyBuilderApp VERSION 1.0.0 LANGUAGES CXX)

# Qt6 is found via third_party/third_party.cmake

# Set Qt policies to suppress warnings
qt_policy(SET QTP0001 NEW)
# Note: QTP0004 may not exist in all Qt versions, so commenting out for compatibility
qt_policy(SET QTP0004 NEW)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Application source files
set(APP_SOURCES
    main.cpp
    GalaxyController.cpp
)

# Create the executable
qt_add_executable(GalaxyBuilderApp
    ${APP_SOURCES}
)

# Add QML module with all QML files
qt_add_qml_module(GalaxyBuilderApp
    URI GalaxyBuilderApp
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/components/ImportExportManager.qml
        qml/components/ParameterPanel.qml
        qml/components/PlanetDetailsDialog.qml
        qml/components/PlanetListView.qml
        qml/components/StarSystemVisualizer.qml
        qml/components/SystemNode.qml
        qml/components/TravelLane.qml
        qml/components/PropertyPanel.qml
        qml/components/ZoomControls.qml
        qml/components/ZoomIndicator.qml
        qml/components/SystemPropertiesDialog.qml
        qml/components/SystemPropertiesManager.qml
        qml/components/GalaxyManager.qml
        qml/views/GalaxyView.qml
        qml/components/PlanetPropertyView.qml
)

# Link against Qt6 and GalaxyCore
target_link_libraries(GalaxyBuilderApp PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    GGH::GalaxyCore::ViewModels
    GGH::GalaxyCore::Models
    GGH::Galaxy::Factions::ViewModels
    GGH::GalaxyExporter
    GGH::GalaxyFactories
)

# Add custom command to ensure all QML modules are deployed before running the app
add_custom_command(TARGET GalaxyBuilderApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Ensuring all QML modules are deployed to runtime location..."
    COMMENT "QML Module deployment verification"
)

# Make sure the app depends on all QML module targets to ensure proper build order
add_dependencies(GalaxyBuilderApp 
    GalaxyCoreViewModels
    GalaxyCoreViewModelsplugin
    GalaxyFactionsViewModels
    GalaxyFactionsViewModelsplugin
    GalaxyFactories
    GalaxyFactoriesplugin
)

# Set target properties

# Set WIN32_EXECUTABLE to FALSE for Debug builds on Windows to enable console output
if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(WIN32_EXECUTABLE_FLAG FALSE)
    else()
        set(WIN32_EXECUTABLE_FLAG TRUE)
    endif()
else()
    set(WIN32_EXECUTABLE_FLAG TRUE)
endif()

set_target_properties(GalaxyBuilderApp PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    WIN32_EXECUTABLE ${WIN32_EXECUTABLE_FLAG}
    MACOSX_BUNDLE TRUE
    OUTPUT_NAME "GalaxyBuilder"
)

# Platform-specific properties
if(WIN32)
    # Set application icon on Windows
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../resources/galaxy-builder.ico")
        target_sources(GalaxyBuilderApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../resources/galaxy-builder.ico")
        set_target_properties(GalaxyBuilderApp PROPERTIES
            WIN32_EXECUTABLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../resources/galaxy-builder.ico"
        )
    endif()
elseif(APPLE)
    set_target_properties(GalaxyBuilderApp PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "Galaxy Builder"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
        MACOSX_BUNDLE_IDENTIFIER "com.galaxybuilder.app"
    )
    
    # Set application icon on macOS
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../resources/galaxy-builder.icns")
        set_target_properties(GalaxyBuilderApp PROPERTIES
            MACOSX_BUNDLE_ICON_FILE "galaxy-builder.icns"
        )
        target_sources(GalaxyBuilderApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../resources/galaxy-builder.icns")
        set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/../resources/galaxy-builder.icns" PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
        )
    endif()
endif()

# Include directories
target_include_directories(GalaxyBuilderApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/GalaxyCore
)

# Install target
install(TARGETS GalaxyBuilderApp
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Deploy Qt runtime dependencies during build (Windows)
if(WIN32)
    include(${CMAKE_SOURCE_DIR}/cmake/WindowsInstall.cmake)
    galaxy_builder_deploy_qt_windows(GalaxyBuilderApp)
endif()

# Install Qt6 runtime dependencies for packaging
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${Qt6_DIR}/../../../bin)
    if(WINDEPLOYQT_EXECUTABLE)
        install(CODE "
            execute_process(COMMAND ${WINDEPLOYQT_EXECUTABLE} 
                --qmldir ${CMAKE_CURRENT_SOURCE_DIR}/qml 
                --dir \${CMAKE_INSTALL_PREFIX}/bin
                \$<TARGET_FILE:GalaxyBuilderApp>)
        ")
    endif()
elseif(APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS ${Qt6_DIR}/../../../bin)
    if(MACDEPLOYQT_EXECUTABLE)
        install(CODE "
            execute_process(COMMAND ${MACDEPLOYQT_EXECUTABLE} \$<TARGET_BUNDLE_DIR:GalaxyBuilderApp> -qmldir=${CMAKE_CURRENT_SOURCE_DIR}/qml)
        ")
    endif()
endif()
