project(GalaxyCoreModels VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set Qt policies
qt_policy(SET QTP0001 NEW)

set(HEADER_FILES
    include/ggh/modules/GalaxyCore/models/GalaxyModel.h
    include/ggh/modules/GalaxyCore/models/PlanetModel.h
    include/ggh/modules/GalaxyCore/models/StarSystemModel.h
    include/ggh/modules/GalaxyCore/models/TravelLaneModel.h
)

set(SRC_FILES
    src/GalaxyModel.cpp
    src/PlanetModel.cpp
    src/StarSystemModel.cpp
    src/TravelLaneModel.cpp
)

qt_add_qml_module(GalaxyCoreModels
    URI GalaxyCore.Model
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/GalaxyCore/Model
    SOURCES
        ${HEADER_FILES}
        ${SRC_FILES}
)

# Also create a regular library for linking by tests
add_library(GalaxyCoreModelsLib STATIC
    ${HEADER_FILES}
    ${SRC_FILES}
)

target_include_directories(GalaxyCoreModels
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(GalaxyCoreModelsLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(GalaxyCoreModels
    PUBLIC
        GGH::GalaxyCore::Utilities
        Qt6::Core
        Qt6::Qml
)

target_link_libraries(GalaxyCoreModelsLib
    PUBLIC
        GGH::GalaxyCore::Utilities
)

# Compiler definitions for shared library export
target_compile_definitions(${PROJECT_NAME} PRIVATE GALAXYCORE_LIBRARY)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

add_library(GGH::GalaxyCore::Models ALIAS GalaxyCoreModelsLib)

# Install headers
install(FILES ${HEADER_FILES}
    DESTINATION include/GalaxyCore/models
)

install(TARGETS ${PROJECT_NAME} GalaxyCoreModelsLib
    EXPORT GalaxyCoreTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyCore/models
)