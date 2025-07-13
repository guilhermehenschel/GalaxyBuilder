#include "GalaxyController.h"
#include "StarSystemListModel.h"
#include "StarSystemQml.h"
#include "SystemDataManager.h"
#include "Common.h"

#include <QTimer>
#include <QRandomGenerator>
#include <QImageWriter>
#include <QPainter>
#include <QStandardPaths>
#include <QDir>
#include <QGuiApplication>

GalaxyController::GalaxyController(QObject *parent)
    : QObject(parent)
    , m_generator(std::make_unique<GalaxyGenerator>())
    , m_systemDataManager(std::make_unique<SystemDataManager>(this))
    , m_generationTimer(new QTimer(this))
{
    // Initialize with a random seed
    m_seed = QRandomGenerator::global()->bounded(1000000);
    
    // Setup generation timer for async generation feedback
    m_generationTimer->setSingleShot(true);
    m_generationTimer->setInterval(100); // Short delay to allow UI updates
    connect(m_generationTimer, &QTimer::timeout, this, &GalaxyController::onGenerationFinished);
}

GalaxyController::~GalaxyController() = default;

void GalaxyController::setSystemCount(int count)
{
    if (m_systemCount != count) {
        m_systemCount = count;
        emit systemCountChanged();
    }
}

void GalaxyController::setGalaxyWidth(int width)
{
    if (m_galaxyWidth != width) {
        m_galaxyWidth = width;
        emit galaxyWidthChanged();
    }
}

void GalaxyController::setGalaxyHeight(int height)
{
    if (m_galaxyHeight != height) {
        m_galaxyHeight = height;
        emit galaxyHeightChanged();
    }
}

void GalaxyController::setGalaxyShape(int shape)
{
    auto newShape = static_cast<GalaxyShape>(shape);
    if (m_galaxyShape != newShape) {
        m_galaxyShape = newShape;
        emit galaxyShapeChanged();
    }
}

void GalaxyController::setSpiralArms(double arms)
{
    if (qAbs(m_spiralArms - arms) > 0.001) {
        m_spiralArms = arms;
        emit spiralArmsChanged();
    }
}

void GalaxyController::setSpiralTightness(double tightness)
{
    if (qAbs(m_spiralTightness - tightness) > 0.001) {
        m_spiralTightness = tightness;
        emit spiralTightnessChanged();
    }
}

void GalaxyController::setCoreRadius(double radius)
{
    if (qAbs(m_coreRadius - radius) > 0.001) {
        m_coreRadius = radius;
        emit coreRadiusChanged();
    }
}

void GalaxyController::setEdgeRadius(double radius)
{
    if (qAbs(m_edgeRadius - radius) > 0.001) {
        m_edgeRadius = radius;
        emit edgeRadiusChanged();
    }
}

void GalaxyController::setSeed(int seed)
{
    if (m_seed != seed) {
        m_seed = seed;
        emit seedChanged();
    }
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

void GalaxyController::setShowInfluenceRadius(bool show)
{
    if (m_showInfluenceRadius != show) {
        m_showInfluenceRadius = show;
        emit showInfluenceRadiusChanged();
    }
}

void GalaxyController::generateGalaxy()
{
    if (m_isGenerating) {
        return; // Already generating
    }

    setIsGenerating(true);
    setStatusMessage("Generating galaxy...");

    try {
        auto params = getGenerationParameters();
        auto newGalaxy = m_generator->generateGalaxy(params);
        
        // Set this controller as parent to ensure proper QObject hierarchy
        newGalaxy->setParent(this);
        m_galaxy = std::move(newGalaxy);
        
        // Use timer to simulate async generation and allow UI updates
        m_generationTimer->start();
    } catch (const std::exception& e) {
        setStatusMessage(QString("Generation failed: %1").arg(e.what()));
        setIsGenerating(false);
    }
}

void GalaxyController::generateRandomGalaxy()
{
    // Generate a new random seed
    setSeed(QRandomGenerator::global()->bounded(1000000));
    generateGalaxy();
}

void GalaxyController::exportGalaxyImage(const QString& filePath, const QSizeF& size)
{
    if (!m_galaxy) {
        setStatusMessage("No galaxy to export");
        return;
    }

    // Create image and painter
    QImage image(size.toSize(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::black);
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    // Basic galaxy rendering (this would need to be more sophisticated)
    setStatusMessage("Exporting galaxy image...");
    
    // Draw star systems
    for (const auto& systemPtr : m_galaxy->getSystems()) {
        if (!systemPtr) continue;
        const auto& system = *systemPtr;
        QPointF pos = system.getPosition();
        // Scale position to image size
        pos.setX((pos.x() / m_galaxyWidth) * size.width());
        pos.setY((pos.y() / m_galaxyHeight) * size.height());
        
        // Draw system as a small circle
        painter.setPen(QPen(Qt::white, 2));
        painter.setBrush(QBrush(Qt::yellow));
        painter.drawEllipse(pos, 3, 3);
        
        // Draw system name if enabled
        if (m_showSystemNames) {
            painter.setPen(Qt::white);
            painter.drawText(pos + QPointF(5, 5), system.getName());
        }
    }
    
    // Draw travel lanes if enabled
    if (m_showTravelLanes) {
        painter.setPen(QPen(Qt::cyan, 1));
        for (const auto& lanePtr : m_galaxy->getTravelLanes()) {
            if (!lanePtr) continue;
            const auto& lane = *lanePtr;
            auto fromSystem = m_galaxy->getSystem(lane.getFromSystem());
            auto toSystem = m_galaxy->getSystem(lane.getToSystem());
            
            if (fromSystem && toSystem) {
                QPointF from = fromSystem->getPosition();
                QPointF to = toSystem->getPosition();
                
                // Scale positions to image size
                from.setX((from.x() / m_galaxyWidth) * size.width());
                from.setY((from.y() / m_galaxyHeight) * size.height());
                to.setX((to.x() / m_galaxyWidth) * size.width());
                to.setY((to.y() / m_galaxyHeight) * size.height());
                
                painter.drawLine(from, to);
            }
        }
    }

    // Save image
    QImageWriter writer(filePath);
    if (writer.write(image)) {
        setStatusMessage(QString("Galaxy exported to: %1").arg(filePath));
    } else {
        setStatusMessage(QString("Failed to export galaxy: %1").arg(writer.errorString()));
    }
}

QString GalaxyController::getSystemInfoAt(const QPointF& position)
{
    if (!m_galaxy) {
        return QString();
    }

    // Find closest system within a reasonable distance
    const double maxDistance = 50.0; // pixels or units
    const StarSystem* closestSystem = nullptr;
    double closestDistance = maxDistance;

    for (const auto& systemPtr : m_galaxy->getSystems()) {
        if (!systemPtr) continue;
        const auto& system = *systemPtr;
        QPointF systemPos = system.getPosition();
        double distance = QPointF(position - systemPos).manhattanLength();
        
        if (distance < closestDistance) {
            closestDistance = distance;
            closestSystem = &system;
        }
    }

    if (closestSystem) {
        return QString("System: %1\nPosition: (%2, %3)\nType: %4")
                .arg(closestSystem->getName())
                .arg(closestSystem->getPosition().x(), 0, 'f', 1)
                .arg(closestSystem->getPosition().y(), 0, 'f', 1)
                .arg(static_cast<int>(closestSystem->getStarType()));
    }

    return QString();
}

void GalaxyController::selectSystemAt(const QPointF& position)
{
    if (!m_galaxy) {
        return;
    }

    // Find closest system within a reasonable distance
    const double maxDistance = 50.0;
    const StarSystem* closestSystem = nullptr;
    double closestDistance = maxDistance;

    for (const auto& systemPtr : m_galaxy->getSystems()) {
        if (!systemPtr) continue;
        const auto& system = *systemPtr;
        QPointF systemPos = system.getPosition();
        double distance = QPointF(position - systemPos).manhattanLength();
        
        if (distance < closestDistance) {
            closestDistance = distance;
            closestSystem = &system;
        }
    }

    if (closestSystem) {
        selectSystem(closestSystem->getId());
    } else {
        clearSelection();
    }
}

void GalaxyController::onGenerationFinished()
{
    setIsGenerating(false);
    setStatusMessage("Galaxy generation completed");
    emit galaxyChanged();
    emit galaxyGenerated();
}

void GalaxyController::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message) {
        m_statusMessage = message;
        emit statusMessageChanged();
    }
}

void GalaxyController::setIsGenerating(bool generating)
{
    if (m_isGenerating != generating) {
        m_isGenerating = generating;
        emit isGeneratingChanged();
    }
}

GalaxyGenerator::GenerationParameters GalaxyController::getGenerationParameters() const
{
    GalaxyGenerator::GenerationParameters params;
    params.systemCount = m_systemCount;
    params.width = m_galaxyWidth;
    params.height = m_galaxyHeight;
    params.shape = m_galaxyShape;
    params.spiralArms = m_spiralArms;
    params.spiralTightness = m_spiralTightness;
    params.coreRadius = m_coreRadius;
    params.edgeRadius = m_edgeRadius;
    params.seed = m_seed;
    return params;
}

// Selected system getters
QString GalaxyController::selectedSystemName() const
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return QString();
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    return system ? system->getName() : QString();
}

int GalaxyController::selectedSystemType() const
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return 0;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    return system ? static_cast<int>(system->getStarType()) : 0;
}

int GalaxyController::selectedSystemSize() const
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return 0;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    return system ? static_cast<int>(system->getSystemSize()) : 0;
}

qreal GalaxyController::selectedSystemX() const
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return 0.0;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    return system ? system->getPosition().x() : 0.0;
}

qreal GalaxyController::selectedSystemY() const
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return 0.0;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    return system ? system->getPosition().y() : 0.0;
}

// Selected system setters
void GalaxyController::setSelectedSystemName(const QString& name)
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    if (system && system->getName() != name) {
        system->setName(name);
        emit selectedSystemChanged();
        // Trigger model update
        if (auto model = m_galaxy->systemsModel()) {
            static_cast<StarSystemListModel*>(model)->notifyDataChanged();
        }
    }
}

void GalaxyController::setSelectedSystemType(int type)
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    if (system && static_cast<int>(system->getStarType()) != type) {
        system->setStarType(static_cast<StarType>(type));
        emit selectedSystemChanged();
        // Trigger model update
        if (auto model = m_galaxy->systemsModel()) {
            static_cast<StarSystemListModel*>(model)->notifyDataChanged();
        }
    }
}

void GalaxyController::setSelectedSystemSize(int size)
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    if (system && static_cast<int>(system->getSystemSize()) != size) {
        system->setSystemSize(static_cast<SystemSize>(size));
        emit selectedSystemChanged();
        // Trigger model update
        if (auto model = m_galaxy->systemsModel()) {
            static_cast<StarSystemListModel*>(model)->notifyDataChanged();
        }
    }
}

void GalaxyController::setSelectedSystemX(qreal x)
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    if (system && qAbs(system->getPosition().x() - x) > 0.001) {
        QPointF pos = system->getPosition();
        pos.setX(x);
        system->setPosition(pos);
        emit selectedSystemChanged();
        // Trigger model update
        if (auto model = m_galaxy->systemsModel()) {
            static_cast<StarSystemListModel*>(model)->notifyDataChanged();
        }
    }
}

void GalaxyController::setSelectedSystemY(qreal y)
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return;
    }
    
    auto system = m_galaxy->getSystem(m_selectedSystemId);
    if (system && qAbs(system->getPosition().y() - y) > 0.001) {
        QPointF pos = system->getPosition();
        pos.setY(y);
        system->setPosition(pos);
        emit selectedSystemChanged();
        // Trigger model update
        if (auto model = m_galaxy->systemsModel()) {
            static_cast<StarSystemListModel*>(model)->notifyDataChanged();
        }
    }
}

void GalaxyController::selectSystem(int systemId)
{
    SystemId sysId = static_cast<SystemId>(systemId);
    if (m_selectedSystemId != sysId) {
        m_selectedSystemId = sysId;
        emit selectedSystemChanged();
        if (systemId != 0) {
            emit systemSelected(sysId);
        }
    }
}

void GalaxyController::clearSelection()
{
    if (m_selectedSystemId != 0) {
        m_selectedSystemId = 0;
        emit selectedSystemChanged();
    }
}

void GalaxyController::openSystemProperties(int systemId)
{
    // For now, just select the system. The QML side will handle opening the dialog
    selectSystem(systemId);
    // In the future, we could emit a signal here to open the dialog
}

StarSystemQml* GalaxyController::selectedStarSystem() const
{
    if (!m_galaxy || m_selectedSystemId == 0) {
        return nullptr;
    }
    
    // Find the StarSystem in the galaxy
    const auto& systems = m_galaxy->getSystems();
    auto it = std::find_if(systems.begin(), systems.end(),
                           [this](const auto& system) {
                               return system->getId() == m_selectedSystemId;
                           });
    
    if (it != systems.end()) {
        // Create a new StarSystemQml object for QML use
        auto* starSystem = new StarSystemQml(it->get()->getId(), it->get()->getPosition(), it->get()->getStarType(), const_cast<GalaxyController*>(this));
        starSystem->setName(it->get()->getName());
        starSystem->setSystemSize(it->get()->getSystemSize());
        return starSystem;
    }
    
    return nullptr;
}
