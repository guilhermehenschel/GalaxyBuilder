/**
 * @file SystemViewModel.h
 * @brief ViewModel for System data
 * @author Galaxy Builder Project
 * @date 2025
 */

#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_VIEW_MODEL_H

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/System.h"

namespace ggh::Galaxy::Factions::viewmodels
{

/**
 * @class SystemViewModel
 * @brief ViewModel wrapper for System model to be used in QML
 * 
 * This class provides a QML-friendly interface for the System model,
 * exposing properties that can be bound to QML UI elements.
 */
class SystemViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    QML_ELEMENT
public:
    /**
     * @brief Default constructor
     * @param parent QObject parent
     */
    explicit SystemViewModel(QObject* parent = nullptr);
    
    /**
     * @brief Constructor with system model
     * @param system The system model to wrap
     * @param parent QObject parent
     */
    explicit SystemViewModel(std::shared_ptr<ggh::Galaxy::Factions::models::System> system, QObject* parent = nullptr);

    /**
     * @brief Gets the system ID
     * @return The system ID
     */
    int id() const;
    
    /**
     * @brief Gets the system name
     * @return The system name
     */
    QString name() const;
    
    /**
     * @brief Gets the system description
     * @return The system description
     */
    QString description() const;
    
    /**
     * @brief Gets the system type
     * @return The system type
     */
    QString type() const;

    /**
     * @brief Sets the system ID
     * @param id The new system ID
     */
    void setId(int id);
    
    /**
     * @brief Sets the system name
     * @param name The new system name
     */
    void setName(const QString& name);
    
    /**
     * @brief Sets the system description
     * @param description The new system description
     */
    void setDescription(const QString& description);
    
    /**
     * @brief Sets the system type
     * @param type The new system type
     */
    void setType(const QString& type);

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
     * @brief Signal emitted when the type changes
     * @param type The new type
     */
    void typeChanged(const QString& type);

private:
    std::shared_ptr<ggh::Galaxy::Factions::models::System> m_system; ///< The wrapped system model
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_VIEW_MODEL_H