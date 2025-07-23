import QtQuick
import QtQuick.Controls
import GalaxyBuilderApp 1.0

Item {
    id: root

    property GalaxyController controller
    property SystemPropertiesDialog currentDialog: null

    // Watch for selected system changes
    Connections {
        target: controller
        function onSelectedSystemChanged() {
            if (controller && controller.hasSelectedSystem && controller.selectedStarSystem) {
                openPropertiesDialog(controller.selectedStarSystem);
            }
        }
    }

    function openPropertiesDialog(starSystem) {
        // Close existing dialog if any
        if (currentDialog) {
            currentDialog.close();
            currentDialog.destroy();
        }

        // Create new dialog
        var component = Qt.createComponent("SystemPropertiesDialog.qml");
        if (component.status === Component.Ready) {
            currentDialog = component.createObject(root, {
                "starSystem": starSystem
            });
            if (currentDialog) {
                currentDialog.show();

                // Handle dialog closing
                currentDialog.closing.connect(function () {
                    currentDialog = null;
                });
            }
        } else {
            console.error("Failed to create SystemPropertiesDialog:", component.errorString());
        }
    }
}
