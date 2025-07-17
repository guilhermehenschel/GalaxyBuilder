#include "ggh/modules/GalaxyFactories/XmlGalaxyImporter.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <concepts>
#include <expected>

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


namespace{

template<typename T>
concept HasXmlTag = requires {
    { T::xmlTag() } -> std::convertible_to<std::string>;
};

template <HasXmlTag T>
bool validateXmlElement(const QDomElement& element) {
    if (element.isNull()) {
        qWarning() << "Invalid XML element: Element is null";
        return false;
    }
    if (element.tagName() != T::xmlTag()) {
        qWarning() << "Invalid XML element: Expected tag" << T::xmlTag() << "but found" << element.tagName();
        return false;
    }
    return true;
}
}

namespace ggh::GalaxyFactories {
std::unique_ptr<GalaxyModel> XmlGalaxyImporter::importGalaxy(const QString& filePath) {
    m_filePath = filePath;
    auto galaxy = std::make_unique<GalaxyModel>(1000, 1000); // Default size
    
    if (!parseXmlFile(*galaxy)) {
        return nullptr; // Parsing failed
    }
    
    return galaxy;
}

void XmlGalaxyImporter::setXmlPath(const QString& filePath) {
    m_filePath = filePath;
}

std::unique_ptr<GalaxyModel> XmlGalaxyImporter::generateGalaxy() {
    return importGalaxy(m_filePath);
}

void XmlGalaxyImporter::setParameters(const GenerationParameters& /*params*/) {
    // Not used in XML importer, parameters are set via file path
}

GenerationParameters XmlGalaxyImporter::getParameters() const {
    GenerationParameters params;
    // Populate params based on XML file or other criteria if needed
    return params;
}

bool XmlGalaxyImporter::parseXmlFile(GalaxyModel& galaxy) {
    // Implement XML parsing logic here
    // This is a placeholder implementation
    // You would typically use an XML parser to read the file and populate the GalaxyModel
    
    // Example: Read from m_filePath and populate galaxy with star systems and travel lanes
    // For now, return true to indicate success
    auto file = QFile(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open XML file:" << m_filePath;
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qWarning() << "Failed to parse XML content from file:" << m_filePath;
        file.close();
        return false;
    }

    file.close();

    QDomElement root = doc.documentElement();
    if (!validateXml(doc)) {
        qWarning() << "Invalid XML structure in file:" << m_filePath;
        return false;
    }

    QDomElement galaxyElement = root.firstChildElement("Galaxy");
    if (galaxyElement.isNull()) {
        qWarning() << "Failed to find Galaxy element in XML";
        return false;
    }

    if(!validateXmlElement<GalaxyModel>(galaxyElement)) {
        return false; // Validation failed
    }

    auto galaxyPtr = std::make_shared<GalaxyModel>(galaxy);

    if (!parseSystems(galaxyPtr, root.elementsByTagName("System"))) {
        qWarning() << "Failed to parse systems from XML";
        return false;
    }
    if (!parseTravelLanes(galaxyPtr, root.elementsByTagName("TravelLane"))) {
        qWarning() << "Failed to parse travel lanes from XML";
        return false;
    }
    return true;
}

bool XmlGalaxyImporter::validateXml(const QDomDocument& doc) const {
    // Implement validation logic for the entire XML document
    // This is a placeholder implementation
    // You would typically check if the document has a valid structure and required elements
    
    if (doc.isNull()) {
        qWarning() << "Invalid XML document: Document is null";
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "Galaxy") {
        qWarning() << "Invalid XML document: Root element must be 'Galaxy'";
        return false;
    }
    return true;
}

bool XmlGalaxyImporter::parseSystems(std::shared_ptr<GalaxyModel> galaxy, const QDomNodeList& systemsNodeList) {
    for(int i = 0; i < systemsNodeList.size(); ++i) {
        QDomNode systemNode = systemsNodeList.at(i);
        if(systemNode.isElement()) {
            QDomElement systemElement = systemNode.toElement();
            if (!parseSystem(galaxy, systemElement)) {
                qWarning() << "Failed to parse system element";
                return false;
            }
        }
    }
    return true;
}

bool XmlGalaxyImporter::parseTravelLanes(std::shared_ptr<GalaxyModel> galaxy, const QDomNodeList& laneNodeList) {
    for(int i = 0; i < laneNodeList.size(); ++i) {
        QDomNode laneNode = laneNodeList.at(i);
        if(laneNode.isElement()) {
            QDomElement laneElement = laneNode.toElement();
            if (!parseTravelLane(galaxy, laneElement)) {
                qWarning() << "Failed to parse travel lane element";
                return false;
            }
        }
    }
    return true;
}

bool XmlGalaxyImporter::parseSystem(std::shared_ptr<GalaxyModel> galaxy, const QDomElement& systemElement) {
    if (!validateXmlElement<StarSystemModel>(systemElement)) {
        return false; // Validation failed
    }

    SystemId id{static_cast<SystemId>(systemElement.attribute("id").toInt())};
    auto name{systemElement.attribute("name")};
    ggh::GalaxyCore::utilities::CartesianCoordinates<double> position(
        systemElement.attribute("x").toDouble(),
        systemElement.attribute("y").toDouble()
    );
    StarType type{static_cast<StarType>(systemElement.attribute("starType").toInt())};
    if (id == 0 || name.isEmpty()) {
        qWarning() << "Invalid system data in XML";
        return false;
    }

    auto system{std::make_shared<StarSystemModel>(id, name.toStdString(), position, type)};
    if (!parsePlanets(system, systemElement.elementsByTagName("Planet"))) {
        qWarning() << "Failed to parse planets for system" << id;
        return false;
    }
    galaxy->addStarSystem(std::move(system));
    return true;
}

bool XmlGalaxyImporter::parseTravelLane(std::shared_ptr<GalaxyModel> galaxy, const QDomElement& laneElement) {
    if (!validateXmlElement<TravelLaneModel>(laneElement)) {
        return false; // Validation failed
    }

    LaneId id{static_cast<LaneId>(laneElement.attribute("id").toInt())};
    SystemId fromSystemId{static_cast<SystemId>(laneElement.attribute("fromSystem").toInt())};
    SystemId toSystemId{static_cast<SystemId>(laneElement.attribute("toSystem").toInt())};

    if (id == 0 || fromSystemId == 0 || toSystemId == 0) {
        qWarning() << "Invalid travel lane data in XML";
        return false;
    }

    auto fromSystem{galaxy->getStarSystem(fromSystemId)};
    auto toSystem{galaxy->getStarSystem(toSystemId)};
    
    if (!fromSystem || !toSystem) {
        qWarning() << "Invalid systems for travel lane" << id;
        return false;
    }

    galaxy->addTravelLane(id, fromSystem, toSystem);
    return true;
}

bool XmlGalaxyImporter::parsePlanets(std::shared_ptr<StarSystemModel> system, const QDomNodeList& planetsElement) {
    for (int i = 0; i < planetsElement.size(); ++i) {
        QDomNode planetNode = planetsElement.at(i);
        if (planetNode.isElement()) {
            QDomElement planetElement = planetNode.toElement();
            if (!parsePlanet(system, planetElement)) {
                qWarning() << "Failed to parse planet element";
                return false;
            }
        }
    }
    return true;
}

bool XmlGalaxyImporter::parsePlanet(std::shared_ptr<StarSystemModel> system, const QDomElement& planetElement) {
    if (!validateXmlElement<Planet>(planetElement)) {
        return false; // Validation failed
    }

    std::string name{planetElement.attribute("name").toStdString()};
    PlanetType type{static_cast<PlanetType>(planetElement.attribute("type").toInt())};
    double size{planetElement.attribute("size").toDouble()};
    double mass{planetElement.attribute("mass").toDouble()};
    int numberOfMoons{planetElement.attribute("numberOfMoons").toInt()};
    double orbitalRadius{planetElement.attribute("orbitalRadius").toDouble()};
    double maxTemperature{planetElement.attribute("maxTemperature").toDouble()};
    double minTemperature{planetElement.attribute("minTemperature").toDouble()};

    if (name.empty() || size <= 0 || mass <= 0 || orbitalRadius < 0) {
        qWarning() << "Invalid planet data in XML";
        return false;
    }

    Planet planet(name, type, size, mass, numberOfMoons, orbitalRadius, maxTemperature, minTemperature);
    system->addPlanet(planet);
    return true;
}
}