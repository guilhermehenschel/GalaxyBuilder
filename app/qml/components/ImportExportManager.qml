import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform 1.1 as Platform
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels
import Galaxy.Exporters

ApplicationWindow {
    id: window
    
    title: "Galaxy Import/Export Manager"
    width: 900
    height: 700
    minimumWidth: 700
    minimumHeight: 500
    modality: Qt.ApplicationModal
    
    color: "#1a1a1a"
    
    // MVVM ViewModel - ExporterObject is a singleton
    property ExporterObject viewModel: ExporterObject
    
    // Properties to receive context from parent
    property var galaxyController: null
    property int selectedCount: 0
    
    // Helper function to update selected count
    function updateSelectedCount() {
        var count = 0
        for (var i = 0; i < systemsList.count; i++) {
            var item = systemsList.itemAtIndex(i)
            if (item && item.selected) {
                count++
            }
        }
        selectedCount = count
    }
        
    Component.onCompleted: {
        // Get controller from GalaxyController singleton
        galaxyController = GalaxyController
        
        // Set model for export - pass the GalaxyViewModel, not the controller
        if (galaxyController && galaxyController.galaxyViewModel) {
            viewModel.setModel(galaxyController.galaxyViewModel)
        }
    }
    
    // Update the exporter model when galaxy changes
    Connections {
        target: galaxyController
        function onGalaxyViewModelChanged() {
            if (galaxyController && galaxyController.galaxyViewModel) {
                viewModel.setModel(galaxyController.galaxyViewModel)
            }
        }
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15
        
        // Header
        Text {
            text: "Galaxy Import/Export Manager"
            color: "#ffffff"
            font.pixelSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }
        
        // System Selection Area
        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Available Systems"
            
            background: Rectangle {
                color: "#2a2a2a"
                border.color: "#404040"
                radius: 5
            }
            
            label: Text {
                color: "#ffffff"
                text: parent.title
                font.bold: true
            }
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 10
                
                // Selection controls
                RowLayout {
                    Layout.fillWidth: true
                    
                    Button {
                        text: "Select All"
                        background: Rectangle {
                            color: "#0066cc"
                            border.color: "#0088ff"
                            radius: 3
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "#ffffff"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: systemsList.selectAll()
                    }
                    
                    Button {
                        text: "Clear Selection"
                        background: Rectangle {
                            color: "#666666"
                            border.color: "#888888"
                            radius: 3
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "#ffffff"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: systemsList.clearSelection()
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Text {
                        text: "Selected: " + selectedCount + " systems"
                        color: "#cccccc"
                    }
                }
                
                // Systems list
                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    
                    ListView {
                        id: systemsList
                        model: galaxyController ? galaxyController.systemsModel : null
                        
                        function selectAll() {
                            for (var i = 0; i < count; i++) {
                                var item = itemAtIndex(i)
                                if (item) {
                                    item.selected = true
                                }
                            }
                            updateSelectedCount()
                        }
                        
                        function clearSelection() {
                            for (var i = 0; i < count; i++) {
                                var item = itemAtIndex(i)
                                if (item) {
                                    item.selected = false
                                }
                            }
                            updateSelectedCount()
                        }
                        
                        delegate: Rectangle {
                            width: systemsList.width
                            height: 60
                            color: selected ? "#404040" : "#2a2a2a"
                            border.color: selected ? "#0088ff" : "#606060"
                            border.width: 1
                            radius: 3
                            
                            property bool selected: false
                            
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    parent.selected = !parent.selected
                                    // Update selected count
                                    updateSelectedCount()
                                }
                            }
                            
                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 10
                                
                                Rectangle {
                                    width: 20
                                    height: 20
                                    radius: 10
                                    color: getStarColor(model.starType)
                                    border.color: "#ffffff"
                                    border.width: 1
                                }
                                
                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2
                                    
                                    Text {
                                        text: model.name || "Unnamed System"
                                        color: "#ffffff"
                                        font.pixelSize: 14
                                        font.bold: true
                                    }
                                    
                                    Text {
                                        text: "ID: " + model.systemId + " | Type: " + getStarTypeName(model.starType) + " | Size: " + getSystemSizeName(model.systemSize)
                                        color: "#cccccc"
                                        font.pixelSize: 10
                                    }
                                    
                                    Text {
                                        text: "Position: (" + 
                                              (model.positionX !== undefined ? model.positionX.toFixed(1) : "0.0") + ", " + 
                                              (model.positionY !== undefined ? model.positionY.toFixed(1) : "0.0") + ")"
                                        color: "#cccccc"
                                        font.pixelSize: 10
                                    }
                                }
                                
                                CheckBox {
                                    checked: parent.parent.selected
                                    onCheckedChanged: {
                                        parent.parent.selected = checked
                                        updateSelectedCount()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Actions
        GroupBox {
            Layout.fillWidth: true
            title: "Actions"
            
            background: Rectangle {
                color: "#2a2a2a"
                border.color: "#404040"
                radius: 5
            }
            
            label: Text {
                color: "#ffffff"
                text: parent.title
                font.bold: true
            }
            
            GridLayout {
                anchors.fill: parent
                columns: 2
                columnSpacing: 15
                rowSpacing: 10
                
                // Export buttons
                Button {
                    Layout.fillWidth: true
                    text: "Export Selected Systems"
                    enabled: viewModel.selectedCount > 0
                    background: Rectangle {
                        color: enabled ? "#006600" : "#333333"
                        border.color: enabled ? "#008800" : "#555555"
                        radius: 3
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: exportSelectedDialog.open()
                }
                
                Button {
                    Layout.fillWidth: true
                    text: "Export All Systems"
                    background: Rectangle {
                        color: "#006600"
                        border.color: "#008800"
                        radius: 3
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: exportAllDialog.open()
                }
                
                // Import buttons
                Button {
                    Layout.fillWidth: true
                    text: "Import Galaxy"
                    background: Rectangle {
                        color: "#0066cc"
                        border.color: "#0088ff"
                        radius: 3
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: importDialog.open()
                }
                
                Button {
                    Layout.fillWidth: true
                    text: "Import Single System"
                    background: Rectangle {
                        color: "#0066cc"
                        border.color: "#0088ff"
                        radius: 3
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: importSystemDialog.open()
                }
            }
        }
        
        // Status area
        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "#333333"
            border.color: "#555555"
            radius: 3
            
            Text {
                id: statusText
                anchors.centerIn: parent
                text: "Ready"
                color: "#ffffff"
            }
        }
    }
    
    // Footer
    footer: ToolBar {
        background: Rectangle {
            color: "#2a2a2a"
            border.color: "#404040"
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            Item { Layout.fillWidth: true }
            
            Button {
                text: "Close"
                background: Rectangle {
                    color: "#666666"
                    border.color: "#888888"
                    radius: 3
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: window.close()
            }
        }
    }
    
    // File Dialogs
    Platform.FileDialog {
        id: exportSelectedDialog
        title: "Export Selected Systems"
        fileMode: Platform.FileDialog.SaveFile
        nameFilters: ["XML files (*.xml)"]
        defaultSuffix: "xml"
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            if (selectedCount > 0) {
                statusText.text = "Export selected systems not yet implemented. Please use 'Export All' for now."
            } else {
                statusText.text = "Error: No systems selected"
            }
        }
    }
    
    Platform.FileDialog {
        id: exportAllDialog
        title: "Export All Systems"
        fileMode: Platform.FileDialog.SaveFile
        nameFilters: ["XML files (*.xml)"]
        defaultSuffix: "xml"
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            if (galaxyController && galaxyController.galaxyViewModel) {
                // Convert file URL to file path
                var filePath = file.toString().replace("file:///", "")
                viewModel.setFilePath(filePath)
                if (viewModel.exportObject()) {
                    statusText.text = "Successfully exported all systems to " + filePath
                } else {
                    statusText.text = "Export failed: " + viewModel.errorString
                }
            } else {
                statusText.text = "Error: Cannot export - no galaxy data available"
            }
        }
    }
    
    Platform.FileDialog {
        id: importDialog
        title: "Import Galaxy"
        fileMode: Platform.FileDialog.OpenFile
        nameFilters: ["XML files (*.xml)"]
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            if (galaxyController) {
                // Convert file URL to file path
                var filePath = file.toString().replace("file:///", "")
                statusText.text = "Importing galaxy from " + filePath + "..."
                
                // Use the GalaxyController's import function
                if (galaxyController.importGalaxy(filePath)) {
                    statusText.text = "Successfully imported galaxy from " + filePath
                } else {
                    statusText.text = "Failed to import galaxy from " + filePath
                }
            } else {
                statusText.text = "Error: Cannot import - missing galaxy controller"
            }
        }
    }
    
    Platform.FileDialog {
        id: importSystemDialog
        title: "Import Single System"
        fileMode: Platform.FileDialog.OpenFile
        nameFilters: ["XML files (*.xml)"]
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            if (galaxyController) {
                statusText.text = "System import functionality not yet implemented - please use 'Import Galaxy' for now"
            } else {
                statusText.text = "Error: Cannot import - missing galaxy controller"
            }
        }
    }
    
    // Connect to GalaxyController signals for import status updates
    Connections {
        target: galaxyController
        function onImportStarted() {
            statusText.text = "Importing galaxy..."
        }
        function onImportFinished(success, message) {
            statusText.text = message
        }
        function onExportStarted() {
            statusText.text = "Exporting galaxy..."
        }
        function onExportFinished(success, message) {
            statusText.text = message
        }
    }
    
    // Connect to ViewModel signals for status updates
    Connections {
        target: viewModel
        function onErrorStringChanged() {
            if (viewModel.errorString) {
                statusText.text = "Error: " + viewModel.errorString
            }
        }
        function onFilePathChanged() {
            statusText.text = "File path changed to: " + viewModel.getFilePath()
        }
    }
    
    // Helper functions
    function getStarColor(type) {
        switch(type) {
            case 0: return "#ff6961"  // Red Dwarf
            case 1: return "#ffcc6f"  // Yellow Star
            case 2: return "#9bb0ff"  // Blue Star
            case 3: return "#ffffff"  // White Dwarf
            case 4: return "#ffab7a"  // Red Giant
            case 5: return "#e6e6e6"  // Neutron Star
            case 6: return "#000000"  // Black Hole
            default: return "#ffff80" // Default/Unknown
        }
    }
    
    function getStarTypeName(type) {
        const types = ["Red Dwarf", "Yellow Star", "Blue Star", "White Dwarf", "Red Giant", "Neutron Star", "Black Hole"]
        return types[type] || "Unknown"
    }
    
    function getSystemSizeName(size) {
        const sizes = ["Tiny", "Small", "Medium", "Large", "Huge"]
        return sizes[size] || "Unknown"
    }
}
