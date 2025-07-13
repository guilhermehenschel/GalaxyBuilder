# Galaxy Builder - CMake Module Documentation

This document describes the modular CMake structure of the Galaxy Builder project.

## File Organization

The CMake configuration is split into logical modules for better maintainability:

### Main Files
- **`CMakeLists.txt`** - Main entry point, project definition, source files, and target creation
- **`CMakePresets.json`** - Build presets for different platforms and configurations

### Configuration Modules (`cmake/` directory)

#### **`ProjectConfig.cmake`**
**Purpose**: Global project configuration and settings
- Project metadata and options
- C++ standard settings (C++23)
- Build type configuration
- Global compiler settings (MSVC, GCC, Clang)
- Platform-specific definitions
- Output directory configuration
- Build information display

#### **`BuildConfig.cmake`**
**Purpose**: Build-specific settings and optimizations
- Target-specific build options function (`galaxy_builder_add_build_options`)
- Link Time Optimization (LTO) configuration
- Debug build optimizations
- Platform-specific linking (Windows, Linux, macOS)
- Installation setup function (`galaxy_builder_setup_installation`)

#### **`Qt6Detection.cmake`**
**Purpose**: Qt6 detection and path resolution
- Cross-platform Qt6 detection
- Custom search paths for Windows, Linux, and macOS
- Fallback mechanisms for common Qt6 installation locations
- Version verification
- Global Qt6 settings (AUTOMOC, AUTOUIC, AUTORCC)

#### **`Qt6Functions.cmake`**
**Purpose**: Qt6-specific build functions using modern Qt6 CMake macros
- `galaxy_builder_setup_qt6(target)` - Modern Qt6 target configuration
- `galaxy_builder_add_qt_resources(target)` - Qt resource handling
- Platform-specific Qt6 plugin imports
- Qt6 compile definitions and features

#### **`WindowsInstall.cmake`**
**Purpose**: Windows-specific installation and deployment
- `galaxy_builder_install_qt_windows(target)` - Windows Qt6 DLL deployment
- Qt6 plugin installation (platforms, styles, imageformats)
- Visual C++ redistributable installation
- Debug/Release DLL handling

## Modern Qt6 CMake Features Used

### Qt6 Project Macros
```cmake
# Modern Qt6 executable creation
qt_add_executable(GalaxyBuilder
    ${GALAXY_BUILDER_SOURCES}
    ${GALAXY_BUILDER_HEADERS}
)

# Qt6 plugin imports
qt_import_plugins(target
    INCLUDE Qt6::QWindowsIntegrationPlugin
)

# Qt6 resource handling
qt_add_resources(target "resources"
    FILES ${QRC_FILES}
)
```

### Target-Based Configuration
All Qt6 setup is done through target-specific functions:
```cmake
galaxy_builder_setup_qt6(GalaxyBuilder)
galaxy_builder_add_build_options(GalaxyBuilder)
galaxy_builder_add_qt_resources(GalaxyBuilder)
galaxy_builder_setup_installation(GalaxyBuilder)
```

## Build Configuration Types

The modular system supports multiple build configurations:

### Debug Configuration
- Debug symbols enabled
- Optimizations disabled
- Sanitizers support (Linux/macOS)
- Enhanced debug definitions

### Release Configuration
- Full optimizations enabled
- Link Time Optimization (LTO)
- Platform-specific optimizations
- NDEBUG definitions

### Platform Support
- **Windows**: MSVC 2022, Visual Studio solutions
- **Linux**: GCC/Clang with pkg-config integration
- **macOS**: Clang with Homebrew support

## Usage Examples

### Basic Build
```bash
# Configure
cmake --preset=windows-debug

# Build
cmake --build build/windows-debug --config Debug

# Install
cmake --install build/windows-debug --config Debug
```

### Cross-Platform Builds
```bash
# Windows Debug/Release
cmake --preset=windows-debug
cmake --preset=windows-release

# Linux Debug/Release  
cmake --preset=linux-debug
cmake --preset=linux-release
```

## Benefits of Modular Structure

1. **Separation of Concerns**: Each module handles a specific aspect
2. **Maintainability**: Easy to modify specific functionality
3. **Reusability**: Functions can be reused across targets
4. **Platform Support**: Clean platform-specific handling
5. **Modern CMake**: Uses Qt6's latest CMake integration
6. **IDE Integration**: Better support for VS Code, Visual Studio, CLion

## Adding New Modules

To add a new CMake module:

1. Create `cmake/NewModule.cmake`
2. Add `include(cmake/NewModule.cmake)` to appropriate parent module
3. Use `function()` for reusable functionality
4. Follow the naming convention: `galaxy_builder_<functionality>`

## Troubleshooting

### Qt6 Not Found
The system searches common Qt6 installation paths. If Qt6 is in a custom location:
```bash
cmake --preset=windows-debug -DQt6_DIR=/path/to/qt6/lib/cmake/Qt6
```

### Build Errors
1. Clean build directory: `rm -rf build/`
2. Reconfigure: `cmake --preset=<preset-name>`
3. Check Qt6 installation and version (6.3.0+ required)

## Dependencies

- **CMake 3.25+**: Required for modern CMake features
- **Qt6 6.3.0+**: Core, Widgets, Gui components
- **C++23**: Modern C++ standard support
- **Platform Tools**: MSVC 2022 (Windows), GCC/Clang (Linux/macOS)
