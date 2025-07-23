import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GalaxyBuilderApp 1.0

/**
 * @brief Main faction management view with list and detail views
 */
ApplicationWindow {
    id: factionWindow

    property GalaxyController controller

    title: "Galaxy Factions Manager"
    width: 900
    height: 600
    minimumWidth: 700
    minimumHeight: 500

    color: "#1e1e1e"

    property bool showDetailView: false
    property int selectedFactionId: -1

    // Main content area
    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: listViewComponent

        Component {
            id: listViewComponent

            FactionListView {
                id: factionListView
                controller: factionWindow.controller

                onFactionSelected: function (factionId) {
                    console.log("Faction selected in main window:", factionId);
                    factionWindow.selectedFactionId = factionId;

                    // Push detail view onto stack
                    stackView.push(detailViewComponent, {
                        "selectedFactionId": factionId,
                        "factionsViewModel": factionListView.factionsViewModel
                    });
                }

                onFactionEditRequested: function (factionId) {
                    console.log("Faction edit requested in main window:", factionId);
                    factionWindow.selectedFactionId = factionId;

                    // Push detail view onto stack for editing
                    stackView.push(detailViewComponent, {
                        "selectedFactionId": factionId,
                        "factionsViewModel": factionListView.factionsViewModel
                    });
                }
            }
        }

        Component {
            id: detailViewComponent

            FactionDetailView {
                id: factionDetailView

                onBackRequested: {
                    console.log("Back to list requested");
                    stackView.pop();
                    factionWindow.selectedFactionId = -1;
                }

                onFactionUpdated: {
                    console.log("Faction updated, refreshing list");
                    // The list should automatically update due to data binding
                }
            }
        }
    }

    // Menu bar
    menuBar: MenuBar {
        Menu {
            title: "File"

            MenuItem {
                text: "New Faction"
                onTriggered: {
                    // Go back to list view and add new faction
                    if (stackView.depth > 1) {
                        stackView.pop();
                    }
                    // Trigger add faction in list view
                    var listView = stackView.currentItem;
                    if (listView && listView.factionsViewModel) {
                        listView.factionsViewModel.addFaction("New Faction", "A newly created faction", "#FF5722");
                    }
                }
            }

            MenuSeparator {}

            MenuItem {
                text: "Close"
                onTriggered: factionWindow.close()
            }
        }

        Menu {
            title: "View"

            MenuItem {
                text: "Faction List"
                enabled: stackView.depth > 1
                onTriggered: {
                    stackView.pop();
                    factionWindow.selectedFactionId = -1;
                }
            }

            MenuItem {
                text: "Refresh"
                onTriggered: {
                    console.log("Refreshing faction data");
                    var currentItem = stackView.currentItem;
                    if (currentItem && currentItem.factionsViewModel) {
                        currentItem.factionsViewModel.initialize();
                    }
                }
            }
        }

        Menu {
            title: "Help"

            MenuItem {
                text: "About Factions"
                onTriggered: {
                    aboutDialog.open();
                }
            }
        }
    }

    // Status bar
    footer: ToolBar {
        id: statusBar
        RowLayout {
            anchors.fill: parent
            anchors.margins: 5

            Label {
                text: stackView.depth > 1 ? "Faction Details" : "Faction List"
                color: "#ffffff"
                font.pixelSize: 12
            }

            Item {
                Layout.fillWidth: true
            }

            Label {
                text: selectedFactionId >= 0 ? "Selected ID: " + selectedFactionId : "No selection"
                color: "#cccccc"
                font.pixelSize: 11
            }
        }
    }

    // About dialog
    Dialog {
        id: aboutDialog
        title: "About Galaxy Factions Manager"
        standardButtons: Dialog.Ok

        anchors.centerIn: parent
        width: 400
        height: 300

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: "Galaxy Factions Manager"
                font.pixelSize: 16
                font.bold: true
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Label {
                text: "Version 1.0.0"
                color: "#cccccc"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: "#404040"
            }

            Label {
                text: "This tool allows you to manage factions in your galaxy. You can create, edit, and delete factions, customize their properties, and view their statistics."
                color: "#cccccc"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Label {
                text: "Features:\n• Create and manage factions\n• Customize faction colors\n• Edit faction descriptions\n• View faction statistics"
                color: "#cccccc"
                Layout.fillWidth: true
            }
        }
    }
}
