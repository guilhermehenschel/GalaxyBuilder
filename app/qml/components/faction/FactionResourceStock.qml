import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction resource stock management component
 */
GroupBox {
    id: root
    
    property var resourceStock: []
    
    signal addResourceStockRequested()
    signal removeResourceStockRequested(int index)
    signal updateResourceStockRequested(int index, int amount)
    
    Layout.fillWidth: true
    title: "Resource Stock"
    Material.theme: Material.Dark
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        RowLayout {
            Layout.fillWidth: true
            
            Label {
                text: "Manage faction resource reserves:"
                color: "#cccccc"
                Layout.fillWidth: true
            }
            
            Button {
                text: "Add Resource"
                Material.background: "#2196F3"
                Material.foreground: "#ffffff"
                onClicked: root.addResourceStockRequested()
            }
        }
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: Math.min(250, contentHeight)
            clip: true
            
            ListView {
                id: stockListView
                model: root.resourceStock
                spacing: 5
                
                delegate: Rectangle {
                    width: stockListView.width
                    height: 50
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
                        
                        SpinBox {
                            from: 0
                            to: 999999
                            value: modelData.amount || 0
                            onValueChanged: root.updateResourceStockRequested(index, value)
                            Material.accent: "#2196F3"
                        }
                        
                        Button {
                            text: "Remove"
                            Material.background: "#f44336"
                            Material.foreground: "#ffffff"
                            onClicked: root.removeResourceStockRequested(index)
                        }
                    }
                }
            }
        }
        
        Label {
            visible: root.resourceStock.length === 0
            text: "No resources in stock"
            color: "#888888"
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }
}
