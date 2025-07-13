import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import Qt.labs.platform 1.1 as Platform
import GalaxyCore 1.0

ApplicationWindow {
    id: window
    
    property StarSystem starSystem: null
    property SystemDataManager dataManager: galaxyController ? galaxyController.systemDataManager : null
    property bool autoSaveEnabled: true
    
    title: starSystem ? "System Properties - " + starSystem.name : "System Properties"
    width: 800
    height: 600
    minimumWidth: 600
    minimumHeight: 400
    modality: Qt.ApplicationModal
    
    color: "#1a1a1a"
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        
        ColumnLayout {
            width: window.width - 20
            spacing: 15
            
            // Star Properties Section
            GroupBox {
                Layout.fillWidth: true
                title: "Star Properties"
                
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
                    columnSpacing: 10
                    rowSpacing: 10
                    
                    Text {
                        text: "Name:"
                        color: "#cccccc"
                    }
                    TextField {
                        id: nameField
                        Layout.fillWidth: true
                        text: starSystem ? starSystem.name : ""
                        color: "#ffffff"
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        onEditingFinished: {
                            if (starSystem) starSystem.name = text
                        }
                    }
                    
                    Text {
                        text: "Star Type:"
                        color: "#cccccc"
                    }
                    ComboBox {
                        id: starTypeCombo
                        Layout.fillWidth: true
                        model: ["Red Dwarf", "Yellow Star", "Blue Star", "Red Giant", "White Dwarf", "Neutron Star", "Black Hole"]
                        currentIndex: starSystem ? starSystem.starType : 1
                        
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        
                        contentItem: Text {
                            text: parent.displayText
                            color: "#ffffff"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 10
                        }
                        
                        onCurrentIndexChanged: {
                            if (starSystem) starSystem.starType = currentIndex
                        }
                    }
                    
                    Text {
                        text: "Mass (Solar masses):"
                        color: "#cccccc"
                    }
                    SpinBox {
                        id: massSpinBox
                        Layout.fillWidth: true
                        from: 1
                        to: 10000
                        stepSize: 1
                        value: starSystem ? Math.round(starSystem.starMass * 100) : 100
                        
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        
                        contentItem: TextInput {
                            text: (parent.value / 100).toFixed(2)
                            color: "#ffffff"
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            readOnly: !parent.editable
                            validator: DoubleValidator {
                                bottom: 0.01
                                top: 100.00
                                decimals: 2
                            }
                        }
                        
                        onValueModified: {
                            if (starSystem) starSystem.starMass = value / 100.0
                        }
                    }
                    
                    Text {
                        text: "Temperature (K):"
                        color: "#cccccc"
                    }
                    SpinBox {
                        id: temperatureSpinBox
                        Layout.fillWidth: true
                        from: 1000
                        to: 100000
                        stepSize: 100
                        value: starSystem ? starSystem.starTemperature : 5778
                        
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        
                        contentItem: TextInput {
                            text: parent.value.toString()
                            color: "#ffffff"
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            readOnly: !parent.editable
                        }
                        
                        onValueModified: {
                            if (starSystem) starSystem.starTemperature = value
                        }
                    }
                    
                    Text {
                        text: "Luminosity (Solar luminosities):"
                        color: "#cccccc"
                    }
                    SpinBox {
                        id: luminositySpinBox
                        Layout.fillWidth: true
                        from: 1
                        to: 1000000
                        stepSize: 1
                        value: starSystem ? Math.round(starSystem.starLuminosity * 1000) : 1000
                        
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        
                        contentItem: TextInput {
                            text: (parent.value / 1000).toFixed(3)
                            color: "#ffffff"
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            readOnly: !parent.editable
                        }
                        
                        onValueModified: {
                            if (starSystem) starSystem.starLuminosity = value / 1000.0
                        }
                    }
                }
            }
            
            // System Visualization Section
            GroupBox {
                Layout.fillWidth: true
                Layout.preferredHeight: 300
                title: "System Visualization"
                
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
                
                Rectangle {
                    anchors.fill: parent
                    color: "#0a0a0a"
                    radius: 3
                    
                    // Star at center
                    Rectangle {
                        id: starViz
                        width: 20
                        height: 20
                        radius: 10
                        anchors.centerIn: parent
                        color: starSystem ? starSystem.starColor : "#ffff80"
                        border.color: "#ffffff"
                        border.width: 1
                        
                        // Star glow effect
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width + 10
                            height: parent.height + 10
                            radius: (parent.width + 10) / 2
                            color: parent.color
                            opacity: 0.3
                        }
                    }
                    
                    // Planet orbits and planets
                    Repeater {
                        model: starSystem ? starSystem.planets : []
                        
                        Item {
                            anchors.centerIn: parent
                            
                            // Orbit circle
                            Rectangle {
                                id: orbit
                                width: modelData.orbitDistance * 2
                                height: modelData.orbitDistance * 2
                                radius: modelData.orbitDistance
                                anchors.centerIn: parent
                                color: "transparent"
                                border.color: "#404040"
                                border.width: 1
                                opacity: 0.5
                            }
                            
                            // Planet
                            Rectangle {
                                id: planet
                                width: 8 + modelData.size * 2
                                height: 8 + modelData.size * 2
                                radius: (8 + modelData.size * 2) / 2
                                color: getPlanetColor(modelData.type)
                                border.color: "#ffffff"
                                border.width: 1
                                x: parent.width/2 + modelData.orbitDistance - width/2
                                y: parent.height/2 - height/2
                                
                                ToolTip {
                                    visible: planetMouseArea.containsMouse
                                    text: modelData.name + "\\nType: " + getPlanetTypeName(modelData.type) + 
                                          "\\nMoons: " + modelData.moonCount + 
                                          "\\nMass: " + modelData.mass.toFixed(2) + " Earth masses"
                                    delay: 500
                                }
                                
                                MouseArea {
                                    id: planetMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        planetListView.currentIndex = index
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            // Planets Section
            GroupBox {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: "Planets"
                
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
                    
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Button {
                            text: "Add Planet"
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
                            onClicked: {
                                if (starSystem) {
                                    starSystem.addPlanet()
                                }
                            }
                        }
                        
                        Button {
                            text: "Remove Selected"
                            enabled: planetListView.currentIndex >= 0
                            background: Rectangle {
                                color: enabled ? "#cc3300" : "#666666"
                                border.color: enabled ? "#ff4400" : "#888888"
                                radius: 3
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                if (starSystem && planetListView.currentIndex >= 0) {
                                    starSystem.removePlanetAt(planetListView.currentIndex)
                                    planetListView.currentIndex = -1
                                }
                            }
                        }
                        
                        Item { Layout.fillWidth: true }
                    }
                    
                    ListView {
                        id: planetListView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: starSystem ? starSystem.planets : []
                        currentIndex: -1
                        
                        delegate: Rectangle {
                            width: planetListView.width
                            height: planetProperties.height + 20
                            color: ListView.isCurrentItem ? "#404040" : "#2a2a2a"
                            border.color: ListView.isCurrentItem ? "#0088ff" : "#606060"
                            border.width: 1
                            radius: 3
                            
                            MouseArea {
                                anchors.fill: parent
                                onClicked: planetListView.currentIndex = index
                            }
                            
                            GridLayout {
                                id: planetProperties
                                anchors.fill: parent
                                anchors.margins: 10
                                columns: 4
                                columnSpacing: 10
                                rowSpacing: 5
                                
                                Text {
                                    text: "Name:"
                                    color: "#cccccc"
                                }
                                TextField {
                                    Layout.fillWidth: true
                                    text: modelData.name
                                    color: "#ffffff"
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    onEditingFinished: modelData.name = text
                                }
                                
                                Text {
                                    text: "Type:"
                                    color: "#cccccc"
                                }
                                ComboBox {
                                    Layout.fillWidth: true
                                    model: ["Terrestrial", "Gas Giant", "Ice Giant", "Desert", "Ocean", "Volcanic", "Frozen"]
                                    currentIndex: modelData.type
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: Text {
                                        text: parent.displayText
                                        color: "#ffffff"
                                        verticalAlignment: Text.AlignVCenter
                                        leftPadding: 10
                                    }
                                    
                                    onCurrentIndexChanged: modelData.type = currentIndex
                                }
                                
                                Text {
                                    text: "Moons:"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 100
                                    value: modelData.moonCount
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: parent.value
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.moonCount = value
                                }
                                
                                Text {
                                    text: "Size:"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 1
                                    to: 10
                                    value: modelData.size
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: parent.value
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.size = value
                                }
                                
                                Text {
                                    text: "Mass (Earth masses):"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 1
                                    to: 100000
                                    stepSize: 1
                                    value: Math.round(modelData.mass * 100)
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: (parent.value / 100).toFixed(2)
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.mass = value / 100.0
                                }
                                
                                Text {
                                    text: "Gravity (Earth gravity):"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 1
                                    to: 1000
                                    stepSize: 1
                                    value: Math.round(modelData.gravity * 100)
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: (parent.value / 100).toFixed(2)
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.gravity = value / 100.0
                                }
                                
                                Text {
                                    text: "Min Temp (K):"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 1000
                                    value: modelData.minTemperature
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: parent.value
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.minTemperature = value
                                }
                                
                                Text {
                                    text: "Max Temp (K):"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 0
                                    to: 1000
                                    value: modelData.maxTemperature
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: parent.value
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.maxTemperature = value
                                }
                                
                                Text {
                                    text: "Orbit Distance:"
                                    color: "#cccccc"
                                }
                                SpinBox {
                                    Layout.fillWidth: true
                                    from: 10
                                    to: 500
                                    value: modelData.orbitDistance
                                    
                                    background: Rectangle {
                                        color: "#404040"
                                        border.color: "#606060"
                                        radius: 3
                                    }
                                    
                                    contentItem: TextInput {
                                        text: parent.value
                                        color: "#ffffff"
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        readOnly: !parent.editable
                                    }
                                    
                                    onValueModified: modelData.orbitDistance = value
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Footer with Import/Export and Close buttons
    footer: ToolBar {
        background: Rectangle {
            color: "#2a2a2a"
            border.color: "#404040"
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            // Auto-save toggle
            CheckBox {
                id: autoSaveCheck
                text: "Auto-save"
                checked: autoSaveEnabled
                
                background: Rectangle {
                    color: "transparent"
                }
                
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    leftPadding: parent.indicator.width + parent.spacing
                    verticalAlignment: Text.AlignVCenter
                }
                
                onCheckedChanged: {
                    autoSaveEnabled = checked
                    if (dataManager) {
                        dataManager.enableAutoSave(checked)
                    }
                }
            }
            
            // Export button
            Button {
                text: "Export XML"
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
                onClicked: exportDialog.open()
            }
            
            // Import button
            Button {
                text: "Import XML"
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
            
            // Save button
            Button {
                text: "Save Now"
                background: Rectangle {
                    color: "#cc6600"
                    border.color: "#ff8800"
                    radius: 3
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: saveSystemData()
            }
            
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
        id: exportDialog
        title: "Export System Data"
        fileMode: Platform.FileDialog.SaveFile
        nameFilters: ["XML files (*.xml)"]
        defaultSuffix: "xml"
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            if (starSystem && dataManager) {
                var success = dataManager.exportSystemToXml(starSystem, file)
                if (success) {
                    statusLabel.text = "System exported successfully"
                } else {
                    statusLabel.text = "Export failed"
                }
            }
        }
    }
    
    Platform.FileDialog {
        id: importDialog
        title: "Import System Data"
        fileMode: Platform.FileDialog.OpenFile
        nameFilters: ["XML files (*.xml)"]
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            if (starSystem && dataManager) {
                var importedSystem = dataManager.importSystemFromXml(file)
                if (importedSystem) {
                    // Copy data from imported system to current system
                    starSystem.name = importedSystem.name
                    starSystem.starType = importedSystem.starType
                    starSystem.systemSize = importedSystem.systemSize
                    starSystem.starMass = importedSystem.starMass
                    starSystem.starTemperature = importedSystem.starTemperature
                    starSystem.starLuminosity = importedSystem.starLuminosity
                    
                    statusLabel.text = "System imported successfully"
                    refreshUI()
                } else {
                    statusLabel.text = "Import failed"
                }
            }
        }
    }
    
    // Status label for feedback
    Text {
        id: statusLabel
        visible: text !== ""
        text: ""
        color: "#ffff00"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        
        Timer {
            id: statusTimer
            interval: 3000
            onTriggered: statusLabel.text = ""
        }
        
        onTextChanged: {
            if (text !== "") {
                statusTimer.restart()
            }
        }
    }
    
    // Component initialization
    Component.onCompleted: {
        if (dataManager) {
            dataManager.enableAutoSave(autoSaveEnabled)
            // Connect to data manager signals for feedback
            dataManager.systemExported.connect(function(filePath) {
                statusLabel.text = "Exported to: " + filePath
            })
            dataManager.systemImported.connect(function(filePath) {
                statusLabel.text = "Imported from: " + filePath
            })
            dataManager.exportError.connect(function(message) {
                statusLabel.text = "Export error: " + message
            })
            dataManager.importError.connect(function(message) {
                statusLabel.text = "Import error: " + message
            })
        }
        
        // Load any existing data for this system
        loadSystemData()
    }
    
    // Helper functions
    function saveSystemData() {
        if (starSystem && dataManager) {
            var success = dataManager.saveSystemChanges(starSystem)
            if (success) {
                statusLabel.text = "System data saved"
            } else {
                statusLabel.text = "Save failed"
            }
        }
    }
    
    function loadSystemData() {
        if (starSystem && dataManager) {
            var success = dataManager.loadSystemData(starSystem)
            if (success) {
                statusLabel.text = "System data loaded"
                refreshUI()
            }
        }
    }
    
    function refreshUI() {
        // Force UI refresh by updating bound properties
        if (starSystem) {
            nameField.text = starSystem.name
            starTypeCombo.currentIndex = starSystem.starType
            massSpinBox.value = Math.round(starSystem.starMass * 100)
            temperatureSpinBox.value = starSystem.starTemperature
            luminositySpinBox.value = Math.round(starSystem.starLuminosity * 1000)
        }
    }
    
    function setupAutoSave() {
        if (!autoSaveEnabled || !dataManager || !starSystem) return
        
        // Connect to all value change signals for auto-save
        nameField.editingFinished.connect(function() {
            if (autoSaveEnabled) saveSystemData()
        })
        
        starTypeCombo.currentIndexChanged.connect(function() {
            if (autoSaveEnabled) saveSystemData()
        })
        
        massSpinBox.valueModified.connect(function() {
            if (autoSaveEnabled) saveSystemData()
        })
        
        temperatureSpinBox.valueModified.connect(function() {
            if (autoSaveEnabled) saveSystemData()
        })
        
        luminositySpinBox.valueModified.connect(function() {
            if (autoSaveEnabled) saveSystemData()
        })
    }
    
    // Auto-save setup when system is set
    onStarSystemChanged: {
        if (starSystem) {
            loadSystemData()
            setupAutoSave()
        }
    }
    
    function getPlanetColor(type) {
        switch(type) {
            case 0: return "#8B4513"  // Terrestrial - Brown
            case 1: return "#FFA500"  // Gas Giant - Orange
            case 2: return "#87CEEB"  // Ice Giant - Light Blue
            case 3: return "#F4A460"  // Desert - Sandy Brown
            case 4: return "#0077BE"  // Ocean - Ocean Blue
            case 5: return "#FF4500"  // Volcanic - Red Orange
            case 6: return "#E0E0E0"  // Frozen - Light Gray
            default: return "#808080" // Default - Gray
        }
    }
    
    function getPlanetTypeName(type) {
        const types = ["Terrestrial", "Gas Giant", "Ice Giant", "Desert", "Ocean", "Volcanic", "Frozen"]
        return types[type] || "Unknown"
    }
}
