TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        application.cpp \
        checkTimer.cpp \
        engine.cpp \
        inputManager.cpp \
        main.cpp \
        playState.cpp \
        window.cpp

HEADERS += \
    application.h \
    checkTimer.h \
    engine.h \
    iGameState.h \
    iStateManager.h \
    inputManager.h \
    playState.h \
    window.h

LIBS += -lSDL2
