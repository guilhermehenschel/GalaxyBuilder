cmake_minimum_required(VERSION 3.24)

# GalaxyFactions Module
project(GalaxyFactionsModels VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt-free module - no Qt dependencies

# Core library header files
set(GALAXY_FACTIONS_HEADERS
    include/ggh/modules/GalaxyFactions/models/GalaxyFactions.h
    include/ggh/modules/GalaxyFactions/models/Faction.h
    include/ggh/modules/GalaxyFactions/models/System.h
    include/ggh/modules/GalaxyFactions/models/ResourceType.h
    include/ggh/modules/GalaxyFactions/models/SystemResourceBonus.h
    include/ggh/modules/GalaxyFactions/models/SystemNotes.h
    include/ggh/modules/GalaxyFactions/models/FactionHandover.h
    include/ggh/modules/GalaxyFactions/models/HandoverType.h
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
add_library(GalaxyFactionsModels STATIC
    ${GALAXY_FACTIONS_SOURCES}
    ${GALAXY_FACTIONS_HEADERS}
)

# Set target properties
set_target_properties(GalaxyFactionsModels PROPERTIES
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
target_link_libraries(GalaxyFactionsModels
    PUBLIC
      GGH::GalaxyCore::Models
)

# Define library export symbol
target_compile_definitions(GalaxyFactionsModels PRIVATE GalaxyFactions_LIBRARY)

# Create an alias for the library
add_library(GGH::Galaxy::Factions::Models ALIAS GalaxyFactionsModels)

# Include directories
target_include_directories(GalaxyFactionsModels
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/GalaxyFactions/models>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Enable testing for this module
if(${BUILD_TESTING})
    enable_testing()
    add_subdirectory(tests)
endif()

# Install the library (without export since it's a QML module)
install(TARGETS GalaxyFactionsModels
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyFactions
)

# Install headers
install(FILES ${GALAXY_FACTIONS_HEADERS}
    DESTINATION include/GalaxyFactions
)
