#ifndef GGH_MODULES_GALAXYFACTORIES_XMLGALAXYIMPORTER_
#define GGH_MODULES_GALAXYFACTORIES_XMLGALAXYIMPORTER_H

#include "ggh/modules/GalaxyFactories/AbstractGalaxyFactory.h"
#include <expected>
#include <memory>
#include <QString>

// Forward declarations
class QDomDocument;
class QDomElement;
class QDomNode;
class QDomNodeList;

namespace ggh::GalaxyCore::models {
    class StarSystemModel;
}

using StarSystemModel = ggh::GalaxyCore::models::StarSystemModel;

namespace ggh::GalaxyFactories {
/**
 * @file XmlGalaxyImporter.h
 * @brief Interface for importing galaxy data from XML files.
 */
class XmlGalaxyImporter : public AbstractGalaxyFactory {
public:
    /**
     * @brief Imports a galaxy from the specified XML file.
     * @param filePath The path to the XML file containing galaxy data.
     * @return A unique pointer to the imported galaxy model.
     */
    std::unique_ptr<GalaxyModel> importGalaxy(const QString& filePath);

    /**
     * @brief Sets the parameters for importing the galaxy.
     * @param params The parameters to use for importing the galaxy.
     */
    void setXmlPath(const QString& filePath);

    /**
     * @brief Generates a galaxy based on the factory's parameters.
     * @return A unique pointer to the generated galaxy model.
     */
    std::unique_ptr<GalaxyModel> generateGalaxy() override;

    /**
     * @brief Sets the parameters for importing the galaxy.
     * @param params The parameters to use for importing the galaxy.
     */
    void setParameters(const GenerationParameters& params);

    /**
     * @brief Gets the current parameters used by the importer.
     * @return The current import parameters.
     */
    GenerationParameters getParameters() const;

private:
    QString m_filePath; // Path to the XML file

    /**
     * @brief Parses the XML file and populates the GalaxyModel.
     * @param galaxy The GalaxyModel to populate.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseXmlFile(GalaxyModel& galaxy);

    /**
     * @brief Converts the XML data to a GalaxyModel.
     * @param xmlData The XML data to convert.
     * @return A unique pointer to the populated GalaxyModel.
     */
    std::expected<std::unique_ptr<GalaxyModel>, std::string> convertXmlToGalaxyModel(const QString& xmlData);

    /**
     * @brief Parses System elements from the XML and adds them to the GalaxyModel.
     * @param galaxy The GalaxyModel to populate.
     * @param systemsElement The XML element containing system data.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseSystems(std::shared_ptr<GalaxyModel> galaxy, const QDomNodeList& systemsNodeList);

    /**
     * @brief Parses TravelLane elements from the XML and adds them to the GalaxyModel.
     * @param galaxy The GalaxyModel to populate.
     * @param lanesElement The XML element containing travel lane data.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseTravelLanes(std::shared_ptr<GalaxyModel> galaxy, const QDomNodeList& laneNodeList);

    /**
     * @brief Parses a single System element and adds it to the GalaxyModel.
     * @param galaxy The GalaxyModel to populate.
     * @param systemElement The XML element containing system data.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseSystem(std::shared_ptr<GalaxyModel> galaxy, const QDomElement& systemElement);

    /**
     * @brief Parses a single TravelLane element and adds it to the GalaxyModel.
     * @param galaxy The GalaxyModel to populate.
     * @param laneElement The XML element containing travel lane data.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseTravelLane(std::shared_ptr<GalaxyModel> galaxy, const QDomElement& laneElement);

    /**
     * @brief Parse planet data from the XML and adds it to the StarSystem
     * @param system The StarSystem to populate.
     * @param planetsElement The XML element containing planet data.
     * @return True if parsing was successful, false otherwise.
     */
    bool parsePlanets(std::shared_ptr<StarSystemModel> system, const QDomNodeList& planetsElement);

    /**
     * @brief Parses a single Planet element and adds it to the StarSystemModel.
     * @param system The StarSystemModel to populate.
     * @param planetElement The XML element containing planet data.
     * @return True if parsing was successful, false otherwise.
     */
    bool parsePlanet(std::shared_ptr<StarSystemModel> system, const QDomElement& planetElement);

    /**
     * @brief Validates the XML structure and data before importing.
     * @param doc The XML document to validate.
     * @return True if the XML is valid, false otherwise.
     */
    bool validateXml(const QDomDocument& doc) const;

    /**
     * @brief Validates a single XML element.
     * @param element The XML element to validate.
     * @return True if the element is valid, false otherwise.
     */
    template<typename T>
    bool validateXmlElement(const QDomElement& element);
};
}

#endif // !GGH_MODULES_GALAXYFACTORIES_XMLGALAXYIMPORTER_H