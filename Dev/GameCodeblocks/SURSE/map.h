#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "basics.h"
#include "main_headers.h"
#include "quests.h"
#include "npc.h"
#include "data.h"


extern class Map{
private:
SDL_Surface *DayNightPalette;
int cDayTime,i,LastNpc,cTerrain[30][30],MapWidth,MapHeight;//Retine ultima usi vazute pt a inta in ea daca se apasa E
SDL_Rect box_ROTATE[4];
SDL_Texture *RendNormalMap,*RendForeLayer,*DayNightPhase[4];
Uint32 tstart,tpause,tSuccSpeed,tAnime;
SDL_Rect AnimeRect;
FreeChunk *LastChunk_back,*LastChunk_fore,*LastChunk_dynam;
bool DaySuccession;
bool change_frame;

public:
int DisplayLocation_x,DisplayLocation_y;
SDL_Surface *Normal_Map[30][30];


void init();
~Map();
void PreLoadChanges();
void PostLoadChanges();
DIALOG_TOPIC *AddDialogTopic();
void AddNPC(NotPLayerCreature* model);
void AddToEnemyList( NotPLayerCreature *target );
void load(int MapNumber);
void addlight(int nr,int orient,int x,int y);
void AddCollider(int x,int y,int w,int h,int id);
//InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id);
void display_back();
void display_fore();
void display_dynamic( int );
void display_lights();
void CleanCMap();
bool NearMob(void);
void handle_time();
void handle_logics();
void manipulate_inter( int id,int val,int info,int type = -999 );
void manipulate_colider( int id,int val );
void SetcDayTime(int nr);
void SetDaySuccesion(bool succ);
void SetDaySuccSpeed(int nr);
void AddFreeChunk( FreeChunk *model,int ground );
void UpdateLastChunk();
string name;
}GAME_MAP;

#endif // MAP_H_INCLUDED
