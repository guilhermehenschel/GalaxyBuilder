import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import GalaxyBuilderApp 1.0

/**
 * @brief Detailed view for a selected faction using modular components
 */
Item {
    id: root
    
    property int selectedFactionId: -1
    property var factionsViewModel: null
    
    signal backRequested()
    signal factionUpdated()
    
    // Current faction data
    property string currentFactionName: ""
    property string currentFactionDescription: ""
    property string currentFactionColor: "#FF5722"
    property var currentResourceBonuses: []
    property var currentResourceStock: []
    property var currentControlledSystems: []
    
    function loadFactionDetails() {
        if (selectedFactionId >= 0 && factionsViewModel && factionsViewModel.factionListModel) {
            console.log("Loading faction details for ID:", selectedFactionId)
            var factionListModel = factionsViewModel.factionListModel
            for (var i = 0; i < factionListModel.rowCount(); i++) {
                var index = factionListModel.index(i, 0)
                var factionId = factionListModel.data(index, factionListModel.IdRole)
                if (factionId === selectedFactionId) {
                    var factionName = factionListModel.data(index, factionListModel.NameRole)
                    var factionDesc = factionListModel.data(index, factionListModel.DescriptionRole)
                    var factionColor = factionListModel.data(index, factionListModel.ColorRole)
                    
                    console.log("Found faction:", factionName, "Description:", factionDesc, "Color:", factionColor)
                    
                    currentFactionName = factionName || "Unnamed Faction"
                    currentFactionDescription = factionDesc || ""
                    currentFactionColor = factionColor || "#FF5722"
                    
                    // Load additional data (these would be populated from the model in a real implementation)
                    currentResourceBonuses = []
                    currentResourceStock = []
                    currentControlledSystems = []
                    
                    console.log("Faction details loaded successfully. Name:", currentFactionName)
                    break
                }
            }
        } else {
            console.log("Cannot load faction details. selectedFactionId:", selectedFactionId, "factionsViewModel:", !!factionsViewModel)
        }
    }
    
    // Call loadFactionDetails when selectedFactionId changes
    onSelectedFactionIdChanged: {
        loadFactionDetails()
    }
    
    function saveFactionDetails() {
        if (selectedFactionId >= 0) {
            console.log("Saving faction details for ID:", selectedFactionId)
            currentFactionName = nameEditor.factionName
            currentFactionDescription = descriptionEditor.factionDescription
            currentFactionColor = colorEditor.selectedColor
            
            // TODO: Save to model
            root.factionUpdated()
        }
    }
    
    ScrollView {
        anchors.fill: parent
        anchors.margins: 10
        
        ColumnLayout {
            width: root.width - 20
            spacing: 15
            
            FactionDetailHeader {
                id: header
                Layout.fillWidth: true
                factionName: root.currentFactionName
                canSave: root.selectedFactionId >= 0
                
                onBackRequested: root.backRequested()
                onSaveRequested: root.saveFactionDetails()
            }
            
            FactionInformation {
                id: information
                Layout.fillWidth: true
                factionId: root.selectedFactionId
            }
            
            FactionNameEditor {
                id: nameEditor
                Layout.fillWidth: true
                text: factionsViewModel.name
                enabled: root.selectedFactionId >= 0
                
                onTextChanged: factionsViewModel.name = factionName
            }
            
            FactionDescriptionEditor {
                id: descriptionEditor
                Layout.fillWidth: true
                text: root.currentFactionDescription
                enabled: root.selectedFactionId >= 0

                onTextChanged: root.currentFactionDescription = text
            }
            
            FactionColorEditor {
                id: colorEditor
                Layout.fillWidth: true
                selectedColor: root.currentFactionColor
                enabled: root.selectedFactionId >= 0
                
                onSelectedColorChanged: root.currentFactionColor = selectedColor
            }
            
            FactionStatistics {
                id: statistics
                Layout.fillWidth: true
                systemCount: root.currentControlledSystems.length
                resourceBonusCount: root.currentResourceBonuses.length
                population: 0 // TODO: Calculate from controlled systems
                militaryStrength: 0 // TODO: Calculate from controlled systems
            }
            
            FactionResourceBonuses {
                id: resourceBonuses
                Layout.fillWidth: true
                resourceBonuses: root.currentResourceBonuses
                
                onAddResourceBonusRequested: {
                    console.log("Add resource bonus requested")
                    // TODO: Implement add resource bonus
                }
                
                onRemoveResourceBonusRequested: function(index) {
                    console.log("Remove resource bonus requested:", index)
                    // TODO: Implement remove resource bonus
                }
            }
            
            FactionResourceStock {
                id: resourceStock
                Layout.fillWidth: true
                resourceStock: root.currentResourceStock
                
                onAddResourceStockRequested: {
                    console.log("Add resource stock requested")
                    // TODO: Implement add resource stock
                }
                
                onRemoveResourceStockRequested: function(index) {
                    console.log("Remove resource stock requested:", index)
                    // TODO: Implement remove resource stock
                }
                
                onUpdateResourceStockRequested: function(index, amount) {
                    console.log("Update resource stock requested:", index, amount)
                    // TODO: Implement update resource stock
                }
            }
            
            FactionSystems {
                id: systems
                Layout.fillWidth: true
                controlledSystems: root.currentControlledSystems
                
                onAddSystemRequested: {
                    console.log("Add system requested")
                    // TODO: Implement add system
                }
                
                onRemoveSystemRequested: function(index) {
                    console.log("Remove system requested:", index)
                    // TODO: Implement remove system
                }
            }
            
            // Spacer
            Item {
                Layout.fillHeight: true
            }
        }
    }
    
    // Helper functions for data retrieval (to be moved to C++ later)
    function getSystemCount() {
        return root.currentControlledSystems.length.toString()
    }
    
    function getResourceBonusCount() {
        return root.currentResourceBonuses.length.toString()
    }
    
    function getResourceBonuses() {
        return root.currentResourceBonuses
    }
    
    function getControlledSystems() {
        return root.currentControlledSystems
    }
    
    function getResourceStock() {
        return root.currentResourceStock
    }
}
