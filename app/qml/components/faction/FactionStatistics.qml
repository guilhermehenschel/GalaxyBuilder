import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

/**
 * @brief Faction statistics display component
 */
GroupBox {
    id: root
    
    property int systemCount: 0
    property int resourceBonusCount: 0
    property int population: 0
    property int militaryStrength: 0
    
    Layout.fillWidth: true
    title: "Statistics"
    Material.theme: Material.Dark
    
    GridLayout {
        anchors.fill: parent
        columns: 2
        rowSpacing: 5
        columnSpacing: 10
        
        Label {
            text: "Systems Controlled:"
            color: "#cccccc"
        }
        Label {
            text: root.systemCount.toString()
            color: "#ffffff"
            font.bold: true
        }
        
        Label {
            text: "Resource Bonuses:"
            color: "#cccccc"
        }
        Label {
            text: root.resourceBonusCount.toString()
            color: "#ffffff"
            font.bold: true
        }
        
        Label {
            text: "Total Population:"
            color: "#cccccc"
        }
        Label {
            text: root.population.toString()
            color: root.population > 0 ? "#ffffff" : "#888888"
            font.bold: true
        }
        
        Label {
            text: "Military Strength:"
            color: "#cccccc"
        }
        Label {
            text: root.militaryStrength.toString()
            color: root.militaryStrength > 0 ? "#ffffff" : "#888888"
            font.bold: true
        }
    }
}
