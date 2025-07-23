import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0

/**
 * @brief Dialog for creating a new faction with name and homeworld selection
 */
Dialog {
    id: root

    property GalaxyController controller
    property var factionsViewModel: controller ? controller.galaxyFactionsViewModel : null

    signal factionCreated(int factionId, int homeworldSystemId, string factionName, string factionDescription, string factionColor)

    title: "Create New Faction"
    modal: true
    width: 500
    height: 400

    Material.theme: Material.Dark

    ScrollView {
        anchors.fill: parent
        clip: true

        background: Rectangle {
            color: "#2d2d2d"
            border.color: "#404040"
            radius: 5
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            // Header
            Label {
                text: "Create New Faction"
                font.pixelSize: 18
                font.bold: true
                color: "#ffffff"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Faction name input
            GroupBox {
                Layout.fillWidth: true
                title: "Faction Name"
                Material.theme: Material.Dark

                TextField {
                    id: factionNameField
                    anchors.fill: parent
                    placeholderText: "Enter faction name..."
                    Material.theme: Material.Dark

                    background: Rectangle {
                        color: "#3e3e3e"
                        border.color: factionNameField.focus ? "#2196F3" : "#555555"
                        border.width: 1
                        radius: 2
                    }
                }
            }

            // Faction description input
            GroupBox {
                Layout.fillWidth: true
                title: "Description (Optional)"
                Material.theme: Material.Dark

                ScrollView {
                    anchors.fill: parent

                    TextArea {
                        id: factionDescriptionField
                        placeholderText: "Enter faction description..."
                        Material.theme: Material.Dark
                        wrapMode: TextArea.WordWrap

                        background: Rectangle {
                            color: "#3e3e3e"
                            border.color: factionDescriptionField.focus ? "#2196F3" : "#555555"
                            border.width: 1
                            radius: 2
                        }
                    }
                }
            }

            // Homeworld system selection
            GroupBox {
                Layout.fillWidth: true
                title: "Homeworld System"
                Material.theme: Material.Dark

                ColumnLayout {
                    anchors.fill: parent

                    ComboBox {
                        id: homeworldComboBox
                        Layout.fillWidth: true
                        Material.theme: Material.Dark

                        model: controller && controller.galaxyViewModel ? controller.galaxyViewModel.starSystems : null
                        textRole: "name"
                        valueRole: "systemId"

                        displayText: currentIndex >= 0 ? currentText : "Select homeworld system..."

                        background: Rectangle {
                            color: "#3e3e3e"
                            border.color: homeworldComboBox.focus ? "#2196F3" : "#555555"
                            border.width: 1
                            radius: 2
                        }

                        contentItem: Text {
                            text: homeworldComboBox.displayText
                            color: "#ffffff"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 10
                        }

                        popup: Popup {
                            y: homeworldComboBox.height - 1
                            width: homeworldComboBox.width
                            implicitHeight: contentItem.implicitHeight
                            padding: 1

                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: homeworldComboBox.popup.visible ? homeworldComboBox.delegateModel : null
                                currentIndex: homeworldComboBox.highlightedIndex

                                ScrollIndicator.vertical: ScrollIndicator {}
                            }

                            background: Rectangle {
                                color: "#3e3e3e"
                                border.color: "#555555"
                                radius: 2
                            }
                        }

                        delegate: ItemDelegate {
                            width: homeworldComboBox.width
                            contentItem: Text {
                                text: model.name
                                color: parent.hovered ? "#ffffff" : "#cccccc"
                                font: homeworldComboBox.font
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                            }
                            highlighted: homeworldComboBox.highlightedIndex === index

                            background: Rectangle {
                                color: parent.highlighted ? "#404040" : (parent.hovered ? "#353535" : "transparent")
                            }
                        }
                    }

                    Text {
                        text: homeworldComboBox.currentIndex >= 0 ? "Selected System ID: " + homeworldComboBox.currentValue : "No system selected"
                        color: "#cccccc"
                        font.pixelSize: 12
                        Layout.fillWidth: true
                    }
                }
            }

            // Faction color selection
            GroupBox {
                Layout.fillWidth: true
                title: "Faction Color"
                Material.theme: Material.Dark

                RowLayout {
                    anchors.fill: parent

                    Rectangle {
                        id: colorPreview
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 30
                        color: colorButton.selectedColor
                        border.color: "#ffffff"
                        border.width: 1
                        radius: 4
                    }

                    Button {
                        id: colorButton
                        property string selectedColor: "#FF5722"

                        text: "Choose Color"
                        Material.theme: Material.Dark

                        onClicked: {
                            colorDialog.open();
                        }
                    }

                    Label {
                        text: colorButton.selectedColor
                        color: "#cccccc"
                        Layout.fillWidth: true
                    }
                }
            }

            // Spacer
            Item {
                Layout.fillHeight: true
            }

            // Button row
            RowLayout {
                Layout.fillWidth: true

                Button {
                    text: "Cancel"
                    Material.theme: Material.Dark
                    Material.accent: "#757575"

                    onClicked: {
                        root.reject();
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: "Create Faction"
                    Material.theme: Material.Dark
                    Material.accent: "#4CAF50"
                    enabled: factionNameField.text.trim().length > 0 && homeworldComboBox.currentIndex >= 0

                    onClicked: {
                        console.log("Creating faction:", factionNameField.text, "Description:", factionDescriptionField.text, "Color:", colorButton.selectedColor, "Homeworld ID:", homeworldComboBox.currentValue);

                        // Prepare faction data
                        var factionName = factionNameField.text.trim();
                        var description = factionDescriptionField.text.trim() || "A faction in the galaxy";
                        var factionColor = colorButton.selectedColor;
                        var homeworldSystemId = homeworldComboBox.currentValue;

                        console.log("Prepared faction data:" + " name='" + factionName + "'" + " description='" + description + "'" + " color='" + factionColor + "'" + " homeworldSystemId=" + homeworldSystemId);

                        // Check if factionsViewModel is available
                        if (!factionsViewModel) {
                            console.error("ERROR: factionsViewModel is not available!");
                            return;
                        }
                        console.log("factionsViewModel is available:", factionsViewModel);

                        // Get the star system model for the homeworld
                        var starSystemViewModel = null;
                        if (controller && controller.getStarSystemViewModel) {
                            console.log("Getting StarSystemViewModel for system ID:", homeworldSystemId);
                            starSystemViewModel = controller.getStarSystemViewModel(homeworldSystemId);
                            console.log("StarSystemViewModel result:", starSystemViewModel);
                        } else {
                            console.warn("WARNING: controller or getStarSystemViewModel not available");
                            console.log("controller:", controller);
                            if (controller) {
                                console.log("controller.getStarSystemViewModel:", controller.getStarSystemViewModel);
                            }
                        }

                        // Create the faction with homeworld system
                        console.log("Calling addFactionWithHomeworld with parameters:" + " name='" + factionName + "'" + " description='" + description + "'" + " color='" + factionColor + "'" + " homeworldSystemId=" + homeworldSystemId + " starSystemViewModel=" + starSystemViewModel);

                        try {
                            var factionId = factionsViewModel.addFactionWithHomeworld(factionName, description, factionColor, homeworldSystemId, starSystemViewModel);

                            console.log("addFactionWithHomeworld returned:", factionId);

                            if (factionId >= 0) {
                                console.log("Faction created successfully with ID:", factionId, "and homeworld system:", homeworldComboBox.currentText, "(ID:", homeworldSystemId, ")");

                                // Emit the faction created signal with all parameters
                                root.factionCreated(factionId, homeworldSystemId, factionName, description, factionColor);
                                root.accept();
                            } else {
                                console.error("Failed to create faction - returned ID:", factionId);
                            }
                        } catch (e) {
                            console.error("Exception occurred while creating faction:", e);
                        }
                    }
                }
            }
        }
    }

    // Color picker dialog
    ColorDialog {
        id: colorDialog
        title: "Choose Faction Color"

        onAccepted: {
            colorButton.selectedColor = selectedColor;
        }
    }
}
