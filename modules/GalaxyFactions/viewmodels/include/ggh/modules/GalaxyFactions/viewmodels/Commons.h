/**
 * @file Commons.h
 * @brief Common definitions and utilities for Galaxy Factions ViewModels
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_COMMONS_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_COMMONS_H

#include <QtQml/qqmlregistration.h>

/**
 * @namespace ggh::Galaxy::Factions::viewmodels
 * @brief Namespace for Galaxy Factions ViewModels
 */
namespace ggh::Galaxy::Factions::viewmodels {

/**
 * @brief Common constants and utilities for ViewModels
 */
class Commons {
public:
    /**
     * @brief Default invalid ID value
     */
    static constexpr int INVALID_ID = -1;
    
    /**
     * @brief Default empty string value
     */
    static constexpr const char* EMPTY_STRING = "";
    
    /**
     * @brief Maximum string length for names and descriptions
     */
    static constexpr int MAX_STRING_LENGTH = 1000;
};

} // namespace ggh::Galaxy::Factions::viewmodels

#endif // GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_COMMONS_H
