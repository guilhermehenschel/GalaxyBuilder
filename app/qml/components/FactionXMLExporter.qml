import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Window

/**
 * @brief Component for exporting factions to XML file
 */
Item {
    id: root

    property var factionsViewModel
    property Window parentWindow: null

    signal exportCompleted(bool success, string filePath)

    // File dialog for XML export
    FileDialog {
        id: exportFileDialog
        title: "Export Factions to XML"
        fileMode: FileDialog.SaveFile
        nameFilters: ["XML files (*.xml)", "All files (*)"]
        defaultSuffix: "xml"

        // Center the dialog if parentWindow is available
        Component.onCompleted: {
            if (parentWindow) {
                x = parentWindow.x + (parentWindow.width - width) / 2;
                y = parentWindow.y + (parentWindow.height - height) / 2;
            }
        }

        onAccepted: {
            console.log("Exporting factions to:", selectedFile);
            exportFactions(selectedFile);
        }

        onRejected: {
            console.log("Export canceled");
        }
    }

    // Function to open export dialog
    function openExportDialog() {
        console.log("openExportDialog called");
        console.log("factionsViewModel at dialog open:", factionsViewModel);
        console.log("factionsViewModel type at dialog open:", typeof factionsViewModel);
        if (factionsViewModel) {
            console.log("factionsViewModel is valid, opening dialog");
            exportFileDialog.open();
        } else {
            console.error("factionsViewModel is null, cannot open export dialog");
        }
    }

    // Function to export factions
    function exportFactions(filePath) {
        console.log("exportFactions called with:", filePath);
        console.log("factionsViewModel is:", factionsViewModel);
        console.log("factionsViewModel type:", typeof factionsViewModel);
        try {
            // Use the C++ method to export to file
            var success = factionsViewModel.exportToXmlFile(filePath);

            if (success) {
                console.log("Factions exported successfully to:", filePath);
                exportCompleted(true, filePath);
            } else {
                console.error("Failed to export factions to file");
                exportCompleted(false, "");
            }
        } catch (error) {
            console.error("Error exporting factions:", error);
            exportCompleted(false, "");
        }
    }

    // Function to generate XML content from factions
    function generateFactionsXML() {
        var xml = '<?xml version="1.0" encoding="UTF-8"?>\n';
        xml += '<Factions>\n';

        var factionList = factionsViewModel.factionListModel;
        for (var i = 0; i < factionList.rowCount(); i++) {
            var index = factionList.index(i, 0);
            var factionId = factionList.data(index, factionList.IdRole);
            var factionName = factionList.data(index, factionList.NameRole);
            var factionDescription = factionList.data(index, factionList.DescriptionRole);
            var factionColor = factionList.data(index, factionList.ColorRole);

            xml += '  <Faction id="' + factionId + '">\n';
            xml += '    <name>' + escapeXml(factionName) + '</name>\n';
            xml += '    <description>' + escapeXml(factionDescription) + '</description>\n';
            xml += '    <color>' + escapeXml(factionColor) + '</color>\n';
            xml += '    <systems>\n';
            // TODO: Add homeworld/controlled systems here when system assignment is implemented
            xml += '    </systems>\n';
            xml += '  </Faction>\n';
        }

        xml += '</Factions>\n';
        return xml;
    }

    // Function to escape XML special characters
    function escapeXml(text) {
        if (!text)
            return "";
        return text.toString().replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;').replace(/'/g, '&apos;');
    }

    // Success notification popup
    Popup {
        id: successPopup
        anchors.centerIn: parentWindow
        width: 300
        height: 150
        modal: true

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        Material.theme: Material.Dark

        background: Rectangle {
            color: "#2d2d2d"
            border.color: "#4CAF50"
            border.width: 2
            radius: 5
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Label {
                text: "Export Successful"
                font.pixelSize: 16
                font.bold: true
                color: "#4CAF50"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: successMessage
                text: "Factions exported successfully!"
                color: "#ffffff"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Button {
                text: "OK"
                Material.theme: Material.Dark
                Material.accent: "#4CAF50"
                Layout.alignment: Qt.AlignHCenter

                onClicked: {
                    successPopup.close();
                }
            }
        }
    }

    // Error notification popup
    Popup {
        id: errorPopup
        anchors.centerIn: parent
        width: 300
        height: 150
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        Material.theme: Material.Dark

        background: Rectangle {
            color: "#2d2d2d"
            border.color: "#F44336"
            border.width: 2
            radius: 5
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Label {
                text: "Export Failed"
                font.pixelSize: 16
                font.bold: true
                color: "#F44336"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: errorMessage
                text: "Failed to export factions to XML."
                color: "#ffffff"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }

            Button {
                text: "OK"
                Material.theme: Material.Dark
                Material.accent: "#F44336"
                Layout.alignment: Qt.AlignHCenter

                onClicked: {
                    errorPopup.close();
                }
            }
        }
    }

    // Handle export completion
    onExportCompleted: function (success, filePath) {
        if (success) {
            successMessage.text = "Factions exported to:\n" + filePath;
            successPopup.open();
        } else {
            errorMessage.text = "Failed to export factions to XML.\nPlease check file permissions and try again.";
            errorPopup.open();
        }
    }
}
