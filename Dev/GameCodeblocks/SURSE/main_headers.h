#ifndef MAIN_HEADERS_H_INCLUDED
#define MAIN_HEADERS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include "structs.h"

using namespace std;

///Constants
const int VERT = 0;
const int HOR  = 1;

const int FIRST = 0;
const int LAST  = 1;
const int ALL   = 2;

const int LEFT  = 0;
const int RIGHT = 1;
const int DOWN  = 2;
const int UP    = 3;

const int BACK     = 0;
const int FORE     = 1;
const int DYNAMIC  = 2;

const int SPAWN   = 1;
const int UNSPAWN = 0;

const int ARMOR_TYPE_CHEST = 0;
const int ARMOR_TYPE_CLOAK = 1;
const int INV_TYPE_EMPTY   = -1;
const int INV_TYPE_ARMOR   = 0;
const int INV_TYPE_WEAPON  = 1;
const int INV_TYPE_SHIELD  = 2;
const int INV_TYPE_BAG     = 3;
const int INV_TYPE_MISC    = 4;

const int NONE         = -1;
const int HIDE         = 0;
const int ADD_QUEST    = 1;
const int FINISH_QUEST = 2;

const int CLOSE              = 3;
const int GIVE_ITEM          = 4;
const int DIALACT_INTERMANIP = 5;
const int DIALACT_COLMANIP   = 6;

const int DEFINED = 1;
const int CUSTOM  = 0;

///SETTINGS
const int FPS_CAP = 60;//60
const int PLAYER_STARTZONE = 0;
const int SCREEN_WIDTH_CONS  = 400;
const int SCREEN_HEIGHT_CONS = 500;
const int BLOCK_WIDTH      = 200;
const int BLOCK_HEIGHT     = 200;
const int TOPIC_CAP        = 4;
const int QUEST_CAP        = 10;
//Flags
const Uint32 FLAG_TALK_TO_PLAYER =  0x01; // 02 04 08 10 20
const Uint32 FLAG_COMBAT_ABLE    =  0x02;

//Game Event constants
const int Event_RequestMapLoad  = 0;
const int Event_EnemyDefeated   = 3;
const int Event_RequestAddQuest = 4;
const int Event_RequestAdvanceQuest = 5;
const int Event_RequestTriggerScript = 6;

///Main Global Var (UNDER HEAVY WORK)
extern SDL_Window   *WINDOW_CONSOLE;
extern SDL_Renderer *RENDERER_MAIN,*RENDERER_CONSOLE;
extern InterSpot *AvailableInterSpot;
extern int look_direct;
extern int NrNpcInCombatWith;
extern unsigned WINDOW_CONSOLE_ID;
extern bool SHIELD_UP,SPEAK_NPC,bQUESTDONE[],CONSOLE_UPDATE,SHOW_LOOT;
extern char ConsText[];
extern SDL_Event event;
extern SDL_Texture *PlayerCurrentSkin,*txEQUIPT_Shield, *txEQUIPT_Chest,*txEQUIPT_Cloak;
extern SDL_Rect PlayerBodyFrames[13][8];

//Map
extern int NrBlockX,NrBlockY,MapWidth,MapHeight,OnScreenPlayer_x,OnScreenPlayer_y;
extern SDL_Surface *Normal_Map[30][30];
//NPCS
extern SDL_Texture *AboveHeadTip;
extern bool SHOW_TIP,InNPCRange;
extern int av_action;
//QUESTS
extern bool SHOW_QUEST;

///FORNTS
extern TTF_Font *Font14,*Font18,*Font20,*Font24,*Font2_14,*Font2_18,*Font2_20,*Font2_24;

///FUNCTIONS
bool init();
void cleanup();
void LoadingTitle(int percent);

//WINDOWS
extern unsigned int WINDOW_CONSOLE_ID;

///Variables
//Colors
extern SDL_Color WHITE;
extern SDL_Color BLACK;
extern SDL_Color RED;
extern SDL_Color BLUE;
extern SDL_Color GREEN;
extern SDL_Color YELLOW_LIGHT;

//Nr Of Blocks
extern int NrBlockX, NrBlockY;

#endif // MAIN_HEADERS_H_INCLUDED
