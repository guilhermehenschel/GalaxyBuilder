import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import Galaxy.Factions.ViewModels 1.0

/**
 * @brief Detailed view for a selected faction
 */
Item {
    id: root
    
    property int selectedFactionId: -1
    property alias factionsViewModel: factionsManager
    
    signal backRequested()
    signal factionUpdated()
    
    // Galaxy Factions ViewModel manager
    GalaxyFactionsViewModel {
        id: factionsManager
        
        onSelectedFactionIdChanged: {
            root.selectedFactionId = selectedFactionId
            loadFactionDetails()
        }
        
        Component.onCompleted: {
            initialize()
        }
    }
    
    // Faction ViewModel for detailed editing
    FactionViewModel {
        id: factionViewModel
    }
    
    function loadFactionDetails() {
        if (selectedFactionId >= 0) {
            console.log("Loading faction details for ID:", selectedFactionId)
            // Get faction from the list model and set it to the ViewModel
            var factionListModel = factionsManager.factionListModel
            for (var i = 0; i < factionListModel.rowCount(); i++) {
                var index = factionListModel.index(i, 0)
                var factionId = factionListModel.data(index, factionListModel.IdRole)
                if (factionId === selectedFactionId) {
                    // Found the faction, populate the detail view
                    nameField.text = factionListModel.data(index, factionListModel.NameRole)
                    descriptionField.text = factionListModel.data(index, factionListModel.DescriptionRole)
                    colorButton.selectedColor = factionListModel.data(index, factionListModel.ColorRole)
                    break
                }
            }
        }
    }
    
    function saveFactionDetails() {
        if (selectedFactionId >= 0) {
            console.log("Saving faction details for ID:", selectedFactionId)
            // Update the faction in the list
            factionViewModel.setId(selectedFactionId)
            factionViewModel.setName(nameField.text)
            factionViewModel.setDescription(descriptionField.text)
            factionViewModel.setColor(colorButton.selectedColor)
            
            root.factionUpdated()
        }
    }
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        
        ColumnLayout {
            width: root.width - 20
            spacing: 15
            
            // Header with back button
            RowLayout {
                Layout.fillWidth: true
                
                Button {
                    text: "← Back to List"
                    Material.theme: Material.Dark
                    Material.accent: "#2196F3"
                    
                    onClicked: {
                        root.backRequested()
                    }
                }
                
                Label {
                    text: "Faction Details"
                    font.pixelSize: 18
                    font.bold: true
                    color: "#ffffff"
                    Layout.fillWidth: true
                }
                
                Button {
                    text: "Save Changes"
                    Material.theme: Material.Dark
                    Material.accent: "#4CAF50"
                    enabled: selectedFactionId >= 0
                    
                    onClicked: {
                        saveFactionDetails()
                    }
                }
            }
            
            // Faction ID (read-only)
            GroupBox {
                Layout.fillWidth: true
                title: "Faction Information"
                Material.theme: Material.Dark
                
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10
                    
                    Label {
                        text: "Faction ID: " + selectedFactionId
                        color: "#cccccc"
                        font.pixelSize: 12
                    }
                    
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Label {
                            text: "Status:"
                            color: "#cccccc"
                        }
                        
                        Label {
                            text: selectedFactionId >= 0 ? "Active" : "No faction selected"
                            color: selectedFactionId >= 0 ? "#4CAF50" : "#F44336"
                            font.bold: true
                        }
                    }
                }
            }
            
            // Faction name
            GroupBox {
                Layout.fillWidth: true
                title: "Name"
                Material.theme: Material.Dark
                
                TextField {
                    id: nameField
                    anchors.fill: parent
                    placeholderText: "Enter faction name..."
                    Material.theme: Material.Dark
                    enabled: selectedFactionId >= 0
                    
                    background: Rectangle {
                        color: "#3e3e3e"
                        border.color: nameField.focus ? "#2196F3" : "#555555"
                        border.width: 1
                        radius: 2
                    }
                }
            }
            
            // Faction description
            GroupBox {
                Layout.fillWidth: true
                title: "Description"
                Material.theme: Material.Dark
                
                ScrollView {
                    anchors.fill: parent
                    
                    TextArea {
                        id: descriptionField
                        placeholderText: "Enter faction description..."
                        Material.theme: Material.Dark
                        enabled: selectedFactionId >= 0
                        wrapMode: TextArea.WordWrap
                        
                        background: Rectangle {
                            color: "#3e3e3e"
                            border.color: descriptionField.focus ? "#2196F3" : "#555555"
                            border.width: 1
                            radius: 2
                        }
                    }
                }
            }
            
            // Faction color
            GroupBox {
                Layout.fillWidth: true
                title: "Color"
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
                        enabled: selectedFactionId >= 0
                        
                        onClicked: {
                            colorDialog.open()
                        }
                    }
                    
                    Label {
                        text: colorButton.selectedColor
                        color: "#cccccc"
                        Layout.fillWidth: true
                    }
                }
            }
            
            // Faction statistics (read-only information)
            GroupBox {
                Layout.fillWidth: true
                title: "Statistics"
                Material.theme: Material.Dark
                
                GridLayout {
                    anchors.fill: parent
                    columns: 2
                    rowSpacing: 5
                    columnSpacing: 10
                    
                    Label {
                        text: "Systems Controlled:"
                        color: "#cccccc"
                    }
                    Label {
                        text: "0" // TODO: Connect to actual system count
                        color: "#ffffff"
                        font.bold: true
                    }
                    
                    Label {
                        text: "Total Population:"
                        color: "#cccccc"
                    }
                    Label {
                        text: "0" // TODO: Connect to actual population
                        color: "#ffffff"
                        font.bold: true
                    }
                    
                    Label {
                        text: "Military Strength:"
                        color: "#cccccc"
                    }
                    Label {
                        text: "0" // TODO: Connect to actual military strength
                        color: "#ffffff"
                        font.bold: true
                    }
                }
            }
            
            // Spacer
            Item {
                Layout.fillHeight: true
            }
        }
    }
    
    // Color picker dialog
    ColorDialog {
        id: colorDialog
        title: "Choose Faction Color"
        
        onAccepted: {
            colorButton.selectedColor = selectedColor
        }
    }
}
