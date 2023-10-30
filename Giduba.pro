TEMPLATE = app

QT += widgets

FORMS += mainwindow.ui
SOURCES += main.cpp

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

CONFIG += c++2a

QMAKE_CXXFLAGS += -O3 -fdata-sections -ffunction-sections -flto
QMAKE_LFLAGS += -Wl,--gc-sections -flto
