#ifndef GGH_GALAXYCORE_VIEWMODELS_COMMONS_H
#define GGH_GALAXYCORE_VIEWMODELS_COMMONS_H

#include <QColor>
#include "ggh/modules/GalaxyCore/utilities/Common.h"

namespace ggh::GalaxyCore::viewmodels::commons {

QColor starColor(utilities::StarType type) 
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

} // namespace ggh::GalaxyCore::viewmodels

#endif // !GGH_GALAXYCORE_VIEWMODELS_COMMONS_H