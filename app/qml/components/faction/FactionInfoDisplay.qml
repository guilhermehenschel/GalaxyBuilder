import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Main information display for faction list item
 */
ColumnLayout {
    id: root
    
    property string factionName: ""
    property string factionDescription: ""
    property int systemCount: 0
    property int resourceCount: 0
    property int stockCount: 0
    
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 4
    
    Label {
        text: root.factionName || "Unnamed Faction"
        font.pixelSize: 16
        font.bold: true
        color: "#ffffff"
        Layout.fillWidth: true
        elide: Text.ElideRight
    }
    
    Label {
        text: root.factionDescription || "No description available"
        font.pixelSize: 12
        color: "#cccccc"
        Layout.fillWidth: true
        Layout.fillHeight: true
        wrapMode: Text.WordWrap
        verticalAlignment: Text.AlignTop
        maximumLineCount: 2
        elide: Text.ElideRight
    }
    
    // Statistics row
    RowLayout {
        Layout.fillWidth: true
        spacing: 15
        
        Label {
            text: "Systems: " + root.systemCount
            font.pixelSize: 10
            color: "#888888"
        }
        
        Label {
            text: "Resources: " + root.resourceCount
            font.pixelSize: 10
            color: "#888888"
        }
    }
    
    Label {
        text: "Stock: " + root.stockCount
        font.pixelSize: 10
        color: "#888888"
        Layout.fillWidth: true
    }
}
