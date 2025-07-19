import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GalaxyCore.ViewModels

Dialog {
    id: root
    title: "Planet Details"
    modal: true
    width: 600
    height: 600
    
    property var planetViewModel: PlanetViewModel{}
    property bool isNewPlanet: false
    property bool editMode: false
    property string originalPlanetName: ""
    
    signal planetUpdated()
    signal planetCreated(string name, int type, real size, real mass, int moons, real radius, real maxTemp, real minTemp)
    signal planetEdited(string originalName, string name, int type, real size, real mass, int moons, real radius, real maxTemp, real minTemp)
    
    background: Rectangle {
        color: "#2d2d2d"
        border.color: "#404040"
        radius: 5
    }
    
    header: Rectangle {
        height: 40
        color: "#333333"
        radius: 5
        
        Text {
            anchors.centerIn: parent
            text: editMode ? "Edit Planet Details" : (isNewPlanet ? "Create New Planet" : "Planet Details")
            color: "#ffffff"
            font.bold: true
            font.pixelSize: 16
        }
    }

    contentItem: ScrollView {
        id: scrollview
        anchors.margins: 5
        anchors.left: root.left
        anchors.right: root.right
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            id: contentLayout
            anchors.top: parent.top
            anchors.margins: 10
            width: scrollview.width - 20
            spacing: 15
            
            // Planet Name
            GridLayout {
                columns: 2
                columnSpacing: 10
                Layout.fillWidth: true
                
                Text {
                    text: "Name:"
                    color: "#cccccc"
                }
                TextField {
                    id: nameField
                    Layout.fillWidth: true
                    text: planetViewModel ? planetViewModel.name : ""
                    color: "#ffffff"
                    background: Rectangle {
                        color: "#404040"
                        border.color: "#606060"
                        radius: 3
                    }
                    onEditingFinished: {
                        if (planetViewModel) planetViewModel.name = text
                    }
                }
                
                // Planet Type
                Text {
                    text: "Type:"
                    color: "#cccccc"
                }
                ComboBox {
                    id: typeCombo
                    Layout.fillWidth: true
                    model: ["Rocky", "Gas Giant", "Ice Giant", "Desert", "Ocean", "Frozen", "Volcanic", "Toxic"]
                    currentIndex: planetViewModel ? planetViewModel.planetType : 0
                    
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
                        if (planetViewModel) planetViewModel.planetType = currentIndex
                    }
                }
                
                // Planet Size
                Text {
                    text: "Size (Earth = 1.0):"
                    color: "#cccccc"
                }
                SpinBox {
                    id: sizeSpinBox
                    Layout.fillWidth: true
                    from: 10  // 0.1 Earth sizes
                    to: 10000  // 100.0 Earth sizes
                    stepSize: 10
                    value: planetViewModel ? (planetViewModel.size * 100) : 100
                    
                    property real realValue: value / 100.0
                    
                    background: Rectangle {
                        color: "#404040"
                        border.color: "#606060"
                        radius: 3
                    }
                    
                    contentItem: TextInput {
                        text: (parent.value / 100.0).toFixed(2)
                        color: "#ffffff"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        readOnly: true
                    }
                    
                    onValueChanged: {
                        if (planetViewModel) planetViewModel.size = realValue
                    }
                }
                
                // Planet Mass
                Text {
                    text: "Mass (Earth = 1.0):"
                    color: "#cccccc"
                }
                SpinBox {
                    id: massSpinBox
                    Layout.fillWidth: true
                    from: 1  // 0.01 Earth masses
                    to: 100000  // 1000.0 Earth masses
                    stepSize: 10
                    value: planetViewModel ? (planetViewModel.mass * 100) : 100
                    
                    property real realValue: value / 100.0
                    
                    background: Rectangle {
                        color: "#404040"
                        border.color: "#606060"
                        radius: 3
                    }
                    
                    contentItem: TextInput {
                        text: (parent.value / 100.0).toFixed(2)
                        color: "#ffffff"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        readOnly: true
                    }
                    
                    onValueChanged: {
                        if (planetViewModel) planetViewModel.mass = realValue
                    }
                }
                
                // Number of Moons
                Text {
                    text: "Number of Moons:"
                    color: "#cccccc"
                }
                SpinBox {
                    id: moonsSpinBox
                    Layout.fillWidth: true
                    from: 0
                    to: 100
                    value: planetViewModel ? planetViewModel.numberOfMoons : 0
                    
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
                        readOnly: true
                    }
                    
                    onValueChanged: {
                        if (planetViewModel) planetViewModel.numberOfMoons = value
                    }
                }
                
                // Orbital Radius
                Text {
                    text: "Orbital Radius (AU):"
                    color: "#cccccc"
                }
                SpinBox {
                    id: radiusSpinBox
                    Layout.fillWidth: true
                    from: 10  // 0.1 AU
                    to: 10000  // 100.0 AU
                    stepSize: 10
                    value: planetViewModel ? (planetViewModel.orbitalRadius * 100) : 100
                    
                    property real realValue: value / 100.0
                    
                    background: Rectangle {
                        color: "#404040"
                        border.color: "#606060"
                        radius: 3
                    }
                    
                    contentItem: TextInput {
                        text: (parent.value / 100.0).toFixed(2)
                        color: "#ffffff"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        readOnly: true
                    }
                    
                    onValueChanged: {
                        if (planetViewModel) planetViewModel.orbitalRadius = realValue
                    }
                }
                
                // Maximum Temperature
                Text {
                    text: "Max Temperature (K):"
                    color: "#cccccc"
                }
                SpinBox {
                    id: maxTempSpinBox
                    Layout.fillWidth: true
                    from: 1  // 1 K
                    to: 200000  // 2000 K
                    stepSize: 100
                    value: planetViewModel ? (planetViewModel.maxTemperature * 100) : 28800  // 288 K default
                    
                    property real realValue: value / 100.0
                    
                    background: Rectangle {
                        color: "#404040"
                        border.color: "#606060"
                        radius: 3
                    }
                    
                    contentItem: TextInput {
                        text: (parent.value / 100.0).toFixed(1)
                        color: "#ffffff"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        readOnly: true
                    }
                    
                    onValueChanged: {
                        if (planetViewModel) planetViewModel.maxTemperature = realValue
                    }
                }
                
                // Minimum Temperature
                Text {
                    text: "Min Temperature (K):"
                    color: "#cccccc"
                }
                SpinBox {
                    id: minTempSpinBox
                    Layout.fillWidth: true
                    from: 1  // 1 K
                    to: 200000  // 2000 K
                    stepSize: 100
                    value: planetViewModel ? (planetViewModel.minTemperature * 100) : 18400  // 184 K default
                    
                    property real realValue: value / 100.0
                    
                    background: Rectangle {
                        color: "#404040"
                        border.color: "#606060"
                        radius: 3
                    }
                    
                    contentItem: TextInput {
                        text: (parent.value / 100.0).toFixed(1)
                        color: "#ffffff"
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        readOnly: true
                    }
                    
                    onValueChanged: {
                        if (planetViewModel) planetViewModel.minTemperature = realValue
                    }
                }
            }
            
            // Planet Preview
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                color: "#1a1a1a"
                border.color: "#606060"
                radius: 5
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15
                    
                    Rectangle {
                        width: 60
                        height: 60
                        radius: 30
                        color: getPlanetColor(typeCombo.currentIndex)
                        border.color: "#ffffff"
                        border.width: 2
                    }
                    
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 5
                        
                        Text {
                            text: nameField.text || "Unnamed Planet"
                            color: "#ffffff"
                            font.bold: true
                            font.pixelSize: 14
                        }
                        
                        Text {
                            text: "Type: " + typeCombo.currentText
                            color: "#cccccc"
                            font.pixelSize: 12
                        }
                        
                        Text {
                            text: "Size: " + sizeSpinBox.realValue.toFixed(2) + " Earth • Mass: " + massSpinBox.realValue.toFixed(2) + " Earth"
                            color: "#cccccc"
                            font.pixelSize: 12
                        }
                        
                        Text {
                            text: "Moons: " + moonsSpinBox.value + " • Orbit: " + radiusSpinBox.realValue.toFixed(2) + " AU"
                            color: "#cccccc"
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }

        background: Rectangle {
            id: redRectangleOffDeath
            color: "#ff0000"
            border.color: "#404040"
            radius: 5
        }
    }
    
    footer: RowLayout {
        anchors.margins: 10
        spacing: 10
        
        Button {
            text: isNewPlanet ? "Create" : "Save"
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
            onClicked: {
                if (editMode) {
                    planetEdited(originalPlanetName,
                               nameField.text || "Planet",
                               typeCombo.currentIndex,
                               sizeSpinBox.realValue,
                               massSpinBox.realValue,
                               moonsSpinBox.value,
                               radiusSpinBox.realValue,
                               maxTempSpinBox.realValue,
                               minTempSpinBox.realValue)
                } else if (isNewPlanet) {
                    planetCreated(nameField.text || "New Planet", 
                                typeCombo.currentIndex,
                                sizeSpinBox.realValue,
                                massSpinBox.realValue,
                                moonsSpinBox.value,
                                radiusSpinBox.realValue,
                                maxTempSpinBox.realValue,
                                minTempSpinBox.realValue)
                } else {
                    planetUpdated()
                }
                root.close()
            }
        }
        
        Item { Layout.fillWidth: true }
        
        Button {
            text: "Cancel"
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
            onClicked: root.close()
        }
    }
    
    function getPlanetColor(type) {
        switch(type) {
            case 0: return "#8B4513"  // Rocky - Brown
            case 1: return "#FFA500"  // Gas Giant - Orange
            case 2: return "#4169E1"  // Ice Giant - Blue
            case 3: return "#DAA520"  // Desert - Golden
            case 4: return "#0000FF"  // Ocean - Blue
            case 5: return "#E0E0E0"  // Frozen - Light Gray
            case 6: return "#FF4500"  // Volcanic - Red-Orange
            case 7: return "#9932CC"  // Toxic - Purple
            default: return "#888888"
        }
    }
    
    function setupForEditing(name, planetType, size, mass, moons, orbitalRadius, maxTemp, minTemp) {
        console.log("Setting up dialog for editing planet:", name)
        nameField.text = name
        typeCombo.currentIndex = planetType
        sizeSpinBox.value = Math.round(size * 100)
        massSpinBox.value = Math.round(mass * 100) 
        moonsSpinBox.value = moons
        radiusSpinBox.value = Math.round(orbitalRadius * 100)
        maxTempSpinBox.value = Math.round(maxTemp * 100)
        minTempSpinBox.value = Math.round(minTemp * 100)
    }
}
