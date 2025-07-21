/**
 * @file FactionListModel.h
 * @brief List model for Faction data
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_LIST_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_LIST_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QtQml/qqmlregistration.h>

#include <memory>

#include "ggh/modules/GalaxyFactions/models/Faction.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class FactionListModel
 * @brief QML-compatible list model for managing Faction data
 * 
 * This class provides a QAbstractListModel implementation for displaying
 * and managing a list of factions in QML views.
 */
class FactionListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    /**
     * @enum Roles
     * @brief Data roles for the model
     */
    enum Roles {
        IdRole = Qt::UserRole + 1,  ///< Faction ID role
        NameRole,                   ///< Faction name role
        DescriptionRole,            ///< Faction description role
        ColorRole                   ///< Faction color role
    };
    Q_ENUM(Roles)

    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit FactionListModel(QObject* parent = nullptr);

    /**
     * @brief Gets the number of rows in the model
     * @param parent The parent model index (unused for list models)
     * @return The number of factions
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
     * @brief Adds a faction to the model
     * @param faction The faction to add
     */
    void addFaction(const ggh::Galaxy::Factions::models::Faction& faction);
    
    /**
     * @brief Clears all factions from the model
     */
    void clearFactions();
    
    /**
     * @brief Removes a faction by ID
     * @param id The ID of the faction to remove
     */
    void removeFaction(int id);
    
    /**
     * @brief Gets a faction by ID
     * @param id The faction ID
     * @return The faction with the specified ID
     */
    ggh::Galaxy::Factions::models::Faction getFaction(int id) const;
    
    /**
     * @brief Gets all factions
     * @return List of all factions
     */
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> getFactions() const;
    
    /**
     * @brief Sets the factions list
     * @param factions The new list of factions
     */
    void setFactions(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>>& factions);

 private:
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> m_factions; ///< List of factions

};

} // namespace ggh::Galaxy::Factions::viewmodels


#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_LIST_MODEL_H