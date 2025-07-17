#include "ggh/modules/GalaxyExporter/ExporterObject.h"

#include "ggh/modules/GalaxyExporter/GalaxyXMLExporter.h"
#include "ggh/modules/GalaxyExporter/StarSystemXMLExporter.h"
#include "ggh/modules/GalaxyExporter/AbstractExporter.h"
#include "ggh/modules/GalaxyExporter/AbstractExporter.h" // Ensure AbstractExporter is included

#include <variant>

namespace {
struct Visitor {
    std::unique_ptr<ggh::Galaxy::Exporter::AbstractExporter> operator()(ggh::GalaxyCore::viewmodels::GalaxyViewModel* model) const {
        return std::make_unique<ggh::Galaxy::Exporter::GalaxyXMLExporter>(model->galaxy());
    }

    std::unique_ptr<ggh::Galaxy::Exporter::AbstractExporter> operator()(ggh::GalaxyCore::viewmodels::StarSystemViewModel* model) const {
        return std::make_unique<ggh::Galaxy::Exporter::StarSystemXMLExporter>(model->starSystem());
    }
};
}
namespace ggh::Galaxy::Exporter {
ExporterObject::ExporterObject(QObject* parent)
    : QObject(parent), m_exporter(nullptr) {
}

QStringList ExporterObject::supportedFormats() const {
    return QStringList() << "XML";
}

std::string ExporterObject::defaultFormat() const {
    return "XML";
}

bool ExporterObject::exportObject() {
    if (m_exporter) {
        return m_exporter->exportToFile(m_filePath);
    }
    setErrorString("No exporter set or model is invalid.");
    return false;
}

void ExporterObject::setFilePath(const QString& filePath) {
    m_filePath = filePath.toStdString();
    emit filePathChanged();
}

QString ExporterObject::getFilePath() const {
    return QString::fromStdString(m_filePath);
}

QString ExporterObject::errorString() const {
    return QString::fromStdString(m_errorString);
}

void ExporterObject::setErrorString(const std::string& error) {
    m_errorString = error;
    emit errorStringChanged();
}

void ExporterObject::setModel(QObject* model) {
    // You need to convert QObject* to the appropriate variant type
    using ModelVariant = std::variant<
        ggh::GalaxyCore::viewmodels::GalaxyViewModel*,
        ggh::GalaxyCore::viewmodels::StarSystemViewModel*
    >;

    if (model) {
        ModelVariant variantModel;
        if (auto galaxyModel = dynamic_cast<ggh::GalaxyCore::viewmodels::GalaxyViewModel*>(model)) {
            variantModel = galaxyModel;
        } else if (auto starSystemModel = dynamic_cast<ggh::GalaxyCore::viewmodels::StarSystemViewModel*>(model)) {
            variantModel = starSystemModel;
        } else {
            setErrorString("Model type not supported for export.");
            m_exporter = nullptr;
            return;
        }
        // Use std::visit with the variant
        m_exporter = std::visit(Visitor{}, variantModel);
    } else {
        setErrorString("Invalid model provided for exporter.");
        m_exporter = nullptr;
    }
}

} // namespace ggh::Galaxy::Exporter