import QtQuick
import QtQuick.Controls.Material

/**
 * @brief Color indicator for faction list item
 */
Rectangle {
    id: root
    
    property string factionColor: "#FF5722"
    
    width: 10
    height: parent ? parent.height - 20 : 80
    color: root.factionColor
    radius: 2
    
    // Subtle glow effect
    Rectangle {
        anchors.centerIn: parent
        width: parent.width + 4
        height: parent.height + 4
        color: "transparent"
        border.color: root.factionColor
        border.width: 1
        radius: parent.radius + 2
        opacity: 0.3
    }
}
