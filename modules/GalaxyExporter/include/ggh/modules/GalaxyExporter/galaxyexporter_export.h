#ifndef GGH_MODULES_GALAXYEXPORTER_EXPORT_H
#define GGH_MODULES_GALAXYEXPORTER_EXPORT_H

#ifdef _WIN32
    #ifdef GalaxyExporter_LIBRARY
        #define GALAXYEXPORTER_EXPORT __declspec(dllexport)
    #else
        #define GALAXYEXPORTER_EXPORT __declspec(dllimport)
    #endif
#else
    #define GALAXYEXPORTER_EXPORT
#endif

#endif // GGH_MODULES_GALAXYEXPORTER_EXPORT_H
