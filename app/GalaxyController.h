#ifndef GALAXYCONTROLLER_H
#define GALAXYCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqml.h>
#include <QString>
#include <QSize>

// GalaxyCore includes
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/GalaxyViewModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"

// GalaxyFactories includes
#include "ggh/modules/GalaxyFactories/GalaxyGenerator.h"
#include "ggh/modules/GalaxyFactories/Types.h"

// GalaxyExporter includes
#include "ggh/modules/GalaxyExporter/ExporterObject.h"
#include "ggh/modules/GalaxyFactories/XmlGalaxyImporter.h"

// GalaxyFactions includes
#include "ggh/modules/GalaxyFactions/models/GalaxyFactions.h"
#include "ggh/modules/GalaxyFactions/viewmodels/GalaxyFactionsViewModel.h"

class GalaxyController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    
    Q_PROPERTY(ggh::GalaxyCore::viewmodels::GalaxyViewModel* galaxyViewModel READ galaxyViewModel NOTIFY galaxyViewModelChanged)
    Q_PROPERTY(ggh::GalaxyFactories::GalaxyGenerator* galaxyGenerator READ galaxyGenerator NOTIFY galaxyGeneratorChanged)
    Q_PROPERTY(ggh::Galaxy::Exporter::ExporterObject* exporterObject READ exporterObject NOTIFY exporterObjectChanged)
    Q_PROPERTY(ggh::GalaxyFactories::XmlGalaxyImporter* xmlImporter READ xmlImporter NOTIFY xmlImporterChanged)
    Q_PROPERTY(ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel* galaxyFactionsViewModel READ galaxyFactionsViewModel NOTIFY galaxyFactionsViewModelChanged)
    
    // System selection properties
    Q_PROPERTY(ggh::GalaxyCore::viewmodels::StarSystemViewModel* selectedStarSystemViewModel READ selectedStarSystemViewModel NOTIFY selectedStarSystemViewModelChanged)
    Q_PROPERTY(bool hasSelectedSystem READ hasSelectedSystem NOTIFY hasSelectedSystemChanged)
    
    // UI state properties
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(bool isGenerating READ isGenerating NOTIFY isGeneratingChanged)
    Q_PROPERTY(bool showSystemNames READ showSystemNames WRITE setShowSystemNames NOTIFY showSystemNamesChanged)
    Q_PROPERTY(bool showTravelLanes READ showTravelLanes WRITE setShowTravelLanes NOTIFY showTravelLanesChanged)
    Q_PROPERTY(bool showInfluenceRadius READ showInfluenceRadius WRITE setShowInfluenceRadius NOTIFY showInfluenceRadiusChanged)
    
    // Galaxy generation parameters
    Q_PROPERTY(int systemCount READ systemCount WRITE setSystemCount NOTIFY systemCountChanged)
    Q_PROPERTY(int galaxyWidth READ galaxyWidth WRITE setGalaxyWidth NOTIFY galaxyWidthChanged)
    Q_PROPERTY(int galaxyHeight READ galaxyHeight WRITE setGalaxyHeight NOTIFY galaxyHeightChanged)
    Q_PROPERTY(int galaxyShape READ galaxyShape WRITE setGalaxyShape NOTIFY galaxyShapeChanged)
    Q_PROPERTY(int spiralArms READ spiralArms WRITE setSpiralArms NOTIFY spiralArmsChanged)
    Q_PROPERTY(double spiralTightness READ spiralTightness WRITE setSpiralTightness NOTIFY spiralTightnessChanged)
    Q_PROPERTY(double coreRadius READ coreRadius WRITE setCoreRadius NOTIFY coreRadiusChanged)
    Q_PROPERTY(double edgeRadius READ edgeRadius WRITE setEdgeRadius NOTIFY edgeRadiusChanged)
    Q_PROPERTY(bool randomizeParameters READ randomizeParameters WRITE setRandomizeParameters NOTIFY randomizeParametersChanged)

public:
    explicit GalaxyController(QObject *parent = nullptr);
    ~GalaxyController() override;

    // Static function for QML singleton factory
    static GalaxyController* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    // Property getters
    ggh::GalaxyCore::viewmodels::GalaxyViewModel* galaxyViewModel() const;
    ggh::GalaxyFactories::GalaxyGenerator* galaxyGenerator() const;
    ggh::Galaxy::Exporter::ExporterObject* exporterObject() const;
    ggh::GalaxyFactories::XmlGalaxyImporter* xmlImporter() const;
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel* galaxyFactionsViewModel() const;
    
    // System selection property getters
    ggh::GalaxyCore::viewmodels::StarSystemViewModel* selectedStarSystemViewModel() const;
    bool hasSelectedSystem() const;
    
    // UI state property getters
    QString statusMessage() const;
    bool isGenerating() const;
    bool showSystemNames() const;
    bool showTravelLanes() const;
    bool showInfluenceRadius() const;
    
    // Galaxy generation parameter getters
    int systemCount() const;
    int galaxyWidth() const;
    int galaxyHeight() const;
    int galaxyShape() const;
    int spiralArms() const;
    double spiralTightness() const;
    double coreRadius() const;
    double edgeRadius() const;
    bool randomizeParameters() const;
    
    // UI state property setters
    void setShowSystemNames(bool show);
    void setShowTravelLanes(bool show);
    void setShowInfluenceRadius(bool show);
    
    // Galaxy generation parameter setters
    void setSystemCount(int count);
    void setGalaxyWidth(int width);
    void setGalaxyHeight(int height);
    void setGalaxyShape(int shape);
    void setSpiralArms(int arms);
    void setSpiralTightness(double tightness);
    void setCoreRadius(double radius);
    void setEdgeRadius(double radius);
    void setRandomizeParameters(bool randomize);

public slots:
    // Galaxy generation functions
    void generateGalaxy();
    void generateRandomGalaxy();
    void generateGalaxyWithParameters(int starSystemCount, double galaxyWidth, double galaxyHeight, int seed = -1);
    
    // System selection functions
    Q_INVOKABLE void selectSystem(quint32 systemId);
    Q_INVOKABLE void clearSelection();
    Q_INVOKABLE ggh::GalaxyCore::viewmodels::StarSystemViewModel* getStarSystemViewModel(quint32 systemId);
    
    // Export functions
    void exportGalaxy(const QString& filePath);
    void exportStarSystem(const QString& systemName, const QString& filePath);
    void exportGalaxyImage(const QString& filePath, const QSize& size);
    
    // Import functions
    Q_INVOKABLE bool importGalaxy(const QString& filePath);
    
    // Parameter management
    void setGenerationSeed(int seed);
    void setGalaxyDimensions(double width, double height);
    void setStarSystemCount(int count);

signals:
    void galaxyViewModelChanged();
    void galaxyGeneratorChanged();
    void exporterObjectChanged();
    void xmlImporterChanged();
    void galaxyFactionsViewModelChanged();
    
    // System selection signals
    void selectedStarSystemViewModelChanged();
    void hasSelectedSystemChanged();
    
    // UI state signals
    void statusMessageChanged();
    void isGeneratingChanged();
    void showSystemNamesChanged();
    void showTravelLanesChanged();
    void showInfluenceRadiusChanged();
    
    // Galaxy generation parameter signals
    void systemCountChanged();
    void galaxyWidthChanged();
    void galaxyHeightChanged();
    void galaxyShapeChanged();
    void spiralArmsChanged();
    void spiralTightnessChanged();
    void coreRadiusChanged();
    void edgeRadiusChanged();
    void randomizeParametersChanged();
    
    void galaxyGenerationStarted();
    void galaxyGenerationFinished();
    void exportStarted();
    void exportFinished(bool success, const QString& message);
    void importStarted();
    void importFinished(bool success, const QString& message);
    
    // Image export signal
    void captureGalaxyImage(const QString& filePath, int width, int height);

private:
    void initializeModels();
    void updateSelectedStarSystemViewModel();
    
    std::shared_ptr<ggh::GalaxyCore::models::GalaxyModel> m_galaxyModel;
    ggh::GalaxyCore::viewmodels::GalaxyViewModel* m_galaxyViewModel;
    ggh::GalaxyFactories::GalaxyGenerator* m_galaxyGenerator;
    ggh::Galaxy::Exporter::ExporterObject* m_exporterObject;
    ggh::GalaxyFactories::XmlGalaxyImporter* m_xmlImporter;
    
    // GalaxyFactions model and view model
    std::shared_ptr<ggh::Galaxy::Factions::models::GalaxyFactions> m_galaxyFactions;
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel* m_galaxyFactionsViewModel;
    
    // System selection state
    quint32 m_selectedSystemId;
    ggh::GalaxyCore::viewmodels::StarSystemViewModel* m_selectedStarSystemViewModel;
    
    // UI state variables
    QString m_statusMessage;
    bool m_isGenerating;
    bool m_showSystemNames;
    bool m_showTravelLanes;
    bool m_showInfluenceRadius;
    
    // Galaxy generation parameters
    int m_systemCount;
    int m_galaxyWidth;
    int m_galaxyHeight;
    int m_galaxyShape;
    int m_spiralArms;
    double m_spiralTightness;
    double m_coreRadius;
    double m_edgeRadius;
    bool m_randomizeParameters;
};

#endif // GALAXYCONTROLLER_H
