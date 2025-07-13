#include "SystemDataManager.h"
#include "Planet.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

SystemDataManager::SystemDataManager(QObject* parent)
    : QObject(parent)
    , m_autoSaveEnabled(false)
{
    setupAutoSave();
}

bool SystemDataManager::exportSystemToXml(StarSystemQml* system, const QString& filePath)
{
    if (!system) {
        emit exportError("System is null");
        return false;
    }

    QDomDocument doc;
    QDomElement root = doc.createElement("StarSystem");
    doc.appendChild(root);

    QDomElement systemElement = systemToXml(system, doc);
    root.appendChild(systemElement);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit exportError("Cannot open file for writing: " + filePath);
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString(4);
    file.close();

    emit systemExported(filePath);
    return true;
}

StarSystemQml* SystemDataManager::importSystemFromXml(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit importError("Cannot open file for reading: " + filePath);
        return nullptr;
    }

    QDomDocument doc;
    QString errorMsg;
    int errorLine;
    if (!doc.setContent(&file, &errorMsg, &errorLine)) {
        emit importError(QString("XML parsing error at line %1: %2").arg(errorLine).arg(errorMsg));
        file.close();
        return nullptr;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "StarSystem") {
        emit importError("Invalid XML format: root element must be 'StarSystem'");
        return nullptr;
    }

    QDomElement systemElement = root.firstChildElement("System");
    if (systemElement.isNull()) {
        emit importError("No System element found in XML");
        return nullptr;
    }

    StarSystemQml* system = xmlToSystem(systemElement);
    if (system) {
        emit systemImported(filePath);
    }
    return system;
}

bool SystemDataManager::exportGalaxyToXml(const QList<StarSystemQml*>& systems, const QString& filePath)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("Galaxy");
    doc.appendChild(root);

    // Add metadata
    QDomElement metadata = doc.createElement("Metadata");
    metadata.setAttribute("version", "1.0");
    metadata.setAttribute("systemCount", systems.size());
    metadata.setAttribute("exportDate", QDateTime::currentDateTime().toString(Qt::ISODate));
    root.appendChild(metadata);

    // Add systems
    QDomElement systemsElement = doc.createElement("Systems");
    root.appendChild(systemsElement);

    for (StarSystemQml* system : systems) {
        if (system) {
            QDomElement systemElement = systemToXml(system, doc);
            systemsElement.appendChild(systemElement);
        }
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit exportError("Cannot open file for writing: " + filePath);
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString(4);
    file.close();

    emit systemExported(filePath);
    return true;
}

QList<StarSystemQml*> SystemDataManager::importGalaxyFromXml(const QString& filePath)
{
    QList<StarSystemQml*> systems;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit importError("Cannot open file for reading: " + filePath);
        return systems;
    }

    QDomDocument doc;
    QString errorMsg;
    int errorLine;
    if (!doc.setContent(&file, &errorMsg, &errorLine)) {
        emit importError(QString("XML parsing error at line %1: %2").arg(errorLine).arg(errorMsg));
        file.close();
        return systems;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "Galaxy") {
        emit importError("Invalid XML format: root element must be 'Galaxy'");
        return systems;
    }

    QDomElement systemsElement = root.firstChildElement("Systems");
    if (systemsElement.isNull()) {
        emit importError("No Systems element found in XML");
        return systems;
    }

    QDomNodeList systemNodes = systemsElement.elementsByTagName("System");
    for (int i = 0; i < systemNodes.count(); ++i) {
        QDomElement systemElement = systemNodes.at(i).toElement();
        StarSystemQml* system = xmlToSystem(systemElement);
        if (system) {
            systems.append(system);
        }
    }

    emit systemImported(filePath);
    return systems;
}

bool SystemDataManager::saveSystemChanges(StarSystemQml* system)
{
    if (!system) return false;
    
    QString dataPath = getSystemDataPath(system->getSystemId());
    QDir().mkpath(QFileInfo(dataPath).absolutePath());
    
    return exportSystemToXml(system, dataPath);
}

bool SystemDataManager::loadSystemData(StarSystemQml* system)
{
    if (!system) return false;
    
    QString dataPath = getSystemDataPath(system->getSystemId());
    if (!QFile::exists(dataPath)) {
        return false; // No saved data exists
    }
    
    StarSystemQml* loadedSystem = importSystemFromXml(dataPath);
    if (!loadedSystem) return false;
    
    // Copy data to existing system
    system->setName(loadedSystem->getName());
    system->setStarType(loadedSystem->getStarType());
    system->setSystemSize(loadedSystem->getSystemSize());
    system->setStarMass(loadedSystem->getStarMass());
    system->setStarTemperature(loadedSystem->getStarTemperature());
    system->setStarLuminosity(loadedSystem->getStarLuminosity());
    
    // Copy planets
    system->clearPlanets();
    for (Planet* planet : loadedSystem->getPlanets()) {
        Planet* newPlanet = system->addPlanet(planet->name(), planet->orbitalRadius());
        if (newPlanet) {
            newPlanet->setType(planet->type());
            newPlanet->setSize(planet->size());
            newPlanet->setMass(planet->mass());
            newPlanet->setGForce(planet->gForce());
            newPlanet->setNumberOfMoons(planet->numberOfMoons());
            newPlanet->setMinTemperature(planet->minTemperature());
            newPlanet->setMaxTemperature(planet->maxTemperature());
        }
    }
    
    loadedSystem->deleteLater();
    return true;
}

void SystemDataManager::enableAutoSave(bool enabled)
{
    if (m_autoSaveEnabled != enabled) {
        m_autoSaveEnabled = enabled;
    }
}

void SystemDataManager::onSystemDataChanged()
{
    if (!m_autoSaveEnabled) return;
    
    StarSystemQml* system = qobject_cast<StarSystemQml*>(sender());
    if (system) {
        saveSystemChanges(system);
        emit autoSaveTriggered(system->getSystemId());
    }
}

QDomElement SystemDataManager::systemToXml(StarSystemQml* system, QDomDocument& doc)
{
    QDomElement systemElement = doc.createElement("System");
    systemElement.setAttribute("id", system->getSystemId());
    systemElement.setAttribute("name", system->getName());
    systemElement.setAttribute("positionX", system->getPosition().x());
    systemElement.setAttribute("positionY", system->getPosition().y());
    systemElement.setAttribute("starType", static_cast<int>(system->getStarType()));
    systemElement.setAttribute("systemSize", static_cast<int>(system->getSystemSize()));
    systemElement.setAttribute("starMass", system->getStarMass());
    systemElement.setAttribute("starTemperature", system->getStarTemperature());
    systemElement.setAttribute("starLuminosity", system->getStarLuminosity());

    // Add planets
    QDomElement planetsElement = doc.createElement("Planets");
    systemElement.appendChild(planetsElement);
    
    for (Planet* planet : system->getPlanets()) {
        QDomElement planetElement = planetToXml(planet, doc);
        planetsElement.appendChild(planetElement);
    }

    return systemElement;
}

QDomElement SystemDataManager::planetToXml(Planet* planet, QDomDocument& doc)
{
    QDomElement planetElement = doc.createElement("Planet");
    planetElement.setAttribute("name", planet->name());
    planetElement.setAttribute("type", static_cast<int>(planet->type()));
    planetElement.setAttribute("size", planet->size());
    planetElement.setAttribute("mass", planet->mass());
    planetElement.setAttribute("gravity", planet->gForce());
    planetElement.setAttribute("moonCount", planet->numberOfMoons());
    planetElement.setAttribute("minTemperature", planet->minTemperature());
    planetElement.setAttribute("maxTemperature", planet->maxTemperature());
    planetElement.setAttribute("orbitDistance", planet->orbitalRadius());
    
    return planetElement;
}

StarSystemQml* SystemDataManager::xmlToSystem(const QDomElement& element)
{
    bool ok;
    SystemId id = element.attribute("id").toInt(&ok);
    if (!ok) return nullptr;
    
    QPointF position(
        element.attribute("positionX").toDouble(),
        element.attribute("positionY").toDouble()
    );
    
    StarType starType = static_cast<StarType>(element.attribute("starType").toInt());
    
    StarSystemQml* system = new StarSystemQml(id, position, starType, this);
    system->setName(element.attribute("name"));
    system->setSystemSize(static_cast<SystemSize>(element.attribute("systemSize").toInt()));
    system->setStarMass(element.attribute("starMass").toDouble());
    system->setStarTemperature(element.attribute("starTemperature").toDouble());
    system->setStarLuminosity(element.attribute("starLuminosity").toDouble());
    
    // Load planets
    QDomElement planetsElement = element.firstChildElement("Planets");
    if (!planetsElement.isNull()) {
        QDomNodeList planetNodes = planetsElement.elementsByTagName("Planet");
        for (int i = 0; i < planetNodes.count(); ++i) {
            QDomElement planetElement = planetNodes.at(i).toElement();
            Planet* planet = xmlToPlanet(planetElement);
            if (planet) {
                planet->setParent(system);
                // The planet will be added to the system's planet list through the Planet constructor
            }
        }
    }
    
    return system;
}

Planet* SystemDataManager::xmlToPlanet(const QDomElement& element)
{
    QString name = element.attribute("name");
    PlanetType type = static_cast<PlanetType>(element.attribute("type").toInt());
    
    Planet* planet = new Planet(name, type);
    planet->setSize(element.attribute("size").toDouble());
    planet->setMass(element.attribute("mass").toDouble());
    planet->setGForce(element.attribute("gravity").toDouble());
    planet->setNumberOfMoons(element.attribute("moonCount").toInt());
    planet->setMinTemperature(element.attribute("minTemperature").toDouble());
    planet->setMaxTemperature(element.attribute("maxTemperature").toDouble());
    planet->setOrbitalRadius(element.attribute("orbitDistance").toDouble());
    
    return planet;
}

QString SystemDataManager::getSystemDataPath(int systemId) const
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(dataDir).filePath(QString("systems/system_%1.xml").arg(systemId));
}

void SystemDataManager::setupAutoSave()
{
    // Auto-save will be connected when systems are being monitored
}
