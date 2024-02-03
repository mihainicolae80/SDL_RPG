#ifndef QUESTS_H_INCLUDED
#define QUESTS_H_INCLUDED

#include "main_headers.h"
#include "basics.h"
#include "data.h"

#include "pixel_manip.h"

extern class Quest{
private:
SDL_Surface *sQuestname[QUEST_CAP],*sQuestprog[QUEST_CAP],*csQuestname;
SDL_Texture *txBackground,*txUp1,*txUp2,*txLeftbackground,*txLeftside,*txLogo,*txAbbandon,*txCoverLeft,
            *txQuestname[QUEST_CAP],*txQuestprog[QUEST_CAP],*txcQuestname,*txQuestdesc[10],
            *txRewGold,*txRewExp,*tooltip_tex,*tooltip_stats[6],*tooltip_lines_desc[6],
            *tooltip_highlight,*tooltip_highlight_flip,*tooltip_canvas,*tooltip_lines_name[3],
            *txToList;

SDL_Rect box[QUEST_CAP],cbox,cbox2,abox,bgbox;
int mouse_x,mouse_y,cquest,Background_w,Background_h,LeftBackground_w,Leftside_w,CloseB_w,CloseB_h,
    abbandon_w,QuestTitle_w,tooltip_statsnr,tooltip_name_nr,tooltip_desc_nr,Crow,Ccol,tooltip_canvas_w,
    tooltip_ln_w[3],tooltip_ld_w[6] ;
bool SHOW_QUEST_DETAIL,SHOW_ABBANDON_BUTTON,DRAG_ABBANDON,SHOW_UP,OnButtonToList;

public:
bool SHOW_QUEST;
bool bQUESTDONE[100];

void init();

void show();
~Quest();
void add(int nr);
void handle_events(SDL_Event newevent);
void setrects();
bool is_finished( int x);
bool is_active(int x );
void end(int nr);
void advance( int nr );
}QUEST;

#endif // QUESTS_H_INCLUDED
