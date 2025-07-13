import QtQuick 2.15
import QtQuick.Controls 2.15
import GalaxyCore 1.0

Rectangle {
    id: root
    
    // Properties that will be bound from the parent
    property GalaxyController controller
    property bool hasSelectedSystem: controller && controller.hasSelectedSystem
    
    // Panel appearance
    color: "#252525"
    border.color: "#606060"
    border.width: 1
    radius: 8
    
    // Content area
    Flickable {
        id: scrollArea
        anchors.fill: parent
        anchors.margins: 15
        contentHeight: contentColumn.height
        clip: true
        
        // Enable scrolling when content is too tall
        boundsBehavior: Flickable.StopAtBounds
        
        Column {
            id: contentColumn
            width: parent.width
            spacing: 15
            
            // Header
            Text {
                text: "System Properties"
                color: "#ffffff"
                font.pixelSize: 16
                font.bold: true
            }
            
            Rectangle {
                width: parent.width
                height: 1
                color: "#606060"
            }
            
            // System Name Section
            PropertySection {
                width: parent.width
                label: "Name:"
                
                content: Rectangle {
                    width: parent.width
                    height: 32
                    color: "#404040"
                    border.color: systemNameInput.activeFocus ? "#0078d4" : "#606060"
                    border.width: 1
                    radius: 4
                    
                    TextInput {
                        id: systemNameInput
                        anchors.fill: parent
                        anchors.margins: 8
                        color: "#ffffff"
                        font.pixelSize: 12
                        selectByMouse: true
                        
                        property bool internalUpdate: false
                        text: controller && !activeFocus && !internalUpdate ? controller.selectedSystemName : text
                        
                        onTextChanged: {
                            if (controller && activeFocus && !internalUpdate) {
                                internalUpdate = true
                                controller.selectedSystemName = text
                                internalUpdate = false
                            }
                        }
                        
                        // Enter key handling
                        Keys.onReturnPressed: focus = false
                        Keys.onEnterPressed: focus = false
                    }
                }
            }
            
            // Star Type Section
            PropertySection {
                width: parent.width
                label: "Star Type:"
                
                content: ClickableProperty {
                    width: parent.width
                    text: getStarTypeName(controller ? controller.selectedSystemType : 0)
                    onClicked: {
                        if (controller) {
                            var currentType = controller.selectedSystemType
                            var nextType = (currentType + 1) % 7  // 7 star types (0-6)
                            controller.selectedSystemType = nextType
                        }
                    }
                }
            }
            
            // System Size Section
            PropertySection {
                width: parent.width
                label: "System Size:"
                
                content: ClickableProperty {
                    width: parent.width
                    text: getSystemSizeName(controller ? controller.selectedSystemSize : 0)
                    onClicked: {
                        if (controller) {
                            var currentSize = controller.selectedSystemSize
                            var nextSize = (currentSize + 1) % 5  // 5 system sizes (0-4)
                            controller.selectedSystemSize = nextSize
                        }
                    }
                }
            }
            
            // Position Section
            PropertySection {
                width: parent.width
                label: "Position:"
                
                content: Row {
                    width: parent.width
                    spacing: 10
                    
                    Column {
                        width: (parent.width - 10) / 2
                        spacing: 3
                        
                        Text {
                            text: "X:"
                            color: "#cccccc"
                            font.pixelSize: 10
                        }
                        
                        Rectangle {
                            width: parent.width
                            height: 28
                            color: "#404040"
                            border.color: systemXInput.activeFocus ? "#0078d4" : "#606060"
                            border.width: 1
                            radius: 4
                            
                            TextInput {
                                id: systemXInput
                                anchors.fill: parent
                                anchors.margins: 6
                                color: "#ffffff"
                                font.pixelSize: 11
                                selectByMouse: true
                                validator: DoubleValidator { bottom: 0; top: 9999; decimals: 1 }
                                
                                property bool internalUpdate: false
                                text: controller && !activeFocus && !internalUpdate ? controller.selectedSystemX.toFixed(1) : text
                                
                                onTextChanged: {
                                    if (controller && activeFocus && !internalUpdate && !isNaN(parseFloat(text))) {
                                        internalUpdate = true
                                        controller.selectedSystemX = parseFloat(text)
                                        internalUpdate = false
                                    }
                                }
                                
                                Keys.onReturnPressed: focus = false
                                Keys.onEnterPressed: focus = false
                            }
                        }
                    }
                    
                    Column {
                        width: (parent.width - 10) / 2
                        spacing: 3
                        
                        Text {
                            text: "Y:"
                            color: "#cccccc"
                            font.pixelSize: 10
                        }
                        
                        Rectangle {
                            width: parent.width
                            height: 28
                            color: "#404040"
                            border.color: systemYInput.activeFocus ? "#0078d4" : "#606060"
                            border.width: 1
                            radius: 4
                            
                            TextInput {
                                id: systemYInput
                                anchors.fill: parent
                                anchors.margins: 6
                                color: "#ffffff"
                                font.pixelSize: 11
                                selectByMouse: true
                                validator: DoubleValidator { bottom: 0; top: 9999; decimals: 1 }
                                
                                property bool internalUpdate: false
                                text: controller && !activeFocus && !internalUpdate ? controller.selectedSystemY.toFixed(1) : text
                                
                                onTextChanged: {
                                    if (controller && activeFocus && !internalUpdate && !isNaN(parseFloat(text))) {
                                        internalUpdate = true
                                        controller.selectedSystemY = parseFloat(text)
                                        internalUpdate = false
                                    }
                                }
                                
                                Keys.onReturnPressed: focus = false
                                Keys.onEnterPressed: focus = false
                            }
                        }
                    }
                }
            }
            
            // Clear Selection Button
            Rectangle {
                width: parent.width
                height: 35
                color: clearMouseArea.pressed ? "#704040" : (clearMouseArea.containsMouse ? "#654040" : "#604040")
                border.color: "#806060"
                border.width: 1
                radius: 4
                
                Text {
                    anchors.centerIn: parent
                    text: "Clear Selection"
                    color: "#ffffff"
                    font.pixelSize: 12
                }
                
                MouseArea {
                    id: clearMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    
                    onClicked: {
                        if (controller) {
                            controller.clearSelection()
                        }
                    }
                }
            }
        }
    }
    
    // Scrollbar indicator
    Rectangle {
        visible: scrollArea.contentHeight > scrollArea.height
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 2
        width: 4
        color: "#404040"
        radius: 2
        
        Rectangle {
            width: parent.width
            height: Math.max(20, parent.height * (scrollArea.height / scrollArea.contentHeight))
            y: parent.height * (scrollArea.contentY / scrollArea.contentHeight)
            color: "#808080"
            radius: 2
        }
    }
    
    // Helper functions for enum display
    function getStarTypeName(type) {
        switch(type) {
            case 0: return "O-Class (Blue Giant)"
            case 1: return "B-Class (Blue-White)"
            case 2: return "A-Class (White)"
            case 3: return "F-Class (Yellow-White)"
            case 4: return "G-Class (Yellow)"
            case 5: return "K-Class (Orange)"
            case 6: return "M-Class (Red Dwarf)"
            default: return "Unknown"
        }
    }
    
    function getSystemSizeName(size) {
        switch(size) {
            case 0: return "Tiny"
            case 1: return "Small"
            case 2: return "Medium"
            case 3: return "Large"
            case 4: return "Huge"
            default: return "Unknown"
        }
    }
    
    // Property section component
    component PropertySection: Column {
        property string label: ""
        property alias content: contentContainer.children
        
        spacing: 5
        
        Text {
            text: label
            color: "#cccccc"
            font.pixelSize: 12
        }
        
        Item {
            id: contentContainer
            width: parent.width
            height: children.length > 0 ? children[0].height : 0
        }
    }
    
    // Clickable property component
    component ClickableProperty: Rectangle {
        property string text: ""
        property alias mouseArea: clickArea
        signal clicked()
        
        height: 32
        color: clickArea.pressed ? "#505050" : (clickArea.containsMouse ? "#454545" : "#404040")
        border.color: "#606060"
        border.width: 1
        radius: 4
        
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            text: parent.text
            color: "#ffffff"
            font.pixelSize: 12
        }
        
        Text {
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            text: "▼"
            color: "#888888"
            font.pixelSize: 10
        }
        
        MouseArea {
            id: clickArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: parent.clicked()
        }
    }
}
