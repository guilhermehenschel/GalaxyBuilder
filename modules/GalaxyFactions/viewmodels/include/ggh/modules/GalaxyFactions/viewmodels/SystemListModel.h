/**
 * @file SystemListModel.h
 * @brief List model for System data
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_LIST_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_LIST_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QtQml/qqmlregistration.h>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/System.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class SystemListModel
 * @brief QML-compatible list model for managing System data
 * 
 * This class provides a QAbstractListModel implementation for displaying
 * and managing a list of systems in QML views.
 */
class SystemListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    /**
     * @enum Roles
     * @brief Data roles for the model
     */
    enum Roles {
        IdRole = Qt::UserRole + 1,  ///< System ID role
        NameRole,                   ///< System name role
        DescriptionRole,            ///< System description role
        TypeRole                    ///< System type role
    };
    Q_ENUM(Roles)

    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit SystemListModel(QObject* parent = nullptr);

    /**
     * @brief Gets the number of rows in the model
     * @param parent The parent model index (unused for list models)
     * @return The number of systems
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
     * @brief Adds a system to the model
     * @param system The system to add
     */
    void addSystem(std::shared_ptr<ggh::Galaxy::Factions::models::System> system);
    
    /**
     * @brief Removes a system from the model
     * @param systemId The ID of the system to remove
     */
    void removeSystem(int systemId);
    
    /**
     * @brief Clears all systems from the model
     */
    void clearSystems();
    
    /**
     * @brief Gets all systems
     * @return List of all systems
     */
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>> getSystems() const;
    
    /**
     * @brief Sets the systems list
     * @param systems The new list of systems
     */
    void setSystems(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>>& systems);

private:
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>> m_systems; ///< List of systems
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_LIST_MODEL_H