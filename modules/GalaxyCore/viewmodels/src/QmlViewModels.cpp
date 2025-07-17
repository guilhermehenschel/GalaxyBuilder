#include "ggh/modules/GalaxyCore/viewmodels/QmlViewModels.h"
#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/PlanetViewModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/TravelLaneViewModel.h"

#include <QtQml>

void registerGalaxyCoreViewModelsTypes()
{
    qmlRegisterType<ggh::GalaxyCore::viewmodels::StarSystemViewModel>("GalaxyCore.ViewModels", 1, 0, "StarSystemViewModel");
    qmlRegisterType<ggh::GalaxyCore::viewmodels::PlanetViewModel>("GalaxyCore.ViewModels", 1, 0, "PlanetViewModel");
    qmlRegisterType<ggh::GalaxyCore::viewmodels::TravelLaneViewModel>("GalaxyCore.ViewModels", 1, 0, "TravelLaneViewModel");
}

// Static initialization to ensure types are registered when module is loaded
static bool typesRegistered = []() {
    registerGalaxyCoreViewModelsTypes();
    return true;
}();
