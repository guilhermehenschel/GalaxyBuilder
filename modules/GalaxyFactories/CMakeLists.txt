cmake_minimum_required(VERSION 3.24)

# GalaxyFactories Module
project(GalaxyFactories VERSION 1.0.0 LANGUAGES CXX)

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
    include/ggh/modules/GalaxyFactories/AbstractGalaxyFactory.h
    include/ggh/modules/GalaxyFactories/GalaxyGenerator.h
    include/ggh/modules/GalaxyFactories/Types.h
    include/ggh/modules/GalaxyFactories/XmlGalaxyImporter.h
)

# Core library source files
set(GALAXY_FACTORIES_SOURCES
    src/GalaxyGenerator.cpp
    src/XmlGalaxyImporter.cpp
)



# Create the core library
qt_add_qml_module(GalaxyFactories 
    URI Galaxy.Factories
    VERSION 1.0
    SOURCES
        ${GALAXY_FACTORIES_SOURCES}
        ${GALAXY_FACTORIES_HEADERS}
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/Galaxy/Factories
)

# Set target properties
set_target_properties(GalaxyFactories PROPERTIES
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
target_link_libraries(GalaxyFactories
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
target_compile_definitions(GalaxyFactories PRIVATE GalaxyFactories_LIBRARY)

# Create an alias for the library
add_library(GGH::GalaxyFactories ALIAS GalaxyFactories)

# Include directories
target_include_directories(GalaxyFactories
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/GalaxyFactories>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add custom command to deploy QML plugin to runtime location
add_custom_command(TARGET GalaxyFactories POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factories"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/qml/Galaxy/Factories/qmldir"
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factories/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/qml/Galaxy/Factories/GalaxyFactories.qmltypes"
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factories/"
    COMMENT "Deploying GalaxyFactories QML plugin to runtime location"
)

# Add separate command to copy plugin DLL after the plugin is built
add_custom_command(TARGET GalaxyFactoriesplugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:GalaxyFactoriesplugin>"
        "${CMAKE_BINARY_DIR}/bin/qml/Galaxy/Factories/"
    COMMENT "Copying GalaxyFactories plugin DLL"
)

# Install the library (without export since it's a QML module)
install(TARGETS GalaxyFactories
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyFactories
)

# Install headers
install(FILES ${GALAXY_FACTORIES_HEADERS}
    DESTINATION include/GalaxyFactories
)
