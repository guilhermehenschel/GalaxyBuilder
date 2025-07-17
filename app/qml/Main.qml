import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0
import GalaxyCore.Model 1.0
import Galaxy.Factories 1.0
import Galaxy.Exporters 1.0
import "components"

ApplicationWindow {
    id: mainWindow
    
    title: "Galaxy Builder - Stellaris Style Galaxy Generator"
    width: 1200
    height: 800
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    
    color: "#1e1e1e"
    
    // Galaxy manager handles model creation and injection
    GalaxyManager {
        id: galaxyManager
        controller: GalaxyController
    }
    
    // Main layout
    RowLayout {
        anchors.fill: parent
        spacing: 0
        
        // Parameters panel
        ParameterPanel {
            id: parameterPanel
            Layout.preferredWidth: 300
            Layout.minimumWidth: 250
            Layout.fillHeight: true
            controller: GalaxyController
        }
        
        // Splitter
        Rectangle {
            Layout.preferredWidth: 2
            Layout.fillHeight: true
            color: "#404040"
        }
        
        // Galaxy view
        GalaxyView {
            id: galaxyView
            Layout.fillWidth: true
            Layout.fillHeight: true
            controller: GalaxyController
            
            onSystemDoubleClicked: function(systemId) {
                showSystemPropertiesDialog(systemId)
            }
        }
    }
    
    // System Properties Window
    SystemPropertiesDialog {
        id: systemPropertiesWindow
        visible: false
    }
    
    // Function to show system properties dialog
    function showSystemPropertiesDialog(systemId) {
        console.log("System properties requested for system ID:", systemId)
        if (GalaxyController) {
            console.log("Opening system properties for system ID:", systemId)
            
            // Select the system first to ensure controller has the right selection
            GalaxyController.selectSystem(systemId)
            
            // Use the selected system from the controller
            if (GalaxyController.hasSelectedSystem && GalaxyController.selectedStarSystemViewModel) {
                console.log("Updating window with StarSystemViewModel:", GalaxyController.selectedStarSystemViewModel.name)
                systemPropertiesWindow.starSystemViewModel = GalaxyController.selectedStarSystemViewModel
                systemPropertiesWindow.show()
                systemPropertiesWindow.raise()
                systemPropertiesWindow.requestActivate()
            } else {
                console.error("Failed to get StarSystemViewModel from controller for ID:", systemId)
            }
        } else {
            console.error("Controller not available")
        }
    }
    
    // Status bar
    Rectangle {
        id: statusBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30
        color: "#2d2d2d"
        border.color: "#404040"
        border.width: 1
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            
            Text {
                text: GalaxyController ? GalaxyController.statusMessage : "Loading..."
                color: "#ffffff"
                font.pixelSize: 12
                Layout.fillWidth: true
            }
            
            // App confirmation button
            Button {
                id: confirmationButton
                text: "✓ App Running OK"
                onClicked: {
                    console.log("User confirmed: Galaxy Builder application is running successfully!")
                    confirmationButton.text = "✓ Confirmed!"
                    confirmationButton.enabled = false
                }
                
                background: Rectangle {
                    color: confirmationButton.enabled ? "#2d7d32" : "#424242"
                    border.color: "#4caf50"
                    border.width: 1
                    radius: 3
                }
                
                contentItem: Text {
                    text: confirmationButton.text
                    color: "#ffffff"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            
            // Generation progress indicator
            BusyIndicator {
                visible: GalaxyController ? GalaxyController.isGenerating : false
                running: GalaxyController ? GalaxyController.isGenerating : false
                implicitWidth: 16
                implicitHeight: 16
            }
        }
    }
    
    // Menu bar
    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem {
                text: "&Generate New Galaxy"
                onTriggered: GalaxyController.generateGalaxy()
            }
            MenuItem {
                text: "Generate &Random Galaxy"
                onTriggered: GalaxyController.generateRandomGalaxy()
            }
            MenuSeparator {}
            MenuItem {
                text: "&Import/Export Manager..."
                onTriggered: openImportExportManager()
            }
            MenuItem {
                text: "&Export Image..."
                onTriggered: exportImageDialog.open()
            }
            MenuSeparator {}
            MenuItem {
                text: "&Quit"
                onTriggered: Qt.quit()
            }
        }
        
        Menu {
            title: "&View"
            MenuItem {
                text: "Show System &Names"
                checkable: true
                checked: GalaxyController ? GalaxyController.showSystemNames : false
                onTriggered: if (GalaxyController) GalaxyController.showSystemNames = checked
            }
            MenuItem {
                text: "Show &Travel Lanes"
                checkable: true
                checked: GalaxyController ? GalaxyController.showTravelLanes : false
                onTriggered: if (GalaxyController) GalaxyController.showTravelLanes = checked
            }
            MenuItem {
                text: "Show &Influence Radius"
                checkable: true
                checked: GalaxyController ? GalaxyController.showInfluenceRadius : false
                onTriggered: if (GalaxyController) GalaxyController.showInfluenceRadius = checked
            }
        }
        
        Menu {
            title: "&Help"
            MenuItem {
                text: "&About"
                onTriggered: aboutDialog.open()
            }
        }
    }
    
    // Import/Export Manager Component (load on demand)
    Component {
        id: importExportManagerComponent
        ImportExportManager {
            // Component will be created dynamically
        }
    }

    // Export image dialog
    FileDialog {
        id: exportImageDialog
        title: "Export Galaxy Image"
        fileMode: FileDialog.SaveFile
        nameFilters: ["PNG files (*.png)", "JPEG files (*.jpg)", "All files (*)"]
        defaultSuffix: "png"
        onAccepted: {
            var size = Qt.size(1920, 1080) // Default export size
            GalaxyController.exportGalaxyImage(selectedFile, size)
        }
    }
    
    // About dialog
    Dialog {
        id: aboutDialog
        title: "About Galaxy Builder"
        standardButtons: Dialog.Ok
        
        Text {
            text: "Galaxy Builder v1.0.0\n\nA Stellaris-style galaxy generator\nBuilt with Qt6 and QML"
            color: "#ffffff"
            wrapMode: Text.WordWrap
            width: 300
        }
    }
    
    // JavaScript functions
    function openImportExportManager() {
        var component = importExportManagerComponent
        if (component.status === Component.Ready) {
            var window = component.createObject(null)
            if (window) {
                window.show()
                window.raise()
                window.requestActivate()
            }
        }
    }
}
