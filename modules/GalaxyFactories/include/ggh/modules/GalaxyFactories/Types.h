#ifndef GGH_GALAXYFACTORIES_TYPES_H
#define GGH_GALAXYFACTORIES_TYPES_H

#include <sstream>
#include <string>
#include "ggh/modules/GalaxyCore/utilities/Common.h"

using GalaxySize = ggh::GalaxyCore::utilities::GalaxySize;
using GalaxyShape = ggh::GalaxyCore::utilities::GalaxyShape;

namespace ggh::GalaxyFactories {
    struct GenerationParameters {
        GalaxySize systemCount = 50;
        GalaxyShape shape = GalaxyShape::Spiral;
        int width = ggh::GalaxyCore::utilities::DEFAULT_GALAXY_WIDTH;
        int height = ggh::GalaxyCore::utilities::DEFAULT_GALAXY_HEIGHT;
        double spiralArms = 2.0;
        double spiralTightness = 0.5;
        double coreRadius = 0.2;
        double edgeRadius = 0.8;
        int seed = 0; // 0 for random seed

        std::string toXml() const {
            // Convert parameters to XML format
            std::ostringstream oss;
            oss << "<GalaxyParameters>"
                << "<SystemCount>" << systemCount << "</SystemCount>"
                << "<Shape>" << static_cast<int>(shape) << "</Shape>"
                << "<Width>" << width << "</Width>"
                << "<Height>" << height << "</Height>"
                << "<SpiralArms>" << spiralArms << "</SpiralArms>"
                << "<SpiralTightness>" << spiralTightness << "</SpiralTightness>"
                << "<CoreRadius>" << coreRadius << "</CoreRadius>"
                << "<EdgeRadius>" << edgeRadius << "</EdgeRadius>"
                << "<Seed>" << seed << "</Seed>"
                << "</GalaxyParameters>";
            return oss.str();
        }
    };
}
    
#endif // !GGH_GALAXYFACTORIES_TYPES_H
