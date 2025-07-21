import QtQuick
import QtQuick.Controls
import QtQml
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0
import GalaxyCore.Model 1.0

Item {
    id: root
    
    property GalaxyController controller
    property real zoomFactor: 1.0
    property point panOffset: Qt.point(0, 0)
    property point lastMousePos: Qt.point(0, 0)
    property bool isPanning: false
    
    // Signal emitted when a system is double-clicked
    signal systemDoubleClicked(int systemId)
    
    // Function to export galaxy content without UI elements
    function exportGalaxyImage(filePath, width, height) {
        // Create a temporary container that shows the entire galaxy without zoom/pan
        var tempContainer = exportContainerComponent.createObject(root)
        if (tempContainer) {
            tempContainer.width = width
            tempContainer.height = height
            tempContainer.visible = true
            
            // Capture the temporary container
            tempContainer.grabToImage(function(result) {
                var success = result.saveToFile(filePath)
                tempContainer.destroy()
                
                if (success) {
                    GalaxyController.exportFinished(true, "Galaxy image exported successfully to " + filePath)
                } else {
                    GalaxyController.exportFinished(false, "Failed to save galaxy image to " + filePath)
                }
            })
        }
    }
    
    // Component for creating export-specific galaxy view
    Component {
        id: exportContainerComponent
        Rectangle {
            color: "#0a0a0a"
            
            Item {
                anchors.fill: parent
                
                // Calculate scale to fit entire galaxy in the export size
                property real exportScale: {
                    if (controller && controller.galaxyWidth && controller.galaxyHeight) {
                        var scaleX = width / controller.galaxyWidth
                        var scaleY = height / controller.galaxyHeight
                        return Math.min(scaleX, scaleY) * 0.9 // 90% to add some padding
                    }
                    return 1.0
                }
                
                transform: [
                    Translate {
                        x: width/2
                        y: height/2
                    },
                    Scale {
                        xScale: exportScale
                        yScale: exportScale
                    },
                    Translate {
                        x: controller ? -controller.galaxyWidth/2 : 0
                        y: controller ? -controller.galaxyHeight/2 : 0
                    }
                ]
                
                // Travel lanes for export
                Repeater {
                    model: controller && controller.galaxyViewModel && controller.showTravelLanes ? controller.galaxyViewModel.travelLanes : null
                    
                    delegate: TravelLane {
                        startX: model.fromX
                        startY: model.fromY
                        endX: model.toX
                        endY: model.toY
                        laneOpacity: model.isActive ? 0.6 : 0.3
                        laneColor: model.laneType === 0 ? "#00ffff" : "#ffff00"
                    }
                }
                
                // Star systems for export
                Repeater {
                    model: controller && controller.galaxyViewModel ? controller.galaxyViewModel.starSystems : null
                    delegate: SystemNode {
                        systemId: model.systemId
                        systemName: model.name
                        systemX: model.positionX
                        systemY: model.positionY
                        systemType: model.starType
                        systemSize: model.systemSize
                        isSelected: false // No selection in export
                        showInfluenceRadius: false // No influence radius in export
                        showSystemNames: true // Always show names in export
                        controller: root.controller
                    }
                }
            }
        }
    }
    
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
                visible: !controller || !controller.galaxyViewModel
            }
            
            // Container for galaxy content with zoom/pan transform
            Item {
                id: galaxyContainer
                anchors.fill: parent
                visible: controller && controller.galaxyViewModel
                
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
                
                // Travel lanes - using actual C++ created travel lane data
                Repeater {
                    id: travelLanesRepeater
                    model: controller && controller.galaxyViewModel && controller.showTravelLanes ? controller.galaxyViewModel.travelLanes : null
                    
                    delegate: TravelLane {
                        startX: model.fromX
                        startY: model.fromY
                        endX: model.toX
                        endY: model.toY
                        laneOpacity: model.isActive ? 0.6 : 0.3
                        laneColor: model.laneType === 0 ? "#00ffff" : "#ffff00"  // Different colors for different lane types
                    }
                }
                
                // Star systems
                Repeater {
                    model: controller && controller.galaxyViewModel ? controller.galaxyViewModel.starSystems : null
                    delegate: SystemNode {
                        systemId: model.systemId
                        systemName: model.name
                        systemX: model.positionX
                        systemY: model.positionY
                        systemType: model.starType
                        systemSize: model.systemSize
                        isSelected: controller && controller.selectedStarSystemViewModel ? (controller.selectedStarSystemViewModel.systemId === model.systemId) : false
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
        
        // System Properties Panel (enabled for testing)
        PropertyPanel {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 150  // Leave space for zoom controls
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            width: 300
            visible: true // Enabled to show the panel
            
            controller: root.controller
        }
    }
}
