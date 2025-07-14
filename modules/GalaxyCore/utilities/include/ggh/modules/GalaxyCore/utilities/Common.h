#ifndef GGH_MODULES_GALAXYCORE_UTILITIES_COMMON_H
#define GGH_MODULES_GALAXYCORE_UTILITIES_COMMON_H
/**
 * @file Common.h
 * @brief Common utilities and type definitions for the GalaxyCore module.
 *
 * This file contains common types, constants, and utility functions used across the GalaxyCore module.
 */

#include <QMetaType>


namespace ggh::GalaxyCore::utilities {
// Type aliases for better readability
using SystemId = std::uint32_t;
using LaneId = std::uint32_t;
using GalaxySize = std::uint32_t;

// Constants
constexpr double PI = 3.14159265358979323846;
constexpr int DEFAULT_GALAXY_WIDTH = 1000;
constexpr int DEFAULT_GALAXY_HEIGHT = 1000;
constexpr int MIN_SYSTEM_DISTANCE = 50;
constexpr int MAX_TRAVEL_LANE_DISTANCE = 150;

// Enums
enum class StarType {
    RedDwarf,
    YellowStar,
    BlueStar,
    WhiteDwarf,
    RedGiant,
    Neutron,
    BlackHole
};

enum class SystemSize {
    Small,
    Medium,
    Large,
    Huge
};

enum class GalaxyShape {
    Spiral,
    Elliptical,
    Ring,
    Cluster
};

enum class PlanetType {
    Rocky,
    GasGiant,
    IceGiant,
    Desert,
    Ocean,
    Frozen,
    Volcanic,
    Toxic
};
}

// Register enums with Qt's meta-type system
Q_DECLARE_METATYPE(ggh::GalaxyCore::utilities::StarType)
Q_DECLARE_METATYPE(ggh::GalaxyCore::utilities::SystemSize)
Q_DECLARE_METATYPE(ggh::GalaxyCore::utilities::GalaxyShape)
Q_DECLARE_METATYPE(ggh::GalaxyCore::utilities::PlanetType)

#endif // !GGH_MODULES_GALAXYCORE_UTILITIES_COMMON_H

