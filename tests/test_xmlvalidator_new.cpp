#include <gtest/gtest.h>
#include <QTemporaryDir>
#include <QTextStream>
#include <QFile>
#include <QCoreApplication>
#include <QTemporaryFile>
#include "../modules/GalaxyCore/XmlValidator.h"

class TestXmlValidator : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;
    
    QString writeToTempFile(const QString& content, const QString& suffix = ".xml");

    QTemporaryDir* m_tempDir;
    XmlValidator* m_validator;
    static QCoreApplication* s_app;
};

QCoreApplication* TestXmlValidator::s_app = nullptr;

void TestXmlValidator::SetUp()
{
    // Create QCoreApplication if it doesn't exist (needed for Qt objects)
    if (!QCoreApplication::instance() && !s_app) {
        int argc = 1;
        char* argv[] = {const_cast<char*>("test_xmlvalidator"), nullptr};
        s_app = new QCoreApplication(argc, argv);
    }
    
    m_tempDir = new QTemporaryDir;
    EXPECT_TRUE(m_tempDir->isValid());
    m_validator = new XmlValidator();
}

void TestXmlValidator::TearDown()
{
    delete m_validator;
    delete m_tempDir;
}

TEST_F(TestXmlValidator, ValidXmlFile)
{
    QString validXml = R"(<?xml version="1.0"?>
<Galaxy>
    <Systems>
        <System id="1" name="Sol" positionX="0.0" positionY="0.0" 
                starType="0" systemSize="1" starMass="1.0" 
                starTemperature="5778.0" starLuminosity="1.0">
            <Planets>
                <Planet name="Earth" type="0" size="1.0" mass="1.0" 
                        gravity="9.81" moonCount="1" minTemperature="184" 
                        maxTemperature="330" orbitDistance="1.0" />
            </Planets>
        </System>
    </Systems>
</Galaxy>)";

    QString filePath = writeToTempFile(validXml);
    auto result = m_validator->validateFile(filePath);
    
    EXPECT_TRUE(result.isValid);
    EXPECT_EQ(result.errors.size(), 0);
    EXPECT_EQ(result.detectedType, XmlValidator::XmlType::Galaxy);
}

TEST_F(TestXmlValidator, InvalidXmlFile)
{
    QString invalidXml = R"(<?xml version="1.0"?>
<Galaxy>
    <Systems>
        <System id="1" name="Sol" positionX="0.0" positionY="0.0">
            <Planets>
                <Planet name="Earth" type="0" />
            </Planets>
        <!-- Missing closing System tag -->
    </Systems>
</Galaxy>)";

    QString filePath = writeToTempFile(invalidXml);
    auto result = m_validator->validateFile(filePath);
    
    EXPECT_FALSE(result.isValid);
    EXPECT_GT(result.errors.size(), 0);
}

QString TestXmlValidator::writeToTempFile(const QString& content, const QString& suffix)
{
    QTemporaryFile tempFile(m_tempDir->filePath("test_XXXXXX" + suffix));
    tempFile.setAutoRemove(false);
    
    if (!tempFile.open()) {
        qWarning() << "Failed to open temp file";
        return QString();
    }
    
    QTextStream stream(&tempFile);
    stream << content;
    tempFile.close();
    
    return tempFile.fileName();
}
