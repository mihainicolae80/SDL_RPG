#ifndef ANIM_H_INCLUDED
#define ANIM_H_INCLUDED

#include "main_headers.h"
#include "structs.h"
#include "basics.h"
#include "quests.h"
#include "map.h"


extern class Anime{
private:
ANIME *anime_prim,*anime_ultim;
SDL_Texture *txAnimeSkin[30];
SDL_Texture *txParticle[5];

SDL_Rect animebox,particlebox;
InterSpot *cIntSpot;
int AHT_filenr;
bool AHT_complex;
public:
int NrTextAboveHead;
SDL_Texture *txTextAboveHead[5];

void init(void);
void addanime(int nr,int x,int y);
void addparticle( int x,int y,int nr,int range );
void addfreeparticle( int x,int y,int nr,int range );
void GetInfoAHeroTxt( InterSpot *cis );
void ReadRenderAHeroTxt();
void handle_anime();
void showanimes(void);
void cleanup(void);
}ALLANIMES;

#endif // ANIM_H_INCLUDED
