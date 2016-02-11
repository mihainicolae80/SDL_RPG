#ifndef UI_LIST_ITEM_H_INCLUDED
#define UI_LIST_ITEM_H_INCLUDED

class UI_List_Item{
public:
    UI_List_Item(int w,int h,SDL_Surface *surf
        ,CallbackClass leftclick,CallbackClass rightclick);

    void handleEvents(SDL_Event *event);
    void draw();
private:
    int x,y,w,h;
    CallbackClass *callbackLeftclick;
    CallbackClass *callbackRightclick;
};

#endif // UI_LIST_ITEM_H_INCLUDED
