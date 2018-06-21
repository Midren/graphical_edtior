TEMPLATE = app
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg
CONFIG += c++11

SOURCES += main.cpp \
    scene.cpp \
    mainwindow.cpp \
    newscenedialog.cpp

HEADERS += \
    scene.h \
    mainwindow.h \
    newscenedialog.h

RESOURCES += \
    resouse_file.qrc
