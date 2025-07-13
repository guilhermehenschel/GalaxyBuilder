/**
 * @file GalaxyController.h
 * @brief Controller class that manages galaxy generation and provides QML interface.
 */

#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QPointF>
#include <QSizeF>
#include <memory>
#include "galaxycore_global.h"
#include "Galaxy.h"
#include "GalaxyGenerator.h"
#include "StarSystemQml.h"
#include "SystemDataManager.h"
#include "StarSystemListModel.h"
#include "TravelLaneListModel.h"

/**
 * @class GalaxyController
 * @brief Main interface between QML and the core galaxy logic.
 *
 * Handles galaxy generation, parameter management, and exposes properties for QML binding.
 */
class GALAXYCORE_EXPORT GalaxyController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    
    // Galaxy parameters - bindable from QML
    Q_PROPERTY(int systemCount READ systemCount WRITE setSystemCount NOTIFY systemCountChanged)
    Q_PROPERTY(int galaxyWidth READ galaxyWidth WRITE setGalaxyWidth NOTIFY galaxyWidthChanged)
    Q_PROPERTY(int galaxyHeight READ galaxyHeight WRITE setGalaxyHeight NOTIFY galaxyHeightChanged)
    Q_PROPERTY(int galaxyShape READ galaxyShape WRITE setGalaxyShape NOTIFY galaxyShapeChanged)
    Q_PROPERTY(double spiralArms READ spiralArms WRITE setSpiralArms NOTIFY spiralArmsChanged)
    Q_PROPERTY(double spiralTightness READ spiralTightness WRITE setSpiralTightness NOTIFY spiralTightnessChanged)
    Q_PROPERTY(double coreRadius READ coreRadius WRITE setCoreRadius NOTIFY coreRadiusChanged)
    Q_PROPERTY(double edgeRadius READ edgeRadius WRITE setEdgeRadius NOTIFY edgeRadiusChanged)
    Q_PROPERTY(int seed READ seed WRITE setSeed NOTIFY seedChanged)
    
    // Display options
    Q_PROPERTY(bool showSystemNames READ showSystemNames WRITE setShowSystemNames NOTIFY showSystemNamesChanged)
    Q_PROPERTY(bool showTravelLanes READ showTravelLanes WRITE setShowTravelLanes NOTIFY showTravelLanesChanged)
    Q_PROPERTY(bool showInfluenceRadius READ showInfluenceRadius WRITE setShowInfluenceRadius NOTIFY showInfluenceRadiusChanged)
    
    // Status properties
    Q_PROPERTY(bool isGenerating READ isGenerating NOTIFY isGeneratingChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(Galaxy* galaxy READ galaxy NOTIFY galaxyChanged)
    Q_PROPERTY(QObject* systemsModel READ systemsModel NOTIFY galaxyChanged)
    
    // Selected system properties
    Q_PROPERTY(bool hasSelectedSystem READ hasSelectedSystem NOTIFY selectedSystemChanged)
    Q_PROPERTY(int selectedSystemId READ selectedSystemId NOTIFY selectedSystemChanged)
    Q_PROPERTY(StarSystemQml* selectedStarSystem READ selectedStarSystem NOTIFY selectedSystemChanged)
    Q_PROPERTY(QString selectedSystemName READ selectedSystemName WRITE setSelectedSystemName NOTIFY selectedSystemChanged)
    Q_PROPERTY(int selectedSystemType READ selectedSystemType WRITE setSelectedSystemType NOTIFY selectedSystemChanged)
    Q_PROPERTY(int selectedSystemSize READ selectedSystemSize WRITE setSelectedSystemSize NOTIFY selectedSystemChanged)
    Q_PROPERTY(qreal selectedSystemX READ selectedSystemX WRITE setSelectedSystemX NOTIFY selectedSystemChanged)
    Q_PROPERTY(qreal selectedSystemY READ selectedSystemY WRITE setSelectedSystemY NOTIFY selectedSystemChanged)
    
    // Data management
    Q_PROPERTY(SystemDataManager* systemDataManager READ systemDataManager CONSTANT)
public:
    Q_INVOKABLE StarSystemListModel* systemsModel() const { return m_galaxy ? m_galaxy->systemsModel() : nullptr; }
    Q_INVOKABLE TravelLaneListModel* lanesModel() const { return m_galaxy ? m_galaxy->lanesModel() : nullptr; }

public:
    explicit GalaxyController(QObject *parent = nullptr);
    ~GalaxyController() override;

    // Property getters
    int systemCount() const { return m_systemCount; }
    int galaxyWidth() const { return m_galaxyWidth; }
    int galaxyHeight() const { return m_galaxyHeight; }
    int galaxyShape() const { return static_cast<int>(m_galaxyShape); }
    double spiralArms() const { return m_spiralArms; }
    double spiralTightness() const { return m_spiralTightness; }
    double coreRadius() const { return m_coreRadius; }
    double edgeRadius() const { return m_edgeRadius; }
    int seed() const { return m_seed; }
    
    bool showSystemNames() const { return m_showSystemNames; }
    bool showTravelLanes() const { return m_showTravelLanes; }
    bool showInfluenceRadius() const { return m_showInfluenceRadius; }
    
    bool isGenerating() const { return m_isGenerating; }
    QString statusMessage() const { return m_statusMessage; }
    Galaxy* galaxy() const { return m_galaxy.get(); }
    SystemDataManager* systemDataManager() const { return m_systemDataManager.get(); }
    
    // Selected system getters
    bool hasSelectedSystem() const { return m_selectedSystemId != 0; }
    int selectedSystemId() const { return static_cast<int>(m_selectedSystemId); }
    QString selectedSystemName() const;
    int selectedSystemType() const;
    int selectedSystemSize() const;
    qreal selectedSystemX() const;
    qreal selectedSystemY() const;
    StarSystemQml* selectedStarSystem() const;

    // Property setters
    void setSystemCount(int count);
    void setGalaxyWidth(int width);
    void setGalaxyHeight(int height);
    void setGalaxyShape(int shape);
    void setSpiralArms(double arms);
    void setSpiralTightness(double tightness);
    void setCoreRadius(double radius);
    void setEdgeRadius(double radius);
    void setSeed(int seed);
    
    void setShowSystemNames(bool show);
    void setShowTravelLanes(bool show);
    void setShowInfluenceRadius(bool show);
    
    // Selected system setters
    void setSelectedSystemName(const QString& name);
    void setSelectedSystemType(int type);
    void setSelectedSystemSize(int size);
    void setSelectedSystemX(qreal x);
    void setSelectedSystemY(qreal y);

public slots:
    /**
     * @brief Generate a new galaxy with current parameters.
     */
    void generateGalaxy();
    
    /**
     * @brief Generate a new galaxy with a random seed.
     */
    void generateRandomGalaxy();
    
    /**
     * @brief Export the galaxy as an image.
     * @param filePath The file path to save the image.
     * @param size The size of the image.
     */
    void exportGalaxyImage(const QString& filePath, const QSizeF& size = QSizeF(1920, 1080));
    
    /**
     * @brief Get system information at the given position
     * @param position Position in galaxy coordinates
     * @return System information or empty string if no system found
     */
    QString getSystemInfoAt(const QPointF& position);
    
    /**
     * @brief Select system at the given position
     * @param position Position in galaxy coordinates
     */
    void selectSystemAt(const QPointF& position);
    
    /**
     * @brief Select system by ID
     * @param systemId ID of the system to select
     */
    Q_INVOKABLE void selectSystem(int systemId);
    
    /**
     * @brief Open system properties dialog for the given system
     * @param systemId ID of the system to open properties for
     */
    Q_INVOKABLE void openSystemProperties(int systemId);
    
    /**
     * @brief Clear system selection.
     */
    Q_INVOKABLE void clearSelection();

signals:
    // Parameter change signals
    void systemCountChanged();
    void galaxyWidthChanged();
    void galaxyHeightChanged();
    void galaxyShapeChanged();
    void spiralArmsChanged();
    void spiralTightnessChanged();
    void coreRadiusChanged();
    void edgeRadiusChanged();
    void seedChanged();
    
    // Display option signals
    void showSystemNamesChanged();
    void showTravelLanesChanged();
    void showInfluenceRadiusChanged();
    
    // Status signals
    void isGeneratingChanged();
    void statusMessageChanged();
    void galaxyChanged();
    
    // Selection signals
    void selectedSystemChanged();
    
    // Action signals
    void systemSelected(SystemId systemId);
    void systemHovered(SystemId systemId);
    void galaxyGenerated();

private slots:
    void onGenerationFinished();

private:
    void setStatusMessage(const QString& message);
    void setIsGenerating(bool generating);
    GalaxyGenerator::GenerationParameters getGenerationParameters() const;

private:
    // Core objects
    std::unique_ptr<GalaxyGenerator> m_generator;
    std::unique_ptr<Galaxy> m_galaxy;
    std::unique_ptr<SystemDataManager> m_systemDataManager;
    QTimer* m_generationTimer;
    
    // Galaxy parameters
    int m_systemCount{50};
    int m_galaxyWidth{1000};
    int m_galaxyHeight{1000};
    GalaxyShape m_galaxyShape{GalaxyShape::Spiral};
    double m_spiralArms{2.0};
    double m_spiralTightness{0.5};
    double m_coreRadius{0.2};
    double m_edgeRadius{0.8};
    int m_seed{0};
    
    // Display options
    bool m_showSystemNames{true};
    bool m_showTravelLanes{true};
    bool m_showInfluenceRadius{false};
    
    // Status
    bool m_isGenerating{false};
    QString m_statusMessage{"Ready"};
    
    // Selection
    SystemId m_selectedSystemId{0};
};

// Register QML type
QML_DECLARE_TYPE(GalaxyController)
