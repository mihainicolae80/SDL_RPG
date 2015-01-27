#include "interface.h"





void Interface::init(){
show_spelltip = false;
last_shown = -1;
sSpell_bg = LoadTex( "textures//interface//spell_background.png" );
hBar    = LoadTex( "textures//interface//healthbar.png" );
hLine   = LoadTex( "textures//interface//healthline.png" );
rBar    = LoadTex( "textures//interface//resbar.png" );
rLine   = LoadTex( "textures//interface//resline.png" );
botline = LoadTex( "textures//interface//botline.png" );
abilitybar_frame = LoadTex( "textures//interface//abilitybar_frame.png" );
sArrows = LoadTex( "textures//combat//abilities//arrows.png" );
sActivated = LoadTex( "textures//combat//abilities//activated.png" );
sAutoAttack = LoadTex( "textures//combat//abilities//w_autoattack.png" );
sAbility[0][0] = LoadTex( "textures//combat//abilities//w_charge.png" );
sAbility[0][1] = LoadTex( "textures//combat//abilities//w_furystance.png" );
sAbility[0][2] = LoadTex( "textures//combat//abilities//w_finishingblow.png" );
sAbility[0][3] = LoadTex( "textures//combat//abilities//w_block.png" );
sAbility[0][4] = LoadTex( "textures//combat//abilities//w_unbreakablewill.png" );
sAbility[1][0] = LoadTex( "textures//combat//abilities//r_precisestrike.png" );
sAbility[1][1] = LoadTex( "textures//combat//abilities//r_hinder.png" );
sAbility[1][2] = LoadTex( "textures//combat//abilities//r_dodge.png" );
sAbility[1][3] = LoadTex( "textures//combat//abilities//r_subtlety.png" );
sAbility[1][4] = LoadTex( "textures//combat//abilities//r_adrenaline.png" );
sAbility[2][0] = LoadTex( "textures//combat//abilities//l_manabolt.png" );
sAbility[2][1] = LoadTex( "textures//combat//abilities//l_heal.png" );
sAbility[2][2] = LoadTex( "textures//combat//abilities//l_swap.png" );
sAbility[2][3] = LoadTex( "textures//combat//abilities//l_cloath.png" );
sAbility[2][4] = LoadTex( "textures//combat//abilities//l_rupture.png" );
sAbility[2][5] = LoadTex( "textures//combat//abilities//l_reserve.png" );
AttackImg = LoadTex( "textures//combat//attack.png" );
DefImg = LoadTex( "textures//combat//defense.png" );

SDL_QueryTexture( hLine,0,0,&HealthBar_w,&HealthBar_h );
SDL_QueryTexture( rLine,0,0,&ResBar_h,&ResBar_w );
SDL_QueryTexture( abilitybar_frame,0,0,&AbilityBar_w,&AbilityBar_h );

sDesc[0] = NULL;
sDesc[1] = NULL;
sName    = NULL;
sCd      = NULL;
sCost    = NULL;

/**
for( i = 0 ; i < 7; i++  )
{
    box_spell[i].h = 41;
    box_spell[i].w = 51;
    box_spell[i].x = SCREEN_WIDTH/2-abilitybar_frame->w/2+3+4*i+sAbility[2][0]->w*i;
    box_spell[i].y = SCREEN_HEIGHT - abilitybar_frame->h + 2;
}
**/
}
void Interface::display(){
/*
SDL_Rect cHealth,cRes;

cHealth.x = 0;
cHealth.y = HealthBar_h - ((PLAYER._health_c*HealthBar_h)/PLAYER._health_max);
cHealth.h = PLAYER._health_c*HealthBar_h/PLAYER._health_max;
cHealth.w = HealthBar_w;

cRes.x = 0;
cRes.y = ResBar_h*(PLAYER._power_max - PLAYER._power_c)/PLAYER._power_max;
cRes.h = ResBar_h - ResBar_h*(PLAYER._power_max - PLAYER._power_c)/PLAYER._power_max;
cRes.w = ResBar_w;

//Health bar
ApplyTex(SCREEN_WIDTH/10,SCREEN_HEIGHT*80/100-2,hBar,NULL);
ApplyTex(SCREEN_WIDTH/10+3,SCREEN_HEIGHT*80/100+(HealthBar_h -
         (PLAYER._health_c *HealthBar_h)/PLAYER._health_max),hLine,&cHealth);
//Mana bar
ApplyTex(SCREEN_WIDTH*80/100,SCREEN_HEIGHT*80/100+5,rBar,NULL);
ApplyTex(SCREEN_WIDTH*80/100,SCREEN_HEIGHT*80/100+5+(ResBar_h*(PLAYER._power_max
//          - PLAYER._power_c)/PLAYER._power_max),rLine,&cRes);
//Ability bar
//ApplyTex(SCREEN_WIDTH/2 - AbilityBar_w/2,SCREEN_HEIGHT -AbilityBar_h-3,abilitybar_frame);
*/

/*
//Def Images
rDef.x = SCREEN_WIDTH/2 - AbilityBar_w /2 - 45;
rDef.y = SCREEN_HEIGHT  - AbilityBar_h - 100;
rDef.w = 90;
rDef.h = 90;

SDL_SetRenderDrawColor( RENDERER_MAIN,255,255,255,255 );
SDL_RenderFillRect( RENDERER_MAIN,&rDef );
ApplyTex( rDef.x ,rDef.y ,DefImg,NULL);

//Atatck Images
rAttack.x = SCREEN_WIDTH/2 + AbilityBar_w/2 - 45;
rAttack.y = SCREEN_HEIGHT - AbilityBar_h  - 100;
rAttack.w = 90;
rAttack.h = 90;

SDL_RenderFillRect( RENDERER_MAIN,&rAttack );
ApplyTex( rAttack.x ,rAttack.y ,AttackImg,NULL);
*/
//Botbar
ApplyTex(0,BASE_SCREEN_HEIGHT - 3,botline,NULL);
}
void Interface::cleanup(){
int i;
SDL_DestroyTexture( hBar );
SDL_DestroyTexture( hLine );
SDL_DestroyTexture( rBar );
SDL_DestroyTexture( rLine );
SDL_DestroyTexture( botline );
SDL_DestroyTexture( abilitybar_frame );
SDL_DestroyTexture( sArrows );
SDL_DestroyTexture( sActivated );
SDL_DestroyTexture( sAutoAttack );

for(i = 0; i< 5; i++)
SDL_DestroyTexture( sAbility[0][i] );
for(i = 0; i< 5; i++)
SDL_DestroyTexture( sAbility[1][i] );
for(i = 0; i< 3; i++)
SDL_DestroyTexture( sAbility[2][i] );
}
void Interface::GiveDefAlpha( int nr ){
SDL_SetTextureAlphaMod( DefImg,nr );
}
void Interface::GiveAttackAlpha( int nr ){
SDL_SetTextureAlphaMod( AttackImg,nr );
}
void Interface::handle_events(SDL_Event newevent,GAME_event Gnewevent){
///int x,y;
stringstream ssaux;
if( Gnewevent.type == 72 )//weapon changed
last_shown = -1;

    if( newevent.type == SDL_MOUSEMOTION )
    {
        ///x = newevent.motion.x;
        ///y = newevent.motion.y;
        show_spelltip = false;
    }
}
