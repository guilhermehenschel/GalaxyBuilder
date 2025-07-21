/**
 * @file SystemResourceBonusViewModel.h
 * @brief ViewModel for SystemResourceBonus data
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_VIEW_MODEL_H

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class SystemResourceBonusViewModel
 * @brief ViewModel wrapper for SystemResourceBonus model to be used in QML
 * 
 * This class provides a QML-friendly interface for the SystemResourceBonus model,
 * exposing properties that can be bound to QML UI elements.
 */
class SystemResourceBonusViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    QML_ELEMENT
public:
    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit SystemResourceBonusViewModel(QObject* parent = nullptr);
    
    /**
     * @brief Constructor with resource bonus model
     * @param resourceBonus The resource bonus model to wrap
     * @param parent QObject parent
     */
    explicit SystemResourceBonusViewModel(std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus, QObject* parent = nullptr);

    /**
     * @brief Gets the resource bonus ID
     * @return The resource bonus ID
     */
    int id() const;
    
    /**
     * @brief Gets the resource bonus name
     * @return The resource bonus name
     */
    QString name() const;
    
    /**
     * @brief Gets the resource bonus value
     * @return The resource bonus value
     */
    double value() const;

    /**
     * @brief Sets the resource bonus ID
     * @param id The new resource bonus ID
     */
    void setId(int id);
    
    /**
     * @brief Sets the resource bonus name
     * @param name The new resource bonus name
     */
    void setName(const QString& name);
    
    /**
     * @brief Sets the resource bonus value
     * @param value The new resource bonus value
     */
    void setValue(double value);

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
     * @brief Signal emitted when the value changes
     * @param value The new value
     */
    void valueChanged(double value);

private:
    std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> m_resourceBonus; ///< The wrapped resource bonus model
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_VIEW_MODEL_H