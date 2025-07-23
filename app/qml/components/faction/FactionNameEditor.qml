import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction name editor component
 */
GroupBox {
    id: root
    
    property alias text: nameField.text
    property bool enabled: true
    
    Layout.fillWidth: true
    title: "Name"
    Material.theme: Material.Dark

    TextField {
        id: nameField
        anchors.fill: parent
        placeholderText: "Enter faction name..."
        Material.theme: Material.Dark
        enabled: root.enabled
        
        background: Rectangle {
            color: "#3e3e3e"
            border.color: nameField.focus ? "#2196F3" : "#555555"
            border.width: 1
            radius: 2
        }

        onTextChanged: {
            root.textChanged(nameField.text)
        }
    }
}
