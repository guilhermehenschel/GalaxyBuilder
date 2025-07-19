import QtQuick
import QtQuick.Controls

Column {
    id: root
    
    // Properties for zoom control
    property real zoomFactor: 1.0
    property real minZoom: 0.1
    property real maxZoom: 5.0
    property real zoomStep: 0.2
    
    // Signals for zoom actions
    signal zoomIn()
    signal zoomOut()
    signal resetView()
    
    spacing: 5
    
    // Zoom In Button
    ZoomButton {
        text: "+"
        tooltip: "Zoom In"
        enabled: zoomFactor < maxZoom
        onClicked: root.zoomIn()
    }
    
    // Zoom Out Button
    ZoomButton {
        text: "−"
        tooltip: "Zoom Out"
        enabled: zoomFactor > minZoom
        onClicked: root.zoomOut()
    }
    
    // Reset View Button
    ZoomButton {
        text: "⌂"
        tooltip: "Reset View"
        fontSize: 12
        onClicked: root.resetView()
    }
    
    // Zoom level indicator
    Rectangle {
        width: 60
        height: 20
        color: "#303030"
        border.color: "#606060"
        border.width: 1
        radius: 4
        
        Text {
            anchors.centerIn: parent
            text: (zoomFactor * 100).toFixed(0) + "%"
            color: "#ffffff"
            font.pixelSize: 9
        }
    }
    
    // Reusable zoom button component
    component ZoomButton: Rectangle {
        property string text: ""
        property string tooltip: ""
        property int fontSize: 16
        property alias mouseArea: buttonMouseArea
        signal clicked()
        
        width: 30
        height: 30
        color: buttonMouseArea.pressed ? "#404040" : (buttonMouseArea.containsMouse ? "#505050" : "#303030")
        border.color: enabled ? "#606060" : "#404040"
        border.width: 1
        radius: 4
        opacity: enabled ? 1.0 : 0.5
        
        Text {
            anchors.centerIn: parent
            text: parent.text
            color: parent.enabled ? "#ffffff" : "#888888"
            font.pixelSize: parent.fontSize
            font.bold: true
        }
        
        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ForbiddenCursor
            onClicked: if (parent.enabled) parent.clicked()
        }
        
        // Tooltip
        Rectangle {
            visible: buttonMouseArea.containsMouse && tooltip !== ""
            anchors.bottom: parent.top
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            width: tooltipText.width + 10
            height: tooltipText.height + 6
            color: "#1e1e1e"
            border.color: "#606060"
            border.width: 1
            radius: 4
            z: 1000
            
            Text {
                id: tooltipText
                anchors.centerIn: parent
                text: tooltip
                color: "#ffffff"
                font.pixelSize: 10
            }
        }
    }
}
