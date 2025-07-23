# QML Linting and Formatting Functions
# This file provides CMake functions for QML code quality tools

# Function to find Qt6 tools
function(find_qt6_tools)
    # Find qmllint
    find_program(QMLLINT_EXECUTABLE_CACHE
        NAMES qmllint qmllint.exe
        HINTS ${Qt6_DIR}/../../../bin
              ${Qt6Core_DIR}/../../../bin
              ${_qt_cmake_dir}/../../../bin
        DOC "Path to qmllint executable"
    )
    
    # Find qmlformat
    find_program(QMLFORMAT_EXECUTABLE_CACHE
        NAMES qmlformat qmlformat.exe
        HINTS ${Qt6_DIR}/../../../bin
              ${Qt6Core_DIR}/../../../bin
              ${_qt_cmake_dir}/../../../bin
        DOC "Path to qmlformat executable"
    )
    
    # Set global variables for use in other functions
    if(QMLLINT_EXECUTABLE_CACHE)
        set(QMLLINT_EXECUTABLE ${QMLLINT_EXECUTABLE_CACHE} CACHE INTERNAL "Path to qmllint executable")
        message(STATUS "Found qmllint: ${QMLLINT_EXECUTABLE}")
    else()
        message(WARNING "qmllint not found - QML linting will not be available")
    endif()
    
    if(QMLFORMAT_EXECUTABLE_CACHE)
        set(QMLFORMAT_EXECUTABLE ${QMLFORMAT_EXECUTABLE_CACHE} CACHE INTERNAL "Path to qmlformat executable")  
        message(STATUS "Found qmlformat: ${QMLFORMAT_EXECUTABLE}")
    else()
        message(WARNING "qmlformat not found - QML formatting will not be available")
    endif()
endfunction()

# Function to add QML linting target
function(add_qml_lint_target target_name)
    if(NOT QMLLINT_EXECUTABLE)
        message(WARNING "Cannot add QML lint target ${target_name} - qmllint not found")
        return()
    endif()
    
    # Get all QML files from the arguments
    set(QML_FILES ${ARGN})
    
    # Create prefixed target name to avoid conflicts
    set(lint_target_name "qml-lint-${target_name}")
    
    # Create custom target for linting
    add_custom_target(${lint_target_name}
        COMMAND ${QMLLINT_EXECUTABLE} ${QML_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running qmllint on QML files for ${target_name}"
        VERBATIM
    )
    
    # Add to a global property to track all lint targets
    get_property(ALL_LINT_TARGETS GLOBAL PROPERTY GALAXY_BUILDER_LINT_TARGETS)
    set_property(GLOBAL PROPERTY GALAXY_BUILDER_LINT_TARGETS ${ALL_LINT_TARGETS} ${lint_target_name})
    
    message(STATUS "Created QML lint target: ${lint_target_name}")
endfunction()

# Function to add QML formatting target
function(add_qml_format_target target_name)
    if(NOT QMLFORMAT_EXECUTABLE)
        message(WARNING "Cannot add QML format target ${target_name} - qmlformat not found")
        return()
    endif()
    
    # Get all QML files from the arguments
    set(QML_FILES ${ARGN})
    
    # Create prefixed target name to avoid conflicts
    set(format_target_name "qml-format-${target_name}")
    
    # Create custom target for formatting
    add_custom_target(${format_target_name}
        COMMAND ${QMLFORMAT_EXECUTABLE} -i ${QML_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running qmlformat on QML files for ${target_name}"
        VERBATIM
    )
    
    # Add to a global property to track all format targets
    get_property(ALL_FORMAT_TARGETS GLOBAL PROPERTY GALAXY_BUILDER_FORMAT_TARGETS)
    set_property(GLOBAL PROPERTY GALAXY_BUILDER_FORMAT_TARGETS ${ALL_FORMAT_TARGETS} ${format_target_name})
    
    message(STATUS "Created QML format target: ${format_target_name}")
endfunction()

# Function to create aggregate targets
function(create_aggregate_qml_targets)
    # Create aggregate lint target
    get_property(ALL_LINT_TARGETS GLOBAL PROPERTY GALAXY_BUILDER_LINT_TARGETS)
    if(ALL_LINT_TARGETS)
        add_custom_target(qml-lint-all
            COMMENT "Running all QML linting targets"
        )
        add_dependencies(qml-lint-all ${ALL_LINT_TARGETS})
        message(STATUS "Created aggregate QML lint target: qml-lint-all")
    endif()
    
    # Create aggregate format target
    get_property(ALL_FORMAT_TARGETS GLOBAL PROPERTY GALAXY_BUILDER_FORMAT_TARGETS)
    if(ALL_FORMAT_TARGETS)
        add_custom_target(qml-format-all
            COMMENT "Running all QML formatting targets"
        )
        add_dependencies(qml-format-all ${ALL_FORMAT_TARGETS})
        message(STATUS "Created aggregate QML format target: qml-format-all")
    endif()
    
    # Create combined QML quality target
    if(ALL_LINT_TARGETS OR ALL_FORMAT_TARGETS)
        add_custom_target(qml-quality
            COMMENT "Running QML quality checks and formatting"
        )
        if(ALL_LINT_TARGETS)
            add_dependencies(qml-quality qml-lint-all)
        endif()
        if(ALL_FORMAT_TARGETS)
            add_dependencies(qml-quality qml-format-all)
        endif()
        message(STATUS "Created combined QML quality target: qml-quality")
    endif()
endfunction()
