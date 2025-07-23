import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Header component for faction detail view
 */
RowLayout {
    id: root
    
    property string factionName: ""
    property bool canSave: false
    
    signal backRequested()
    signal saveRequested()
    
    Layout.fillWidth: true
    
    Button {
        text: "← Back to Faction List"
        Material.theme: Material.Dark
        Material.accent: "#2196F3"
        
        onClicked: {
            root.backRequested()
        }
    }
    
    Label {
        text: root.factionName ? "Managing: " + root.factionName : "Faction Details"
        font.pixelSize: 20
        font.bold: true
        color: "#ffffff"
        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
    }
    
    Button {
        text: "Save All Changes"
        Material.theme: Material.Dark
        Material.accent: "#4CAF50"
        enabled: root.canSave
        
        onClicked: {
            root.saveRequested()
        }
    }
}
