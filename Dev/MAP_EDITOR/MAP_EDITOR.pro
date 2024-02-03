TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L"C:\bin\SDL2-2.28.5\x86_64-w64-mingw32\lib" -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDEPATH += "C:\bin\SDL2-2.28.5\x86_64-w64-mingw32\include"
INCLUDEPATH += -I"src"
#INCLUDEPATH += -I"src\ui"

SOURCES += \
    main.cpp \
    src/Interface.cc \
    src/PixelManipulation.cc \
    src/Placeable.cc \
    src/Run.cc \
    src/base.cpp \
    src/basics.cpp \
    src/debug.cpp \
    src/main_header.cc \
    src/map.cc \
    src/textures.cc \
    src/ui/ui_bar.cc \
    src/ui/ui_group.cc \
    src/ui/ui_list.cc \
    src/ui/ui_list_item.cc

HEADERS += \
    src/Interface.h \
    src/PixelManipulation.h \
    src/Placeable.h \
    src/Run.h \
    src/base.h \
    src/basics.h \
    src/debug.h \
    src/main_headers.h \
    src/map.h \
    src/textures.h \
    src/ui/CallbackClass.h \
    src/ui/ui_bar.h \
    src/ui/ui_bar.h \
    src/ui/ui_core.h \
    src/ui/ui_group.h \
    src/ui/ui_item.h \
    src/ui/ui_list.h \
    src/ui/ui_list.h \
    src/ui/ui_list_item.h \
    src/ui/ui_manager.h \
    src/ui/ui_window.h
