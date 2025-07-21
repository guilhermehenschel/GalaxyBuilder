#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_VIEW_MODEL_H

#include <QObject>
#include <QString>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"

namespace ggh::Galaxy::Factions::viewmodels
{
class SystemResourceBonusViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    QML_ELEMENT
public:
    explicit SystemResourceBonusViewModel(QObject* parent = nullptr);
    explicit SystemResourceBonusViewModel(std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus, QObject* parent = nullptr);

    // Getters
    int id() const;
    QString name() const;
    double value() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setValue(double value);

signals:
    void idChanged(int id);
    void nameChanged(const QString& name);
    void valueChanged(double value);

private:
    std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> m_resourceBonus;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_VIEW_MODEL_H