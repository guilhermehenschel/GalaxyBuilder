import QtQuick
import GalaxyBuilderApp 1.0

Item {
    id: root
    
    property GalaxyController controller
    
    // Provide access to the models via the controller's galaxy view model
    readonly property var systemsModel: controller ? controller.galaxyViewModel : null
    readonly property var lanesModel: controller ? controller.galaxyViewModel : null
}
