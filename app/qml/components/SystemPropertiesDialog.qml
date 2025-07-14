import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.platform 1.1 as Platform
import GalaxyCore 1.0

ApplicationWindow {
    id: window
    
    property StarSystem starSystem: null
    
    title: viewModel.starSystem ? "System Properties - " + viewModel.starSystem.name : "System Properties"
    width: 800
    height: 600
    minimumWidth: 600
    minimumHeight: 400
    modality: Qt.ApplicationModal
    
    color: "#1a1a1a"
    
    // MVVM ViewModel
    SystemPropertiesViewModel {
        id: viewModel
        starSystem: window.starSystem
        isAutoSaveEnabled: true
        
        Component.onCompleted: {
            // Set data manager from global context
            console.log("ViewModel Component.onCompleted - galaxyController:", galaxyController)
            if (galaxyController && galaxyController.systemDataManager) {
                console.log("Setting data manager:", galaxyController.systemDataManager)
                setDataManager(galaxyController.systemDataManager)
                console.log("Data manager set, hasDataManager:", hasDataManager())
            } else {
                console.log("Error: galaxyController or systemDataManager not available")
            }
        }
    }
    
    // Save data when dialog is closing
    onClosing: {
        viewModel.saveSystemData()
    }
    
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
                        text: viewModel.starSystem ? viewModel.starSystem.name : ""
                        color: "#ffffff"
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        onEditingFinished: {
                            if (viewModel.starSystem) viewModel.starSystem.name = text
                        }
                    }
                    
                    Text {
                        text: "Star Type:"
                        color: "#cccccc"
                    }
                    ComboBox {
                        id: starTypeCombo
                        Layout.fillWidth: true
                        model: ["Red Dwarf", "Yellow Star", "Blue Star", "White Dwarf", "Red Giant", "Neutron Star", "Black Hole"]
                        currentIndex: viewModel.starSystem ? viewModel.starSystem.starType : 1
                        
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
                            if (viewModel.starSystem) viewModel.starSystem.starType = currentIndex
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
                        value: viewModel.starSystem ? Math.round(viewModel.starSystem.starMass * 100) : 100
                        
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
                            if (viewModel.starSystem) viewModel.starSystem.starMass = value / 100.0
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
                        value: viewModel.starSystem ? viewModel.starSystem.starTemperature : 5778
                        
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
                            if (viewModel.starSystem) viewModel.starSystem.starTemperature = value
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
                        value: viewModel.starSystem ? Math.round(viewModel.starSystem.starLuminosity * 1000) : 1000
                        
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
                            if (viewModel.starSystem) viewModel.starSystem.starLuminosity = value / 1000.0
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
                        color: viewModel.starSystem ? viewModel.starSystem.starColor : "#ffff80"
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
                        model: viewModel.starSystem ? viewModel.starSystem.planetsModel : null
                        
                        Item {
                            anchors.centerIn: parent
                            
                            // Orbit circle
                            Rectangle {
                                id: orbit
                                width: model.orbitDistance * 2
                                height: model.orbitDistance * 2
                                radius: model.orbitDistance
                                anchors.centerIn: parent
                                color: "transparent"
                                border.color: "#404040"
                                border.width: 1
                                opacity: 0.5
                            }
                            
                            // Planet
                            Rectangle {
                                id: planet
                                width: 8 + model.size * 2
                                height: 8 + model.size * 2
                                radius: (8 + model.size * 2) / 2
                                color: getPlanetColor(model.type)
                                border.color: "#ffffff"
                                border.width: 1
                                x: parent.width/2 + model.orbitDistance - width/2
                                y: parent.height/2 - height/2
                                
                                ToolTip {
                                    visible: planetMouseArea.containsMouse
                                    text: model.name + "\\nType: " + getPlanetTypeName(model.type) + 
                                          "\\nMoons: " + model.moons + 
                                          "\\nMass: " + model.mass.toFixed(2) + " Earth masses"
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
                                console.log("Add Planet button clicked")
                                if (viewModel) {
                                    console.log("ViewModel available, hasDataManager:", viewModel.hasDataManager())
                                    viewModel.addPlanet()
                                    if (viewModel.isAutoSaveEnabled) {
                                        console.log("Auto-save enabled, calling saveSystemData")
                                        viewModel.saveSystemData()
                                    } else {
                                        console.log("Auto-save disabled")
                                    }
                                } else {
                                    console.log("Error: ViewModel not available")
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
                                if (viewModel && planetListView.currentIndex >= 0) {
                                    viewModel.removePlanet(planetListView.currentIndex)
                                    planetListView.currentIndex = -1
                                    if (viewModel.isAutoSaveEnabled) viewModel.saveSystemData()
                                }
                            }
                        }
                        
                        Item { Layout.fillWidth: true }
                    }
                    
                    ListView {
                        id: planetListView
                        Layout.fillWidth: true
                        Layout.preferredHeight: Math.max(100, contentHeight)
                        Layout.maximumHeight: 400
                        clip: true
                        model: viewModel.starSystem ? viewModel.starSystem.planetsModel : []
                        
                        delegate: PlanetPropertyView{
                            width: planetListView.width
                            isCurrentItem: ListView.isCurrentItem
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
            //anchors.margins: 10
            
            // Auto-save toggle
            CheckBox {
                id: autoSaveCheck
                text: "Auto-save"
                checked: viewModel.isAutoSaveEnabled
                
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
                    viewModel.isAutoSaveEnabled = checked
                }
            }
            
            // Export button
            Button {
                text: "Export XML"
                enabled: !viewModel.isSaving && !viewModel.isLoading
                background: Rectangle {
                    color: enabled ? "#006600" : "#444444"
                    border.color: enabled ? "#008800" : "#666666"
                    radius: 3
                }
                contentItem: Text {
                    text: parent.text
                    color: enabled ? "#ffffff" : "#999999"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: exportDialog.open()
            }
            
            // Import button
            Button {
                text: "Import XML"
                enabled: !viewModel.isSaving && !viewModel.isLoading
                background: Rectangle {
                    color: enabled ? "#0066cc" : "#444444"
                    border.color: enabled ? "#0088ff" : "#666666"
                    radius: 3
                }
                contentItem: Text {
                    text: parent.text
                    color: enabled ? "#ffffff" : "#999999"
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
            viewModel.exportToXml(file)
        }
    }
    
    Platform.FileDialog {
        id: importDialog
        title: "Import System Data"
        fileMode: Platform.FileDialog.OpenFile
        nameFilters: ["XML files (*.xml)"]
        folder: Platform.StandardPaths.writableLocation(Platform.StandardPaths.DocumentsLocation)
        
        onAccepted: {
            viewModel.importFromXml(file)
        }
    }
    
    // Status label for feedback
    Text {
        id: statusLabel
        visible: text !== ""
        text: viewModel.statusMessage
        color: "#ffff00"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        
        Timer {
            id: statusTimer
            interval: 3000
            onTriggered: {
                if (viewModel) {
                    viewModel.clearStatusMessage()
                }
            }
        }
        
        onTextChanged: {
            if (text !== "") {
                statusTimer.restart()
            }
        }
    }
    
    // Component initialization
    Component.onCompleted: {
        // ViewModel handles all data manager interactions
        console.log("SystemPropertiesDialog initialized with ViewModel")
        // Load any existing data for this system
        loadSystemData()
    }
    
    // Helper functions
    function saveSystemData() {
        if (viewModel) {
            return viewModel.saveSystemData()
        }
        return false
    }
    
    function loadSystemData() {
        if (viewModel) {
            return viewModel.loadSystemData()
        }
        return false
    }
    
    function refreshUI() {
        // Force UI refresh by updating bound properties
        if (viewModel.starSystem) {
            nameField.text = viewModel.starSystem.name
            starTypeCombo.currentIndex = viewModel.starSystem.starType
            massSpinBox.value = Math.round(viewModel.starSystem.starMass * 100)
            temperatureSpinBox.value = viewModel.starSystem.starTemperature
            luminositySpinBox.value = Math.round(viewModel.starSystem.starLuminosity * 1000)
        }
    }
    
    function setupAutoSave() {
        if (!viewModel.isAutoSaveEnabled || !viewModel.starSystem) return
        
        // Connect to all value change signals for auto-save
        nameField.editingFinished.connect(function() {
            if (viewModel.isAutoSaveEnabled) viewModel.saveSystemData()
        })
        
        starTypeCombo.currentIndexChanged.connect(function() {
            if (viewModel.isAutoSaveEnabled) viewModel.saveSystemData()
        })
        
        massSpinBox.valueModified.connect(function() {
            if (viewModel.isAutoSaveEnabled) viewModel.saveSystemData()
        })
        
        temperatureSpinBox.valueModified.connect(function() {
            if (viewModel.isAutoSaveEnabled) viewModel.saveSystemData()
        })
        
        luminositySpinBox.valueModified.connect(function() {
            if (viewModel.isAutoSaveEnabled) viewModel.saveSystemData()
        })
    }
    
    // Auto-save setup when viewModel changes
    Connections {
        target: viewModel
        function onStarSystemChanged() {
            if (viewModel.starSystem) {
                loadSystemData()
                setupAutoSave()
            }
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
