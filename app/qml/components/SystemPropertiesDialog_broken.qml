import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Window
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0
import GalaxyCore.Model 1.0

ApplicationWindow {
    id: window
    
    property var starSystemViewModel: null
    
    title: starSystemViewModel ? "System Properties - " + starSystemViewModel.name : "System Properties"
    width: 800
    height: 600
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    
    // Background
    color: "#1a1a1a"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ColumnLayout {
                width: window.width - 40
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
                        text: starSystemViewModel ? starSystemViewModel.name : ""
                        color: "#ffffff"
                        background: Rectangle {
                            color: "#404040"
                            border.color: "#606060"
                            radius: 3
                        }
                        onEditingFinished: {
                            if (starSystemViewModel) starSystemViewModel.name = text
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
                        currentIndex: starSystemViewModel ? starSystemViewModel.starType : 1

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
                            if (starSystemViewModel) starSystemViewModel.starType = currentIndex
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
                        value: starSystemViewModel ? Math.round(starSystemViewModel.starMass * 100) : 100
                        
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
                            if (starSystemViewModel) starSystemViewModel.starMass = value / 100.0
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
                        value: starSystemViewModel ? starSystemViewModel.starTemperature : 5778

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
                            if (starSystemViewModel) starSystemViewModel.starTemperature = value
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
                        color: (starSystemViewModel && starSystemViewModel.starSystem) ? starSystemViewModel && starSystemViewModel.starSystem.color : "#ffff80"
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
                        model: starSystemViewModel ? starSystemViewModel && starSystemViewModel.planetsModel : null

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
                                if (starSystemViewModel) {
                                    console.log("starSystemViewModel available, hasDataManager:", starSystemViewModel && starSystemViewModel.hasDataManager())
                                    starSystemViewModel && starSystemViewModel.addPlanet()
                                } else {
                                    console.log("Error: starSystemViewModel not available")
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
                                if (starSystemViewModel && planetListView.currentIndex >= 0) {
                                    starSystemViewModel && starSystemViewModel.removePlanet(planetListView.currentIndex)
                                    planetListView.currentIndex = -1
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
                        model: (starSystemViewModel && starSystemViewModel.starSystem) ? starSystemViewModel && starSystemViewModel.starSystem.planetsModel : []
                        
                        delegate: PlanetPropertyView{
                            width: planetListView.width
                            isCurrentItem: ListView.isCurrentItem
                        }
                    }
                }
            }
        }
            } // Close ColumnLayout (ScrollView content)
        } // Close ScrollView
        
        // Button row at the bottom (outside ScrollView)
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            spacing: 10
            
            // Export button
            Button {
                text: "Export XML"
                enabled: starSystemViewModel && !starSystemViewModel.isSaving && !starSystemViewModel.isLoading
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
                onClicked: {
                    // TODO: Re-enable when FileDialog is fixed
                    console.log("Export functionality temporarily disabled")
                }
            }
            
            // Import button
            Button {
                text: "Import XML"
                enabled: starSystemViewModel && !starSystemViewModel.isSaving && !starSystemViewModel.isLoading
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
                onClicked: {
                    // TODO: Re-enable when FileDialog is fixed
                    console.log("Import functionality temporarily disabled")
                }
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
    FileDialog {
        id: exportDialog
        title: "Export System Data"
        fileMode: FileDialog.SaveFile
        nameFilters: ["XML files (*.xml)"]
        defaultSuffix: "xml"
        
        onAccepted: {
            starSystemViewModel && starSystemViewModel.exportToXml(selectedFile)
        }
    }
    
    FileDialog {
        id: importDialog
        title: "Import System Data"
        fileMode: FileDialog.OpenFile
        nameFilters: ["XML files (*.xml)"]
        
        onAccepted: {
            starSystemViewModel && starSystemViewModel.importFromXml(selectedFile)
        }
    }
    
    // Status label for feedback
    Text {
        id: statusLabel
        visible: text !== ""
        text: starSystemViewModel && starSystemViewModel.statusMessage
        color: "#ffff00"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10
        
        Timer {
            id: statusTimer
            interval: 3000
            onTriggered: {
                if (starSystemViewModel) {
                    starSystemViewModel && starSystemViewModel.clearStatusMessage()
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
        // starSystemViewModel handles all data manager interactions
        console.log("SystemPropertiesDialog initialized with starSystemViewModel")
        // Load any existing data for this system
        loadSystemData()
    }
    
    // Helper functions
    function saveSystemData() {
        if (starSystemViewModel) {
            return starSystemViewModel && starSystemViewModel.saveSystemData()
        }
        return false
    }
    
    function loadSystemData() {
        if (starSystemViewModel) {
            return starSystemViewModel && starSystemViewModel.loadSystemData()
        }
        return false
    }
    
    function refreshUI() {
        // Force UI refresh by updating bound properties
        if (starSystemViewModel && starSystemViewModel.starSystem) {
            nameField.text = starSystemViewModel && starSystemViewModel.starSystem.name
            starTypeCombo.currentIndex = starSystemViewModel && starSystemViewModel.starSystem.starType
            massSpinBox.value = Math.round(starSystemViewModel && starSystemViewModel.starSystem.starMass * 100)
            temperatureSpinBox.value = starSystemViewModel && starSystemViewModel.starSystem.starTemperature
            luminositySpinBox.value = Math.round(starSystemViewModel && starSystemViewModel.starSystem.starLuminosity * 1000)
        }
    }
    
    // Auto-setup when starSystemViewModel changes
    Connections {
        target: starSystemViewModel
        function onStarSystemChanged() {
            if (starSystemViewModel && starSystemViewModel.starSystem) {
                loadSystemData()
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








