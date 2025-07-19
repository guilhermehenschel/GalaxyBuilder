#include "ggh/modules/GalaxyFactories/XmlGalaxyImporter.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <unordered_map>
#include <limits>

#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"
#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"

using SystemId = ggh::GalaxyCore::utilities::SystemId;
using StarType = ggh::GalaxyCore::utilities::StarType;
using PlanetType = ggh::GalaxyCore::utilities::PlanetType;
using LaneId = ggh::GalaxyCore::utilities::LaneId;
using GalaxyModel = ggh::GalaxyCore::models::GalaxyModel;
using StarSystemModel = ggh::GalaxyCore::models::StarSystemModel;
using Planet = ggh::GalaxyCore::models::Planet;
using TravelLaneModel = ggh::GalaxyCore::models::TravelLaneModel;


namespace ggh::GalaxyFactories {
std::unique_ptr<GalaxyModel> XmlGalaxyImporter::importGalaxy(const QString& filePath) {
    setXmlPath(filePath);
    return generateGalaxy();
}

void XmlGalaxyImporter::setXmlPath(const QString& filePath) {
    m_filePath = filePath;
}

std::unique_ptr<GalaxyModel> XmlGalaxyImporter::generateGalaxy() {
    if (m_filePath.isEmpty()) {
        qWarning() << "XML file path not set";
        return nullptr;
    }

    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading:" << m_filePath;
        return nullptr;
    }

    // Create galaxy with default dimensions, we'll update them later
    auto galaxy = std::make_unique<GalaxyModel>(1000, 1000);
    std::unordered_map<quint32, std::shared_ptr<StarSystemModel>> systemMap;

    QXmlStreamReader xml(&file);
    
    // First find the root element and validate it
    bool foundGalaxyRoot = false;
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "Galaxy") {
                foundGalaxyRoot = true;
                break;
            } else {
                qWarning() << "Invalid XML: Root element must be 'Galaxy', found:" << xml.name();
                return nullptr;
            }
        }
    }
    
    if (!foundGalaxyRoot) {
        qWarning() << "Invalid XML: No 'Galaxy' root element found";
        return nullptr;
    }
    
    // Parse XML using streaming parser for better performance
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        
        if (xml.isStartElement()) {
            if (xml.name() == "StarSystem") {
                if (!parseSystemFromStream(galaxy.get(), systemMap, xml)) {
                    qWarning() << "Failed to parse star system";
                    return nullptr;
                }
            }
            else if (xml.name() == "TravelLane") {
                if (!parseTravelLaneFromStream(galaxy.get(), systemMap, xml)) {
                    qWarning() << "Failed to parse travel lane";
                    return nullptr;
                }
            }
        }
    }
    
    if (xml.hasError()) {
        qWarning() << "XML parsing error:" << xml.errorString();
        return nullptr;
    }
    
    file.close();
    
    // Set galaxy dimensions based on the systems (find bounding box and add some padding)
    updateGalaxyDimensions(galaxy.get(), systemMap);

    return galaxy;
}

void XmlGalaxyImporter::setParameters(const GenerationParameters& /*params*/) {
    // Not used in XML importer, parameters are set via file path
}

GenerationParameters XmlGalaxyImporter::getParameters() const {
    GenerationParameters params;
    // Populate params based on XML file or other criteria if needed
    return params;
}

// DOM-based methods removed - using streaming parser implementation instead

bool XmlGalaxyImporter::parseSystemFromStream(GalaxyModel* galaxy, 
                                            std::unordered_map<quint32, std::shared_ptr<StarSystemModel>>& systemMap,
                                            QXmlStreamReader& xml) {
    QXmlStreamAttributes attributes = xml.attributes();
    
    quint32 id = attributes.value("id").toUInt();
    QString name = attributes.value("name").toString();
    double positionX = attributes.value("positionX").toDouble();
    double positionY = attributes.value("positionY").toDouble();
    int starType = attributes.value("starType").toInt();
    int systemSize = attributes.value("systemSize").toInt();
    
    if (id == 0 || name.isEmpty()) {
        qWarning() << "Invalid star system data: id=" << id << "name=" << name;
        return false;
    }
    
    // Create position coordinates
    ggh::GalaxyCore::utilities::CartesianCoordinates<double> position(positionX, positionY);
    
    auto starSystem = std::make_shared<StarSystemModel>(
        id, name.toStdString(), position, 
        static_cast<StarType>(starType)
    );
    
    // Set system size separately
    starSystem->setSystemSize(static_cast<ggh::GalaxyCore::utilities::SystemSize>(systemSize));
    
    // Parse any nested Planet elements
    while (xml.readNextStartElement()) {
        if (xml.name() == "Planet") {
            if (!parsePlanetFromStream(starSystem, xml)) {
                qWarning() << "Failed to parse planet in system" << id;
                return false;
            }
        } else {
            xml.skipCurrentElement();
        }
    }
    
    systemMap[id] = starSystem;
    galaxy->addStarSystem(std::move(starSystem));
    return true;
}

bool XmlGalaxyImporter::parseTravelLaneFromStream(GalaxyModel* galaxy,
                                                std::unordered_map<quint32, std::shared_ptr<StarSystemModel>>& systemMap,
                                                QXmlStreamReader& xml) {
    QXmlStreamAttributes attributes = xml.attributes();
    
    quint32 id = attributes.value("id").toUInt();
    quint32 fromSystemId = attributes.value("fromSystem").toUInt();
    quint32 toSystemId = attributes.value("toSystem").toUInt();
    // Note: length is calculated automatically by TravelLaneModel
    
    if (id == 0 || fromSystemId == 0 || toSystemId == 0) {
        qWarning() << "Invalid travel lane data: id=" << id << "fromSystem=" << fromSystemId << "toSystem=" << toSystemId;
        return false;
    }
    
    // Find the from and to systems
    auto fromSystemIt = systemMap.find(fromSystemId);
    auto toSystemIt = systemMap.find(toSystemId);
    
    if (fromSystemIt != systemMap.end() && toSystemIt != systemMap.end()) {
        auto travelLane = std::make_shared<TravelLaneModel>(
            id, fromSystemIt->second, toSystemIt->second
        );
        
        galaxy->addTravelLane(std::move(travelLane));
        return true;
    } else {
        qWarning() << "Travel lane" << id << "references non-existent systems:" 
                   << fromSystemId << "or" << toSystemId;
        return false;
    }
}

bool XmlGalaxyImporter::parsePlanetFromStream(std::shared_ptr<StarSystemModel> system, QXmlStreamReader& xml) {
    QXmlStreamAttributes attributes = xml.attributes();
    
    std::string name = attributes.value("name").toString().toStdString();
    PlanetType type = static_cast<PlanetType>(attributes.value("type").toInt());
    double size = attributes.value("size").toDouble();
    double mass = attributes.value("mass").toDouble();
    int numberOfMoons = attributes.value("numberOfMoons").toInt();
    double orbitalRadius = attributes.value("orbitalRadius").toDouble();
    double maxTemperature = attributes.value("maxTemperature").toDouble();
    double minTemperature = attributes.value("minTemperature").toDouble();

    if (name.empty() || size <= 0 || mass <= 0 || orbitalRadius < 0) {
        qWarning() << "Invalid planet data in XML";
        return false;
    }

    Planet planet(name, type, size, mass, numberOfMoons, orbitalRadius, maxTemperature, minTemperature);
    system->addPlanet(std::move(planet));
    
    xml.skipCurrentElement(); // Skip to end of Planet element
    return true;
}

void XmlGalaxyImporter::updateGalaxyDimensions(GalaxyModel* galaxy,
                                             const std::unordered_map<quint32, std::shared_ptr<StarSystemModel>>& systemMap) {
    if (!systemMap.empty()) {
        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::lowest();
        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::lowest();
        
        for (const auto& [id, system] : systemMap) {
            auto pos = system->getPosition();
            minX = std::min(minX, pos.x);
            maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);
        }
        
        // Add 10% padding
        double width = (maxX - minX) * 1.1;
        double height = (maxY - minY) * 1.1;
        
        galaxy->setWidth(static_cast<int>(std::max(width, 1000.0))); // Minimum 1000
        galaxy->setHeight(static_cast<int>(std::max(height, 1000.0))); // Minimum 1000
    } else {
        // Default dimensions if no systems
        galaxy->setWidth(1000);
        galaxy->setHeight(1000);
    }
}
}