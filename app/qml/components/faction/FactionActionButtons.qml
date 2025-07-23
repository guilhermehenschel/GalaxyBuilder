import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Action buttons for faction list item
 */
ColumnLayout {
    id: root
    
    signal editRequested()
    signal deleteRequested()
    signal resourcesRequested()
    
    spacing: 5
    
    Button {
        text: "Edit"
        Layout.preferredWidth: 60
        Layout.preferredHeight: 28
        
        font.pixelSize: 10
        Material.theme: Material.Dark
        Material.foreground: "#ffffff"
        
        background: Rectangle {
            color: parent.hovered ? "#2196F3" : "#1976D2"
            radius: 4
            border.color: parent.hovered ? "#42A5F5" : "#1976D2"
            border.width: 1
            
            Behavior on color {
                ColorAnimation { duration: 150 }
            }
        }
        
        onClicked: root.editRequested()
    }
    
    Button {
        text: "Delete"
        Layout.preferredWidth: 60
        Layout.preferredHeight: 28
        
        font.pixelSize: 10
        Material.theme: Material.Dark
        Material.foreground: "#ffffff"
        
        background: Rectangle {
            color: parent.hovered ? "#f44336" : "#d32f2f"
            radius: 4
            border.color: parent.hovered ? "#ef5350" : "#d32f2f"
            border.width: 1
            
            Behavior on color {
                ColorAnimation { duration: 150 }
            }
        }
        
        onClicked: root.deleteRequested()
    }
    
    Button {
        text: "Resources"
        Layout.preferredWidth: 60
        Layout.preferredHeight: 28
        
        font.pixelSize: 10
        Material.theme: Material.Dark
        Material.foreground: "#ffffff"
        
        background: Rectangle {
            color: parent.hovered ? "#4CAF50" : "#388E3C"
            radius: 4
            border.color: parent.hovered ? "#66BB6A" : "#388E3C"
            border.width: 1
            
            Behavior on color {
                ColorAnimation { duration: 150 }
            }
        }
        
        onClicked: root.resourcesRequested()
    }
}
