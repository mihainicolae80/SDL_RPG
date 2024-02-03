#ifndef LOOT_H_INCLUDED
#define LOOT_H_INCLUDED

#include "basics.h"
#include "main_headers.h"
#include "inventory.h"

extern class Loot{
private:
SDL_Texture *txBackground,*txButtonTake_up,*txButtonClose_up,
            *tooltip_highlight,*tooltip_canvas,*tooltip_lines_name[3],
            *tooltip_stats[6],*tooltip_lines_desc[6],*txMiscQuant[5];
Inventory_item LootItems[10],NULLii;
int NrItems,tooltip_canvas_w,tooltip_canvas_h,tooltip_lines_name_w[3],tooltip_lines_name_h[3],
    tooltip_name_nr,tooltip_statsnr,tooltip_desc_nr,cItem ;

SDL_Rect rItemBox[5],rButtonBox_take,rButtonBox_close ;

public:
bool SHOW_LOOT;



void init(void);
void loadtable( int x );
void handle_events(SDL_Event);
void handle_logics(void);
void show(void);
void cleanup();
}LOOT;

#endif // LOOT_H_INCLUDED
