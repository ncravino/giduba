TEMPLATE = app

QT += widgets

VERSION = "$$cat(VERSION)"
DEFINES += VERSION=\\\"$$VERSION\\\"

FORMS += src/main.ui
SOURCES += src/main.cpp src/editor.cpp src/config.cpp
RESOURCES += resources/main.qrc

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

CONFIG += c++2a

QMAKE_CXXFLAGS += -s -O3 -fdata-sections -ffunction-sections -flto
QMAKE_LFLAGS += -s -Wl,--gc-sections -flto

