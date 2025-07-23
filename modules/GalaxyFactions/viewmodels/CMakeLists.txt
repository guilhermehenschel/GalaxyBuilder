cmake_minimum_required(VERSION 3.24)

project(GalaxyFactionsViewModels VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard to match project requirements
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find Qt6 components
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Qml Quick)
qt_policy(SET QTP0001 NEW)

# Automatically handle MOC, RCC, and UIC for this target
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Header files
set(VIEWMODELS_HEADERS
    include/ggh/modules/GalaxyFactions/viewmodels/Commons.h
    include/ggh/modules/GalaxyFactions/viewmodels/FactionViewModel.h
    include/ggh/modules/GalaxyFactions/viewmodels/FactionListModel.h
    include/ggh/modules/GalaxyFactions/viewmodels/SystemViewModel.h
    include/ggh/modules/GalaxyFactions/viewmodels/SystemListModel.h
    include/ggh/modules/GalaxyFactions/viewmodels/SystemResourceBonusViewModel.h
    include/ggh/modules/GalaxyFactions/viewmodels/SystemResourceBonusListModel.h
    include/ggh/modules/GalaxyFactions/viewmodels/GalaxyFactionsViewModel.h
)

# Source files
set(VIEWMODELS_SOURCES
    src/FactionViewModel.cpp
    src/FactionListModel.cpp
    src/SystemViewModel.cpp
    src/SystemListModel.cpp
    src/SystemResourceBonusViewModel.cpp
    src/SystemResourceBonusListModel.cpp
    src/GalaxyFactionsViewModel.cpp
)

# Create the QML module
qt_add_qml_module(GalaxyFactionsViewModels
    URI Galaxy.Factions.ViewModels
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/Galaxy/Factions/ViewModels
    SOURCES ${VIEWMODELS_SOURCES} ${VIEWMODELS_HEADERS}
)

# Create the main library first
add_library(GalaxyFactionsViewModelsLib STATIC
    ${VIEWMODELS_SOURCES}
    ${VIEWMODELS_HEADERS}
)

# Set target properties
set_target_properties(GalaxyFactionsViewModels PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Include directories
target_include_directories(GalaxyFactionsViewModels
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ggh/modules/GalaxyFactions/viewmodels
)

# Link libraries for QML module
target_link_libraries(GalaxyFactionsViewModels
    PUBLIC
        GGH::Galaxy::Factions::Models
        GGH::GalaxyCore::Models
        GGH::GalaxyCore::Utilities
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
)

target_link_libraries(GalaxyFactionsViewModelsLib
    PUBLIC
        GGH::Galaxy::Factions::Models
        GGH::GalaxyCore::Models
        GGH::GalaxyCore::Utilities
        Qt6::Gui
        Qt6::Core
        Qt6::Qml
)

target_include_directories(GalaxyFactionsViewModelsLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Create alias for the library
add_library(GGH::Galaxy::Factions::ViewModels ALIAS GalaxyFactionsViewModelsLib)

# Enable testing for this module
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Add custom command to deploy QML plugin to runtime location
add_custom_command(TARGET GalaxyFactionsViewModels POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factions/ViewModels"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/qml/Galaxy/Factions/ViewModels/qmldir"
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factions/ViewModels/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/qml/Galaxy/Factions/ViewModels/GalaxyFactionsViewModels.qmltypes"
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factions/ViewModels/"
    COMMENT "Deploying GalaxyFactionsViewModels QML plugin to runtime location"
)

# Add separate command to copy plugin DLL after the plugin is built
add_custom_command(TARGET GalaxyFactionsViewModelsplugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:GalaxyFactionsViewModelsplugin>"
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factions/ViewModels/"
    COMMENT "Copying GalaxyFactionsViewModels plugin DLL"
)

# Install the library
install(TARGETS GalaxyFactionsViewModels GalaxyFactionsViewModelsLib
    EXPORT GalaxyFactionsTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyFactions
)

# Install headers
install(FILES ${VIEWMODELS_HEADERS}
    DESTINATION include/GalaxyFactions/viewmodels
)
