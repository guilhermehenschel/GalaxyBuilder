cmake_minimum_required(VERSION 3.24)

# Project definition (must be at top level)
project(GalaxyBuilder 
    VERSION 1.0.0 
    LANGUAGES CXX
    DESCRIPTION "Stellaris-style procedural galaxy map generator"
    HOMEPAGE_URL "https://github.com/guilhermehenschel/GalaxyBuilder"
)

# Build options
option(BUILD_TESTING "Build tests" ON)
option(BUILD_QT_APP "Build Qt application" ON)

# Include project configuration
include(cmake/ProjectConfig.cmake)

# Include third-party dependencies
include(third_party/third_party.cmake)

# Set up output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build the core module first
add_subdirectory(modules)

# Build the application if enabled
if(BUILD_QT_APP)
    add_subdirectory(app)
endif()

# Enable testing if requested
if(BUILD_TESTING)
    enable_testing()
endif()

# Include packaging configuration
include(cmake/PackagingConfig.cmake)

# Setup packaging and installers for the app
galaxy_builder_setup_packaging(GalaxyBuilderApp)
