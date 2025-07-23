import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import GalaxyBuilderApp 1.0

/**
 * @brief A list view component for displaying and managing factions
 */
Item {
    id: root

    property var factionsViewModel: controller ? controller.galaxyFactionsViewModel : null
    property GalaxyController controller

    signal factionSelected(int factionId)
    signal factionEditRequested(int factionId)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        // Header
        RowLayout {
            Layout.fillWidth: true

            Label {
                text: "Galaxy Factions"
                font.pixelSize: 18
                font.bold: true
                color: "#ffffff"
                Layout.fillWidth: true
            }

            Button {
                id: addFactionBtn
                text: "Add New Faction"
                Material.theme: Material.Dark
                Material.accent: "#4CAF50"

                onClicked: {
                    console.log("Add new faction clicked");
                    newFactionDialog.open();
                }
            }

            Button {
                text: "Export XML"
                Material.theme: Material.Dark
                Material.accent: "#2196F3"

                onClicked: {
                    console.log("Export XML clicked");
                    factionXMLExporter.openExportDialog();
                }
            }
        }

        // Faction List
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: factionListView

                model: factionsViewModel ? factionsViewModel.factionListModel : null
                spacing: 5

                delegate: FactionListItem {
                    width: factionListView.width
                    factionId: model.id
                    factionName: model.name
                    factionDescription: model.description
                    factionColor: model.color
                    factionsViewModel: root.factionsViewModel

                    onClicked: {
                        console.log("Faction selected:", factionId, factionName);
                        root.factionSelected(factionId);
                    }

                    onEditRequested: {
                        console.log("Faction edit requested:", factionId);
                        root.factionEditRequested(factionId);
                    }

                    onDeleteRequested: {
                        console.log("Faction delete requested:", factionId);
                        if (factionsViewModel) {
                            factionsViewModel.removeFaction(factionId);
                        }
                    }
                }
            }
        }

        // Status bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "#2e2e2e"
            border.color: "#404040"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5

                Label {
                    text: "Factions: " + (factionsViewModel && factionsViewModel.factionListModel ? factionsViewModel.factionListModel.rowCount() : 0)
                    color: "#cccccc"
                    font.pixelSize: 12
                }

                Label {
                    text: factionsViewModel && factionsViewModel.selectedFactionId >= 0 ? "Selected: " + factionsViewModel.selectedFactionId : "No selection"
                    color: "#cccccc"
                    font.pixelSize: 12
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }

    // New Faction Creation Dialog
    NewFactionDialog {
        id: newFactionDialog
        controller: root.controller
        modal: true
        popupType: Popup.Native

        onFactionCreated: function (factionId, homeworldSystemId, factionName, factionDescription, factionColor) {
            console.log("New faction created with ID:", factionId);
            console.log("Homeworld System ID:", homeworldSystemId);
            console.log("Faction Name:", factionName);
            console.log("Faction Description:", factionDescription);
            console.log("Faction Color:", factionColor);

            // Force a refresh of the faction count display
            if (factionsViewModel && factionsViewModel.factionListModel) {
                console.log("Current faction count after creation:", factionsViewModel.factionListModel.rowCount());
            }

            // The faction list should automatically update since we're using the same instance
            console.log("Faction list will be automatically updated");
        }
    }

    // XML Export Component
    FactionXMLExporter {
        id: factionXMLExporter
        factionsViewModel: root.factionsViewModel
        parentWindow: root.Window.window

        onExportCompleted: function (success, filePath) {
            if (success) {
                console.log("Factions exported successfully to:", filePath);
            } else {
                console.error("Failed to export factions");
            }
        }
    }
}
