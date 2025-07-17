#ifndef GGH_MODULES_GALAXYEXPORTER_STAR_SYSTEM_XML_EXPORTER_H
#define GGH_MODULES_GALAXYEXPORTER_STAR_SYSTEM_XML_EXPORTER_H

#include "ggh/modules/GalaxyExporter/galaxyexporter_export.h"
#include "ggh/modules/GalaxyExporter/AbstractExporter.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"

namespace ggh::Galaxy::Exporter {
/**
 * @class StarSystemXMLExporter
 * @brief An exporter for star system data in XML format.
 */
class GALAXYEXPORTER_EXPORT StarSystemXMLExporter : public AbstractExporter {
    Q_OBJECT
public:
    /**
     * @brief Constructs a StarSystemXMLExporter.
     * @param model The model to export.
     * @param parent The parent QObject.
     */
    StarSystemXMLExporter(std::shared_ptr<GalaxyCore::models::StarSystemModel> model, QObject* parent = nullptr);

    /**
     * @brief Exports the star system data to the specified file.
     * @param filePath The path to the file to export to.
     * @return True if the export was successful, false otherwise.
     */
    bool exportToFile(const std::string& filePath) const override;

    /**
     * @brief Gets the format of the exported data.
     * @return A string representing the format of the exported data.
     */
    std::string getFormat() const override;

private:
    std::shared_ptr<GalaxyCore::models::StarSystemModel> m_model; ///< The model to export
};

} // namespace ggh::Galaxy::Exporter

#endif // !GGH_MODULES_GALAXYEXPORTER_STAR_SYSTEM_XML_EXPORTER_H