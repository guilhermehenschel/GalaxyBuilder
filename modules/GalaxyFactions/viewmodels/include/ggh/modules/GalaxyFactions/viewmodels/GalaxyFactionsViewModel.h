/**
 * @file GalaxyFactionsViewModel.h
 * @brief Main ViewModel for Galaxy Factions module
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_GALAXY_FACTIONS_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_GALAXY_FACTIONS_VIEW_MODEL_H

#include <QObject>
#include <QQmlListProperty>
#include <QtQml/qqmlregistration.h>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/GalaxyFactions.h"
#include "ggh/modules/GalaxyFactions/viewmodels/FactionViewModel.h"
#include "ggh/modules/GalaxyFactions/viewmodels/FactionListModel.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class GalaxyFactionsViewModel
 * @brief Main ViewModel for the Galaxy Factions module
 * 
 * This class serves as the main entry point for the Galaxy Factions module,
 * providing access to faction data and operations for QML views.
 */
class GalaxyFactionsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FactionListModel* factionListModel READ factionListModel NOTIFY factionListModelChanged)
    Q_PROPERTY(int selectedFactionId READ selectedFactionId WRITE setSelectedFactionId NOTIFY selectedFactionIdChanged)
    QML_ELEMENT
public:
    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit GalaxyFactionsViewModel(QObject* parent = nullptr);

    /**
     * @brief Gets the faction list model
     * @return Pointer to the faction list model
     */
    FactionListModel* factionListModel();
    
    /**
     * @brief Gets the currently selected faction ID
     * @return The selected faction ID
     */
    int selectedFactionId() const;
    
    /**
     * @brief Sets the selected faction ID
     * @param factionId The ID of the faction to select
     */
    void setSelectedFactionId(int factionId);

    /**
     * @brief Initializes the view model with galaxy factions data
     * @param galaxyFactions The galaxy factions model
     */
    Q_INVOKABLE void initialize(std::shared_ptr<ggh::Galaxy::Factions::models::GalaxyFactions> galaxyFactions);
    
    /**
     * @brief Adds a new faction
     * @param name The faction name
     * @param description The faction description
     * @param color The faction color
     */
    Q_INVOKABLE void addFaction(const QString& name, const QString& description, const QString& color);
    
    /**
     * @brief Adds a new faction with homeworld system
     * @param name The faction name
     * @param description The faction description
     * @param color The faction color
     * @param homeworldSystemId The ID of the homeworld star system
     * @param starSystemModel The star system model for the homeworld
     * @return The ID of the created faction, or -1 if creation failed
     */
    Q_INVOKABLE int addFactionWithHomeworld(const QString& name, const QString& description, const QString& color, quint32 homeworldSystemId, QObject* starSystemModel);
    
    /**
     * @brief Removes a faction by ID
     * @param factionId The ID of the faction to remove
     */
    Q_INVOKABLE void removeFaction(int factionId);
    
    /**
     * @brief Exports all factions to XML format
     * @return XML string containing all faction data
     */
    Q_INVOKABLE QString exportToXml() const;
    
    /**
     * @brief Exports all factions to XML file
     * @param filePath The file path to export to
     * @return True if export was successful, false otherwise
     */
    Q_INVOKABLE bool exportToXmlFile(const QString& filePath) const;

signals:
    /**
     * @brief Signal emitted when the faction list model changes
     */
    void factionListModelChanged();
    
    /**
     * @brief Signal emitted when a new faction is created
     * @param factionId The ID of the newly created faction
     * @param homeworldSystemId The ID of the homeworld star system
     * @param factionName The name of the faction
     * @param factionDescription The description of the faction
     * @param factionColor The color of the faction
     */
    void factionCreated(int factionId, quint32 homeworldSystemId, const QString& factionName, const QString& factionDescription, const QString& factionColor);
    
    /**
     * @brief Signal emitted when the selected faction ID changes
     * @param factionId The new selected faction ID
     */
    void selectedFactionIdChanged(int factionId);

private:
    std::shared_ptr<ggh::Galaxy::Factions::models::GalaxyFactions> m_galaxyFactions; ///< The galaxy factions model
    FactionListModel* m_factionListModel; ///< The faction list model
    int m_selectedFactionId; ///< The currently selected faction ID
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_GALAXY_FACTIONS_VIEW_MODEL_H