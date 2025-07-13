#include "StarSystem.h"
#include <QRandomGenerator>
#include <QStringList>
#include <cmath>

StarSystem::StarSystem(SystemId id, const QPointF& position, StarType type)
    : m_id(id), m_position(position), m_starType(type), m_systemSize(SystemSize::Medium) {
    generateName();
}

QColor StarSystem::getStarColor() const noexcept {
    return getColorForStarType(m_starType);
}

double StarSystem::getInfluenceRadius() const noexcept {
    switch (m_systemSize) {
        case SystemSize::Small: return 15.0;
        case SystemSize::Medium: return 20.0;
        case SystemSize::Large: return 25.0;
        default: return 20.0;
    }
}

double StarSystem::distanceTo(const StarSystem& other) const {
    const double dx = m_position.x() - other.m_position.x();
    const double dy = m_position.y() - other.m_position.y();
    return std::sqrt(dx * dx + dy * dy);
}

bool StarSystem::isWithinRange(const StarSystem& other, double maxDistance) const {
    return distanceTo(other) <= maxDistance;
}

void StarSystem::generateName() {
    static const QStringList prefixes = {
        "Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta",
        "Kappa", "Lambda", "Mu", "Nu", "Xi", "Omicron", "Pi", "Rho", "Sigma",
        "Tau", "Upsilon", "Phi", "Chi", "Psi", "Omega"
    };
    
    static const QStringList suffixes = {
        "Centauri", "Draconis", "Orionis", "Cygni", "Lyrae", "Aquilae",
        "Bootis", "Leonis", "Ursae", "Andromedae", "Cassiopeiae", "Persei"
    };
    
    auto* generator = QRandomGenerator::global();
    QString prefix = prefixes.at(generator->bounded(prefixes.size()));
    QString suffix = suffixes.at(generator->bounded(suffixes.size()));
    int number = generator->bounded(1, 9999);
    
    m_name = QString("%1 %2-%3")
        .arg(prefix)
        .arg(suffix)
        .arg(number);
}

QColor StarSystem::getColorForStarType(StarType type) {
    switch (type) {
        case StarType::RedDwarf: return QColor(255, 100, 100);
        case StarType::YellowStar: return QColor(255, 255, 150);
        case StarType::BlueStar: return QColor(150, 200, 255);
        case StarType::WhiteDwarf: return QColor(255, 255, 255);
        case StarType::RedGiant: return QColor(255, 150, 100);
        case StarType::Neutron: return QColor(200, 200, 255);
        case StarType::BlackHole: return QColor(100, 50, 150);
        default: return QColor(255, 255, 150);
    }
}
