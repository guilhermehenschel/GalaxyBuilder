#include "ggh/modules/GalaxyCore/viewmodels/PlanetListModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/PlanetViewModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/Commons.h"

namespace ggh::GalaxyCore::viewmodels {

// PlanetListModel implementation
PlanetListModel::PlanetListModel(std::shared_ptr<models::StarSystemModel> starSystem, QObject* parent)
    : QAbstractListModel(parent), m_starSystem(starSystem)
{
}

int PlanetListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_starSystem ? static_cast<int>(m_starSystem->getPlanets().size()) : 0;
}

QVariant PlanetListModel::data(const QModelIndex& index, int role) const
{
    if (!m_starSystem || !index.isValid() || index.row() >= static_cast<int>(m_starSystem->getPlanets().size())) {
        return QVariant();
    }

    const auto& planet = m_starSystem->getPlanets()[index.row()];

    switch (role) {
        case NameRole:
            return QString::fromStdString(planet->name());
        case TypeRole:
            return static_cast<int>(planet->type());
        case TypeNameRole:
            return commons::planetTypeName(planet->type());
        case TypeColorRole:
            return commons::planetColor(planet->type());
        case SizeRole:
            return planet->size();
        case MassRole:
            return planet->mass();
        case MoonsRole:
            return planet->numberOfMoons();
        case OrbitRole:
            return planet->orbitalRadius();
        case MaxTempRole:
            return planet->maxTemperature();
        case MinTempRole:
            return planet->minTemperature();
        case ViewModelRole:

            return QVariant::fromValue(new PlanetViewModel(planet));
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> PlanetListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "planetType";
    roles[TypeNameRole] = "planetTypeName"; // Optional: if you want to expose type name
    roles[TypeColorRole] = "planetTypeColor"; // Optional: if you want to expose type color
    roles[SizeRole] = "size";
    roles[MassRole] = "mass";
    roles[MoonsRole] = "numberOfMoons";
    roles[OrbitRole] = "orbitalRadius";
    roles[MaxTempRole] = "maxTemperature";
    roles[MinTempRole] = "minTemperature";
    roles[ViewModelRole] = "viewModel"; // For QML binding to PlanetViewModel
    return roles;
}

bool PlanetListModel::removeRows(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(parent)
    if (!m_starSystem || row < 0 || row + count > static_cast<int>(m_starSystem->getPlanets().size())) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i{row + count - 1}; i >= row; --i) {
        auto planet = m_starSystem->getPlanets()[i];
        if(!m_starSystem->removePlanet(planet)) {
            endRemoveRows();
            return false;
        }
    }
    endRemoveRows();
    
    return true;
}

void PlanetListModel::refresh()
{
    beginResetModel();
    endResetModel();
}

}