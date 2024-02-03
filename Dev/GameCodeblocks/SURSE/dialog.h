#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include "basics.h"
#include "inventory.h"
#include "quests.h"
#include "data.h"
#include "pixel_manip.h"
#include "npc.h"
#include "map.h"


extern class Dialog{
private:
char cName[20],cDialogtxt[15][25],cTopic[4][20];
SDL_Surface *sBackground,*sItmbg,*sClose,
            *sName,*sQuestrew,*sQuestitms,
            *sRewskin[3],*sRewDesc[3][4];
SDL_Surface *sSlotback,*sSlotlock,*sFinish,*sPgEffect;

SDL_Texture *slot_empty,*FQP_ItmSelect,*txFQP_ButtonFQ_up,*FQP_Text,
            *txFQP_ButtonFQ_down,*txDialogtxt[15],*txTopic[4],*Tip_tx,
            *tooltip_highlight,*tooltip_canvas,*tooltip_lines_name[3],
            *tooltip_stats[6],*tooltip_lines_desc[6],*BotEdge_Left,*BotEdge_Right;

int NPC_nr,iLine,iTopic,iCTOPIC,quest_finished,cRewitm,cRewleft,cx,CurrentNPC_filenr,
    FQP_ItmRewNr,FQP_RewItmLeft,FQP_LastRewMotion,
    tooltip_name_nr,tooltip_statsnr,tooltip_desc_nr,FQP_CItm,FQP_FinishStatus,
    FQP_QuestNr,CTopicInstr[4],BotBackslice_w,BotBackslice_h,BackFog_w,BackFog_h,
    FQP_Text_w,Tip_tx_w,tooltip_canvas_w,tooltip_canvas_h,tooltip_lines_name_w[3],
    tooltip_lines_name_h[3],FQP_RewItmTaken,Percent_ShowBotBack;
bool itm_chose[3],finish_drag,acces_topic[TOPIC_CAP],run_dialog,bFinQuePanel,
     FQP_RENDER_ONCE,FQP_RENDER_TOOLTIP,FQP_ItmSelected[10],FQP_ButtonPressed,
     FQP_FinishQuest,RecItm_ShowTip,AllowInvAcces ;
float NrBotSlices;

Uint32 FQP_ButtonTimer;

Inventory_item *FQP_rew_items[10] ;
SDL_Rect box_finish,FQP_ItmBox[10],FQP_ButtonFinish ;
Armor tempArmor;
Weapon tempWeapon;
DIALOG_TOPIC *CDepth,*CurrentDialogTopic;

//New Dialog
SDL_Texture *bot_backslice,*back_fog,
            *text_backline,*text_backfog;
SDL_Rect box_topic[4];
bool UPDATE_DIALOG;

public:
void init(void);
~Dialog();
void open(DIALOG_TOPIC *CDialTopic);
void show(void);
void handle_events(SDL_Event dialog_event);
void handle_logics(void);
}DIALOG;

#endif // DIALOG_H_INCLUDED
