#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <random>
#include <QPointF>
#include <QColor>
#include <QString>
#include <QMetaType>

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

// Forward declarations
class StarSystem;
class TravelLane;
class Galaxy;

// Register enums with Qt's meta-type system
Q_DECLARE_METATYPE(StarType)
Q_DECLARE_METATYPE(SystemSize)
Q_DECLARE_METATYPE(GalaxyShape)
class Galaxy;
