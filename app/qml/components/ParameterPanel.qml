import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GalaxyBuilderApp 1.0
import Galaxy.Factories

ScrollView {
    id: root

    property GalaxyController controller

    clip: true

    background: Rectangle {
        color: "#2d2d2d"
        border.color: "#404040"
        border.width: 1
    }

    ColumnLayout {
        width: root.width - 20
        anchors.margins: 10
        spacing: 15

        // Title
        Text {
            text: "Galaxy Parameters"
            color: "#ffffff"
            font.pixelSize: 16
            font.bold: true
            Layout.fillWidth: true
        }

        // Basic Parameters Group
        GroupBox {
            title: "Basic Parameters"
            Layout.fillWidth: true

            background: Rectangle {
                color: "#3d3d3d"
                border.color: "#505050"
                border.width: 1
                radius: 4
            }

            label: Text {
                text: parent.title
                color: "#ffffff"
                font.pixelSize: 14
                font.bold: true
            }

            GridLayout {
                anchors.fill: parent
                columns: 2
                columnSpacing: 10
                rowSpacing: 8

                Text {
                    text: "System Count:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                SpinBox {
                    from: 10
                    to: 1000
                    value: controller ? controller.systemCount : 50
                    editable: true
                    onValueChanged: if (controller)
                        controller.systemCount = value
                    Layout.fillWidth: true
                }

                Text {
                    text: "Galaxy Width:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                SpinBox {
                    from: 500
                    to: 5000
                    stepSize: 100
                    value: controller ? controller.galaxyWidth : 1000
                    editable: true
                    onValueChanged: if (controller)
                        controller.galaxyWidth = value
                    Layout.fillWidth: true
                }

                Text {
                    text: "Galaxy Height:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                SpinBox {
                    from: 500
                    to: 5000
                    stepSize: 100
                    value: controller ? controller.galaxyHeight : 1000
                    editable: true
                    onValueChanged: if (controller)
                        controller.galaxyHeight = value
                    Layout.fillWidth: true
                }

                Text {
                    text: "Shape:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                ComboBox {
                    model: ["Spiral", "Elliptical", "Ring", "Cluster"]
                    currentIndex: controller ? controller.galaxyShape : 0
                    onCurrentIndexChanged: if (controller)
                        controller.galaxyShape = currentIndex
                    Layout.fillWidth: true
                }

                Text {
                    text: "Seed:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                SpinBox {
                    from: 0
                    to: 999999
                    value: 0 // TODO: Add seed property to GalaxyController
                    editable: true
                    onValueChanged: if (controller)
                        controller.setGenerationSeed(value)
                    Layout.fillWidth: true
                }
            }
        }

        // Spiral Parameters Group
        GroupBox {
            title: "Spiral Parameters"
            Layout.fillWidth: true
            visible: controller ? controller.galaxyShape === 0 : true // Spiral shape

            background: Rectangle {
                color: "#3d3d3d"
                border.color: "#505050"
                border.width: 1
                radius: 4
            }

            label: Text {
                text: parent.title
                color: "#ffffff"
                font.pixelSize: 14
                font.bold: true
            }

            GridLayout {
                anchors.fill: parent
                columns: 2
                columnSpacing: 10
                rowSpacing: 8

                Text {
                    text: "Spiral Arms:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                RowLayout {
                    Layout.fillWidth: true
                    Slider {
                        from: 1.0
                        to: 6.0
                        stepSize: 0.5
                        value: controller ? controller.spiralArms : 2.0
                        onValueChanged: if (controller)
                            controller.spiralArms = value
                        Layout.fillWidth: true
                    }
                    Text {
                        text: controller ? controller.spiralArms.toFixed(1) : "2.0"
                        color: "#ffffff"
                        font.pixelSize: 12
                        Layout.preferredWidth: 30
                    }
                }

                Text {
                    text: "Tightness:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                RowLayout {
                    Layout.fillWidth: true
                    Slider {
                        from: 0.1
                        to: 1.0
                        stepSize: 0.1
                        value: controller ? controller.spiralTightness : 0.5
                        onValueChanged: if (controller)
                            controller.spiralTightness = value
                        Layout.fillWidth: true
                    }
                    Text {
                        text: controller ? controller.spiralTightness.toFixed(1) : "0.5"
                        color: "#ffffff"
                        font.pixelSize: 12
                        Layout.preferredWidth: 30
                    }
                }
            }
        }

        // Density Parameters Group
        GroupBox {
            title: "Density Parameters"
            Layout.fillWidth: true

            background: Rectangle {
                color: "#3d3d3d"
                border.color: "#505050"
                border.width: 1
                radius: 4
            }

            label: Text {
                text: parent.title
                color: "#ffffff"
                font.pixelSize: 14
                font.bold: true
            }

            GridLayout {
                anchors.fill: parent
                columns: 2
                columnSpacing: 10
                rowSpacing: 8

                Text {
                    text: "Core Radius:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                RowLayout {
                    Layout.fillWidth: true
                    Slider {
                        from: 0.1
                        to: 0.5
                        stepSize: 0.05
                        value: controller ? controller.coreRadius : 0.2
                        onValueChanged: if (controller)
                            controller.coreRadius = value
                        Layout.fillWidth: true
                    }
                    Text {
                        text: controller ? controller.coreRadius.toFixed(2) : "0.20"
                        color: "#ffffff"
                        font.pixelSize: 12
                        Layout.preferredWidth: 35
                    }
                }

                Text {
                    text: "Edge Radius:"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                RowLayout {
                    Layout.fillWidth: true
                    Slider {
                        from: 0.5
                        to: 1.0
                        stepSize: 0.05
                        value: controller ? controller.edgeRadius : 0.8
                        onValueChanged: if (controller)
                            controller.edgeRadius = value
                        Layout.fillWidth: true
                    }
                    Text {
                        text: controller ? controller.edgeRadius.toFixed(2) : "0.80"
                        color: "#ffffff"
                        font.pixelSize: 12
                        Layout.preferredWidth: 35
                    }
                }
            }
        }

        // Galaxy Management Group
        GroupBox {
            title: "Galaxy Management"
            Layout.fillWidth: true

            background: Rectangle {
                color: "#3d3d3d"
                border.color: "#505050"
                border.width: 1
                radius: 4
            }

            label: Text {
                text: parent.title
                color: "#ffffff"
                font.pixelSize: 14
                font.bold: true
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                Button {
                    id: factionManagerButton
                    text: "Manage Factions"
                    Layout.fillWidth: true

                    background: Rectangle {
                        color: parent.pressed ? "#304080" : (parent.hovered ? "#405090" : "#304070")
                        border.color: "#5080c0"
                        border.width: 1
                        radius: 4
                    }

                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        font.pixelSize: 12
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: {
                        console.log("Opening Faction Manager...");
                        mainWindow.showFactionManager();
                    }
                }

                Button {
                    text: "Manage Systems"
                    Layout.fillWidth: true
                    enabled: false // TODO: Implement system management

                    background: Rectangle {
                        color: parent.enabled ? (parent.pressed ? "#804030" : (parent.hovered ? "#905040" : "#704030")) : "#505050"
                        border.color: parent.enabled ? "#c08050" : "#707070"
                        border.width: 1
                        radius: 4
                    }

                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#ffffff" : "#a0a0a0"
                        font.pixelSize: 12
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    text: "Trade Routes"
                    Layout.fillWidth: true
                    enabled: false // TODO: Implement trade route management

                    background: Rectangle {
                        color: parent.enabled ? (parent.pressed ? "#308040" : (parent.hovered ? "#409050" : "#307040")) : "#505050"
                        border.color: parent.enabled ? "#50c080" : "#707070"
                        border.width: 1
                        radius: 4
                    }

                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#ffffff" : "#a0a0a0"
                        font.pixelSize: 12
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        // Display Options Group
        GroupBox {
            title: "Display Options"
            Layout.fillWidth: true

            background: Rectangle {
                color: "#3d3d3d"
                border.color: "#505050"
                border.width: 1
                radius: 4
            }

            label: Text {
                text: parent.title
                color: "#ffffff"
                font.pixelSize: 14
                font.bold: true
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                CheckBox {
                    text: "Show System Names"
                    checked: controller ? controller.showSystemNames : true
                    onCheckedChanged: if (controller)
                        controller.showSystemNames = checked
                    Layout.fillWidth: true

                    indicator: Rectangle {
                        implicitWidth: 18
                        implicitHeight: 18
                        x: parent.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 2
                        border.color: parent.checked ? "#4080ff" : "#808080"
                        border.width: 2
                        color: parent.checked ? "#4080ff" : "transparent"

                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            color: "white"
                            font.pixelSize: 12
                            visible: parent.parent.checked
                        }
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 12
                        color: "#ffffff"
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                CheckBox {
                    text: "Show Travel Lanes"
                    checked: controller ? controller.showTravelLanes : true
                    onCheckedChanged: if (controller)
                        controller.showTravelLanes = checked
                    Layout.fillWidth: true

                    indicator: Rectangle {
                        implicitWidth: 18
                        implicitHeight: 18
                        x: parent.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 2
                        border.color: parent.checked ? "#4080ff" : "#808080"
                        border.width: 2
                        color: parent.checked ? "#4080ff" : "transparent"

                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            color: "white"
                            font.pixelSize: 12
                            visible: parent.parent.checked
                        }
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 12
                        color: "#ffffff"
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                CheckBox {
                    text: "Show Influence Radius"
                    checked: controller ? controller.showInfluenceRadius : false
                    onCheckedChanged: if (controller)
                        controller.showInfluenceRadius = checked
                    Layout.fillWidth: true

                    indicator: Rectangle {
                        implicitWidth: 18
                        implicitHeight: 18
                        x: parent.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 2
                        border.color: parent.checked ? "#4080ff" : "#808080"
                        border.width: 2
                        color: parent.checked ? "#4080ff" : "transparent"

                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            color: "white"
                            font.pixelSize: 12
                            visible: parent.parent.checked
                        }
                    }

                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 12
                        color: "#ffffff"
                        leftPadding: parent.indicator.width + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        // Action buttons
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Button {
                text: "Generate Galaxy"
                enabled: controller ? !controller.isGenerating : true
                Layout.fillWidth: true
                onClicked: if (controller)
                    controller.generateGalaxy()

                background: Rectangle {
                    color: parent.pressed ? "#3080ff" : (parent.hovered ? "#4080ff" : "#2060df")
                    border.color: "#4080ff"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    font.pixelSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "Random Galaxy"
                enabled: controller ? !controller.isGenerating : true
                Layout.fillWidth: true
                onClicked: if (controller)
                    controller.generateRandomGalaxy()

                background: Rectangle {
                    color: parent.pressed ? "#30a030" : (parent.hovered ? "#40c040" : "#20a020")
                    border.color: "#40c040"
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    font.pixelSize: 12
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Spacer
        Item {
            Layout.fillHeight: true
        }
    }
}
