#ifndef COMBAT_H_INCLUDED
#define COMBAT_H_INCLUDED

#include "basics.h"
#include "gevents.h"
#include "console.h"
#include "map.h"

#define CONSOUT CONS.out

extern class Combat{
struct GAME_combattxt{
SDL_Texture *txText;
Uint32 tFinish;
int x,y;
GAME_combattxt *next;
}*prim,*ultim;
int ix,iy,cx,cy,freex,freey,matx,maty,direction,bx,by;//
Uint32 Stepcd,tPlace;
SDL_Surface *sAbilityObj[2];
public:
void init();
~Combat();
void AttackAsPLAYER();
void AttackAsNPC(NotPLayerCreature *cnpc);
void addtext(int x,int y,string sText,SDL_Color txtcolor);
void showtexts(void);
void handletexts(void);
int Getx();
int Gety();
}COMBAT;

#endif // COMBAT_H_INCLUDED
