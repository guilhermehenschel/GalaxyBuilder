import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import "./faction"

/**
 * @brief Individual faction item in the list view (refactored with modular components)
 */
Rectangle {
    id: root

    property int factionId: -1
    property string factionName: ""
    property string factionDescription: ""
    property string factionColor: "#FF5722"
    property var factionsViewModel: null

    signal clicked
    signal editRequested
    signal deleteRequested

    height: 140
    color: mouseArea.containsMouse ? "#3e3e3e" : "#2e2e2e"
    border.color: "#404040"
    border.width: 1
    radius: 4

    Behavior on color {
        ColorAnimation {
            duration: 150
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton

        onClicked: {
            root.clicked();
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        FactionColorIndicator {
            id: colorIndicator
            factionColor: root.factionColor
        }
        
        FactionInfoDisplay {
            id: infoDisplay
            Layout.fillWidth: true
            Layout.fillHeight: true
            factionName: root.factionName
            factionDescription: root.factionDescription
            systemCount: getSystemCount()
            resourceCount: getResourceBonusCount()
            stockCount: getTotalResourceStock()
        }
        
        FactionActionButtons {
            id: actionButtons
            
            onEditRequested: root.editRequested()
            onDeleteRequested: root.deleteRequested()
            onResourcesRequested: {
                console.log("Resource management requested for faction:", root.factionName)
                // TODO: Implement resource management
            }
        }
    }
    
    // Helper functions (to be moved to C++ later)
    function getSystemCount() {
        if (root.factionId >= 0 && root.factionsViewModel && root.factionsViewModel.factionListModel) {
            try {
                var faction = root.factionsViewModel.factionListModel.getFaction(root.factionId);
                if (faction) {
                    var systems = faction.getSystems();
                    return systems ? systems.length : 0;
                }
            } catch (e) {
                console.log("Error getting system count:", e);
            }
        }
        return 0;
    }
    
    function getResourceBonusCount() {
        if (root.factionId >= 0 && root.factionsViewModel && root.factionsViewModel.factionListModel) {
            try {
                var faction = root.factionsViewModel.factionListModel.getFaction(root.factionId);
                if (faction) {
                    var bonuses = faction.getResourceBonuses();
                    return bonuses ? bonuses.length : 0;
                }
            } catch (e) {
                console.log("Error getting resource bonus count:", e);
            }
        }
        return 0;
    }
    
    function getTotalResourceStock() {
        if (root.factionId >= 0 && root.factionsViewModel && root.factionsViewModel.factionListModel) {
            try {
                var faction = root.factionsViewModel.factionListModel.getFaction(root.factionId);
                if (faction) {
                    var resourceStock = faction.getResourceStock();
                    if (resourceStock && resourceStock.length > 0) {
                        var total = 0;
                        for (var i = 0; i < resourceStock.length; i++) {
                            total += resourceStock[i].stock || 0;
                        }
                        return total;
                    }
                }
            } catch (e) {
                console.log("Error getting resource stock:", e);
            }
        }
        return 0;
    }
}
