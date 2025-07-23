import QtQuick

Rectangle {
    id: root

    // Properties that will be bound from the parent
    property real startX: 0
    property real startY: 0
    property real endX: 0
    property real endY: 0
    property color laneColor: "#00ffff"
    property real laneOpacity: 0.6
    property real laneWidth: 1

    // Calculate lane dimensions and position
    readonly property real laneLength: Math.sqrt(Math.pow(endX - startX, 2) + Math.pow(endY - startY, 2))
    readonly property real laneAngle: Math.atan2(endY - startY, endX - startX) * 180 / Math.PI

    width: laneLength
    height: laneWidth
    color: laneColor
    opacity: laneOpacity
    x: startX
    y: startY
    rotation: laneAngle
    transformOrigin: Item.TopLeft

    // Optional: Add animation for lane drawing effect
    property bool animateDrawing: false

    // Clip to create drawing animation
    clip: animateDrawing

    Rectangle {
        id: animatedPortion
        width: animateDrawing ? parent.width : parent.width
        height: parent.height
        color: parent.color

        PropertyAnimation {
            id: drawAnimation
            target: animatedPortion
            property: "width"
            from: 0
            to: root.width
            duration: 300
            running: animateDrawing
        }
    }

    // Optional: Hover effect for lanes
    property bool enableHover: false

    MouseArea {
        anchors.fill: parent
        hoverEnabled: enableHover

        onEntered: {
            if (enableHover) {
                parent.opacity = Math.min(1.0, laneOpacity + 0.3);
                parent.height = laneWidth + 1;
            }
        }

        onExited: {
            if (enableHover) {
                parent.opacity = laneOpacity;
                parent.height = laneWidth;
            }
        }
    }
}
