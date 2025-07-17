#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include <QQuickStyle>
#include <iostream>

// Import the GalaxyController
#include "GalaxyController.h"

#ifdef _WIN32
// #include <io.h>
// #include <fcntl.h>
// #include <windows.h>
#endif

int main(int argc, char *argv[])
{

    // Enable QML debugging
    QLoggingCategory::setFilterRules("qt.qml.debug=true;qt.qml*=true;qt.qml.import*=true");
    qputenv("QML_IMPORT_TRACE", "1");
    
    // Set Qt Quick style to Basic (supports customization)
    QQuickStyle::setStyle("Basic");
    
    QGuiApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Galaxy Builder");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Galaxy Builder Team");
    app.setApplicationDisplayName("Galaxy Builder - Stellaris Style Galaxy Generator");
    
    std::cout << "Starting Galaxy Builder application..." << std::endl;
    qDebug() << "Starting Galaxy Builder application...";
    
    // Create QML engine
    QQmlApplicationEngine engine;
    
    // Register GalaxyController as singleton
    qmlRegisterSingletonType<GalaxyController>("GalaxyBuilderApp", 1, 0, "GalaxyController", 
                                               &GalaxyController::create);
    
    std::cout << "Registered GalaxyController singleton" << std::endl;
    qDebug() << "Registered GalaxyController singleton";
    
    // Add QML import paths
    engine.addImportPath("qrc:/qml");
    engine.addImportPath(":/qml");
    // Add runtime QML module paths for Galaxy modules
    QString buildPath = QCoreApplication::applicationDirPath() + "/qml";
    engine.addImportPath(buildPath);
    std::cout << "Added QML import paths including: " << buildPath.toStdString() << std::endl;
    qDebug() << "Added QML import paths including:" << buildPath;
    
    // Print all import paths for debugging
    QStringList importPaths = engine.importPathList();
    std::cout << "All QML import paths:" << std::endl;
    for (const QString& path : importPaths) {
        std::cout << "  - " << path.toStdString() << std::endl;
    }
    
    // Load main QML file - Qt6 automatic module system
    const QUrl url(QStringLiteral("qrc:/qt/qml/GalaxyBuilderApp/qml/Main.qml"));
    std::cout << "Loading QML from: " << url.toString().toStdString() << std::endl;
    qDebug() << "Loading QML from:" << url.toString();

    // Load main QML file with error handling
    try {
        engine.load(url);
        
        if (engine.rootObjects().isEmpty()) {
            std::cerr << "CRITICAL ERROR: No root objects created, QML loading failed!" << std::endl;
            qCritical() << "CRITICAL ERROR: No root objects created, QML loading failed!";
            return -1;
        }
        
        std::cout << "QML engine loaded successfully, starting event loop..." << std::endl;
        qDebug() << "QML engine loaded successfully, starting event loop...";
        
    } catch (const std::exception& e) {
        std::cerr << "EXCEPTION during QML loading: " << e.what() << std::endl;
        qCritical() << "EXCEPTION during QML loading:" << e.what();
        return -1;
    } catch (...) {
        std::cerr << "UNKNOWN EXCEPTION during QML loading!" << std::endl;
        qCritical() << "UNKNOWN EXCEPTION during QML loading!";
        return -1;
    }
    return app.exec();
}
