TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L"SDL2-2.0.3/x86_64-w64-mingw32/lib" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDEPATH += "SDL2-2.0.3/x86_64-w64-mingw32/include"
INCPATH += -Isrc

SOURCES += \
    main.cpp \
    src/anim.cpp \
    src/base.cpp \
    src/basics.cpp \
    src/combat.cpp \
    src/console.cpp \
    src/data.cpp \
    src/debug.cpp \
    src/dialog.cpp \
    src/gevents.cpp \
    src/init.cpp \
    src/interface.cpp \
    src/inventory.cpp \
    src/loot.cpp \
    src/map.cpp \
    src/misc.cpp \
    src/npc.cpp \
    src/pixel_manip.cpp \
    src/player.cpp \
    src/quests.cpp \
    src/run.cpp \
    src/scripts.cpp \
    src/settings.cpp

HEADERS += \
    src/anim.h \
    src/base.h \
    src/basics.h \
    src/combat.h \
    src/conf_graphics.h \
    src/console.h \
    src/data.h \
    src/debug.h \
    src/dialog.h \
    src/gevents.h \
    src/interface.h \
    src/inventory.h \
    src/loot.h \
    src/main_headers.h \
    src/map.h \
    src/misc.h \
    src/npc.h \
    src/pixel_manip.h \
    src/player.h \
    src/quests.h \
    src/run.h \
    src/scripts.h \
    src/settings.h \
    src/structs.h
