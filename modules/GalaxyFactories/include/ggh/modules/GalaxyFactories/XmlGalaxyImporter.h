#ifndef GGH_MODULES_GALAXYFACTORIES_XMLGALAXYIMPORTER_
#define GGH_MODULES_GALAXYFACTORIES_XMLGALAXYIMPORTER_H

#include "ggh/modules/GalaxyFactories/AbstractGalaxyFactory.h"
#include "galaxyfactories_global.h"
#include <expected>
#include <memory>
#include <QString>
#include <QXmlStreamReader>
#include <unordered_map>

namespace ggh::GalaxyCore::models {
    class StarSystemModel;
}

using StarSystemModel = ggh::GalaxyCore::models::StarSystemModel;

namespace ggh::GalaxyFactories {
/**
 * @file XmlGalaxyImporter.h
 * @brief Interface for importing galaxy data from XML files.
 */
class GALAXYFACTORIES_EXPORT XmlGalaxyImporter : public AbstractGalaxyFactory {
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
     * @brief Parses a StarSystem element using QXmlStreamReader.
     * @param galaxy The GalaxyModel to populate.
     * @param systemMap Map to track systems for travel lane parsing.
     * @param xml The XML stream reader.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseSystemFromStream(GalaxyModel* galaxy, 
                              std::unordered_map<quint32, std::shared_ptr<StarSystemModel>>& systemMap,
                              QXmlStreamReader& xml);

    /**
     * @brief Parses a TravelLane element using QXmlStreamReader.
     * @param galaxy The GalaxyModel to populate.
     * @param systemMap Map to find referenced systems.
     * @param xml The XML stream reader.
     * @return True if parsing was successful, false otherwise.
     */
    bool parseTravelLaneFromStream(GalaxyModel* galaxy,
                                  std::unordered_map<quint32, std::shared_ptr<StarSystemModel>>& systemMap,
                                  QXmlStreamReader& xml);

    /**
     * @brief Parses a Planet element using QXmlStreamReader.
     * @param system The StarSystemModel to populate.
     * @param xml The XML stream reader.
     * @return True if parsing was successful, false otherwise.
     */
    bool parsePlanetFromStream(std::shared_ptr<StarSystemModel> system, QXmlStreamReader& xml);

    /**
     * @brief Updates galaxy dimensions based on system positions.
     * @param galaxy The GalaxyModel to update.
     * @param systemMap Map of all systems to calculate bounding box.
     */
    void updateGalaxyDimensions(GalaxyModel* galaxy,
                               const std::unordered_map<quint32, std::shared_ptr<StarSystemModel>>& systemMap);
};
}

#endif // !GGH_MODULES_GALAXYFACTORIES_XMLGALAXYIMPORTER_H