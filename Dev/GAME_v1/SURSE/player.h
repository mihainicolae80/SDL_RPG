#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "basics.h"
#include "main_headers.h"
#include "anim.h"
#include "structs.h"
#include "map.h"
#include "dialog.h"
#include "loot.h"

#define ACTION_OPENDOOR      1
#define ACTION_TEXTABOVEHEAD 2
#define ACTION_DIALOG        3
#define ACTION_LOOT          4

const int hero_movement_speed = 7 ;//min 4  (step length)
const int cloak_frames = 5;
const int HERO_RUN_TWAIT = 15;//min  0 (delway between steps)10

extern class CLS_PLAYER{
private:
SDL_Rect _SkinSlices[13][8];
int _czone;
bool HideTip,ShowTAH;
//Modes + Frames

SDL_Texture *_txSkin;

public:
int look_direct,av_action;
int armor_mode,cloak_cframe,cloak_cmode,shield_cframe,shield_cmode,look_categ,look_frame,NrNpcInCombatWith;
SDL_Texture *AboveHeadTip = NULL,*txEQUIPT_Shield = NULL,*txEQUIPT_Chest,*txEQUIPT_Cloak;
InterSpot *AvailableInterSpot;
Uint32 CombatCounter;
bool SHIELD_UP,SHOW_TIP,InNPCRange,SPEAK_NPC;

    struct Quests
    {
        int nr;
        char name[50],desc[300];
        int fvalue,cvalue,rew_g,rew_e,rew_i_nr;
        struct
        {
            char type;
            int rnr,quant;
        }rew_i[8];

        bool active,finished;
    }quest[5];


    SDL_Rect _PhysicalBody;
    int _health_c,_health_max,_power_c,_power_max;
    int def_base,def_itm_chest,def_itm_shield,protect_itm_shield,def_itm_cloak;
    int dmg_base,dmg_weapon,dmg_var;
    Uint32 tMove,tAttack,tAnim_cloak,tStart;
    void init();
    //bool detect_collision(void);
    void TeleportXY(int x,int y);
    void teleportZone(int zone);
    int GetZone(void);
    void TurnBody(int direction);
    void handleMovement(void);
    void handleMisc(void);
    void handleEvents( SDL_Event *EVENT );
    void draw();
    void cleanup();
}PLAYER;

#endif // PLAYER_H_INCLUDED
