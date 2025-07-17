#ifndef GGH_MODULES_GALAXYEXPORTER_ABSTRACT_EXPORTER_H
#define GGH_MODULES_GALAXYEXPORTER_ABSTRACT_EXPORTER_H

#include <memory>
#include <string>
#include "ggh/modules/GalaxyExporter/galaxyexporter_export.h"
#include <QObject>

namespace ggh::Galaxy::Exporter {
/**
 * @class AbstractExporter
 * @brief An abstract base class for exporting galaxy data.
 *
 * This class defines the interface for exporting galaxy data to various formats.
 */

class GALAXYEXPORTER_EXPORT AbstractExporter : public QObject {
    Q_OBJECT
public:
    explicit AbstractExporter(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~AbstractExporter() = default;

    /**
     * @brief Exports the galaxy data to a specified file.
     * @param filePath The path to the file where the data will be exported.
     * @return True if the export was successful, false otherwise.
     */
    virtual bool exportToFile(const std::string& filePath) const = 0;

    /**
     * @brief Gets the format of the exported data.
     * @return A string representing the format of the exported data.
     */
    virtual std::string getFormat() const = 0;
};
} // namespace ggh::Galaxy::Exporter

#endif // !GGH_MODULES_GALAXYEXPORTER_ABSTRACT_EXPORTER_H