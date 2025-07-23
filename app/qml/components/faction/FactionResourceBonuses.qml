import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction resource bonuses management component
 */
GroupBox {
    id: root
    
    property var resourceBonuses: []
    
    signal addResourceBonusRequested()
    signal removeResourceBonusRequested(int index)
    
    Layout.fillWidth: true
    title: "Resource Bonuses"
    Material.theme: Material.Dark
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        RowLayout {
            Layout.fillWidth: true
            
            Label {
                text: "Configure faction resource production bonuses:"
                color: "#cccccc"
                Layout.fillWidth: true
            }
            
            Button {
                text: "Add Bonus"
                Material.background: "#2196F3"
                Material.foreground: "#ffffff"
                onClicked: root.addResourceBonusRequested()
            }
        }
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: Math.min(200, contentHeight)
            clip: true
            
            ListView {
                id: bonusListView
                model: root.resourceBonuses
                spacing: 5
                
                delegate: Rectangle {
                    width: bonusListView.width
                    height: 40
                    color: "#424242"
                    radius: 4
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 8
                        
                        Label {
                            text: modelData.resourceType || "Unknown Resource"
                            color: "#ffffff"
                            Layout.fillWidth: true
                        }
                        
                        Label {
                            text: "+" + (modelData.bonusAmount || 0) + "%"
                            color: "#4CAF50"
                            font.bold: true
                        }
                        
                        Button {
                            text: "Remove"
                            Material.background: "#f44336"
                            Material.foreground: "#ffffff"
                            onClicked: root.removeResourceBonusRequested(index)
                        }
                    }
                }
            }
        }
        
        Label {
            visible: root.resourceBonuses.length === 0
            text: "No resource bonuses configured"
            color: "#888888"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }
}
