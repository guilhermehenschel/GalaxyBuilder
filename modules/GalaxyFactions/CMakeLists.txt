cmake_minimum_required(VERSION 3.24)

# GalaxyFactions Module
project(GalaxyFactions VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt-free module - no Qt dependencies

# Core library header files
set(GALAXY_FACTIONS_HEADERS
    include/ggh/modules/GalaxyFactions/GalaxyFactions.h
    include/ggh/modules/GalaxyFactions/Faction.h
    include/ggh/modules/GalaxyFactions/System.h
    include/ggh/modules/GalaxyFactions/ResourceType.h
    include/ggh/modules/GalaxyFactions/SystemResourceBonus.h
    include/ggh/modules/GalaxyFactions/SystemNotes.h
    include/ggh/modules/GalaxyFactions/FactionHandover.h
    include/ggh/modules/GalaxyFactions/HandoverType.h
)

# Core library source files
set(GALAXY_FACTIONS_SOURCES
    src/GalaxyFactions.cpp
    src/Faction.cpp
    src/System.cpp
    src/ResourceType.cpp
    src/SystemResourceBonus.cpp
    src/SystemNotes.cpp
    src/FactionHandover.cpp
    src/HandoverType.cpp
)

# Create the core library as a standard C++ library
add_library(GalaxyFactions STATIC
    ${GALAXY_FACTIONS_SOURCES}
    ${GALAXY_FACTIONS_HEADERS}
)

# Set target properties
set_target_properties(GalaxyFactions PROPERTIES
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

# Link standard C++ libraries and minimal dependencies
target_link_libraries(GalaxyFactions
    PUBLIC
      GGH::GalaxyCore::Models
)

# Define library export symbol
target_compile_definitions(GalaxyFactions PRIVATE GalaxyFactions_LIBRARY)

# Create an alias for the library
add_library(GGH::Galaxy::Factions ALIAS GalaxyFactions)

# Include directories
target_include_directories(GalaxyFactions
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/GalaxyFactions>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Enable testing for this module
if(${BUILD_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

# Install the library (without export since it's a QML module)
install(TARGETS GalaxyFactions
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyFactions
)

# Install headers
install(FILES ${GALAXY_FACTIONS_HEADERS}
    DESTINATION include/GalaxyFactions
)
