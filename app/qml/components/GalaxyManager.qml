import QtQuick 2.15
import GalaxyCore 1.0

Item {
    id: root
    
    property GalaxyController controller
    property alias systemsModel: systemsModel
    property alias lanesModel: lanesModel
    
    // Create the models that will be injected into the Galaxy
    StarSystemListModel {
        id: systemsModel
    }
    
    TravelLaneListModel {
        id: lanesModel
    }
    
    // Connect to galaxy changes and inject models
    Connections {
        target: controller
        function onGalaxyChanged() {
            if (controller.galaxy) {
                controller.galaxy.systemsModel = systemsModel
                controller.galaxy.lanesModel = lanesModel
            }
        }
    }
    
    Component.onCompleted: {
        // Inject models if galaxy already exists
        if (controller && controller.galaxy) {
            controller.galaxy.systemsModel = systemsModel
            controller.galaxy.lanesModel = lanesModel
        }
    }
}
