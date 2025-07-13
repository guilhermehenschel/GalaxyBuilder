# Galaxy Builder - Build Configuration
# This file contains build-specific settings and optimizations

# Compiler-specific optimizations for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release" AND GALAXY_BUILDER_ENABLE_LTO)
    message(STATUS "Enabling Link Time Optimization (LTO)")
    
    if(MSVC)
        # Visual Studio LTO
        add_compile_options(/GL)
        add_link_options(/LTCG)
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # GCC LTO
        add_compile_options(-flto)
        add_link_options(-flto)
        
        # Enable additional optimizations
        add_compile_options(-march=native -mtune=native)
        
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Clang LTO
        add_compile_options(-flto=thin)
        add_link_options(-flto=thin)
        
        # Enable additional optimizations
        add_compile_options(-march=native -mtune=native)
    endif()
endif()

# Debug build optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build configuration")
    
    # Enable sanitizers if requested
    if(GALAXY_BUILDER_ENABLE_SANITIZERS AND NOT WIN32)
        message(STATUS "Enabling sanitizers")
        add_compile_options(-fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
    
    # Enable additional debug information
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-fno-inline -fno-optimize-sibling-calls)
    endif()
endif()

# Function to add build optimizations to a target
function(galaxy_builder_add_build_options target_name)
    # Set target-specific properties
    set_target_properties(${target_name} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
    
    # Add debug postfix
    set_target_properties(${target_name} PROPERTIES
        DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
    )
    
    # Platform-specific target settings
    if(WIN32)
        set_target_properties(${target_name} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
        target_link_options(${target_name} PRIVATE "/SUBSYSTEM:WINDOWS")
        
    elseif(UNIX AND NOT APPLE)
        # Link pthread and other system libraries
        find_package(Threads REQUIRED)
        target_link_libraries(${target_name} PRIVATE Threads::Threads)
        
        # Link fontconfig if available
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(FONTCONFIG QUIET fontconfig)
            if(FONTCONFIG_FOUND)
                target_link_libraries(${target_name} PRIVATE ${FONTCONFIG_LIBRARIES})
                target_include_directories(${target_name} PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
            endif()
        endif()
        
    elseif(APPLE)
        set_target_properties(${target_name} PROPERTIES
            MACOSX_BUNDLE TRUE
        )
    endif()
    
    # Release-specific optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        if(MSVC)
            target_compile_options(${target_name} PRIVATE /O2)
            if(GALAXY_BUILDER_ENABLE_LTO)
                target_compile_options(${target_name} PRIVATE /GL)
                target_link_options(${target_name} PRIVATE /LTCG)
            endif()
            
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${target_name} PRIVATE -O3)
            if(GALAXY_BUILDER_ENABLE_LTO)
                target_compile_options(${target_name} PRIVATE -flto -march=native)
                target_link_options(${target_name} PRIVATE -flto)
            endif()
            
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            target_compile_options(${target_name} PRIVATE -O3)
            if(GALAXY_BUILDER_ENABLE_LTO)
                target_compile_options(${target_name} PRIVATE -flto=thin -march=native)
                target_link_options(${target_name} PRIVATE -flto=thin)
            endif()
        endif()
    endif()
endfunction()

# Function to setup installation for a target
function(galaxy_builder_setup_installation target_name)
    # Installation rules
    install(TARGETS ${target_name}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
    )
    
    # Platform-specific installation
    if(WIN32)
        # Install Qt6 libraries and plugins (handled in Qt6Config.cmake)
        include(cmake/WindowsInstall.cmake)
        galaxy_builder_install_qt_windows(${target_name})
    endif()
endfunction()
