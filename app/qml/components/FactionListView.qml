import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Galaxy.Factions.ViewModels 1.0
import GalaxyBuilderApp 1.0

/**
 * @brief A list view component for displaying and managing factions
 */
Item {
    id: root
    
    property alias factionsViewModel: factionsManager
    
    signal factionSelected(int factionId)
    signal factionEditRequested(int factionId)
    
    // Galaxy Factions ViewModel manager
    GalaxyFactionsViewModel {
        id: factionsManager
        
        Component.onCompleted: {
            console.log("GalaxyFactionsViewModel initialized")
            initialize()
        }
    }
    
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
                text: "Add Faction"
                Material.theme: Material.Dark
                Material.accent: "#4CAF50"
                
                onClicked: {
                    console.log("Add faction clicked")
                    // Create a new faction with default values
                    factionsManager.addFaction("New Faction", "A newly created faction", "#FF5722")
                }
            }
        }
        
        // Faction List
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ListView {
                id: factionListView
                
                model: factionsManager.factionListModel
                spacing: 5
                
                delegate: FactionListItem {
                    width: factionListView.width
                    factionId: model.id
                    factionName: model.name
                    factionDescription: model.description
                    factionColor: model.color
                    
                    onClicked: {
                        console.log("Faction selected:", factionId, factionName)
                        root.factionSelected(factionId)
                    }
                    
                    onEditRequested: {
                        console.log("Faction edit requested:", factionId)
                        root.factionEditRequested(factionId)
                    }
                    
                    onDeleteRequested: {
                        console.log("Faction delete requested:", factionId)
                        factionsManager.removeFaction(factionId)
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
                    text: "Factions: " + (factionsManager.factionListModel ? factionsManager.factionListModel.rowCount() : 0)
                    color: "#cccccc"
                    font.pixelSize: 12
                }
                
                Label {
                    text: factionsManager.selectedFactionId >= 0 ? "Selected: " + factionsManager.selectedFactionId : "No selection"
                    color: "#cccccc"
                    font.pixelSize: 12
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }
}
