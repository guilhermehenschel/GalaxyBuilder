#pragma once

#include <QtCore/qglobal.h>

#ifndef GALAXYCORE_STATIC
#  if defined(GALAXYCORE_LIBRARY)
#    define GALAXYCORE_EXPORT Q_DECL_EXPORT
#  else
#    define GALAXYCORE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define GALAXYCORE_EXPORT
#endif
