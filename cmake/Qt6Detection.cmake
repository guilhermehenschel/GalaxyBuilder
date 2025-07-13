# Galaxy Builder - Qt6 Detection and Configuration
# This file handles Qt6 detection and configuration

# Prevent double inclusion
if(GALAXY_BUILDER_QT6_CONFIGURED)
    return()
endif()
set(GALAXY_BUILDER_QT6_CONFIGURED TRUE)

# Find Qt6 with better error handling
find_package(Qt6 QUIET COMPONENTS Core Widgets Gui)

if(NOT Qt6_FOUND)
    message(STATUS "Qt6 not found in default locations, searching in common installation paths...")
    
    # Define search paths for different platforms
    if(WIN32)
        set(QT_SEARCH_PATHS
            "C:/Qt/6.9.1/msvc2022_64"
            "C:/Qt/6.7.2/msvc2022_64" 
            "C:/Qt/6.7.2/msvc2019_64"
            "C:/Qt/6.6.0/msvc2022_64"
            "C:/Qt/6.5.0/msvc2022_64"
            "C:/Qt/6.4.0/msvc2022_64"
            "C:/Qt/6.3.0/msvc2022_64"
        )
    elseif(UNIX AND NOT APPLE)
        set(QT_SEARCH_PATHS
            "/usr/lib/x86_64-linux-gnu/cmake/Qt6"
            "/usr/lib64/cmake/Qt6"
            "/opt/Qt/6.9.1/gcc_64"
            "/opt/Qt/6.7.2/gcc_64"
            "/opt/Qt/6.6.0/gcc_64"
            "/opt/Qt/6.5.0/gcc_64"
            "/usr/local/Qt-6.9.1"
            "/usr/local/Qt-6.7.2"
            "/usr/local/Qt-6.6.0"
            "/usr/local/Qt-6.5.0"
            "/usr/local/lib/cmake/Qt6"
        )
    elseif(APPLE)
        set(QT_SEARCH_PATHS
            "/opt/homebrew/lib/cmake/Qt6"
            "/usr/local/lib/cmake/Qt6"
            "/Applications/Qt/6.9.1/macos"
            "/Applications/Qt/6.7.2/macos"
        )
    endif()
    
    # Search for Qt6 installation
    set(QT_FOUND_PATH "")
    foreach(QT_PATH ${QT_SEARCH_PATHS})
        if(EXISTS "${QT_PATH}")
            set(QT_FOUND_PATH "${QT_PATH}")
            message(STATUS "Found Qt6 installation at: ${QT_PATH}")
            break()
        endif()
    endforeach()
    
    if(QT_FOUND_PATH)
        # Set CMake paths for Qt6
        list(PREPEND CMAKE_PREFIX_PATH "${QT_FOUND_PATH}")
        
        # Set Qt6_DIR for better CMake detection
        if(WIN32 OR APPLE)
            set(Qt6_DIR "${QT_FOUND_PATH}/lib/cmake/Qt6")
        else()
            set(Qt6_DIR "${QT_FOUND_PATH}")
        endif()
        
        message(STATUS "Set Qt6_DIR to: ${Qt6_DIR}")
    else()
        message(FATAL_ERROR 
            "Qt6 not found in any of the searched paths. "
            "Please install Qt6 or set Qt6_DIR manually using: "
            "-DQt6_DIR=/path/to/qt6/lib/cmake/Qt6"
        )
    endif()
endif()

# Now try to find Qt6 (either it was found initially or we set the paths)
if(NOT Qt6_FOUND)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
endif()

# Verify Qt6 version
if(Qt6_VERSION VERSION_LESS "6.3.0")
    message(FATAL_ERROR "Qt6 version 6.3.0 or higher is required. Found version: ${Qt6_VERSION}")
endif()

message(STATUS "Found Qt6: ${Qt6_VERSION}")
message(STATUS "Qt6 installation path: ${Qt6_DIR}")

# Enable Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6 compile definitions
add_compile_definitions(
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060300
)
