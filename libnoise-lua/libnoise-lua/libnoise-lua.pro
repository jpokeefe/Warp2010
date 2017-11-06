#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T13:26:06
#
#-------------------------------------------------


TARGET = libnoise-lua
TEMPLATE = lib

CONFIG += c++14

LIBS += -llua

DEFINES += LIBNOISELUA_LIBRARY

SOURCES += libnoiselua.cpp

HEADERS += libnoiselua.h\
        libnoise-lua_global.h \
    sol.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libnoise-warp/release/ -llibnoise-warp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libnoise-warp/debug/ -llibnoise-warp
else:unix: LIBS += -L$$OUT_PWD/../../libnoise-warp/ -llibnoise-warp

INCLUDEPATH += $$PWD/../../libnoise/src
DEPENDPATH += $$PWD/../../libnoise/src

#INCLUDEPATH += $$PWD/../../libnoise-warp
#DEPENDPATH += $$PWD/../../libnoise-warp