cmake_minimum_required(VERSION 3.24)

# GalaxyExporter Module
project(GalaxyExporter VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set Qt policies
qt_policy(SET QTP0001 NEW)

# Enable testing for this module
if(${BUILD_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

# Core library header files
set(GALAXY_FACTORIES_HEADERS
    include/ggh/modules/GalaxyExporter/galaxyexporter_export.h
    include/ggh/modules/GalaxyExporter/AbstractExporter.h
    include/ggh/modules/GalaxyExporter/ExporterObject.h
    include/ggh/modules/GalaxyExporter/GalaxyXMLExporter.h
    include/ggh/modules/GalaxyExporter/StarSystemXMLExporter.h
)

# Core library source files
set(GALAXY_FACTORIES_SOURCES
    src/ExporterObject.cpp
    src/GalaxyXMLExporter.cpp
    src/StarSystemXMLExporter.cpp
)



# Create the core library
qt_add_qml_module(GalaxyExporter 
    URI Galaxy.Exporter
    VERSION 1.0
    SOURCES
        ${GALAXY_FACTORIES_SOURCES}
        ${GALAXY_FACTORIES_HEADERS}
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/Galaxy/Exporter
)

# Set target properties
set_target_properties(GalaxyExporter PROPERTIES
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
target_link_libraries(GalaxyExporter
    PUBLIC
        GGH::GalaxyCore::Models
        GGH::GalaxyCore::ViewModels
        GGH::GalaxyCore::Utilities
        Qt6::Core
        Qt6::Qml
        Qt6::Quick
        Qt6::Xml
)

# Define library export symbol
target_compile_definitions(GalaxyExporter PRIVATE GalaxyExporter_LIBRARY)

# Create an alias for the library
add_library(GGH::GalaxyExporter ALIAS GalaxyExporter)

# Include directories
target_include_directories(GalaxyExporter
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/GalaxyExporter>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ggh/modules/GalaxyExporter
)

# Install the library (without export since it's a QML module)
install(TARGETS GalaxyExporter
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyExporter
)

# Install headers
install(FILES ${GALAXY_FACTORIES_HEADERS}
    DESTINATION include/GalaxyExporter
)
