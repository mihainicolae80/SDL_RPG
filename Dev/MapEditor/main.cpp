#define SDL_MAIN_HANDLED

#include "src/map_editor.h"

int main(int argc, char *args[])
{
    (void)argc;
    (void)args;

    MapEditor mapEditor;
    mapEditor.Run();

    return 0;
}
