project(GalaxyCoreViewModels VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(HEADER_FILES
    include/ggh/modules/GalaxyCore/viewmodels/GalaxyViewModel.h
    include/ggh/modules/GalaxyCore/viewmodels/PlanetViewModel.h
    include/ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h
    include/ggh/modules/GalaxyCore/viewmodels/TravelLaneViewModel.h
)

set(SRC_FILES
    src/GalaxyViewModel.cpp
    src/PlanetViewModel.cpp
    src/StarSystemViewModel.cpp
    src/TravelLaneViewModel.cpp
)

qt_add_qml_module(GalaxyCoreViewModels
    URI GalaxyCore.ViewModel
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/GalaxyCore/ViewModel
    SOURCES
        ${HEADER_FILES}
        ${SRC_FILES}
)

# Also create a regular library for linking by tests
add_library(GalaxyCoreViewModelsLib STATIC
    ${HEADER_FILES}
    ${SRC_FILES}
)

target_include_directories(GalaxyCoreViewModels
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_include_directories(GalaxyCoreViewModelsLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(GalaxyCoreViewModels
    PUBLIC
        GGH::GalaxyCore::Utilities
        Qt6::Core
        Qt6::Qml
)

target_link_libraries(GalaxyCoreViewModelsLib
    PUBLIC
        GGH::GalaxyCore::Models
        GGH::GalaxyCore::Utilities
        Qt6::Core
        Qt6::Qml
)

# Compiler definitions for shared library export
target_compile_definitions(${PROJECT_NAME} PRIVATE GALAXYCORE_LIBRARY)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

add_library(GGH::GalaxyCore::ViewModels ALIAS GalaxyCoreViewModelsLib)

# Install headers
install(FILES ${HEADER_FILES}
    DESTINATION include/GalaxyCore/models
)

install(TARGETS ${PROJECT_NAME}
    EXPORT GalaxyCoreTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyCore/viewmodel
)