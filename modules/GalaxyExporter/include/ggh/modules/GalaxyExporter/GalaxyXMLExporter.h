#ifndef GGH_MODULES_GALAXYEXPORTER_GALAXYXML_EXPORTER_H
#define GGH_MODULES_GALAXYEXPORTER_GALAXYXML_EXPORTER_H

#include "ggh/modules/GalaxyExporter/galaxyexporter_export.h"
#include "ggh/modules/GalaxyExporter/AbstractExporter.h"
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include <string>

namespace ggh::Galaxy::Exporter {

/**
 * @class GalaxyXMLExporter
 * @brief An exporter for the galaxy data in XML format.
 */
class GALAXYEXPORTER_EXPORT GalaxyXMLExporter : public AbstractExporter {
public:
    /**
     * @brief Constructs a GalaxyXMLExporter.
     * @param model The model to export.
     * @param parent The parent QObject.
     */
    GalaxyXMLExporter(std::shared_ptr<GalaxyCore::models::GalaxyModel> model, QObject* parent = nullptr);

    /**
     * @brief Exports the galaxy data to the specified file.
     * @param filePath The path to the file to export to.
     * @return True if the export was successful, false otherwise.
     */
    bool exportToFile(const std::string& filePath) const override;

    std::string getFormat() const override;

private:
    std::shared_ptr<GalaxyCore::models::GalaxyModel> m_model; ///< The model to export
};

} // namespace ggh::Galaxy::Exporter

#endif // !GGH_MODULES_GALAXYEXPORTER_GALAXYXML_EXPORTER_H
