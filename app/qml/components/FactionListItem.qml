import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * @brief Individual faction item in the list view
 */
Rectangle {
    id: root
    
    property int factionId: -1
    property string factionName: ""
    property string factionDescription: ""
    property string factionColor: "#FF5722"
    
    signal clicked()
    signal editRequested()
    signal deleteRequested()
    
    height: 80
    color: mouseArea.containsMouse ? "#3e3e3e" : "#2e2e2e"
    border.color: "#404040"
    border.width: 1
    radius: 4
    
    Behavior on color {
        ColorAnimation { duration: 150 }
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        
        onClicked: {
            root.clicked()
        }
    }
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        
        // Faction color indicator
        Rectangle {
            Layout.preferredWidth: 16
            Layout.preferredHeight: 16
            Layout.alignment: Qt.AlignTop
            color: root.factionColor
            radius: 8
            border.color: "#ffffff"
            border.width: 1
        }
        
        // Faction info
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 4
            
            Label {
                text: root.factionName
                font.pixelSize: 14
                font.bold: true
                color: "#ffffff"
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
            
            Label {
                text: root.factionDescription
                font.pixelSize: 11
                color: "#cccccc"
                Layout.fillWidth: true
                Layout.fillHeight: true
                wrapMode: Text.WordWrap
                elide: Text.ElideRight
                maximumLineCount: 2
            }
            
            Label {
                text: "ID: " + root.factionId
                font.pixelSize: 10
                color: "#888888"
            }
        }
        
        // Action buttons
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            spacing: 4
            
            Button {
                id: editBtn
                text: "Edit"
                font.pixelSize: 10
                Material.theme: Material.Dark
                Material.accent: "#2196F3"
                
                Layout.preferredWidth: 50
                Layout.preferredHeight: 25
                
                onClicked: {
                    root.editRequested()
                }
            }
            
            Button {
                id: deleteBtn
                text: "Delete"
                font.pixelSize: 10
                Material.theme: Material.Dark
                Material.accent: "#F44336"
                
                Layout.preferredWidth: 50
                Layout.preferredHeight: 25
                
                onClicked: {
                    root.deleteRequested()
                }
            }
        }
    }
}
