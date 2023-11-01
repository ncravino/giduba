TEMPLATE = app

QT += widgets

FORMS += mainwindow.ui
SOURCES += main.cpp
RESOURCES += main.qrc

MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build

DESTDIR = build

CONFIG += c++2a

QMAKE_CXXFLAGS += -s -O3 -fdata-sections -ffunction-sections -flto
QMAKE_LFLAGS += -s -Wl,--gc-sections -flto

