#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_VIEW_MODEL_H

#include <QObject>

#include "ggh/modules/GalaxyFactions/models/Faction.h"

namespace ggh::Galaxy::Factions::viewmodels
{
class FactionViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    QML_ELEMENT
public:
    explicit FactionViewModel(QObject* parent = nullptr);
    explicit FactionViewModel(const ggh::Galaxy::Factions::models::Faction& faction, QObject* parent = nullptr);

    // Getters
    int id() const;
    QString name() const;
    QString description() const;
    QString color() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setDescription(const QString& description);
    void setColor(const QString& color);

signals:
    void idChanged(int id);
    void nameChanged(const QString& name);
    void descriptionChanged(const QString& description);
    void colorChanged(const QString& color);

private:
    ggh::Galaxy::Factions::models::Faction m_faction;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_VIEW_MODEL_H