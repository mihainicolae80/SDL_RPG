#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include "structs.h"
#include "basics.h"
#include "main_headers.h"
#include "combat.h"

extern class NPCs{
private:
NotPLayerCreature* cNPC_talk;
bool StillTalkNPC;
//Health Bar
SDL_Texture *healthbar, *healthgreen,*healthboss,*healthbossfill ;
SDL_Rect rHealthBar,NPC_BodyFrames_NORMAL[13][8];;

public:
void init();

void manipulate( int action,int ,int iTarget,string sTarget );
void behave();
void handle_interaction();
void show(int);
void show_BG();
void show_FG();
DIALOG_TOPIC *GetNPC_talk();
void cleanup_allnpc();
~NPCs();
}GAME_NPCS;

#endif // NPC_H_INCLUDED
