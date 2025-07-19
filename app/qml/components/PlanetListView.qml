import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GalaxyCore.ViewModels

Item {
    id: root
    
    property alias model: planetListView.model // ViewModel for the current star system
    property alias currentIndex: planetListView.currentIndex // Current selected planet index
    property alias currentItem: planetListView.currentItem // Current selected planet item
    property alias contentHeight: planetListView.contentHeight // Total height of the list content
    property alias count: planetListView.count // Number of planets in the list
    property alias clip: planetListView.clip // Whether to clip the list view
    
    // Delegate for each planet in the list
    ListView {
        id: planetListView

        anchors.fill: parent
        clip: true
        
        delegate: Rectangle {
            width: planetListView.width
            height: 60
            color: ListView.isCurrentItem ? "#404040" : "#2a2a2a"
            border.color: "#606060"
            radius: 3
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                Rectangle {
                    width: 20
                    height: 20
                    radius: 10
                    color: model.planetTypeColor || "#808080"
                    border.color: "#ffffff"
                    border.width: 1
                }
                
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    
                    Text {
                        text: model.name
                        color: "#ffffff"
                        font.bold: true
                    }
                    
                    Text {
                        text: "Type: " + model.planetTypeName + ", Moons: " + model.numberOfMoons
                        color: "#cccccc"
                        font.pixelSize: 12
                    }
                }
                
                Text {
                    text: model.mass.toFixed(2) + " ME"
                    color: "#cccccc"
                    font.pixelSize: 12
                }

                Button {
                    text: "Remove"
                    Layout.alignment: Qt.AlignVCenter
                    
                    onClicked: {
                        console.debug("Removing planet at index:", index)
                        model.remove(index)
                    }
                }

                
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: planetListView.currentIndex = index
                propagateComposedEvents: true
                onDoubleClicked: {
                    // Set the current index and use the same logic as the Edit button
                    planetDetailsDialog.planetViewModel = model.viewModel
                    planetDetailsDialog.open()
                }
            }
        }
    }

    PlanetDetailsDialog {
        id: planetDetailsDialog
        visible: false
        modal: true
        popupType: Popup.Native

        onPlanetUpdated: {
            root.model.refresh() // Refresh the model after updating planet details
        }
    }
}