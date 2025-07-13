#pragma once

#include "galaxycore_global.h"
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QStringList>

/**
 * @brief XML validation utility for Galaxy Builder XML files
 * 
 * This class provides comprehensive validation for XML files used in Galaxy Builder,
 * including schema validation, structure verification, and data integrity checks.
 */
class GALAXYCORE_EXPORT XmlValidator : public QObject
{
    Q_OBJECT

public:
    enum class ValidationLevel {
        Basic,      // Basic XML well-formedness
        Structure,  // Structure and required elements
        Data,       // Data type and range validation
        Complete    // All validation levels
    };

    enum class XmlType {
        Unknown,
        SingleSystem,
        Galaxy
    };

    struct ValidationResult {
        bool isValid;
        QStringList errors;
        QStringList warnings;
        XmlType detectedType;
        int systemCount;
        QString version;
    };

    explicit XmlValidator(QObject* parent = nullptr);
    ~XmlValidator() = default;

    // Main validation methods
    Q_INVOKABLE ValidationResult validateFile(const QString& filePath, 
                                             ValidationLevel level = ValidationLevel::Complete);
    Q_INVOKABLE ValidationResult validateXmlString(const QString& xmlContent, 
                                                  ValidationLevel level = ValidationLevel::Complete);
    Q_INVOKABLE bool isValidGalaxyXml(const QString& filePath);
    Q_INVOKABLE bool isValidSystemXml(const QString& filePath);

    // Schema validation
    Q_INVOKABLE bool validateAgainstSchema(const QString& filePath, const QString& schemaPath = "");
    
    // Structure validation
    Q_INVOKABLE bool validateXmlStructure(const QDomDocument& doc, XmlType expectedType = XmlType::Unknown);
    Q_INVOKABLE bool validateSystemElement(const QDomElement& element);
    Q_INVOKABLE bool validatePlanetElement(const QDomElement& element);
    Q_INVOKABLE bool validateMetadataElement(const QDomElement& element);

    // Data validation
    Q_INVOKABLE bool validateSystemData(const QDomElement& element);
    Q_INVOKABLE bool validatePlanetData(const QDomElement& element);
    
    // Utility methods
    Q_INVOKABLE XmlType detectXmlType(const QDomDocument& doc);
    Q_INVOKABLE QString generateValidationReport(const ValidationResult& result);
    Q_INVOKABLE QStringList getSupportedVersions();
    
    // Schema generation
    Q_INVOKABLE QString generateXmlSchema();
    Q_INVOKABLE bool exportValidationSchema(const QString& filePath);

signals:
    void validationStarted(const QString& filePath);
    void validationProgress(int percentage);
    void validationCompleted(bool success);
    void validationError(const QString& error);

private:
    ValidationResult m_lastResult;
    
    // Internal validation methods
    ValidationResult performValidation(const QDomDocument& doc, ValidationLevel level);
    bool validateWellFormedness(const QDomDocument& doc, QStringList& errors);
    bool validateRootElement(const QDomElement& root, QStringList& errors, QStringList& warnings);
    bool validateSystemAttributes(const QDomElement& element, QStringList& errors, QStringList& warnings);
    bool validatePlanetAttributes(const QDomElement& element, QStringList& errors, QStringList& warnings);
    bool validateDataRanges(const QDomElement& element, QStringList& errors, QStringList& warnings);
    
    // Data range validation
    bool isValidStarType(int type);
    bool isValidSystemSize(int size);
    bool isValidPlanetType(int type);
    bool isValidStarMass(double mass);
    bool isValidStarTemperature(double temperature);
    bool isValidStarLuminosity(double luminosity);
    bool isValidPlanetMass(double mass);
    bool isValidPlanetSize(double size);
    bool isValidTemperature(double temperature);
    bool isValidOrbitDistance(double distance);
    
    // Helper methods
    QString formatValidationMessage(const QString& level, const QString& element, 
                                   const QString& attribute, const QString& message);
    void addError(QStringList& errors, const QString& element, const QString& message);
    void addWarning(QStringList& warnings, const QString& element, const QString& message);
};
