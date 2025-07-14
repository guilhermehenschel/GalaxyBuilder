cmake_minimum_required(VERSION 3.24)

# GalaxyCore Module
project(GalaxyCore VERSION 1.0.0 LANGUAGES CXX)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Xml)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set Qt policies
if(COMMAND qt_policy)
    qt_policy(SET QTP0001 NEW)
endif()

# Core library source files
set(GALAXY_CORE_SOURCES
    StarSystem.cpp
    StarSystemQml.cpp
    Planet.cpp
    PlanetListModel.cpp
    TravelLane.cpp
    Galaxy.cpp
    GalaxyGenerator.cpp
    GalaxyController.cpp
    StarSystemListModel.cpp
    TravelLaneListModel.cpp
    SystemDataManager.cpp
    XmlValidator.cpp
    SystemPropertiesViewModel.cpp
    ImportExportViewModel.cpp
)

# Core library header files
set(GALAXY_CORE_HEADERS
    Common.h
    StarSystem.h
    StarSystemQml.h
    Planet.h
    PlanetListModel.h
    TravelLane.h
    Galaxy.h
    GalaxyGenerator.h
    GalaxyController.h
    StarSystemListModel.h
    TravelLaneListModel.h
    SystemDataManager.h
    galaxycore_global.h
    SystemPropertiesViewModel.h
    ImportExportViewModel.h
)

# Create the core library
qt_add_qml_module(GalaxyCore 
    URI GalaxyCore
    VERSION 1.0
    SOURCES
        ${GALAXY_CORE_SOURCES}
        ${GALAXY_CORE_HEADERS}
)

# Set target properties
set_target_properties(GalaxyCore PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    # Output directories
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib  
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Link Qt6 libraries
target_link_libraries(GalaxyCore
    PUBLIC
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        Qt6::Xml
)

# Define library export symbol
target_compile_definitions(GalaxyCore PRIVATE GALAXYCORE_LIBRARY)

# Include directories
target_include_directories(GalaxyCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/GalaxyCore>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Export the library
install(TARGETS GalaxyCore
    EXPORT GalaxyCoreTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyCore
)

# Install headers
install(FILES ${GALAXY_CORE_HEADERS}
    DESTINATION include/GalaxyCore
)

# Export configuration
install(EXPORT GalaxyCoreTargets
    FILE GalaxyCoreTargets.cmake
    NAMESPACE GalaxyCore::
    DESTINATION lib/cmake/GalaxyCore
)
