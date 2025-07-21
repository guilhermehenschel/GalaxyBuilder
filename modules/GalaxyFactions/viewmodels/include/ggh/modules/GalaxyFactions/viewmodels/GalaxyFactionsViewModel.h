#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_GALAXY_FACTIONS_VIEW_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_GALAXY_FACTIONS_VIEW_MODEL_H

#include <QObject>
#include <QQmlListProperty>

#include "ggh/modules/GalaxyFactions/models/Faction.h"
#include "ggh/modules/GalaxyFactions/viewmodels/FactionViewModel.h"

namespace ggh::Galaxy::Factions::viewmodels
{
class GalaxyFactionsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<FactionViewModel> factions READ factions NOTIFY factionsChanged)
    QML_ELEMENT
public:
    explicit GalaxyFactionsViewModel(QObject* parent = nullptr);

    // Getters
    QQmlListProperty<FactionViewModel> factions();

signals:
    void factionsChanged();

private:
    std::shared_ptr<models::GalaxyFactions> m_galaxy_factions;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_GALAXY_FACTIONS_VIEW_MODEL_H