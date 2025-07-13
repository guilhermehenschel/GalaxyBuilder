import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml 2.15
import GalaxyCore 1.0
import GalaxyBuilderApp 1.0
import "../components"

Item {
    id: root
    
    property GalaxyController controller
    property real zoomFactor: 1.0
    property point panOffset: Qt.point(0, 0)
    property point lastMousePos: Qt.point(0, 0)
    property bool isPanning: false
    
    // Signal emitted when a system is double-clicked
    signal systemDoubleClicked(int systemId)
    
    // Watch for zoom and pan changes (canvas removed, using model-based rendering)
    onZoomFactorChanged: {
        // View updates automatically through transform changes
    }
    
    onPanOffsetChanged: {
        // View updates automatically through transform changes  
    }
    
    Rectangle {
        anchors.fill: parent
        color: "#0a0a0a"
        
        // Galaxy display using C++ models
        Item {
            id: galaxyScene
            anchors.fill: parent
            
            // Message when no galaxy is available
            Text {
                anchors.centerIn: parent
                text: "No galaxy generated. Click 'Generate Galaxy' to start."
                color: "#404040"
                font.pixelSize: 16
                visible: !controller || !controller.galaxy
            }
            
            // Container for galaxy content with zoom/pan transform
            Item {
                id: galaxyContainer
                anchors.fill: parent
                visible: controller && controller.galaxy
                
                transform: [
                    Translate {
                        x: parent.width/2 + root.panOffset.x
                        y: parent.height/2 + root.panOffset.y
                    },
                    Scale {
                        xScale: root.zoomFactor
                        yScale: root.zoomFactor
                    },
                    Translate {
                        x: controller ? -controller.galaxyWidth/2 : 0
                        y: controller ? -controller.galaxyHeight/2 : 0
                    }
                ]
                
                // Travel lanes (drawn first, behind systems)
                Repeater {
                    model: controller && controller.galaxy ? controller.galaxy.lanesModel : null
                    delegate: TravelLane {
                        startX: model.startX
                        startY: model.startY
                        endX: model.endX
                        endY: model.endY
                        visible: controller ? controller.showTravelLanes : false
                        laneColor: "#00ffff"
                        laneOpacity: 0.6
                        laneWidth: 1
                    }
                }
                
                // Star systems
                Repeater {
                    model: controller && controller.galaxy ? controller.galaxy.systemsModel : null
                    delegate: SystemNode {
                        systemId: model.id
                        systemName: model.name
                        systemX: model.x
                        systemY: model.y
                        systemType: model.type
                        systemSize: model.size
                        isSelected: controller && controller.hasSelectedSystem && model && (model.id === controller.selectedSystemId)
                        showInfluenceRadius: controller ? controller.showInfluenceRadius : false
                        showSystemNames: controller ? controller.showSystemNames : true
                        controller: root.controller
                        
                        onSystemDoubleClicked: function(systemId) {
                            root.systemDoubleClicked(systemId)
                        }
                    }
                }
            }
        }
        
        // Mouse interaction
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            propagateComposedEvents: true
            
            onPressed: function(mouse) {
                lastMousePos = Qt.point(mouse.x, mouse.y)
                isPanning = true
            }
            
            onReleased: function(mouse) {
                isPanning = false
            }
            
            onPositionChanged: function(mouse) {
                if (isPanning) {
                    var delta = Qt.point(mouse.x - lastMousePos.x, mouse.y - lastMousePos.y)
                    panOffset = Qt.point(panOffset.x + delta.x, panOffset.y + delta.y)
                    lastMousePos = Qt.point(mouse.x, mouse.y)
                }
            }
            
            onWheel: function(wheel) {
                var scaleFactor = wheel.angleDelta.y > 0 ? 1.1 : 0.9
                var newZoom = Math.max(0.1, Math.min(5.0, zoomFactor * scaleFactor))
                
                // Zoom towards mouse position
                var mousePos = Qt.point(wheel.x, wheel.y)
                var centerPos = Qt.point(width/2, height/2)
                var offset = Qt.point(mousePos.x - centerPos.x - panOffset.x, 
                                    mousePos.y - centerPos.y - panOffset.y)
                
                panOffset = Qt.point(
                    panOffset.x + offset.x * (1 - newZoom/zoomFactor),
                    panOffset.y + offset.y * (1 - newZoom/zoomFactor)
                )
                
                zoomFactor = newZoom
            }
        }
        
        // Zoom controls
        ZoomControls {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 10
            
            zoomFactor: root.zoomFactor
            minZoom: 0.1
            maxZoom: 5.0
            
            onZoomIn: {
                root.zoomFactor = Math.min(5.0, root.zoomFactor * 1.2)
            }
            
            onZoomOut: {
                root.zoomFactor = Math.max(0.1, root.zoomFactor * 0.8)
            }
            
            onResetView: {
                root.zoomFactor = 1.0
                root.panOffset = Qt.point(0, 0)
            }
        }
        
        // Zoom level indicator
        ZoomIndicator {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 10
            
            zoomFactor: root.zoomFactor
            panOffset: root.panOffset
        }
        
        // System Properties Panel
        PropertyPanel {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 150  // Leave space for zoom controls
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            width: 300
            visible: controller && controller.hasSelectedSystem
            
            controller: root.controller
        }
    }
}
