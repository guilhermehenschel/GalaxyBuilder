# MVVM Architecture Implementation

## Overview

The Galaxy Builder application now follows a proper MVVM (Model-View-ViewModel) pattern with optimized XML creation that only occurs when import/export operations are actually needed.

## Architecture Layers

### Model Layer
- **StarSystemQml** - Core data model for star systems
- **Planet** - Core data model for planets  
- **SystemDataManager** - Data persistence and XML serialization (only when needed)
- **Galaxy** - Galaxy data model with star systems collection

### ViewModel Layer
- **SystemPropertiesViewModel** - Business logic for system property editing
- **ImportExportViewModel** - Business logic for import/export operations
- **GalaxyController** - Main application controller (existing)

### View Layer
- **SystemPropertiesDialog.qml** - System properties editing interface
- **ImportExportManager.qml** - Import/export management interface
- **GalaxyView.qml** - Main galaxy visualization

## Key MVVM Principles Implemented

### 1. Separation of Concerns
- **Models** contain only data and core business rules
- **ViewModels** handle UI logic, validation, and coordinate between Models and Views
- **Views** contain only presentation logic and user interaction

### 2. Data Binding
- Views bind to ViewModel properties, not directly to Models
- ViewModels expose computed properties and commands for Views
- Changes flow through ViewModels to maintain consistency

### 3. Command Pattern
- ViewModels expose methods as commands (e.g., `saveSystemData()`, `exportToXml()`)
- Views invoke commands instead of directly manipulating data
- Commands include validation and error handling

## Lazy XML Creation

### Memory-Based Persistence
```cpp
// SystemDataManager uses JSON for frequent operations
QJsonObject SystemDataManager::systemToJson(StarSystemQml* system) const
bool SystemDataManager::systemFromJson(StarSystemQml* system, const QJsonObject& json) const
```

### On-Demand XML Generation
```cpp
// XML is only created when export is actually requested
bool SystemDataManager::exportSystemToXml(StarSystemQml* system, const QString& filePath)
{
    // Validate first
    if (!validateSystemData(system)) {
        return false;
    }
    
    // Create XML document only when export is confirmed
    QDomDocument doc;
    if (!createXmlDocumentForSystem(system, doc)) {
        return false;
    }
    
    // Write to file
    // ...
}
```

### XML Preview Without File Creation
```cpp
// Generate XML string for preview without file I/O
QString SystemDataManager::generateXmlPreview(StarSystemQml* system) const
```

## ViewModel Features

### SystemPropertiesViewModel
- **State Management**: Tracks dirty state, saving/loading status
- **Auto-Save**: Configurable auto-save with debouncing
- **Validation**: Real-time validation with error reporting
- **Commands**: Save, load, export, import, reset operations

```qml
SystemPropertiesViewModel {
    id: viewModel
    starSystem: window.starSystem
    autoSaveEnabled: true
    
    // Reactive properties
    property bool isDirty
    property bool isSaving
    property string statusMessage
}
```

### ImportExportViewModel
- **System Selection**: Multi-select with toggle operations
- **Batch Operations**: Export selected or all systems
- **Progress Tracking**: Loading states and status messages
- **Integration**: Seamless integration with galaxy controller

```qml
ImportExportViewModel {
    id: viewModel
    
    // Reactive properties
    property var selectedSystems
    property bool isExporting
    property string statusMessage
}
```

## Benefits of This Implementation

### Performance
- **Reduced XML Creation**: XML is only generated when actually needed for export
- **Memory Caching**: Frequently accessed data stays in memory as JSON
- **Lazy Loading**: Data is loaded on-demand

### Maintainability
- **Clear Separation**: Each layer has distinct responsibilities
- **Testable**: ViewModels can be unit tested independently
- **Reusable**: ViewModels can be used with different Views

### User Experience
- **Responsive UI**: Operations are asynchronous with progress feedback
- **Real-time Validation**: Immediate feedback on data entry
- **Auto-Save**: Automatic persistence with manual override

## Usage Examples

### System Properties Dialog
```qml
ApplicationWindow {
    property StarSystem starSystem: null
    
    SystemPropertiesViewModel {
        id: viewModel
        starSystem: window.starSystem
        
        onStatusMessageChanged: {
            statusLabel.text = statusMessage
        }
    }
    
    TextField {
        text: viewModel.starSystem ? viewModel.starSystem.name : ""
        onEditingFinished: {
            if (viewModel.starSystem) {
                viewModel.starSystem.name = text
            }
        }
    }
    
    Button {
        text: "Export XML"
        enabled: !viewModel.isSaving
        onClicked: viewModel.exportToXml(filePath)
    }
}
```

### Import/Export Manager
```qml
ApplicationWindow {
    ImportExportViewModel {
        id: viewModel
        
        onExportCompleted: function(success, filePath, count) {
            if (success) {
                console.log("Exported", count, "systems to", filePath)
            }
        }
    }
    
    Button {
        text: "Export Selected"
        enabled: viewModel.selectedCount > 0 && !viewModel.isExporting
        onClicked: viewModel.exportSelectedSystems(filePath)
    }
}
```

## Migration Notes

### Breaking Changes
- `SystemPropertiesDialog.qml` now uses `SystemPropertiesViewModel` instead of direct `SystemDataManager` access
- `ImportExportManager.qml` now uses `ImportExportViewModel` instead of direct system access
- Properties are accessed through ViewModels: `viewModel.starSystem.name` instead of `starSystem.name`

### Backward Compatibility
- Existing `SystemDataManager` API remains unchanged for legacy code
- Core data models (`StarSystemQml`, `Planet`) maintain same interface
- Galaxy controller continues to work as before

## Future Enhancements

### Planned Improvements
1. **Command History**: Undo/redo functionality through ViewModels
2. **Data Validation**: More comprehensive validation rules
3. **Offline Support**: Local storage with sync capabilities
4. **Batch Import**: Multiple file import with progress tracking
5. **Export Templates**: Customizable export formats and templates

This MVVM implementation provides a solid foundation for scalable, maintainable UI development while optimizing performance through lazy XML creation and efficient data management.
