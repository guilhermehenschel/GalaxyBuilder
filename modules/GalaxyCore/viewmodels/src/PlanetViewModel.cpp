#include "ggh/modules/GalaxyCore/viewmodels/PlanetViewModel.h"

namespace ggh::GalaxyCore::viewmodels {

PlanetViewModel::PlanetViewModel(QObject* parent)
    : QObject(parent), m_planet(std::make_shared<models::Planet>())
{
    connectToModel();
}

PlanetViewModel::PlanetViewModel(std::shared_ptr<models::Planet> planet, QObject* parent)
    : QObject(parent), m_planet(planet)
{
    if (!m_planet) {
        m_planet = std::make_shared<models::Planet>();
    }
    connectToModel();
}

QString PlanetViewModel::name() const
{
    return QString::fromStdString(m_planet->name());
}

int PlanetViewModel::planetType() const
{
    return static_cast<int>(m_planet->type());
}

double PlanetViewModel::size() const
{
    return m_planet->size();
}

double PlanetViewModel::mass() const
{
    return m_planet->mass();
}

int PlanetViewModel::numberOfMoons() const
{
    return m_planet->numberOfMoons();
}

double PlanetViewModel::orbitalRadius() const
{
    return m_planet->orbitalRadius();
}

double PlanetViewModel::maxTemperature() const
{
    return m_planet->maxTemperature();
}

double PlanetViewModel::minTemperature() const
{
    return m_planet->minTemperature();
}

void PlanetViewModel::setName(const QString& name)
{
    if (QString::fromStdString(m_planet->name()) != name) {
        m_planet->setName(name.toStdString());
        emit nameChanged();
    }
}

void PlanetViewModel::setPlanetType(int type)
{
    auto planetType = static_cast<PlanetType>(type);
    if (m_planet->type() != planetType) {
        m_planet->setType(planetType);
        emit planetTypeChanged();
    }
}

void PlanetViewModel::setSize(double size)
{
    if (m_planet->size() != size) {
        m_planet->setSize(size);
        emit sizeChanged();
    }
}

void PlanetViewModel::setMass(double mass)
{
    if (m_planet->mass() != mass) {
        m_planet->setMass(mass);
        emit massChanged();
    }
}

void PlanetViewModel::setNumberOfMoons(int moons)
{
    if (m_planet->numberOfMoons() != moons) {
        m_planet->setNumberOfMoons(moons);
        emit numberOfMoonsChanged();
    }
}

void PlanetViewModel::setOrbitalRadius(double radius)
{
    if (m_planet->orbitalRadius() != radius) {
        m_planet->setOrbitalRadius(radius);
        emit orbitalRadiusChanged();
    }
}

void PlanetViewModel::setMaxTemperature(double temp)
{
    if (m_planet->maxTemperature() != temp) {
        m_planet->setMaxTemperature(temp);
        emit maxTemperatureChanged();
    }
}

void PlanetViewModel::setMinTemperature(double temp)
{
    if (m_planet->minTemperature() != temp) {
        m_planet->setMinTemperature(temp);
        emit minTemperatureChanged();
    }
}

std::shared_ptr<models::Planet> PlanetViewModel::planet() const
{
    return m_planet;
}

void PlanetViewModel::setPlanet(std::shared_ptr<models::Planet> planet)
{
    if (m_planet != planet) {
        m_planet = planet;
        if (!m_planet) {
            m_planet = std::make_shared<models::Planet>();
        }
        connectToModel();
        
        // Emit all change signals
        emit nameChanged();
        emit planetTypeChanged();
        emit sizeChanged();
        emit massChanged();
        emit numberOfMoonsChanged();
        emit orbitalRadiusChanged();
        emit maxTemperatureChanged();
        emit minTemperatureChanged();
    }
}

QString PlanetViewModel::toXml() const
{
    return QString::fromStdString(m_planet->toXml());
}

QString PlanetViewModel::planetTypeString() const
{
    switch (m_planet->type()) {
        case PlanetType::Rocky: return "Rocky";
        case PlanetType::GasGiant: return "Gas Giant";
        case PlanetType::IceGiant: return "Ice Giant";
        case PlanetType::Desert: return "Desert";
        case PlanetType::Ocean: return "Ocean";
        case PlanetType::Frozen: return "Frozen";
        case PlanetType::Volcanic: return "Volcanic";
        case PlanetType::Toxic: return "Toxic";
        default: return "Unknown";
    }
}

void PlanetViewModel::connectToModel()
{
    // Since the model doesn't inherit from QObject, we don't need to connect to signals
    // All changes will be handled through the ViewModel properties
}

} // namespace ggh::GalaxyCore::viewmodels
