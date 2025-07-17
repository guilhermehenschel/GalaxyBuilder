#include "ggh/modules/GalaxyExporter/GalaxyXMLExporter.h"

#include <QFile>
#include <QTextStream>

namespace ggh::Galaxy::Exporter {
GalaxyXMLExporter::GalaxyXMLExporter(std::shared_ptr<GalaxyCore::models::GalaxyModel> model, QObject* parent)
    : AbstractExporter(parent), m_model(model) {}

bool GalaxyXMLExporter::exportToFile(const std::string& filePath) const {
    if (!m_model) {
        return false;
    }
    
    // Serialize the document to a file
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream << m_model->toXml().c_str(); // Assuming GalaxyModel has a toXml() method that returns XML as QString
    return true;
}

std::string GalaxyXMLExporter::getFormat() const {
    return "XML";
}

}