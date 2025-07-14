project(GalaxyCoreUtilities VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

qt_add_qml_module(GalaxyCoreUtilities
    URI GalaxyCore.Utilities
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/GalaxyCore/Utilities
    SOURCES
        include/ggh/modules/GalaxyCore/utilities/Common.h
        include/ggh/modules/GalaxyCore/utilities/Coordinates.h)

target_include_directories(GalaxyCoreUtilities
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(GalaxyCoreUtilities
    PUBLIC
        Qt6::Core
        Qt6::Qml
)

# Compiler definitions for shared library export
target_compile_definitions(${PROJECT_NAME} PRIVATE GALAXYCORE_LIBRARY)


add_library(GGH::GalaxyCore::Utilities ALIAS ${PROJECT_NAME})

# Install headers
install(FILES include/ggh/modules/GalaxyCore/utilities/Common.h
    DESTINATION include/GalaxyCore/utilities
)

install(TARGETS ${PROJECT_NAME}
    EXPORT GalaxyCoreTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/GalaxyCore/utilities
)