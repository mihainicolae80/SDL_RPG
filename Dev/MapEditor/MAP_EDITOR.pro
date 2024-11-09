TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L"C:\lib\SDL2-2.28.5\x86_64-w64-mingw32\lib" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDEPATH += "C:\lib\SDL2-2.28.5\x86_64-w64-mingw32\include"
INCLUDEPATH += "src"
#INCLUDEPATH += "src\ui"
QMAKE_CXXFLAGS += -std=c++23
QMAKE_CFLAGS += -std=c17

SOURCES += \
    main.cpp \
    src/Interface.cc \
    src/map_editor.cpp \
    src/PixelManipulation.cc \
    src/Placeable.cc \
    src/base.cpp \
    src/basics.cpp \
    src/debug.cpp \
    src/main_header.cc \
    src/map.cc \
    src/textures.cc \
    src/ui/button.cpp \
    src/ui/context.cpp \
    src/ui/rect.cpp \
    src/ui/label.cpp \
    src/ui/texture_list.cpp

HEADERS += \
    src/Interface.h \
    src/map_editor.h \
    src/PixelManipulation.h \
    src/Placeable.h \
    src/base.h \
    src/basics.h \
    src/debug.h \
    src/main_headers.h \
    src/map.h \
    src/textures.h \
    src/ui/button.h \
    src/ui/context.h \
    src/ui/context_defaults.h \
    src/ui/iitem.h \
    src/ui/rect.h \
    src/ui/label.h \
    src/ui/texture_list.h
