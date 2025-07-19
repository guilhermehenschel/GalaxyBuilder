import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels

Rectangle {
    required property PlanetViewModel planet
    required property int index
    property bool isCurrentItem: false

    width: parent ? parent.width : 400
    implicitHeight: planet ? planetProperties.implicitHeight + 20 : 60
    height: implicitHeight
    color: isCurrentItem ? "#404040" : "#2a2a2a"
    border.color: isCurrentItem ? "#0088ff" : "#606060"
    border.width: 1
    radius: 3
    
    // Show placeholder when planet is null
    Text {
        anchors.centerIn: parent
        text: "No planet data"
        color: "#666666"
        visible: !planet
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ListView.view.currentIndex = index
        }
    }

    GridLayout {
        id: planetProperties
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        visible: planet !== null

        anchors.margins: 10
        columns: 4
        columnSpacing: 10
        rowSpacing: 5

        Text {
            text: "Name:"
            color: "#cccccc"
        }
        TextField {
            Layout.fillWidth: true
            text: planet ? planet.name : ""
            color: "#ffffff"
            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }
            onEditingFinished: if (planet) planet.name = text
        }

        Text {
            text: "Type:"
            color: "#cccccc"
        }
        ComboBox {
            Layout.fillWidth: true
            model: ["Rocky", "GasGiant", "IceGiant", "Desert", "Ocean", "Frozen", "Volcanic", "Toxic"]
            currentIndex: planet ? planet.type : 0

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: Text {
                text: parent.displayText
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
                leftPadding: 10
            }

            onCurrentIndexChanged: if (planet) planet.type = currentIndex
        }

        Text {
            text: "Moons:"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 0
            to: 100
            value: planet ? planet.numberOfMoons : 0

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: parent.value
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.numberOfMoons = value
        }

        Text {
            text: "Size:"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 1
            to: 10
            value: planet ? planet.size : 1

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: parent.value
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.size = value
        }

        Text {
            text: "Mass (Earth masses):"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 1
            to: 100000
            stepSize: 1
            value: planet ? Math.round(planet.mass * 100) : 100

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: (parent.value / 100).toFixed(2)
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.mass = value / 100.0
        }

        Text {
            text: "Gravity (Earth gravity):"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 1
            to: 1000
            stepSize: 1
            value: planet ? Math.round(planet.gForce * 100) : 100

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: (parent.value / 100).toFixed(2)
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.gForce = value / 100.0
        }

        Text {
            text: "Min Temp (K):"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 0
            to: 1000
            value: planet ? planet.minTemperature : 273

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: parent.value
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.minTemperature = value
        }

        Text {
            text: "Max Temp (K):"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 0
            to: 1000
            value: planet ? planet.maxTemperature : 300

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: parent.value
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.maxTemperature = value
        }

        Text {
            text: "Orbit Distance:"
            color: "#cccccc"
        }
        SpinBox {
            Layout.fillWidth: true
            from: 20
            to: 500
            value: planet ? planet.orbitalRadius : 50

            background: Rectangle {
                color: "#404040"
                border.color: "#606060"
                radius: 3
            }

            contentItem: TextInput {
                text: parent.value
                color: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                readOnly: !parent.editable
            }

            onValueModified: if (planet) planet.orbitalRadius = value
        }
    }
}
