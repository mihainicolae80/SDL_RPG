#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "basics.h"
#include "main_headers.h"
#include "player.h"

extern class Interface{
private:
SDL_Texture *hBar,*hLine,*rBar,*rLine,*abilitybar_frame,*botline,*sSpell_bg;
SDL_Texture *sAbility[3][6],*sCdtxt[6],*sAutoAttack,*sAutoAttackcd,*sArrows,*sActivated;
SDL_Texture *sDesc[2],*sName,*sCd,*sCost;
SDL_Texture *DefImg,*AttackImg;
SDL_Rect box_spell[7];
int i,last_shown,HealthBar_h,HealthBar_w,ResBar_h,ResBar_w,AbilityBar_w,AbilityBar_h;
bool show_spelltip;
public:
void init();
void display();
void cleanup();
void GiveDefAlpha( int nr );
void GiveAttackAlpha( int nr );
void handle_logics();
void handle_events(SDL_Event newevent,GAME_event Gnewevent);
}cInterface;


#endif // INTERFACE_H_INCLUDED
