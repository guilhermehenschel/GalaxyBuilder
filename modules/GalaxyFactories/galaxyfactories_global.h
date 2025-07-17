#pragma once

#include <QtCore/qglobal.h>

#ifndef GALAXYFACTORIES_STATIC
#  if defined(GalaxyFactories_LIBRARY)
#    define GALAXYFACTORIES_EXPORT Q_DECL_EXPORT
#  else
#    define GALAXYFACTORIES_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define GALAXYFACTORIES_EXPORT
#endif
