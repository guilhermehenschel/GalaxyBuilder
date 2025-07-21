#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_VIEW_MODEL_H

#include <QObject>

#include "ggh/modules/GalaxyFactions/models/System.h"

namespace ggh::Galaxy::Factions::viewmodels
{
class SystemViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    QML_ELEMENT
public:
    explicit SystemViewModel(QObject* parent = nullptr);
    explicit SystemViewModel(std::shared_ptr<ggh::Galaxy::Factions::models::System> system QObject* parent = nullptr);

    // Getters
    int id() const;
    QString name() const;
    QString description() const;
    QString type() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setDescription(const QString& description);
    void setType(const QString& type);

signals:
    void idChanged(int id);
    void nameChanged(const QString& name);
    void descriptionChanged(const QString& description);
    void typeChanged(const QString& type);

private:
    std::shared_ptr<models::System> m_system;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_VIEW_MODEL_H