#ifndef GGH_MODULES_GALAXYEXPORTER_EXPORTER_Object_H
#define GGH_MODULES_GALAXYEXPORTER_EXPORTER_Object_H

#include <memory>
#include <string>
#include "ggh/modules/GalaxyExporter/galaxyexporter_export.h"
#include "ggh/modules/GalaxyExporter/AbstractExporter.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml/qqml.h>

// Galaxy Core Includes
#include "ggh/modules/GalaxyCore/viewmodels/GalaxyViewModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"

// Galaxy Exporters Include
#include "ggh/modules/GalaxyExporter/GalaxyXMLExporter.h"
#include "ggh/modules/GalaxyExporter/StarSystemXMLExporter.h"

namespace ggh::Galaxy::Exporter {

/**
 * @class ExporterObject
 * @brief A factory class for creating instances of different exporters.
 */
class GALAXYEXPORTER_EXPORT ExporterObject : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    QML_SINGLETON
public:
    explicit ExporterObject(QObject* parent = nullptr);
    ~ExporterObject() override = default;

    Q_DISABLE_COPY_MOVE(ExporterObject)

    /**
     * @brief Gets the list of supported formats.
     * @return A list of strings representing the supported formats.
     */
    Q_INVOKABLE QStringList supportedFormats() const;

    /**
     * @brief Gets the default format for exporting.
     * @return A string representing the default export format.
     */
    Q_INVOKABLE std::string defaultFormat() const;

    Q_INVOKABLE void setFilePath(const QString& filePath);

    /**
     * @brief Gets the file path for the exporter.
     * @return A string representing the file path.
     */
    Q_INVOKABLE QString getFilePath() const;

    /**
     * @brief Exports the galaxy data using the specified exporter.
     * @return True if the export was successful, false otherwise.
     */
    Q_INVOKABLE bool exportObject();

    /**
     * @brief Gets the error string if the export fails.
     * @return A string containing the error message.
     */
    QString errorString() const;

    /**
     * @brief Set model for the exporter.
     * @param model The model to set.
     */
    Q_INVOKABLE void setModel(QObject* model);

signals:
    void filePathChanged();
    void errorStringChanged();

private:
    std::string m_filePath{""};  ///< The file path to export the galaxy data to
    std::string m_errorString{""};///< The error string if the export fails
    std::unique_ptr<AbstractExporter> m_exporter; ///< The exporter instance used for exporting

    
    /**
     * @brief Sets the error string.
     * @param error The error message to set.
     */
    void setErrorString(const std::string& error);
};

} // namespace ggh::Galaxy::Exporter

#endif // !GGH_MODULES_GALAXYEXPORTER_EXPORTER_Object_H