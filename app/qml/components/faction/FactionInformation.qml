import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction information display component
 */
GroupBox {
    id: root
    
    property int factionId: -1
    
    Layout.fillWidth: true
    title: "Faction Information"
    Material.theme: Material.Dark
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        Label {
            text: "Faction ID: " + root.factionId
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
                text: root.factionId >= 0 ? "Active" : "No faction selected"
                color: root.factionId >= 0 ? "#4CAF50" : "#F44336"
                font.bold: true
            }
        }
    }
}
