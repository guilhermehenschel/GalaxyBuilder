import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction description editor component
 */
GroupBox {
    id: root
    
    property alias text: descriptionField.text
    property bool enabled: true
    
    Layout.fillWidth: true
    title: "Description"
    Material.theme: Material.Dark
    
    ScrollView {
        anchors.fill: parent
        
        TextArea {
            id: descriptionField
            placeholderText: "Enter faction description..."
            Material.theme: Material.Dark
            enabled: root.enabled
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
