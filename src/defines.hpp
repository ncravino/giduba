#ifndef DEFINES_HPP
#define DEFINES_HPP 1

// Defined via qmake
#ifndef VERSION
#define VERSION "testing"
#endif

#define APPNAME "Giduba"
#define GIDUBA_ICON ":/icons/giduba.svg"
#define CONFIG_PATH QDir::homePath()+"/.config/giduba/"
#define CONFIG_FILE_PATH CONFIG_PATH + "giduba.conf"
#define CONFIG_GEOM_FILE CONFIG_PATH + "giduba.geom"

#endif
