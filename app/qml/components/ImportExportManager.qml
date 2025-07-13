import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1 as Platform
import GalaxyCore 1.0

ApplicationWindow {
    id: window
    
    property SystemDataManager dataManager: galaxyController ? galaxyController.systemDataManager : null
    property var selectedSystems: []
    
    title: "Galaxy Import/Export Manager"
    width: 900
    height: 700
    minimumWidth: 700
    minimumHeight: 500
    modality: Qt.ApplicationModal
    
    color: "#1a1a1a"
    
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
                        text: "Selected: " + selectedSystems.length + " systems"
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
                            selectedSystems = []
                            for (var i = 0; i < count; i++) {
                                var item = itemAtIndex(i)
                                if (item) {
                                    item.selected = true
                                    selectedSystems.push(model.data(model.index(i, 0), Qt.UserRole))
                                }
                            }
                            selectedSystemsChanged()
                        }
                        
                        function clearSelection() {
                            selectedSystems = []
                            for (var i = 0; i < count; i++) {
                                var item = itemAtIndex(i)
                                if (item) {
                                    item.selected = false
                                }
                            }
                            selectedSystemsChanged()
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
                                    
                                    var system = model.starSystem
                                    if (parent.selected) {
                                        if (selectedSystems.indexOf(system) === -1) {
                                            selectedSystems.push(system)
                                        }
                                    } else {
                                        var index = selectedSystems.indexOf(system)
                                        if (index !== -1) {
                                            selectedSystems.splice(index, 1)
                                        }
                                    }
                                    window.selectedSystemsChanged()
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
                                        text: "Position: (" + model.positionX.toFixed(1) + ", " + model.positionY.toFixed(1) + ")"
                                        color: "#cccccc"
                                        font.pixelSize: 10
                                    }
                                }
                                
                                CheckBox {
                                    checked: parent.parent.selected
                                    onCheckedChanged: parent.parent.selected = checked
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
                    enabled: selectedSystems.length > 0
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
            if (dataManager && selectedSystems.length > 0) {
                var success = dataManager.exportGalaxyToXml(selectedSystems, file)
                if (success) {
                    statusText.text = "Exported " + selectedSystems.length + " systems successfully"
                } else {
                    statusText.text = "Export failed"
                }
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
            if (dataManager && galaxyController && galaxyController.galaxy) {
                var allSystems = []
                var model = galaxyController.systemsModel
                if (model) {
                    for (var i = 0; i < model.rowCount(); i++) {
                        var system = model.data(model.index(i, 0), Qt.UserRole)
                        if (system) {
                            allSystems.push(system)
                        }
                    }
                }
                
                var success = dataManager.exportGalaxyToXml(allSystems, file)
                if (success) {
                    statusText.text = "Exported " + allSystems.length + " systems successfully"
                } else {
                    statusText.text = "Export failed"
                }
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
            if (dataManager) {
                var systems = dataManager.importGalaxyFromXml(file)
                if (systems.length > 0) {
                    statusText.text = "Imported " + systems.length + " systems successfully"
                    // Note: In a full implementation, you'd want to integrate these systems into the current galaxy
                } else {
                    statusText.text = "Import failed or no systems found"
                }
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
            if (dataManager) {
                var system = dataManager.importSystemFromXml(file)
                if (system) {
                    statusText.text = "Imported system: " + system.name
                    // Note: In a full implementation, you'd want to integrate this system into the current galaxy
                } else {
                    statusText.text = "Import failed"
                }
            }
        }
    }
    
    // Component initialization
    Component.onCompleted: {
        if (dataManager) {
            // Connect to data manager signals for feedback
            dataManager.systemExported.connect(function(filePath) {
                statusText.text = "Exported to: " + filePath
            })
            dataManager.systemImported.connect(function(filePath) {
                statusText.text = "Imported from: " + filePath
            })
            dataManager.exportError.connect(function(message) {
                statusText.text = "Export error: " + message
            })
            dataManager.importError.connect(function(message) {
                statusText.text = "Import error: " + message
            })
        }
    }
    
    // Helper functions
    function getStarColor(type) {
        switch(type) {
            case 0: return "#9bb0ff"  // O-Class (Blue Giant)
            case 1: return "#aabfff"  // B-Class (Blue-White)
            case 2: return "#ffffff"  // A-Class (White)
            case 3: return "#fff2a1"  // F-Class (Yellow-White)
            case 4: return "#ffcc6f"  // G-Class (Yellow)
            case 5: return "#ffab7a"  // K-Class (Orange)
            case 6: return "#ff6961"  // M-Class (Red Dwarf)
            default: return "#ffff80" // Default/Unknown
        }
    }
    
    function getStarTypeName(type) {
        const types = ["O-Class", "B-Class", "A-Class", "F-Class", "G-Class", "K-Class", "M-Class"]
        return types[type] || "Unknown"
    }
    
    function getSystemSizeName(size) {
        const sizes = ["Tiny", "Small", "Medium", "Large", "Huge"]
        return sizes[size] || "Unknown"
    }
}
