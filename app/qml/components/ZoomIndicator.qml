import QtQuick 2.15

Rectangle {
    id: root
    
    property real zoomFactor: 1.0
    property point panOffset: Qt.point(0, 0)
    property bool showPanOffset: false
    
    width: showPanOffset ? 140 : 80
    height: showPanOffset ? 45 : 25
    color: "#303030"
    border.color: "#606060"
    border.width: 1
    radius: 4
    
    Column {
        anchors.centerIn: parent
        spacing: 2
        
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Zoom: " + (zoomFactor * 100).toFixed(0) + "%"
            color: "#ffffff"
            font.pixelSize: 10
        }
        
        Text {
            visible: showPanOffset
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Pan: " + panOffset.x.toFixed(0) + ", " + panOffset.y.toFixed(0)
            color: "#cccccc"
            font.pixelSize: 8
        }
    }
    
    // Click to toggle pan offset display
    MouseArea {
        anchors.fill: parent
        onClicked: showPanOffset = !showPanOffset
    }
}
