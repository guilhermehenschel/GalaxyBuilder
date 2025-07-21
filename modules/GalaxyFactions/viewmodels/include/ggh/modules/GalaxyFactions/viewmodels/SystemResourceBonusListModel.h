/**
 * @file SystemResourceBonusListModel.h
 * @brief List model for SystemResourceBonus data
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_LIST_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_LIST_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QtQml/qqmlregistration.h>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class SystemResourceBonusListModel
 * @brief QML-compatible list model for managing SystemResourceBonus data
 * 
 * This class provides a QAbstractListModel implementation for displaying
 * and managing a list of system resource bonuses in QML views.
 */
class SystemResourceBonusListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    /**
     * @enum Roles
     * @brief Data roles for the model
     */
    enum Roles {
        IdRole = Qt::UserRole + 1,  ///< Resource bonus ID role
        NameRole,                   ///< Resource bonus name role
        ValueRole                   ///< Resource bonus value role
    };
    Q_ENUM(Roles)

    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit SystemResourceBonusListModel(QObject* parent = nullptr);

    /**
     * @brief Gets the number of rows in the model
     * @param parent The parent model index (unused for list models)
     * @return The number of resource bonuses
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    
    /**
     * @brief Gets data for a specific role and index
     * @param index The model index
     * @param role The data role
     * @return The data for the specified role
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    /**
     * @brief Gets the role names for QML access
     * @return Hash map of role IDs to role names
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Adds a resource bonus to the model
     * @param resourceBonus The resource bonus to add
     */
    void addResourceBonus(std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus);
    
    /**
     * @brief Removes a resource bonus from the model
     * @param resourceBonusId The ID of the resource bonus to remove
     */
    void removeResourceBonus(int resourceBonusId);
    
    /**
     * @brief Clears all resource bonuses from the model
     */
    void clearResourceBonuses();
    
    /**
     * @brief Gets all resource bonuses
     * @return List of all resource bonuses
     */
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus>> getResourceBonuses() const;
    
    /**
     * @brief Sets the resource bonuses list
     * @param resourceBonuses The new list of resource bonuses
     */
    void setResourceBonuses(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus>>& resourceBonuses);

private:
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus>> m_resourceBonuses; ///< List of resource bonuses
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_LIST_MODEL_H