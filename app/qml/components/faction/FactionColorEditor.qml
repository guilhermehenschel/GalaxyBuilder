import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction color editor component
 */
GroupBox {
    id: root
    
    property string selectedColor: "#FF5722"
    property bool enabled: true
    
    signal colorChanged(string color)
    
    Layout.fillWidth: true
    title: "Color"
    Material.theme: Material.Dark
    
    RowLayout {
        anchors.fill: parent
        
        Rectangle {
            id: colorPreview
            Layout.preferredWidth: 50
            Layout.preferredHeight: 30
            color: root.selectedColor
            border.color: "#ffffff"
            border.width: 1
            radius: 4
        }
        
        Button {
            id: colorButton
            text: "Choose Color"
            Material.theme: Material.Dark
            enabled: root.enabled
            
            onClicked: {
                colorDialog.open()
            }
        }
        
        Label {
            text: root.selectedColor
            color: "#cccccc"
            Layout.fillWidth: true
        }
    }
    
    // Color selection popup
    Popup {
        id: colorDialog
        width: 300
        height: 200
        anchors.centerIn: parent
        modal: true
        
        property color dialogSelectedColor: "#FF0000"
        
        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10
            
            Text {
                text: "Choose Faction Color"
                font.pointSize: 16
                color: Material.foreground
            }
            
            Rectangle {
                width: 100
                height: 50
                color: colorDialog.dialogSelectedColor
                border.color: Material.accent
                border.width: 2
            }
            
            Row {
                spacing: 10
                Button {
                    text: "Red"
                    onClicked: colorDialog.dialogSelectedColor = "#FF0000"
                }
                Button {
                    text: "Blue" 
                    onClicked: colorDialog.dialogSelectedColor = "#0000FF"
                }
                Button {
                    text: "Green"
                    onClicked: colorDialog.dialogSelectedColor = "#00FF00"
                }
            }
            
            Row {
                spacing: 10
                Button {
                    text: "OK"
                    onClicked: {
                        root.selectedColor = colorDialog.dialogSelectedColor
                        root.colorChanged(root.selectedColor)
                        colorDialog.close()
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: colorDialog.close()
                }
            }
        }
    }
}
