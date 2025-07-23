import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0

Item {
    id: root

    property alias model: planetRepeater.model // ViewModel for the current star system
    property var starModel: null // Reference to the StarSystemVisualizer component

    signal planetSelected(int planetIndex)

    // Star at center
    Rectangle {
        id: starViz
        width: 20
        height: 20
        radius: 10
        anchors.centerIn: parent
        color: (starModel && starModel.color) ? starModel.color : "#ffff80"
        border.color: "#ffffff"
        border.width: 1

        // Star glow effect
        Rectangle {
            anchors.centerIn: parent
            width: parent.width + 10
            height: parent.height + 10
            radius: (parent.width + 10) / 2
            color: parent.color
            opacity: 0.3
        }
    }

    Repeater {
        id: planetRepeater

        Item {
            anchors.centerIn: parent

            // Orbit circle
            Rectangle {
                id: orbit
                width: (starViz.width + model.orbitalRadius * 15) * 2
                height: (starViz.width + model.orbitalRadius * 15) * 2
                radius: (starViz.width + model.orbitalRadius * 15)
                anchors.centerIn: parent
                color: "transparent"
                border.color: "#404040"
                border.width: 1
                opacity: 0.5
            }

            // Planet
            Rectangle {
                id: planet
                width: 8 + model.size * 2
                height: 8 + model.size * 2
                radius: (8 + model.size * 2) / 2
                color: model.planetColor || "#808080"
                border.color: "#ffffff"
                border.width: 1
                x: starViz.width + parent.width / 2 + model.orbitalRadius * 15 - width / 2
                y: parent.height / 2 - height / 2

                ToolTip {
                    visible: planetMouseArea.containsMouse
                    text: model.name + "\nType: " + model.planetTypeName + "\nMoons: " + model.numberOfMoons + "\nMass: " + model.mass.toFixed(2) + " Earth masses"
                    delay: 500
                }

                MouseArea {
                    id: planetMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        root.planetSelected(index);
                    }
                }
            }
        }
    }
}
