import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import GalaxyBuilderApp 1.0
import GalaxyCore.ViewModels 1.0
import GalaxyCore.Model 1.0

ApplicationWindow {
    id: window
    
    property var starSystemViewModel: null
    
    title: "System Properties"
    width: 800
    height: 600
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        
        Text {
            text: "System Properties Dialog"
            Layout.alignment: Qt.AlignHCenter
        }
        
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            
            Button {
                text: "Close"
                onClicked: window.close()
            }
        }
    }
}
