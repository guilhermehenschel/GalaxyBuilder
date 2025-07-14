#include "SystemDataManager.h"
#include "Planet.h"
#include "Common.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPointF>

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

    // Validate system data before export
    if (!validateSystemData(system)) {
        QString errors = getValidationErrors(system);
        emit exportError("Validation failed: " + errors);
        return false;
    }

    // Create XML document on-demand only when export is actually requested
    QDomDocument doc;
    if (!createXmlDocumentForSystem(system, doc)) {
        emit exportError("Failed to create XML document");
        return false;
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
    QDomDocument::ParseResult parseResult = doc.setContent(&file, QDomDocument::ParseOption::UseNamespaceProcessing, &errorMsg, &errorLine);
    if (!parseResult) {
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
    QDomDocument::ParseResult parseResult = doc.setContent(&file, QDomDocument::ParseOption::UseNamespaceProcessing, &errorMsg, &errorLine);
    if (!parseResult) {
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
    
    // Use memory-based persistence (JSON) for frequent saves to avoid XML overhead
    QJsonObject systemJson = systemToJson(system);
    if (systemJson.isEmpty()) {
        return false;
    }
    
    // Cache in memory for fast access
    m_systemCache[system->getSystemId()] = systemJson;
    
    // Optionally persist to disk for long-term storage
    QString dataPath = getSystemDataPath(system->getSystemId());
    QDir().mkpath(QFileInfo(dataPath).absolutePath());
    
    QFile file(dataPath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(systemJson);
        file.write(doc.toJson());
        file.close();
        return true;
    }
    
    return false;
}

bool SystemDataManager::loadSystemData(StarSystemQml* system)
{
    if (!system) return false;
    
    // First try to load from memory cache
    int systemId = system->getSystemId();
    if (m_systemCache.contains(systemId)) {
        return systemFromJson(system, m_systemCache[systemId]);
    }
    
    // Fall back to disk storage
    QString dataPath = getSystemDataPath(systemId);
    if (!QFile::exists(dataPath)) {
        return false; // No saved data exists
    }
    
    QFile file(dataPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
    if (doc.isObject()) {
        QJsonObject systemJson = doc.object();
        m_systemCache[systemId] = systemJson; // Cache for future access
        return systemFromJson(system, systemJson);
    }
    
    return false;
}

bool SystemDataManager::validateSystemData(StarSystemQml* system) const
{
    if (!system) return false;
    
    // Basic validation
    if (system->getName().isEmpty()) return false;
    if (system->getStarMass() <= 0) return false;
    if (system->getStarTemperature() <= 0) return false;
    if (system->getStarLuminosity() <= 0) return false;
    
    // Validate planets
    auto* planetsModel = system->getPlanetsModel();
    if (planetsModel) {
        const auto& planets = planetsModel->getPlanets();
        for (const auto& planet : planets) {
            if (planet->name().isEmpty()) return false;
            if (planet->orbitalRadius() <= 0) return false;
        }
    }
    
    return true;
}

QString SystemDataManager::getValidationErrors(StarSystemQml* system) const
{
    QStringList errors;
    
    if (!system) {
        errors << "System is null";
        return errors.join("; ");
    }
    
    if (system->getName().isEmpty()) {
        errors << "System name is empty";
    }
    if (system->getStarMass() <= 0) {
        errors << "Star mass must be positive";
    }
    if (system->getStarTemperature() <= 0) {
        errors << "Star temperature must be positive";
    }
    if (system->getStarLuminosity() <= 0) {
        errors << "Star luminosity must be positive";
    }
    
    // Validate planets
    auto* planetsModel = system->getPlanetsModel();
    if (planetsModel) {
        const auto& planets = planetsModel->getPlanets();
        for (int i = 0; i < planets.size(); ++i) {
            const auto& planet = planets[i];
            if (planet->name().isEmpty()) {
                errors << QString("Planet %1 name is empty").arg(i + 1);
            }
            if (planet->orbitalRadius() <= 0) {
                errors << QString("Planet %1 orbital radius must be positive").arg(i + 1);
            }
        }
    }
    
    return errors.join("; ");
}

QString SystemDataManager::generateXmlPreview(StarSystemQml* system) const
{
    if (!system) return QString();
    
    QDomDocument doc;
    if (createXmlDocumentForSystem(system, doc)) {
        return doc.toString(2);
    }
    
    return QString();
}

QString SystemDataManager::generateGalaxyXmlPreview(const QList<StarSystemQml*>& systems) const
{
    QDomDocument doc;
    if (createXmlDocumentForGalaxy(systems, doc)) {
        return doc.toString(2);
    }
    
    return QString();
}

// Memory-based JSON serialization methods
QJsonObject SystemDataManager::systemToJson(StarSystemQml* system) const
{
    if (!system) return QJsonObject();
    
    QJsonObject json;
    json["id"] = system->getSystemId();
    json["name"] = system->getName();
    json["positionX"] = system->getPosition().x();
    json["positionY"] = system->getPosition().y();
    json["starType"] = static_cast<int>(system->getStarType());
    json["systemSize"] = static_cast<int>(system->getSystemSize());
    json["starMass"] = system->getStarMass();
    json["starTemperature"] = system->getStarTemperature();
    json["starLuminosity"] = system->getStarLuminosity();
    json["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    // Add planets
    QJsonArray planetsArray;
    auto* planetsModel = system->getPlanetsModel();
    if (planetsModel) {
        for (int i = 0; i < planetsModel->getPlanetCount(); ++i) {
            Planet* planet = planetsModel->getPlanetAt(i);
            if (planet) {
                QJsonObject planetJson;
                planetJson["name"] = planet->name();
                planetJson["type"] = static_cast<int>(planet->type());
                planetJson["size"] = planet->size();
                planetJson["mass"] = planet->mass();
                planetJson["gravity"] = planet->gForce();
                planetJson["moonCount"] = planet->numberOfMoons();
                planetJson["minTemperature"] = planet->minTemperature();
                planetJson["maxTemperature"] = planet->maxTemperature();
                planetJson["orbitDistance"] = planet->orbitalRadius();
                planetsArray.append(planetJson);
            }
        }
    }
    json["planets"] = planetsArray;
    
    return json;
}

bool SystemDataManager::systemFromJson(StarSystemQml* system, const QJsonObject& json) const
{
    if (!system || json.isEmpty()) return false;
    
    // Load basic properties
    if (json.contains("name")) {
        system->setName(json["name"].toString());
    }
    if (json.contains("starType")) {
        system->setStarType(static_cast<StarType>(json["starType"].toInt()));
    }
    if (json.contains("systemSize")) {
        system->setSystemSize(static_cast<SystemSize>(json["systemSize"].toInt()));
    }
    if (json.contains("starMass")) {
        system->setStarMass(json["starMass"].toDouble());
    }
    if (json.contains("starTemperature")) {
        system->setStarTemperature(json["starTemperature"].toDouble());
    }
    if (json.contains("starLuminosity")) {
        system->setStarLuminosity(json["starLuminosity"].toDouble());
    }
    
    // Load planets
    if (json.contains("planets")) {
        QJsonArray planetsArray = json["planets"].toArray();
        
        // Clear existing planets
        system->clearPlanets();
        
        // Add planets from JSON
        for (const auto& planetValue : planetsArray) {
            QJsonObject planetJson = planetValue.toObject();
            QString name = planetJson["name"].toString();
            double orbitDistance = planetJson["orbitDistance"].toDouble();
            
            Planet* planet = system->addPlanet(name, orbitDistance);
            if (planet) {
                planet->setType(static_cast<PlanetType>(planetJson["type"].toInt()));
                planet->setSize(planetJson["size"].toDouble());
                planet->setMass(planetJson["mass"].toDouble());
                planet->setGForce(planetJson["gravity"].toDouble());
                planet->setNumberOfMoons(planetJson["moonCount"].toInt());
                planet->setMinTemperature(planetJson["minTemperature"].toDouble());
                planet->setMaxTemperature(planetJson["maxTemperature"].toDouble());
            }
        }
    }
    
    return true;
}

bool SystemDataManager::createXmlDocumentForSystem(StarSystemQml* system, QDomDocument& doc) const
{
    if (!system) return false;
    
    QDomElement root = doc.createElement("StarSystem");
    doc.appendChild(root);

    QDomElement systemElement = systemToXml(const_cast<StarSystemQml*>(system), const_cast<QDomDocument&>(doc));
    root.appendChild(systemElement);
    
    return true;
}

bool SystemDataManager::createXmlDocumentForGalaxy(const QList<StarSystemQml*>& systems, QDomDocument& doc) const
{
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
            QDomElement systemElement = systemToXml(system, const_cast<QDomDocument&>(doc));
            systemsElement.appendChild(systemElement);
        }
    }
    
    return true;
}

bool SystemDataManager::exportSystemToJson(StarSystemQml* system, const QString& filePath)
{
    if (!system) {
        emit exportError("System is null");
        return false;
    }

    // Validate system data before export
    if (!validateSystemData(system)) {
        QString errors = getValidationErrors(system);
        emit exportError("Validation failed: " + errors);
        return false;
    }

    // Create JSON object on-demand only when export is actually requested
    QJsonObject systemJson = systemToJson(system);
    if (systemJson.isEmpty()) {
        emit exportError("Failed to create JSON object");
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit exportError("Cannot open file for writing: " + filePath);
        return false;
    }

    QJsonDocument doc(systemJson);
    file.write(doc.toJson());
    file.close();

    emit systemExported(filePath);
    return true;
}

StarSystemQml* SystemDataManager::importSystemFromJson(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit importError("Cannot open file for reading: " + filePath);
        return nullptr;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        emit importError("Invalid JSON format: root element must be an object");
        return nullptr;
    }

    QJsonObject root = doc.object();
    StarSystemQml* system = new StarSystemQml(this);
    if (systemFromJson(system, root)) {
        emit systemImported(filePath);
        return system;
    }

    delete system;
    return nullptr;
}

bool SystemDataManager::exportGalaxyToJson(const QList<StarSystemQml*>& systems, const QString& filePath)
{
    QJsonArray systemsArray;
    for (StarSystemQml* system : systems) {
        if (system) {
            QJsonObject systemJson = systemToJson(system);
            systemsArray.append(systemJson);
        }
    }

    QJsonObject galaxyJson;
    galaxyJson["systems"] = systemsArray;
    galaxyJson["exportDate"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit exportError("Cannot open file for writing: " + filePath);
        return false;
    }

    QJsonDocument doc(galaxyJson);
    file.write(doc.toJson());
    file.close();

    emit systemExported(filePath);
    return true;
}

QList<StarSystemQml*> SystemDataManager::importGalaxyFromJson(const QString& filePath)
{
    QList<StarSystemQml*> systems;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit importError("Cannot open file for reading: " + filePath);
        return systems;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        emit importError("Invalid JSON format: root element must be an object");
        return systems;
    }

    QJsonObject root = doc.object();
    if (root.contains("systems") && root["systems"].isArray()) {
        QJsonArray systemsArray = root["systems"].toArray();
        for (const auto& value : systemsArray) {
            if (value.isObject()) {
                StarSystemQml* system = new StarSystemQml(this);
                if (systemFromJson(system, value.toObject())) {
                    systems.append(system);
                } else {
                    delete system;
                }
            }
        }
    }

    emit systemImported(filePath);
    return systems;
}

// Helper methods for auto-save and system data paths
void SystemDataManager::setupAutoSave()
{
    // Set up auto-save timer if needed
    if (m_autoSaveEnabled) {
        // Auto-save implementation
    }
}

QString SystemDataManager::getSystemDataPath(int systemId) const
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QString("%1/systems/system_%2.json").arg(dataDir).arg(systemId);
}

void SystemDataManager::onSystemDataChanged()
{
    if (m_autoSaveEnabled) {
        emit autoSaveTriggered(0); // Trigger auto-save
    }
}

void SystemDataManager::enableAutoSave(bool enabled)
{
    if (m_autoSaveEnabled != enabled) {
        m_autoSaveEnabled = enabled;
        if (enabled) {
            setupAutoSave();
        }
    }
}

// XML serialization implementations
QDomElement SystemDataManager::systemToXml(StarSystemQml* system, QDomDocument& doc) const
{
    QDomElement systemElement = doc.createElement("System");
    
    // Set system attributes
    systemElement.setAttribute("id", system->getSystemId());
    systemElement.setAttribute("name", system->getName());
    systemElement.setAttribute("positionX", system->getPosition().x());
    systemElement.setAttribute("positionY", system->getPosition().y());
    systemElement.setAttribute("starType", static_cast<int>(system->getStarType()));
    systemElement.setAttribute("systemSize", static_cast<int>(system->getSystemSize()));
    systemElement.setAttribute("starMass", system->getStarMass());
    systemElement.setAttribute("starTemperature", system->getStarTemperature());
    systemElement.setAttribute("starLuminosity", system->getStarLuminosity());
    
    // Add planets - Always create planets element for consistency
    auto* planetsModel = system->getPlanetsModel();
    QDomElement planetsElement = doc.createElement("Planets");
    systemElement.appendChild(planetsElement);
    
    if (planetsModel && planetsModel->getPlanetCount() > 0) {
        for (int i = 0; i < planetsModel->getPlanetCount(); ++i) {
            Planet* planet = planetsModel->getPlanetAt(i);
            if (planet) {
                QDomElement planetElement = planetToXml(planet, doc);
                planetsElement.appendChild(planetElement);
            }
        }
    }
    
    return systemElement;
}

QDomElement SystemDataManager::planetToXml(Planet* planet, QDomDocument& doc) const
{
    QDomElement planetElement = doc.createElement("Planet");
    
    planetElement.setAttribute("name", planet->name());
    planetElement.setAttribute("type", static_cast<int>(planet->type()));
    planetElement.setAttribute("size", QString::number(planet->size(), 'f', 1));
    planetElement.setAttribute("mass", QString::number(planet->mass(), 'f', 1));
    planetElement.setAttribute("gravity", QString::number(planet->gForce(), 'f', 2));
    planetElement.setAttribute("moonCount", planet->numberOfMoons());
    planetElement.setAttribute("minTemperature", planet->minTemperature());
    planetElement.setAttribute("maxTemperature", planet->maxTemperature());
    planetElement.setAttribute("orbitDistance", planet->orbitalRadius());
    
    return planetElement;
}

StarSystemQml* SystemDataManager::xmlToSystem(const QDomElement& element)
{
    // Create system with ID from constructor
    int systemId = element.attribute("id").toInt();
    QPointF position(element.attribute("positionX").toDouble(),
                     element.attribute("positionY").toDouble());
    StarType starType = static_cast<StarType>(element.attribute("starType").toInt());
    
    StarSystemQml* system = new StarSystemQml(static_cast<SystemId>(systemId), position, starType, this);
    
    // Set other properties
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
                // Set planet properties and add it using the addPlanet method
                QString name = planet->name();
                double orbitDistance = planet->orbitalRadius();
                
                Planet* addedPlanet = system->addPlanet(name, orbitDistance);
                if (addedPlanet) {
                    // Copy all properties from the temporary planet to the added planet
                    addedPlanet->setType(planet->type());
                    addedPlanet->setSize(planet->size());
                    addedPlanet->setMass(planet->mass());
                    addedPlanet->setGForce(planet->gForce());
                    addedPlanet->setNumberOfMoons(planet->numberOfMoons());
                    addedPlanet->setMinTemperature(planet->minTemperature());
                    addedPlanet->setMaxTemperature(planet->maxTemperature());
                }
                
                // Clean up temporary planet
                delete planet;
            }
        }
    }
    
    return system;
}

Planet* SystemDataManager::xmlToPlanet(const QDomElement& element)
{
    Planet* planet = new Planet(this);
    planet->setName(element.attribute("name"));
    planet->setOrbitalRadius(element.attribute("orbitDistance").toDouble());
    planet->setType(static_cast<PlanetType>(element.attribute("type").toInt()));
    planet->setSize(element.attribute("size").toDouble());
    planet->setMass(element.attribute("mass").toDouble());
    planet->setGForce(element.attribute("gravity").toDouble());
    planet->setNumberOfMoons(element.attribute("moonCount").toInt());
    planet->setMinTemperature(element.attribute("minTemperature").toDouble());
    planet->setMaxTemperature(element.attribute("maxTemperature").toDouble());
    
    return planet;
}
