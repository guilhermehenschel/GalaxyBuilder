/**
 * @file Common.h
 * @brief Common types and constants for GalaxyCore module
 */

#pragma once

// Include the utilities Common.h and bring types into this namespace
#include "utilities/include/ggh/modules/GalaxyCore/utilities/Common.h"

// Bring the commonly used types from utilities namespace to GalaxyCore namespace
using SystemId = ggh::GalaxyCore::utilities::SystemId;
using LaneId = ggh::GalaxyCore::utilities::LaneId;
using GalaxySize = ggh::GalaxyCore::utilities::GalaxySize;
using StarType = ggh::GalaxyCore::utilities::StarType;
using SystemSize = ggh::GalaxyCore::utilities::SystemSize;
using GalaxyShape = ggh::GalaxyCore::utilities::GalaxyShape;

// Bring constants into the global namespace for backward compatibility
constexpr double PI = ggh::GalaxyCore::utilities::PI;
constexpr int DEFAULT_GALAXY_WIDTH = ggh::GalaxyCore::utilities::DEFAULT_GALAXY_WIDTH;
constexpr int DEFAULT_GALAXY_HEIGHT = ggh::GalaxyCore::utilities::DEFAULT_GALAXY_HEIGHT;
constexpr int MIN_SYSTEM_DISTANCE = ggh::GalaxyCore::utilities::MIN_SYSTEM_DISTANCE;
constexpr int MAX_TRAVEL_LANE_DISTANCE = ggh::GalaxyCore::utilities::MAX_TRAVEL_LANE_DISTANCE;
