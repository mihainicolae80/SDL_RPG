#ifndef UI_MANAGER_H_INCLUDED
#define UI_MANAGER_H_INCLUDED

#include <list>
#include "base.h"
#include "ui_group.h"

class UI_Manager{

private:
    std::list <UI_Group> groups;

public:
    /* Group management */
    void addGroup(UI_Group target);
    void cleanup(void);

    void handleEvents(SDL_Event *event);
};

#endif // UI_MANAGER_H_INCLUDED
