#include "ggh/modules/GalaxyExporter/StarSystemXMLExporter.h"

#include <QFile>
#include <QTextStream>
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"


namespace ggh::Galaxy::Exporter {

StarSystemXMLExporter::StarSystemXMLExporter(std::shared_ptr<GalaxyCore::models::StarSystemModel> model, QObject* parent)
    : AbstractExporter(parent), m_model(model) {}

bool StarSystemXMLExporter::exportToFile(const std::string& filePath) const {
    if (!m_model) {
        return false;
    }
    
    // Serialize the document to a file
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream << m_model->toXml().c_str(); // Assuming StarSystemModel has a toXml() method that returns XML as QString
    return true;
}

std::string StarSystemXMLExporter::getFormat() const {
    return "XML";
}

} // namespace ggh::Galaxy::Exporter
