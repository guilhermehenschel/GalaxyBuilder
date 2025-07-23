import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0

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
                                if (starSystemViewModel)
                                    starSystemViewModel.name = text;
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
                                if (starSystemViewModel)
                                    starSystemViewModel.starType = currentIndex;
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
                            value: 100  // Default value since starMass doesn't exist in model
                            enabled: false  // Disabled since property doesn't exist

                            visible: false  // Hide since we don't have starMass in the model

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
                                readOnly: true
                                validator: DoubleValidator {
                                    bottom: 0.01
                                    top: 100.00
                                    decimals: 2
                                }
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
                            value: 5778  // Default value since starTemperature doesn't exist in model
                            enabled: false  // Disabled since property doesn't exist

                            visible: false  // Hide since we don't have starTemperature in the model

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
                        }

                        Text {
                            text: "Luminosity (Solar luminosities):"
                            color: "#cccccc"
                        }
                        SpinBox {
                            id: luminositySpinBox
                            Layout.fillWidth: true
                            from: 1
                            to: 100000
                            stepSize: 1
                            value: 1000  // Default value since starLuminosity doesn't exist in model
                            enabled: false  // Disabled since property doesn't exist

                            visible: false  // Hide since we don't have starLuminosity in the model

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
                                readOnly: true
                                validator: DoubleValidator {
                                    bottom: 0.001
                                    top: 100.000
                                    decimals: 3
                                }
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

                        // Planet orbits and planets
                        StarSystemVisualizer {
                            model: starSystemViewModel ? starSystemViewModel.planets : null
                            starModel: starSystemViewModel

                            anchors.fill: parent
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
                                    planetDetailsDialog.isNewPlanet = true;
                                    planetDetailsDialog.planetViewModel = null;
                                    planetDetailsDialog.open();
                                }
                            }

                            Button {
                                text: "Edit Selected"
                                enabled: planetListView.currentIndex >= 0
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
                                    if (starSystemViewModel && planetListView.currentIndex >= 0) {
                                        // Get the planet data from the model using the proper QML model access
                                        var planetModel = starSystemViewModel.planets;
                                        if (planetModel && planetListView.currentIndex < planetModel.rowCount()) {
                                            var index = planetModel.index(planetListView.currentIndex, 0);
                                            var planetName = planetModel.data(index, planetModel.NameRole);
                                            if (planetName) {
                                                editPlanetByName(planetName);
                                            }
                                        }
                                    }
                                }
                            }

                            Button {
                                text: "Remove Selected"
                                enabled: planetListView.currentIndex >= 0
                                background: Rectangle {
                                    color: enabled ? "#cc0000" : "#444444"
                                    border.color: enabled ? "#ff0000" : "#666666"
                                    radius: 3
                                }
                                contentItem: Text {
                                    text: parent.text
                                    color: enabled ? "#ffffff" : "#999999"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                onClicked: {
                                    if (starSystemViewModel && planetListView.currentIndex >= 0) {
                                        // Get the planet data from the model using the proper QML model access
                                        var planetModel = starSystemViewModel.planets;
                                        if (planetModel && planetListView.currentIndex < planetModel.rowCount()) {
                                            var index = planetModel.index(planetListView.currentIndex, 0);
                                            var planetName = planetModel.data(index, planetModel.NameRole);
                                            if (planetName) {
                                                starSystemViewModel.removePlanetByName(planetName);
                                            }
                                        }
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                            }
                        }

                        PlanetListView {
                            id: planetListView
                            Layout.fillWidth: true
                            Layout.preferredHeight: Math.max(100, contentHeight)
                            Layout.maximumHeight: 400
                            clip: true
                            model: starSystemViewModel ? starSystemViewModel.planets : null
                        }
                        // ListView {
                        //
                        //     Layout.fillWidth: true
                        //     Layout.preferredHeight: Math.max(100, contentHeight)
                        //     Layout.maximumHeight: 400
                        //     clip: true
                        //     model: starSystemViewModel ? starSystemViewModel.planets : null

                        //     delegate: Rectangle {
                        //         width: planetListView.width
                        //         height: 60
                        //         color: ListView.isCurrentItem ? "#404040" : "#2a2a2a"
                        //         border.color: "#606060"
                        //         radius: 3

                        //         RowLayout {
                        //             anchors.fill: parent
                        //             anchors.margins: 10
                        //             spacing: 10

                        //             Rectangle {
                        //                 width: 20
                        //                 height: 20
                        //                 radius: 10
                        //                 color: getPlanetColor(model.type)
                        //                 border.color: "#ffffff"
                        //                 border.width: 1
                        //             }

                        //             ColumnLayout {
                        //                 Layout.fillWidth: true
                        //                 spacing: 2

                        //                 Text {
                        //                     text: model.name
                        //                     color: "#ffffff"
                        //                     font.bold: true
                        //                 }

                        //                 Text {
                        //                     text: "Type: " + getPlanetTypeName(model.type) + ", Moons: " + model.moons
                        //                     color: "#cccccc"
                        //                     font.pixelSize: 12
                        //                 }
                        //             }

                        //             Text {
                        //                 text: model.mass.toFixed(2) + " ME"
                        //                 color: "#cccccc"
                        //                 font.pixelSize: 12
                        //             }
                        //         }

                        //         MouseArea {
                        //             anchors.fill: parent
                        //             onClicked: planetListView.currentIndex = index
                        //             onDoubleClicked: {
                        //                 // Set the current index and use the same logic as the Edit button
                        //                 planetListView.currentIndex = index
                        //                 if (starSystemViewModel) {
                        //                     var planetModel = starSystemViewModel.planets
                        //                     if (planetModel && index < planetModel.rowCount()) {
                        //                         var modelIndex = planetModel.index(index, 0)
                        //                         var planetName = planetModel.data(modelIndex, planetModel.NameRole)
                        //                         if (planetName) {
                        //                             editPlanetByName(planetName)
                        //                         }
                        //                     }
                        //                 }
                        //             }
                        //         }
                        //     }
                        // }
                    }
                }
            }
        }

        // Button row at the bottom (outside ScrollView)
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            spacing: 10

            // Save button
            Button {
                text: "Save"
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

            Item {
                Layout.fillWidth: true
            }

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

    // Status label for feedback
    Text {
        id: statusLabel
        visible: text !== ""
        text: ""  // No status message property available in ViewModel
        color: "#ffff00"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 10

        Timer {
            id: statusTimer
            interval: 3000
            onTriggered: {
                statusLabel.text = "";
            }
        }

        onTextChanged: {
            if (text !== "") {
                statusTimer.restart();
            }
        }
    }

    // Component initialization
    Component.onCompleted: {
        console.log("SystemPropertiesDialog initialized with starSystemViewModel");
        loadSystemData();
    }

    // Helper functions
    function saveSystemData() {
        // Data is automatically saved when properties are modified
        // No additional saving required
        return true;
    }

    function loadSystemData() {
        // Data is automatically loaded when starSystemViewModel is set
        // No additional loading required
        return true;
    }

    function refreshUI() {
        // Force UI refresh by updating bound properties
        if (starSystemViewModel) {
            nameField.text = starSystemViewModel.name;
            starTypeCombo.currentIndex = starSystemViewModel.starType;
            // Note: mass, temperature, and luminosity are disabled as they don't exist in the model
        }
    }

    // Auto-setup when starSystemViewModel changes
    onStarSystemViewModelChanged: {
        if (starSystemViewModel) {
            loadSystemData();
        }
    }

    function editPlanetByName(planetName) {
        console.log("Editing planet:", planetName);
        if (starSystemViewModel) {
            // Find the planet in the model
            var planetModel = starSystemViewModel.planets;
            if (planetModel) {
                for (var i = 0; i < planetModel.rowCount(); i++) {
                    var index = planetModel.index(i, 0);
                    var name = planetModel.data(index, planetModel.NameRole);
                    if (name === planetName) {
                        // Set up the dialog with the planet's current data
                        var planetType = planetModel.data(index, planetModel.TypeRole);
                        var size = planetModel.data(index, planetModel.SizeRole);
                        var mass = planetModel.data(index, planetModel.MassRole);
                        var moons = planetModel.data(index, planetModel.MoonsRole);
                        var orbitalRadius = planetModel.data(index, planetModel.OrbitRole);
                        var maxTemp = planetModel.data(index, planetModel.MaxTempRole);
                        var minTemp = planetModel.data(index, planetModel.MinTempRole);

                        // Set the dialog for editing mode
                        planetDetailsDialog.editMode = true;
                        planetDetailsDialog.originalPlanetName = planetName;
                        planetDetailsDialog.setupForEditing(name, planetType, size, mass, moons, orbitalRadius, maxTemp, minTemp);
                        planetDetailsDialog.open();
                        return;
                    }
                }
            }
        }
        statusLabel.text = "Could not find planet '" + planetName + "' for editing";
    }

    // Planet Details Dialog
    PlanetDetailsDialog {
        id: planetDetailsDialog

        onPlanetCreated: function (name, type, size, mass, moons, radius, maxTemp, minTemp) {
            if (starSystemViewModel) {
                starSystemViewModel.addPlanet(name, type, size, mass, moons, radius, maxTemp, minTemp);
                statusLabel.text = "Planet '" + name + "' added successfully";
            }
        }

        onPlanetEdited: function (originalName, name, type, size, mass, moons, radius, maxTemp, minTemp) {
            if (starSystemViewModel) {
                // Remove the old planet and add the updated one
                starSystemViewModel.removePlanetByName(originalName);
                starSystemViewModel.addPlanet(name, type, size, mass, moons, radius, maxTemp, minTemp);
                statusLabel.text = "Planet '" + name + "' updated successfully";
            }
        }

        onPlanetUpdated: {
            statusLabel.text = "Planet updated successfully";
        }
    }
}
