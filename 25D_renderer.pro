TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        application.cpp \
        camera.cpp \
        checkTimer.cpp \
        inputManager.cpp \
        main.cpp \
        physicsengine.cpp \
        playState.cpp \
        renderengine.cpp \
        shader.cpp \
        window.cpp

HEADERS += \
    application.h \
    camera.h \
    checkTimer.h \
    iGameState.h \
    iStateManager.h \
    inputManager.h \
    physicsengine.h \
    playState.h \
    renderengine.h \
    shader.h \
    types.h \
    window.h

LIBS += -lSDL2 -lGL -lGLU -lGLEW
