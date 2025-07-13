# Galaxy Builder - Build System Guide

This document explains how to use the comprehensive build system for Galaxy Builder, which supports both Windows and Linux platforms with Debug and Release configurations.

## Overview

The build system uses CMake 3.25+ with presets to provide a modern, cross-platform build experience. It supports:

- **Windows**: Visual Studio 2022 with MSVC
- **Linux**: GCC/Clang with Ninja generator
- **Build Types**: Debug and Release configurations
- **IDE Integration**: Full VS Code support with IntelliSense

## Prerequisites

### Windows
- **Visual Studio 2022** (with C++ workload)
- **CMake 3.25+**
- **Qt6** (6.3.0 or newer, MSVC build)
- **PowerShell** (for build scripts)

### Linux
- **GCC 11+** or **Clang 14+** (with C++23 support)
- **CMake 3.25+**
- **Ninja** build system
- **Qt6** development packages
- **pkg-config**

### Installing Dependencies

#### Windows
```powershell
# Install Qt6 from official installer
# https://www.qt.io/download-qt-installer

# Install Visual Studio 2022 Community (free)
# https://visualstudio.microsoft.com/vs/community/

# Install CMake
# https://cmake.org/download/
```

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake ninja-build pkg-config
sudo apt install qt6-base-dev qt6-tools-dev libqt6core6 libqt6gui6 libqt6widgets6
```

#### Fedora/RHEL
```bash
sudo dnf install gcc-c++ cmake ninja-build pkg-config
sudo dnf install qt6-qtbase-devel qt6-qttools-devel
```

#### Arch Linux
```bash
sudo pacman -S base-devel cmake ninja pkg-config
sudo pacman -S qt6-base qt6-tools
```

## Build Methods

### Method 1: CMake Presets (Recommended)

CMake presets provide the most consistent experience across platforms and IDEs.

#### Windows
```powershell
# Configure and build Debug
cmake --preset windows-debug
cmake --build build/windows-debug --config Debug

# Configure and build Release
cmake --preset windows-release
cmake --build build/windows-release --config Release
```

#### Linux
```bash
# Configure and build Debug
cmake --preset linux-debug
cmake --build build/linux-debug

# Configure and build Release
cmake --preset linux-release
cmake --build build/linux-release
```

### Method 2: Platform-Specific Scripts

#### Windows PowerShell Script
```powershell
# Build Release (default)
.\build.ps1

# Build Debug
.\build.ps1 -BuildType Debug

# Specify Qt6 path
.\build.ps1 -QtPath "C:\Qt\6.6.0\msvc2022_64"

# Clean build and run after building
.\build.ps1 -BuildType Debug -Clean -Run

# Install after building
.\build.ps1 -BuildType Release -Install

# Show help
.\build.ps1 -Help
```

#### Linux Bash Script
```bash
# Make script executable
chmod +x build-linux.sh

# Build Release (default)
./build-linux.sh

# Build Debug
./build-linux.sh --debug

# Clean build and run after building
./build-linux.sh --debug --clean --run

# Install after building
./build-linux.sh --release --install

# Show help
./build-linux.sh --help
```

### Method 3: VS Code Integration

VS Code provides the best development experience with full IntelliSense, debugging, and task integration.

#### Tasks Available:
- **Configure Debug/Release**: Configure CMake
- **Build Debug/Release**: Build specific configuration
- **Build Galaxy Builder**: Build using PowerShell script
- **Clean All**: Remove all build directories
- **Run Debug/Release**: Build and run application

#### Using Tasks:
1. Press `Ctrl+Shift+P`
2. Type "Tasks: Run Task"
3. Select desired task

#### Keyboard Shortcuts:
- `Ctrl+Shift+B`: Build (default Debug task)
- `F5`: Debug (launches debugger)
- `Ctrl+F5`: Run without debugging

## Build Configurations

### Debug Configuration
- **Compiler Flags**: Maximum debugging information, no optimization
- **Runtime**: Debug runtime libraries
- **Output**: `GalaxyBuilderd.exe` (Windows) or `GalaxyBuilderd` (Linux)
- **Location**: `build/{platform}-debug/bin/`

### Release Configuration
- **Compiler Flags**: Full optimization, no debug info
- **Runtime**: Release runtime libraries
- **Output**: `GalaxyBuilder.exe` (Windows) or `GalaxyBuilder` (Linux)
- **Location**: `build/{platform}-release/bin/`

## Directory Structure

```
GalaxyBuilder/
├── build/                          # Build outputs
│   ├── windows-debug/               # Windows Debug build
│   ├── windows-release/             # Windows Release build
│   ├── linux-debug/                # Linux Debug build
│   └── linux-release/              # Linux Release build
├── src/                            # Source code
├── .vscode/                        # VS Code configuration
│   ├── settings.json               # Project settings
│   ├── tasks.json                  # Build tasks
│   └── launch.json                 # Debug configurations
├── CMakeLists.txt                  # Main CMake file
├── CMakePresets.json               # CMake presets
├── build.ps1                      # Windows build script
├── build-linux.sh                 # Linux build script
└── README.md                       # Project documentation
```

## Troubleshooting

### Common Issues

#### Qt6 Not Found
**Windows:**
```powershell
# Specify Qt6 path explicitly
.\build.ps1 -QtPath "C:\Qt\6.6.0\msvc2022_64"

# Or set environment variable
$env:Qt6_DIR = "C:\Qt\6.6.0\msvc2022_64\lib\cmake\Qt6"
```

**Linux:**
```bash
# Install Qt6 development packages
sudo apt install qt6-base-dev qt6-tools-dev

# Or specify Qt6 path
export CMAKE_PREFIX_PATH="/opt/Qt/6.6.0/gcc_64:$CMAKE_PREFIX_PATH"
```

#### C++23 Not Supported
Make sure you have a recent compiler:
- **Windows**: Visual Studio 2022 17.4+
- **Linux**: GCC 11+ or Clang 14+

#### Missing Dependencies
**Windows:**
- Install Visual Studio 2022 with "Desktop development with C++" workload
- Install Qt6 from official installer

**Linux:**
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake ninja-build qt6-base-dev

# Fedora
sudo dnf install gcc-c++ cmake ninja-build qt6-qtbase-devel
```

### Build Errors

#### "Qt6 not found"
1. Ensure Qt6 is installed
2. Check Qt6 installation path
3. Use `-QtPath` parameter on Windows
4. Set `CMAKE_PREFIX_PATH` on Linux

#### "C++23 features not available"
1. Update your compiler
2. Check CMake version (3.25+ required)
3. Verify C++23 support: `g++ --std=c++23` or check VS version

#### "Ninja not found" (Linux)
```bash
# Install Ninja
sudo apt install ninja-build        # Ubuntu/Debian
sudo dnf install ninja-build        # Fedora
sudo pacman -S ninja                # Arch
```

## VS Code Extensions

For the best development experience, install these VS Code extensions:

- **C/C++** (ms-vscode.cpptools)
- **CMake Tools** (ms-vscode.cmake-tools)
- **Qt for Python** (seanwu.vscode-qt-for-python) - For Qt syntax highlighting

## Performance Tips

### Faster Builds
- Use **Ninja** generator on all platforms (Linux default)
- Enable **parallel builds**: `cmake --build build --parallel`
- Use **Unity builds** for large projects
- Consider **ccache** on Linux for incremental builds

### IDE Performance
- Enable **compile_commands.json** (automatically generated)
- Use **precompiled headers** for large Qt projects
- Configure **IntelliSense** to use CMake's compile commands

## Advanced Usage

### Custom Build Configurations
You can create additional presets by modifying `CMakePresets.json`:

```json
{
    "name": "windows-debug-asan",
    "inherits": "windows-debug",
    "cacheVariables": {
        "CMAKE_CXX_FLAGS": "-fsanitize=address"
    }
}
```

### Cross-Compilation
For advanced users, you can add cross-compilation presets for different architectures or operating systems.

### Packaging
The build system includes basic installation rules. For distribution:

```bash
# Install to custom prefix
cmake --install build/linux-release --prefix /opt/galaxybuilder

# Create packages (if CPack is configured)
cpack --config build/linux-release/CPackConfig.cmake
```

## Getting Help

If you encounter issues:

1. Check this guide first
2. Verify all prerequisites are installed
3. Try cleaning and rebuilding: `Clean All` task
4. Check the CMake output for specific error messages
5. Consult Qt6 and CMake documentation

For development help:
- Qt6 Documentation: https://doc.qt.io/qt-6/
- CMake Documentation: https://cmake.org/documentation/
- C++23 Reference: https://en.cppreference.com/
