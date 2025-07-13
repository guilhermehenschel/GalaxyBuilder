#!/bin/bash

# Galaxy Builder - Linux Build Script
# Supports both Debug and Release builds using CMake Presets

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${BLUE}Galaxy Builder - Linux Build Script${NC}"
echo -e "${BLUE}====================================${NC}"

# Default values
BUILD_TYPE="release"
CLEAN=false
INSTALL=false
RUN=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="release"
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -i|--install)
            INSTALL=true
            shift
            ;;
        --run)
            RUN=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -d, --debug      Build in debug mode"
            echo "  -r, --release    Build in release mode (default)"
            echo "  -c, --clean      Clean before building"
            echo "  -i, --install    Install after building"
            echo "  --run            Run the application after building"
            echo "  -h, --help       Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

echo -e "${GREEN}Build type: ${BUILD_TYPE}${NC}"

# Check for required tools
check_tool() {
    if ! command -v $1 &> /dev/null; then
        echo -e "${RED}Error: $1 is not installed${NC}"
        exit 1
    fi
}

echo -e "${BLUE}Checking required tools...${NC}"
check_tool cmake
check_tool ninja
check_tool pkg-config

# Check C++ compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo -e "${RED}Error: No C++ compiler found (g++ or clang++)${NC}"
    exit 1
fi

# Check Qt6
if ! pkg-config --exists Qt6Core; then
    echo -e "${YELLOW}Warning: Qt6 not found via pkg-config${NC}"
    echo -e "${YELLOW}Make sure Qt6 is installed and available${NC}"
fi

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo -e "${BLUE}Cleaning build directory...${NC}"
    rm -rf build/linux-${BUILD_TYPE}
fi

# Configure
PRESET_NAME="linux-${BUILD_TYPE}"
echo -e "${BLUE}Configuring with preset: ${PRESET_NAME}${NC}"
cmake --preset ${PRESET_NAME}

if [ $? -ne 0 ]; then
    echo -e "${RED}Configuration failed!${NC}"
    exit 1
fi

# Build
echo -e "${BLUE}Building...${NC}"
cmake --build build/linux-${BUILD_TYPE} --config ${BUILD_TYPE^}

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

# Install if requested
if [ "$INSTALL" = true ]; then
    echo -e "${BLUE}Installing...${NC}"
    cmake --install build/linux-${BUILD_TYPE} --config ${BUILD_TYPE^}
fi

echo -e "${GREEN}Build completed successfully!${NC}"

# Show executable location
if [ "$BUILD_TYPE" = "debug" ]; then
    EXECUTABLE="build/linux-debug/bin/GalaxyBuilderd"
else
    EXECUTABLE="build/linux-release/bin/GalaxyBuilder"
fi

echo -e "${GREEN}Executable: ${EXECUTABLE}${NC}"

# Run if requested
if [ "$RUN" = true ]; then
    if [ -f "$EXECUTABLE" ]; then
        echo -e "${BLUE}Running Galaxy Builder...${NC}"
        ./$EXECUTABLE
    else
        echo -e "${RED}Executable not found: $EXECUTABLE${NC}"
        exit 1
    fi
fi
