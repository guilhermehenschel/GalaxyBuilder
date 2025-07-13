# Galaxy Builder - Stellaris Style Galaxy Generator

A C++23 Qt application that generates procedural galaxy maps in the style of Stellaris, featuring star systems and travel lanes.

## Features

- **Procedural Galaxy Generation**: Create galaxies with different shapes (Spiral, Elliptical, Ring, Cluster)
- **Multiple Star Types**: Red Dwarf, Yellow Star, Blue Star, White Dwarf, Red Giant, Neutron Star, Black Hole
- **Travel Lane Networks**: Automatically generated connections between nearby star systems
- **Interactive Visualization**: 
  - Pan and zoom the galaxy view
  - Click and hover interactions with star systems
  - Toggle display options (system names, travel lanes, influence radius)
- **Customizable Parameters**:
  - System count (10-1000)
  - Galaxy dimensions
  - Spiral parameters (arms, tightness)
  - Core and edge radius
  - Random seed for reproducible generation
- **Export Functionality**: Save galaxy images as PNG files

## Technology Stack

- **C++23**: Modern C++ with latest features
- **Qt6**: Cross-platform GUI framework
- **CMake**: Build system
- **Standard Library**: Uses modern C++ containers and algorithms

## Project Structure

```
GalaxyBuilder/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── build.ps1              # Windows build script
└── src/
    ├── main.cpp           # Application entry point
    ├── Common.h           # Common types and constants
    ├── MainWindow.h/cpp   # Main application window
    ├── GalaxyMapWidget.h/cpp    # Galaxy visualization widget
    ├── GalaxyGenerator.h/cpp    # Procedural generation logic
    ├── Galaxy.h/cpp       # Galaxy data model
    ├── StarSystem.h/cpp   # Star system representation
    └── TravelLane.h/cpp   # Travel lane representation
```

## Building the Project

### Prerequisites

- **Qt6**: Install Qt6 development libraries
- **CMake**: Version 3.25 or higher
- **C++23 Compiler**: Visual Studio 2022, GCC 11+, or Clang 14+

### Windows (Visual Studio)

1. Install Qt6 from the official installer
2. Install Visual Studio 2022 with C++ support
3. Install CMake

```powershell
# Run the build script
.\build.ps1
```

### Manual Build

```bash
mkdir build
cd build
cmake .. -DQt6_DIR="path/to/qt6/lib/cmake/Qt6"
cmake --build . --config Release
```

## Usage

1. **Launch the Application**: Run the executable from the build directory
2. **Adjust Parameters**: Use the parameter panel on the right to customize generation
3. **Generate Galaxy**: Click "Generate Galaxy" or use Ctrl+G
4. **Navigate**: 
   - Click and drag to pan
   - Mouse wheel to zoom
   - Click on systems for information
5. **Export**: Use File → Export Image or Ctrl+E to save the galaxy

## Generation Parameters

- **System Count**: Number of star systems (10-1000)
- **Galaxy Shape**: 
  - Spiral: Classic spiral arms
  - Elliptical: Elliptical distribution
  - Ring: Ring-shaped galaxy
  - Cluster: Multiple clustered regions
- **Dimensions**: Galaxy width and height in pixels
- **Spiral Arms**: Number of spiral arms (for spiral galaxies)
- **Spiral Tightness**: How tightly wound the spirals are
- **Core/Edge Radius**: Controls central density vs outer regions
- **Seed**: Random seed for reproducible results (0 = random)

## Display Options

- **Show System Names**: Toggle system name labels
- **Show Travel Lanes**: Toggle hyperlane connections
- **Show Influence Radius**: Toggle system influence circles

## Architecture Highlights

### Modern C++23 Features Used

- **Smart Pointers**: `std::unique_ptr` for memory management
- **Move Semantics**: Efficient object transfers
- **Range-based Loops**: Clean iteration syntax
- **Auto Type Deduction**: Cleaner code
- **Uniform Initialization**: Consistent syntax
- **Strong Type Aliases**: Better code clarity

### Design Patterns

- **Model-View Architecture**: Separation of data and presentation
- **Strategy Pattern**: Different galaxy generation algorithms
- **Observer Pattern**: Qt signals/slots for UI updates
- **Factory Pattern**: Star system creation
- **RAII**: Resource management through constructors/destructors

### Performance Considerations

- **Efficient Data Structures**: Hash maps for O(1) lookups
- **Minimal Copying**: Move semantics and references
- **Cached Calculations**: Screen transformations
- **Level-of-Detail**: Adaptive rendering based on zoom

## Future Enhancements

- **Save/Load**: Serialize galaxy data
- **Advanced Generation**: More realistic stellar distribution
- **Empire Borders**: Faction territories
- **Resource Distribution**: Strategic resources on systems
- **3D Visualization**: True 3D galaxy representation
- **Animation**: Animated generation process
- **Networking**: Multi-user galaxy editing

## Contributing

1. Fork the repository
2. Create a feature branch
3. Follow the existing code style
4. Add tests for new features
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by Stellaris galaxy generation
- Qt Framework for excellent GUI capabilities
- Modern C++ community for language improvements
