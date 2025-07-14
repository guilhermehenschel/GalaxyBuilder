import QtQuick 2.15
import QtQuick.Controls 2.15
import GalaxyCore 1.0

Item {
    id: root
    
    // Properties that will be bound from the parent
    property int systemId: 0
    property string systemName: ""
    property real systemX: 0
    property real systemY: 0
    property int systemType: 0
    property int systemSize: 0
    property bool isSelected: false
    property bool showInfluenceRadius: false
    property bool showSystemNames: true
    
    // Controller for handling clicks
    property GalaxyController controller
    
    // Signal emitted when system is double-clicked
    signal systemDoubleClicked(int systemId)
    
    // Calculate dynamic node size based on system size (radius 15-30)
    readonly property real nodeRadius: getSystemRadius(systemSize)
    readonly property real nodeDiameter: nodeRadius * 2
    
    x: systemX - nodeRadius
    y: systemY - nodeRadius
    width: nodeDiameter
    height: nodeDiameter
    
    // Influence radius circle
    Rectangle {
        id: influenceRadius
        visible: showInfluenceRadius
        width: 100
        height: 100
        radius: 50
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        color: "transparent"
        border.color: "#404080"
        border.width: 0.5
        opacity: 0.7
    }
    
    // Main system visual
    Rectangle {
        id: systemDot
        width: parent.width
        height: parent.height
        radius: nodeRadius
        color: getStarColor(systemType)
        border.color: "#ffffff"
        border.width: 1
        
        // Selection indicator ring
        Rectangle {
            id: selectionRing
            visible: isSelected
            width: parent.width + 6
            height: parent.height + 6
            radius: (parent.width + 6) / 2
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            color: "transparent"
            border.color: "#ff6600"
            border.width: 2
            
            // Animated pulse effect for selection
            SequentialAnimation {
                running: isSelected
                loops: Animation.Infinite
                PropertyAnimation {
                    target: selectionRing
                    property: "opacity"
                    from: 1.0
                    to: 0.6
                    duration: 800
                }
                PropertyAnimation {
                    target: selectionRing
                    property: "opacity"
                    from: 0.6
                    to: 1.0
                    duration: 800
                }
            }
        }
        
        // Hover effect
        Rectangle {
            id: hoverRing
            visible: mouseArea.containsMouse && !isSelected
            width: parent.width + 4
            height: parent.height + 4
            radius: (parent.width + 4) / 2
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            color: "transparent"
            border.color: "#ffffff"
            border.width: 1
            opacity: 0.8
        }
        
        // Click handling
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            anchors.margins: -4  // Expand clickable area
            z: 99
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            
            onClicked: (mouse)=>{
                console.log("Single click detected - systemId:", systemId, "systemName:", systemName)
                if (controller) {
                    controller.selectSystem(systemId)
                }
            }
            
            onDoubleClicked: {
                console.log("Double click detected - systemId:", systemId, "systemName:", systemName)
                // Emit signal to let parent handle dialog
                systemDoubleClicked(systemId)
            }
        }
    }
    
    // System name label
    Text {
        id: systemLabel
        visible: showSystemNames
        text: systemName
        color: "#ffffff"
        font.pixelSize: 10
        font.bold: isSelected
        x: nodeRadius + 4  // Position relative to node size
        y: -6
        
        // Add background for better readability when selected
        Rectangle {
            visible: isSelected
            anchors.fill: parent
            anchors.margins: -2
            color: "#000000"
            opacity: 0.7
            radius: 2
            z: -1
        }
    }
    
    // Helper functions for visual styling
    function getStarColor(type) {
        switch(type) {
            case 0: return "#ff6961"  // Red Dwarf
            case 1: return "#ffcc6f"  // Yellow Star
            case 2: return "#9bb0ff"  // Blue Star
            case 3: return "#ffffff"  // White Dwarf
            case 4: return "#ffab7a"  // Red Giant
            case 5: return "#e6e6e6"  // Neutron Star
            case 6: return "#000000"  // Black Hole
            default: return "#ffff80" // Default/Unknown
        }
    }
    
    function getSystemRadius(size) {
        // Map system size to radius between 15 and 30 pixels
        switch(size) {
            case 0: return 15   // Tiny
            case 1: return 18   // Small
            case 2: return 22   // Medium
            case 3: return 26   // Large
            case 4: return 30   // Huge
            default: return 22  // Default (Medium)
        }
    }
    
    // Legacy function for compatibility (now returns 1.0 since we use direct sizing)
    function getSystemScale(size) {
        return 1.0
    }
}
