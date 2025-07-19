#ifndef GGH_GALAXYCORE_VIEWMODELS_COMMONS_H
#define GGH_GALAXYCORE_VIEWMODELS_COMMONS_H

#include <QColor>
#include "ggh/modules/GalaxyCore/utilities/Common.h"

namespace ggh::GalaxyCore::viewmodels::commons {

inline QColor starColor(utilities::StarType type) 
{
    switch (type) {
        case utilities::StarType::RedDwarf: return QColor(255, 0, 0);
        case utilities::StarType::YellowStar: return QColor(255, 255, 0);
        case utilities::StarType::BlueStar: return QColor(0, 0, 255);
        case utilities::StarType::WhiteDwarf: return QColor(255, 255, 255);
        case utilities::StarType::RedGiant: return QColor(255, 0, 0);
        case utilities::StarType::Neutron: return QColor(0, 255, 255);
        case utilities::StarType::BlackHole: return QColor(0, 0, 0);
        default: return QColor(128, 128, 128); // Unknown
    }
}

inline QColor planetColor(utilities::PlanetType type) 
{
    switch (type) {
        case utilities::PlanetType::Rocky: return QColor(139, 69, 19); // Brown
        case utilities::PlanetType::GasGiant: return QColor(0, 0, 255); // Blue
        case utilities::PlanetType::IceGiant: return QColor(173, 216, 230); // Light Blue
        case utilities::PlanetType::Desert: return QColor(210, 180, 140); // Tan
        case utilities::PlanetType::Ocean: return QColor(0, 191, 255); // Deep Sky Blue
        case utilities::PlanetType::Frozen: return QColor(240, 248, 255); // Alice Blue
        case utilities::PlanetType::Volcanic: return QColor(255, 69, 0); // Red-Orange
        case utilities::PlanetType::Toxic: return QColor(0, 128, 0); // Green
        default: return QColor(128, 128, 128); // Unknown
    }
}

inline QString planetTypeName(utilities::PlanetType type) 
{
    switch (type) {
        case utilities::PlanetType::Rocky: return "Rocky";
        case utilities::PlanetType::GasGiant: return "Gas Giant";
        case utilities::PlanetType::IceGiant: return "Ice Giant";
        case utilities::PlanetType::Desert: return "Desert";
        case utilities::PlanetType::Ocean: return "Ocean";
        case utilities::PlanetType::Frozen: return "Frozen";
        case utilities::PlanetType::Volcanic: return "Volcanic";
        case utilities::PlanetType::Toxic: return "Toxic";
        default: return "Unknown";
    }
}



} // namespace ggh::GalaxyCore::viewmodels

#endif // !GGH_GALAXYCORE_VIEWMODELS_COMMONS_H