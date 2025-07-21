/**
 * @file FactionViewModel.h
 * @brief ViewModel for Faction data
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_VIEW_MODEL_H

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

#include "ggh/modules/GalaxyFactions/models/Faction.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class FactionViewModel
 * @brief ViewModel wrapper for Faction model to be used in QML
 * 
 * This class provides a QML-friendly interface for the Faction model,
 * exposing properties that can be bound to QML UI elements.
 */
class FactionViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    QML_ELEMENT
public:
    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit FactionViewModel(QObject* parent = nullptr);
    
    /**
     * @brief Constructor with faction model
     * @param faction The faction model to wrap
     * @param parent QObject parent
     */
    explicit FactionViewModel(const ggh::Galaxy::Factions::models::Faction& faction, QObject* parent = nullptr);

    /**
     * @brief Gets the faction ID
     * @return The faction ID
     */
    int id() const;
    
    /**
     * @brief Gets the faction name
     * @return The faction name
     */
    QString name() const;
    
    /**
     * @brief Gets the faction description
     * @return The faction description
     */
    QString description() const;
    
    /**
     * @brief Gets the faction color
     * @return The faction color as hex string
     */
    QString color() const;

    /**
     * @brief Sets the faction ID
     * @param id The new faction ID
     */
    void setId(int id);
    
    /**
     * @brief Sets the faction name
     * @param name The new faction name
     */
    void setName(const QString& name);
    
    /**
     * @brief Sets the faction description
     * @param description The new faction description
     */
    void setDescription(const QString& description);
    
    /**
     * @brief Sets the faction color
     * @param color The new faction color as hex string
     */
    void setColor(const QString& color);

signals:
    /**
     * @brief Signal emitted when the ID changes
     * @param id The new ID
     */
    void idChanged(int id);
    
    /**
     * @brief Signal emitted when the name changes
     * @param name The new name
     */
    void nameChanged(const QString& name);
    
    /**
     * @brief Signal emitted when the description changes
     * @param description The new description
     */
    void descriptionChanged(const QString& description);
    
    /**
     * @brief Signal emitted when the color changes
     * @param color The new color
     */
    void colorChanged(const QString& color);

private:
    ggh::Galaxy::Factions::models::Faction m_faction;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_VIEW_MODEL_H