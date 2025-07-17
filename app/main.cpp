#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDir>
#include <QDebug>
#include <QLoggingCategory>
#include <QQuickStyle>
#include <iostream>

#ifdef _WIN32
// #include <io.h>
// #include <fcntl.h>
// #include <windows.h>
#endif

int main(int argc, char *argv[])
{

    // Enable QML debugging
    QLoggingCategory::setFilterRules("qt.qml.debug=true;qt.qml*=true");
    
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
    std::cout << "Set galaxyController context property" << std::endl;
    qDebug() << "Set galaxyController context property";
    
    // Add QML import paths
    engine.addImportPath("qrc:/qml");
    engine.addImportPath(":/qml");
    std::cout << "Added QML import paths" << std::endl;
    qDebug() << "Added QML import paths";
    
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
