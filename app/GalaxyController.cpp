#include "GalaxyController.h"
#include <QDebug>
#include <QRandomGenerator>

GalaxyController::GalaxyController(QObject *parent)
    : QObject(parent)
    , m_galaxyModel(nullptr)
    , m_galaxyViewModel(nullptr)
    , m_galaxyGenerator(nullptr)
    , m_exporterObject(nullptr)
    , m_statusMessage("Ready")
    , m_isGenerating(false)
    , m_showSystemNames(true)
    , m_showTravelLanes(true)
    , m_showInfluenceRadius(false)
    , m_systemCount(50)
    , m_galaxyWidth(1000)
    , m_galaxyHeight(1000)
    , m_galaxyShape(0)
    , m_spiralArms(2)
    , m_spiralTightness(1.0)
    , m_coreRadius(0.1)
    , m_edgeRadius(0.9)
    , m_randomizeParameters(false)
    , m_selectedSystemId(0)
    , m_selectedStarSystemViewModel(nullptr)
{
    initializeModels();
}

GalaxyController::~GalaxyController()
{
    delete m_galaxyGenerator;
}

GalaxyController* GalaxyController::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    return new GalaxyController();
}

void GalaxyController::initializeModels()
{
    // Create the core galaxy model with default dimensions
    m_galaxyModel = std::make_shared<ggh::GalaxyCore::models::GalaxyModel>(1000, 1000);
    
    // Create the view model
    m_galaxyViewModel = new ggh::GalaxyCore::viewmodels::GalaxyViewModel(this);
    m_galaxyViewModel->setGalaxy(m_galaxyModel);
    
    // Create the galaxy generator
    m_galaxyGenerator = new ggh::GalaxyFactories::GalaxyGenerator();
    
    // Set up default parameters and sync with member variables
    ggh::GalaxyFactories::GenerationParameters defaultParams;
    defaultParams.systemCount = m_systemCount;
    defaultParams.width = m_galaxyWidth;
    defaultParams.height = m_galaxyHeight;
    defaultParams.shape = static_cast<ggh::GalaxyCore::utilities::GalaxyShape>(m_galaxyShape);
    defaultParams.spiralArms = m_spiralArms;
    defaultParams.spiralTightness = m_spiralTightness;
    defaultParams.coreRadius = m_coreRadius;
    defaultParams.edgeRadius = m_edgeRadius;
    defaultParams.seed = QRandomGenerator::global()->bounded(1000000);
    m_galaxyGenerator->setParameters(defaultParams);
    
    // Create the exporter object
    m_exporterObject = new ggh::Galaxy::Exporter::ExporterObject(this);
    
    qDebug() << "GalaxyController initialized successfully";
}

ggh::GalaxyCore::viewmodels::GalaxyViewModel* GalaxyController::galaxyViewModel() const
{
    return m_galaxyViewModel;
}

ggh::GalaxyFactories::GalaxyGenerator* GalaxyController::galaxyGenerator() const
{
    return m_galaxyGenerator;
}

ggh::Galaxy::Exporter::ExporterObject* GalaxyController::exporterObject() const
{
    return m_exporterObject;
}

void GalaxyController::generateGalaxy()
{
    if (!m_galaxyGenerator) {
        qWarning() << "Galaxy generator not initialized";
        m_statusMessage = "Error: Galaxy generator not initialized";
        emit statusMessageChanged();
        return;
    }
    
    m_isGenerating = true;
    emit isGeneratingChanged();
    emit galaxyGenerationStarted();
    
    m_statusMessage = "Generating galaxy...";
    emit statusMessageChanged();
    
    auto params = m_galaxyGenerator->getParameters();
    qDebug() << "Generating galaxy with parameters:"
             << "Systems:" << params.systemCount
             << "Size:" << params.width << "x" << params.height
             << "Seed:" << params.seed;
    
    try {
        auto newGalaxy = m_galaxyGenerator->generateGalaxy();
        
        if (newGalaxy) {
            m_galaxyModel = std::shared_ptr<ggh::GalaxyCore::models::GalaxyModel>(newGalaxy.release());
            m_galaxyViewModel->setGalaxy(m_galaxyModel);
            emit galaxyViewModelChanged();
            
            m_statusMessage = QString("Galaxy generated successfully with %1 star systems")
                             .arg(m_galaxyModel->getAllStarSystems().size());
            emit statusMessageChanged();
            
            qDebug() << "Galaxy generated successfully with" 
                     << m_galaxyModel->getAllStarSystems().size() << "star systems";
        } else {
            qWarning() << "Failed to generate galaxy - null result";
            m_statusMessage = "Failed to generate galaxy";
            emit statusMessageChanged();
        }
        
    } catch (const std::exception& e) {
        qCritical() << "Exception during galaxy generation:" << e.what();
        m_statusMessage = QString("Error during generation: %1").arg(e.what());
        emit statusMessageChanged();
    }
    
    m_isGenerating = false;
    emit isGeneratingChanged();
    emit galaxyGenerationFinished();
}

void GalaxyController::generateRandomGalaxy()
{
    if (!m_galaxyGenerator) {
        qWarning() << "Galaxy generator not initialized";
        m_statusMessage = "Error: Galaxy generator not initialized";
        emit statusMessageChanged();
        return;
    }
    
    // Set a random seed
    auto params = m_galaxyGenerator->getParameters();
    params.seed = QRandomGenerator::global()->bounded(1000000);
    m_galaxyGenerator->setParameters(params);
    
    // Generate with the random seed
    generateGalaxy();
}

void GalaxyController::generateGalaxyWithParameters(int starSystemCount, double galaxyWidth, double galaxyHeight, int seed)
{
    if (!m_galaxyGenerator) {
        qWarning() << "Galaxy generator not initialized";
        return;
    }
    
    // Update parameters
    ggh::GalaxyFactories::GenerationParameters params;
    params.systemCount = starSystemCount;
    params.width = static_cast<int>(galaxyWidth);
    params.height = static_cast<int>(galaxyHeight);
    
    if (seed >= 0) {
        params.seed = seed;
    } else {
        params.seed = QRandomGenerator::global()->bounded(1000000);
    }
    
    m_galaxyGenerator->setParameters(params);
    emit galaxyGeneratorChanged();
    
    // Generate with new parameters
    generateGalaxy();
}

void GalaxyController::exportGalaxy(const QString& filePath)
{
    if (!m_exporterObject || !m_galaxyViewModel) {
        qWarning() << "Exporter or galaxy view model not initialized";
        emit exportFinished(false, "Exporter not initialized");
        return;
    }
    
    emit exportStarted();
    
    try {
        // Set the galaxy model for export
        m_exporterObject->setModel(m_galaxyViewModel);
        m_exporterObject->setFilePath(filePath);
        
        bool success = m_exporterObject->exportObject();
        QString message = success ? "Galaxy exported successfully" : m_exporterObject->errorString();
        
        qDebug() << "Galaxy export to" << filePath << (success ? "succeeded" : "failed");
        if (!success) {
            qWarning() << "Export error:" << message;
        }
        
        emit exportFinished(success, message);
        
    } catch (const std::exception& e) {
        QString errorMsg = QString("Exception during export: %1").arg(e.what());
        qCritical() << errorMsg;
        emit exportFinished(false, errorMsg);
    }
}

void GalaxyController::exportGalaxyImage(const QString& filePath, const QSize& size)
{
    Q_UNUSED(size)
    
    if (!m_galaxyModel) {
        qWarning() << "Galaxy model not initialized";
        emit exportFinished(false, "Galaxy not generated yet");
        return;
    }
    
    qDebug() << "Galaxy image export requested to:" << filePath;
    
    // For now, just indicate that image export is not implemented
    QString message = "Galaxy image export not yet implemented";
    qWarning() << message;
    emit exportFinished(false, message);
}

void GalaxyController::exportStarSystem(const QString& systemName, const QString& filePath)
{
    if (!m_exporterObject || !m_galaxyModel) {
        qWarning() << "Exporter or galaxy model not initialized";
        emit exportFinished(false, "Exporter not initialized");
        return;
    }
    
    emit exportStarted();
    
    try {
        // Find the star system by name
        const auto& systems = m_galaxyModel->getAllStarSystems();
        auto it = std::find_if(systems.begin(), systems.end(),
            [&systemName](const auto& system) {
                return QString::fromStdString(system->getName()) == systemName;
            });
        
        if (it == systems.end()) {
            QString errorMsg = QString("Star system '%1' not found").arg(systemName);
            qWarning() << errorMsg;
            emit exportFinished(false, errorMsg);
            return;
        }
        
        // Create a star system view model for export
        auto systemViewModel = new ggh::GalaxyCore::viewmodels::StarSystemViewModel(this);
        systemViewModel->setStarSystem(*it);
        
        // Set up exporter
        m_exporterObject->setModel(systemViewModel);
        m_exporterObject->setFilePath(filePath);
        
        bool success = m_exporterObject->exportObject();
        QString message = success ? QString("Star system '%1' exported successfully").arg(systemName) 
                                  : m_exporterObject->errorString();
        
        qDebug() << "Star system" << systemName << "export to" << filePath << (success ? "succeeded" : "failed");
        if (!success) {
            qWarning() << "Export error:" << message;
        }
        
        emit exportFinished(success, message);
        
        // Clean up
        systemViewModel->deleteLater();
        
    } catch (const std::exception& e) {
        QString errorMsg = QString("Exception during star system export: %1").arg(e.what());
        qCritical() << errorMsg;
        emit exportFinished(false, errorMsg);
    }
}

void GalaxyController::setGenerationSeed(int seed)
{
    if (m_galaxyGenerator) {
        auto params = m_galaxyGenerator->getParameters();
        params.seed = seed;
        m_galaxyGenerator->setParameters(params);
        emit galaxyGeneratorChanged();
    }
}

void GalaxyController::setGalaxyDimensions(double width, double height)
{
    if (m_galaxyGenerator) {
        auto params = m_galaxyGenerator->getParameters();
        params.width = static_cast<int>(width);
        params.height = static_cast<int>(height);
        m_galaxyGenerator->setParameters(params);
        emit galaxyGeneratorChanged();
    }
}

void GalaxyController::setStarSystemCount(int count)
{
    if (m_galaxyGenerator) {
        auto params = m_galaxyGenerator->getParameters();
        params.systemCount = count;
        m_galaxyGenerator->setParameters(params);
        emit galaxyGeneratorChanged();
    }
}

// UI state property implementations
QString GalaxyController::statusMessage() const
{
    return m_statusMessage;
}

bool GalaxyController::isGenerating() const
{
    return m_isGenerating;
}

bool GalaxyController::showSystemNames() const
{
    return m_showSystemNames;
}

bool GalaxyController::showTravelLanes() const
{
    return m_showTravelLanes;
}

void GalaxyController::setShowSystemNames(bool show)
{
    if (m_showSystemNames != show) {
        m_showSystemNames = show;
        emit showSystemNamesChanged();
    }
}

void GalaxyController::setShowTravelLanes(bool show)
{
    if (m_showTravelLanes != show) {
        m_showTravelLanes = show;
        emit showTravelLanesChanged();
    }
}

bool GalaxyController::showInfluenceRadius() const
{
    return m_showInfluenceRadius;
}

void GalaxyController::setShowInfluenceRadius(bool show)
{
    if (m_showInfluenceRadius != show) {
        m_showInfluenceRadius = show;
        emit showInfluenceRadiusChanged();
    }
}

// Galaxy generation parameter implementations
int GalaxyController::systemCount() const
{
    return m_systemCount;
}

void GalaxyController::setSystemCount(int count)
{
    if (m_systemCount != count) {
        m_systemCount = count;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.systemCount = count;
            m_galaxyGenerator->setParameters(params);
        }
        emit systemCountChanged();
    }
}

int GalaxyController::galaxyWidth() const
{
    return m_galaxyWidth;
}

void GalaxyController::setGalaxyWidth(int width)
{
    if (m_galaxyWidth != width) {
        m_galaxyWidth = width;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.width = width;
            m_galaxyGenerator->setParameters(params);
        }
        emit galaxyWidthChanged();
    }
}

int GalaxyController::galaxyHeight() const
{
    return m_galaxyHeight;
}

void GalaxyController::setGalaxyHeight(int height)
{
    if (m_galaxyHeight != height) {
        m_galaxyHeight = height;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.height = height;
            m_galaxyGenerator->setParameters(params);
        }
        emit galaxyHeightChanged();
    }
}

int GalaxyController::galaxyShape() const
{
    return m_galaxyShape;
}

void GalaxyController::setGalaxyShape(int shape)
{
    if (m_galaxyShape != shape) {
        m_galaxyShape = shape;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.shape = static_cast<ggh::GalaxyCore::utilities::GalaxyShape>(shape);
            m_galaxyGenerator->setParameters(params);
        }
        emit galaxyShapeChanged();
    }
}

int GalaxyController::spiralArms() const
{
    return m_spiralArms;
}

void GalaxyController::setSpiralArms(int arms)
{
    if (m_spiralArms != arms) {
        m_spiralArms = arms;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.spiralArms = arms;
            m_galaxyGenerator->setParameters(params);
        }
        emit spiralArmsChanged();
    }
}

double GalaxyController::spiralTightness() const
{
    return m_spiralTightness;
}

void GalaxyController::setSpiralTightness(double tightness)
{
    if (m_spiralTightness != tightness) {
        m_spiralTightness = tightness;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.spiralTightness = tightness;
            m_galaxyGenerator->setParameters(params);
        }
        emit spiralTightnessChanged();
    }
}

double GalaxyController::coreRadius() const
{
    return m_coreRadius;
}

void GalaxyController::setCoreRadius(double radius)
{
    if (m_coreRadius != radius) {
        m_coreRadius = radius;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.coreRadius = radius;
            m_galaxyGenerator->setParameters(params);
        }
        emit coreRadiusChanged();
    }
}

double GalaxyController::edgeRadius() const
{
    return m_edgeRadius;
}

void GalaxyController::setEdgeRadius(double radius)
{
    if (m_edgeRadius != radius) {
        m_edgeRadius = radius;
        if (m_galaxyGenerator) {
            auto params = m_galaxyGenerator->getParameters();
            params.edgeRadius = radius;
            m_galaxyGenerator->setParameters(params);
        }
        emit edgeRadiusChanged();
    }
}

bool GalaxyController::randomizeParameters() const
{
    return m_randomizeParameters;
}

void GalaxyController::setRandomizeParameters(bool randomize)
{
    if (m_randomizeParameters != randomize) {
        m_randomizeParameters = randomize;
        emit randomizeParametersChanged();
    }
}

// System selection implementation

ggh::GalaxyCore::viewmodels::StarSystemViewModel* GalaxyController::selectedStarSystemViewModel() const
{
    return m_selectedStarSystemViewModel;
}

bool GalaxyController::hasSelectedSystem() const
{
    return m_selectedStarSystemViewModel != nullptr;
}

void GalaxyController::selectSystem(quint32 systemId)
{
    if (m_selectedSystemId == systemId && m_selectedStarSystemViewModel != nullptr) {
        return; // Already selected
    }
    
    m_selectedSystemId = systemId;
    updateSelectedStarSystemViewModel();
}

void GalaxyController::clearSelection()
{
    if (m_selectedStarSystemViewModel == nullptr) {
        return; // Already cleared
    }
    
    m_selectedSystemId = 0;
    if (m_selectedStarSystemViewModel) {
        m_selectedStarSystemViewModel->deleteLater();
        m_selectedStarSystemViewModel = nullptr;
    }
    
    emit selectedStarSystemViewModelChanged();
    emit hasSelectedSystemChanged();
}

ggh::GalaxyCore::viewmodels::StarSystemViewModel* GalaxyController::getStarSystemViewModel(quint32 systemId)
{
    if (!m_galaxyModel) {
        return nullptr;
    }
    
    auto starSystemModel = m_galaxyModel->getStarSystem(systemId);
    if (!starSystemModel) {
        return nullptr;
    }
    
    return new ggh::GalaxyCore::viewmodels::StarSystemViewModel(starSystemModel, this);
}

void GalaxyController::updateSelectedStarSystemViewModel()
{
    if (m_selectedStarSystemViewModel) {
        m_selectedStarSystemViewModel->deleteLater();
        m_selectedStarSystemViewModel = nullptr;
    }
    
    if (m_selectedSystemId != 0 && m_galaxyModel) {
        auto starSystemModel = m_galaxyModel->getStarSystem(m_selectedSystemId);
        if (starSystemModel) {
            m_selectedStarSystemViewModel = new ggh::GalaxyCore::viewmodels::StarSystemViewModel(starSystemModel, this);
        }
    }
    
    emit selectedStarSystemViewModelChanged();
    emit hasSelectedSystemChanged();
}
