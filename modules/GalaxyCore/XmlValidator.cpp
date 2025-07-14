#include "XmlValidator.h"
#include "Common.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDebug>
#include <QRegularExpression>
#include <QUrl>

XmlValidator::XmlValidator(QObject* parent)
    : QObject(parent)
{
}

XmlValidator::ValidationResult XmlValidator::validateFile(const QString& filePath, ValidationLevel level)
{
    emit validationStarted(filePath);
    emit validationProgress(0);
    
    ValidationResult result;
    result.isValid = false;
    result.detectedType = XmlType::Unknown;
    result.systemCount = 0;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        result.errors << QString("Cannot open file: %1").arg(filePath);
        emit validationCompleted(false);
        return result;
    }
    
    emit validationProgress(25);
    
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument::ParseResult parseResult = doc.setContent(&file, QDomDocument::ParseOption::UseNamespaceProcessing, &errorMsg, &errorLine, &errorColumn);
    if (!parseResult) {
        result.errors << QString("XML parsing error at line %1, column %2: %3")
                                .arg(errorLine).arg(errorColumn).arg(errorMsg);
        file.close();
        emit validationCompleted(false);
        return result;
    }
    file.close();
    
    emit validationProgress(50);
    
    result = performValidation(doc, level);
    
    emit validationProgress(100);
    emit validationCompleted(result.isValid);
    
    m_lastResult = result;
    return result;
}

XmlValidator::ValidationResult XmlValidator::validateXmlString(const QString& xmlContent, ValidationLevel level)
{
    ValidationResult result;
    result.isValid = false;
    result.detectedType = XmlType::Unknown;
    result.systemCount = 0;
    
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorColumn;
    QDomDocument::ParseResult parseResult = doc.setContent(xmlContent, QDomDocument::ParseOption::UseNamespaceProcessing, &errorMsg, &errorLine, &errorColumn);
    if (!parseResult) {
        result.errors << QString("XML parsing error at line %1, column %2: %3")
                                .arg(errorLine).arg(errorColumn).arg(errorMsg);
        return result;
    }
    
    return performValidation(doc, level);
}

bool XmlValidator::isValidGalaxyXml(const QString& filePath)
{
    ValidationResult result = validateFile(filePath, ValidationLevel::Structure);
    return result.isValid && result.detectedType == XmlType::Galaxy;
}

bool XmlValidator::isValidSystemXml(const QString& filePath)
{
    ValidationResult result = validateFile(filePath, ValidationLevel::Structure);
    return result.isValid && result.detectedType == XmlType::SingleSystem;
}

XmlValidator::ValidationResult XmlValidator::performValidation(const QDomDocument& doc, ValidationLevel level)
{
    ValidationResult result;
    result.isValid = true;
    result.detectedType = detectXmlType(doc);
    result.systemCount = 0;
    
    // Basic validation - XML well-formedness (already done in parsing)
    if (level >= ValidationLevel::Basic) {
        if (!validateWellFormedness(doc, result.errors)) {
            result.isValid = false;
        }
    }
    
    // Structure validation
    if (level >= ValidationLevel::Structure && result.isValid) {
        if (!validateXmlStructure(doc, result.detectedType)) {
            result.isValid = false;
            result.errors << "XML structure validation failed";
        }
        
        QDomElement root = doc.documentElement();
        if (!validateRootElement(root, result.errors, result.warnings)) {
            result.isValid = false;
        }
        
        // Count systems and validate structure
        if (result.detectedType == XmlType::Galaxy) {
            QDomElement systemsElement = root.firstChildElement("Systems");
            if (!systemsElement.isNull()) {
                QDomNodeList systemNodes = systemsElement.elementsByTagName("System");
                result.systemCount = systemNodes.count();
                
                for (int i = 0; i < systemNodes.count(); ++i) {
                    if (!validateSystemElement(systemNodes.at(i).toElement())) {
                        result.isValid = false;
                        result.errors << QString("System validation failed for system %1").arg(i + 1);
                    }
                }
            }
        } else if (result.detectedType == XmlType::SingleSystem) {
            QDomElement systemElement = root.firstChildElement("System");
            if (!systemElement.isNull()) {
                result.systemCount = 1;
                if (!validateSystemElement(systemElement)) {
                    result.isValid = false;
                    result.errors << "System validation failed";
                }
            }
        }
        
        // Validate metadata if present
        QDomElement metadata = root.firstChildElement("Metadata");
        if (!metadata.isNull()) {
            result.version = metadata.attribute("version", "unknown");
            if (!validateMetadataElement(metadata)) {
                result.warnings << "Metadata validation issues detected";
            }
        }
    }
    
    // Data validation
    if (level >= ValidationLevel::Data && result.isValid) {
        QDomNodeList systemNodes;
        if (result.detectedType == XmlType::Galaxy) {
            QDomElement systemsElement = doc.documentElement().firstChildElement("Systems");
            systemNodes = systemsElement.elementsByTagName("System");
        } else {
            systemNodes = doc.elementsByTagName("System");
        }
        
        for (int i = 0; i < systemNodes.count(); ++i) {
            QDomElement systemElement = systemNodes.at(i).toElement();
            if (!validateSystemData(systemElement)) {
                result.isValid = false;
                result.errors << QString("Data validation failed for system %1").arg(i + 1);
            }
            
            // Validate planet data
            QDomElement planetsElement = systemElement.firstChildElement("Planets");
            if (!planetsElement.isNull()) {
                QDomNodeList planetNodes = planetsElement.elementsByTagName("Planet");
                for (int j = 0; j < planetNodes.count(); ++j) {
                    if (!validatePlanetData(planetNodes.at(j).toElement())) {
                        result.isValid = false;
                        result.errors << QString("Planet data validation failed for system %1, planet %2")
                                                .arg(i + 1).arg(j + 1);
                    }
                }
            }
        }
    }
    
    return result;
}

bool XmlValidator::validateWellFormedness(const QDomDocument& doc, QStringList& errors)
{
    // XML well-formedness is validated during parsing
    // Additional checks can be added here
    if (doc.isNull()) {
        errors << "Document is null or empty";
        return false;
    }
    
    QDomElement root = doc.documentElement();
    if (root.isNull()) {
        errors << "No root element found";
        return false;
    }
    
    return true;
}

bool XmlValidator::validateRootElement(const QDomElement& root, QStringList& errors, QStringList& warnings)
{
    if (root.tagName() != "StarSystem" && root.tagName() != "Galaxy") {
        addError(errors, "root", QString("Invalid root element: %1. Expected 'StarSystem' or 'Galaxy'").arg(root.tagName()));
        return false;
    }
    
    return true;
}

bool XmlValidator::validateXmlStructure(const QDomDocument& doc, XmlType expectedType)
{
    QDomElement root = doc.documentElement();
    
    if (expectedType == XmlType::Galaxy || (expectedType == XmlType::Unknown && root.tagName() == "Galaxy")) {
        // Galaxy XML structure
        QDomElement systemsElement = root.firstChildElement("Systems");
        if (systemsElement.isNull()) {
            return false;
        }
        
        QDomNodeList systemNodes = systemsElement.elementsByTagName("System");
        if (systemNodes.isEmpty()) {
            return false; // Galaxy should have at least one system
        }
        
    } else if (expectedType == XmlType::SingleSystem || (expectedType == XmlType::Unknown && root.tagName() == "StarSystem")) {
        // Single system XML structure
        QDomElement systemElement = root.firstChildElement("System");
        if (systemElement.isNull()) {
            return false;
        }
    } else {
        return false; // Unknown or unsupported type
    }
    
    return true;
}

bool XmlValidator::validateSystemElement(const QDomElement& element)
{
    if (element.isNull() || element.tagName() != "System") {
        return false;
    }
    
    // Check required attributes
    QStringList requiredAttrs = {"id", "name", "positionX", "positionY", 
                                 "starType", "systemSize", "starMass", 
                                 "starTemperature", "starLuminosity"};
    
    for (const QString& attr : requiredAttrs) {
        if (!element.hasAttribute(attr)) {
            return false;
        }
    }
    
    // Validate planets section if present
    QDomElement planetsElement = element.firstChildElement("Planets");
    if (!planetsElement.isNull()) {
        QDomNodeList planetNodes = planetsElement.elementsByTagName("Planet");
        for (int i = 0; i < planetNodes.count(); ++i) {
            if (!validatePlanetElement(planetNodes.at(i).toElement())) {
                return false;
            }
        }
    }
    
    return true;
}

bool XmlValidator::validatePlanetElement(const QDomElement& element)
{
    if (element.isNull() || element.tagName() != "Planet") {
        return false;
    }
    
    QStringList requiredAttrs = {"name", "type", "size", "mass", "gravity", 
                                 "moonCount", "minTemperature", "maxTemperature", 
                                 "orbitDistance"};
    
    for (const QString& attr : requiredAttrs) {
        if (!element.hasAttribute(attr)) {
            return false;
        }
    }
    
    return true;
}

bool XmlValidator::validateMetadataElement(const QDomElement& element)
{
    if (element.isNull() || element.tagName() != "Metadata") {
        return false;
    }
    
    // Check version format
    QString version = element.attribute("version");
    if (version.isEmpty()) {
        return false;
    }
    
    // Validate version format (e.g., "1.0", "2.1.3")
    QRegularExpression versionRegex(R"(^\d+\.\d+(\.\d+)?$)");
    if (!versionRegex.match(version).hasMatch()) {
        return false;
    }
    
    return true;
}

bool XmlValidator::validateSystemData(const QDomElement& element)
{
    bool ok;
    
    // Validate star type
    int starType = element.attribute("starType").toInt(&ok);
    if (!ok || !isValidStarType(starType)) {
        return false;
    }
    
    // Validate system size
    int systemSize = element.attribute("systemSize").toInt(&ok);
    if (!ok || !isValidSystemSize(systemSize)) {
        return false;
    }
    
    // Validate star mass
    double starMass = element.attribute("starMass").toDouble(&ok);
    if (!ok || !isValidStarMass(starMass)) {
        return false;
    }
    
    // Validate star temperature
    double starTemperature = element.attribute("starTemperature").toDouble(&ok);
    if (!ok || !isValidStarTemperature(starTemperature)) {
        return false;
    }
    
    // Validate star luminosity
    double starLuminosity = element.attribute("starLuminosity").toDouble(&ok);
    if (!ok || !isValidStarLuminosity(starLuminosity)) {
        return false;
    }
    
    // Validate position
    double posX = element.attribute("positionX").toDouble(&ok);
    if (!ok) return false;
    
    double posY = element.attribute("positionY").toDouble(&ok);
    if (!ok) return false;
    
    return true;
}

bool XmlValidator::validatePlanetData(const QDomElement& element)
{
    bool ok;
    
    // Validate planet type
    int planetType = element.attribute("type").toInt(&ok);
    if (!ok || !isValidPlanetType(planetType)) {
        return false;
    }
    
    // Validate planet size
    double size = element.attribute("size").toDouble(&ok);
    if (!ok || !isValidPlanetSize(size)) {
        return false;
    }
    
    // Validate planet mass
    double mass = element.attribute("mass").toDouble(&ok);
    if (!ok || !isValidPlanetMass(mass)) {
        return false;
    }
    
    // Validate temperatures
    double minTemp = element.attribute("minTemperature").toDouble(&ok);
    if (!ok || !isValidTemperature(minTemp)) {
        return false;
    }
    
    double maxTemp = element.attribute("maxTemperature").toDouble(&ok);
    if (!ok || !isValidTemperature(maxTemp)) {
        return false;
    }
    
    if (minTemp >= maxTemp) {
        return false; // Min temperature should be less than max
    }
    
    // Validate orbit distance
    double orbitDistance = element.attribute("orbitDistance").toDouble(&ok);
    if (!ok || !isValidOrbitDistance(orbitDistance)) {
        return false;
    }
    
    return true;
}

XmlValidator::XmlType XmlValidator::detectXmlType(const QDomDocument& doc)
{
    QDomElement root = doc.documentElement();
    
    if (root.tagName() == "Galaxy") {
        return XmlType::Galaxy;
    } else if (root.tagName() == "StarSystem") {
        return XmlType::SingleSystem;
    }
    
    return XmlType::Unknown;
}

QString XmlValidator::generateValidationReport(const ValidationResult& result)
{
    QString report;
    QTextStream stream(&report);
    
    stream << "=== XML Validation Report ===\n";
    stream << "Status: " << (result.isValid ? "VALID" : "INVALID") << "\n";
    stream << "Type: ";
    
    switch (result.detectedType) {
        case XmlType::Galaxy:
            stream << "Galaxy XML\n";
            break;
        case XmlType::SingleSystem:
            stream << "Single System XML\n";
            break;
        default:
            stream << "Unknown\n";
            break;
    }
    
    stream << "System Count: " << result.systemCount << "\n";
    
    if (!result.version.isEmpty()) {
        stream << "Version: " << result.version << "\n";
    }
    
    if (!result.errors.isEmpty()) {
        stream << "\n--- ERRORS ---\n";
        for (const QString& error : result.errors) {
            stream << "ERROR: " << error << "\n";
        }
    }
    
    if (!result.warnings.isEmpty()) {
        stream << "\n--- WARNINGS ---\n";
        for (const QString& warning : result.warnings) {
            stream << "WARNING: " << warning << "\n";
        }
    }
    
    if (result.isValid) {
        stream << "\n✓ XML validation passed successfully!\n";
    } else {
        stream << "\n✗ XML validation failed. Please fix the errors above.\n";
    }
    
    return report;
}

QStringList XmlValidator::getSupportedVersions()
{
    return QStringList() << "1.0" << "1.1" << "2.0";
}

// Data range validation methods
bool XmlValidator::isValidStarType(int type)
{
    return type >= 0 && type <= 6; // Based on StarType enum
}

bool XmlValidator::isValidSystemSize(int size)
{
    return size >= 0 && size <= 4; // Based on SystemSize enum
}

bool XmlValidator::isValidPlanetType(int type)
{
    return type >= 0 && type <= 7; // Based on PlanetType enum
}

bool XmlValidator::isValidStarMass(double mass)
{
    return mass >= 0.08 && mass <= 150.0; // Reasonable star mass range in solar masses
}

bool XmlValidator::isValidStarTemperature(double temperature)
{
    return temperature >= 1000.0 && temperature <= 100000.0; // Kelvin
}

bool XmlValidator::isValidStarLuminosity(double luminosity)
{
    return luminosity >= 0.0001 && luminosity <= 1000000.0; // Solar luminosities
}

bool XmlValidator::isValidPlanetMass(double mass)
{
    return mass >= 0.001 && mass <= 5000.0; // Earth masses
}

bool XmlValidator::isValidPlanetSize(double size)
{
    return size >= 0.1 && size <= 20.0; // Earth radii
}

bool XmlValidator::isValidTemperature(double temperature)
{
    return temperature >= 0.0 && temperature <= 10000.0; // Kelvin
}

bool XmlValidator::isValidOrbitDistance(double distance)
{
    return distance >= 0.01 && distance <= 1000.0; // AU
}

// Helper methods
void XmlValidator::addError(QStringList& errors, const QString& element, const QString& message)
{
    errors << formatValidationMessage("ERROR", element, "", message);
}

void XmlValidator::addWarning(QStringList& warnings, const QString& element, const QString& message)
{
    warnings << formatValidationMessage("WARNING", element, "", message);
}

QString XmlValidator::formatValidationMessage(const QString& level, const QString& element, 
                                             const QString& attribute, const QString& message)
{
    QString formatted = QString("[%1]").arg(level);
    
    if (!element.isEmpty()) {
        formatted += QString(" Element '%1'").arg(element);
    }
    
    if (!attribute.isEmpty()) {
        formatted += QString(", Attribute '%1'").arg(attribute);
    }
    
    formatted += QString(": %1").arg(message);
    return formatted;
}

bool XmlValidator::validateAgainstSchema(const QString& filePath, const QString& schemaPath)
{
    Q_UNUSED(filePath)
    Q_UNUSED(schemaPath)
    // TODO: Implement XML schema validation
    // For now, return true as a placeholder
    qWarning() << "validateAgainstSchema not yet implemented";
    return true;
}

QString XmlValidator::generateXmlSchema()
{
    // Generate a basic XSD schema for Galaxy XML format
    return R"(<?xml version="1.0" encoding="UTF-8"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
           targetNamespace="http://galaxybuilder.com/schema"
           xmlns:gb="http://galaxybuilder.com/schema"
           elementFormDefault="qualified">

  <!-- Galaxy root element -->
  <xs:element name="Galaxy">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="Metadata" type="gb:MetadataType" minOccurs="0"/>
        <xs:element name="Systems" type="gb:SystemsType"/>
      </xs:sequence>
    </xs:complexType>
  </xs:element>

  <!-- Metadata type -->
  <xs:complexType name="MetadataType">
    <xs:attribute name="version" type="xs:string" use="required"/>
    <xs:attribute name="systemCount" type="xs:int"/>
    <xs:attribute name="creator" type="xs:string"/>
    <xs:attribute name="created" type="xs:dateTime"/>
  </xs:complexType>

  <!-- Systems container type -->
  <xs:complexType name="SystemsType">
    <xs:sequence>
      <xs:element name="System" type="gb:SystemType" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>

  <!-- System type -->
  <xs:complexType name="SystemType">
    <xs:sequence>
      <xs:element name="Planets" type="gb:PlanetsType" minOccurs="0"/>
    </xs:sequence>
    <xs:attribute name="id" type="xs:string" use="required"/>
    <xs:attribute name="name" type="xs:string" use="required"/>
    <xs:attribute name="positionX" type="xs:double" use="required"/>
    <xs:attribute name="positionY" type="xs:double" use="required"/>
    <xs:attribute name="starType" type="xs:int"/>
    <xs:attribute name="systemSize" type="xs:int"/>
    <xs:attribute name="starMass" type="xs:double"/>
    <xs:attribute name="starTemperature" type="xs:double"/>
    <xs:attribute name="starLuminosity" type="xs:double"/>
  </xs:complexType>

  <!-- Planets container type -->
  <xs:complexType name="PlanetsType">
    <xs:sequence>
      <xs:element name="Planet" type="gb:PlanetType" maxOccurs="unbounded"/>
    </xs:sequence>
  </xs:complexType>

  <!-- Planet type -->
  <xs:complexType name="PlanetType">
    <xs:attribute name="name" type="xs:string" use="required"/>
    <xs:attribute name="type" type="xs:int" use="required"/>
    <xs:attribute name="size" type="xs:double" use="required"/>
    <xs:attribute name="mass" type="xs:double" use="required"/>
    <xs:attribute name="gravity" type="xs:double"/>
    <xs:attribute name="moonCount" type="xs:int"/>
    <xs:attribute name="minTemperature" type="xs:double"/>
    <xs:attribute name="maxTemperature" type="xs:double"/>
    <xs:attribute name="orbitDistance" type="xs:double"/>
  </xs:complexType>

</xs:schema>)";
}

bool XmlValidator::exportValidationSchema(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return false;
    }
    
    QTextStream out(&file);
    out << generateXmlSchema();
    file.close();
    
    return true;
}
