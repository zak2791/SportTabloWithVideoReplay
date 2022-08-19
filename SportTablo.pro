QT += core gui widgets qml
HEADERS += \
    camera.h \
    cameraviewer.h \
    lcdtimer.h \
    pcscreen.h \
    player.h \
    playerviewer.h \
    rate.h \
    tvscreen.h

SOURCES += \
    camera.cpp \
    cameraviewer.cpp \
    lcdtimer.cpp \
    main.cpp \
    pcscreen.cpp \
    player.cpp \
    playerviewer.cpp \
    rate.cpp \
    tvscreen.cpp

FORMS += \
    dialogSetUrl.ui

DISTFILES +=

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavformat

INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavutil

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavfilter

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lswscale



