import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction controlled systems management component
 */
GroupBox {
    id: root
    
    property var controlledSystems: []
    
    signal addSystemRequested()
    signal removeSystemRequested(int index)
    
    Layout.fillWidth: true
    title: "Controlled Systems"
    Material.theme: Material.Dark
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        RowLayout {
            Layout.fillWidth: true
            
            Label {
                text: "Systems under faction control:"
                color: "#cccccc"
                Layout.fillWidth: true
            }
            
            Button {
                text: "Add System"
                Material.background: "#2196F3"
                Material.foreground: "#ffffff"
                onClicked: root.addSystemRequested()
            }
        }
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: Math.min(200, contentHeight)
            clip: true
            
            ListView {
                id: systemsListView
                model: root.controlledSystems
                spacing: 5
                
                delegate: Rectangle {
                    width: systemsListView.width
                    height: 60
                    color: "#424242"
                    radius: 4
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            
                            Label {
                                text: modelData.name || "Unknown System"
                                color: "#ffffff"
                                font.bold: true
                            }
                            
                            Label {
                                text: "Population: " + (modelData.population || 0).toLocaleString()
                                color: "#cccccc"
                                font.pointSize: 9
                            }
                        }
                        
                        Label {
                            text: "Coords: " + (modelData.x || 0) + ", " + (modelData.y || 0)
                            color: "#888888"
                            font.pointSize: 9
                        }
                        
                        Button {
                            text: "Remove"
                            Material.background: "#f44336"
                            Material.foreground: "#ffffff"
                            onClicked: root.removeSystemRequested(index)
                        }
                    }
                }
            }
        }
        
        Label {
            visible: root.controlledSystems.length === 0
            text: "No systems under control"
            color: "#888888"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }
}
