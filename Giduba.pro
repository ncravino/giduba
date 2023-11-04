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

CONFIG += release "c++2a"

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE *= -s -Os -flto
QMAKE_LFLAGS += -s -flto

