#include "combat.h"


void Combat::init(){
Stepcd = 0;
prim = new GAME_combattxt;
ultim = new GAME_combattxt;
prim ->next = ultim;
ultim->next = NULL;

}
void Combat::addtext(int x,int y,string sText,SDL_Color txtcolor){
SDL_Surface *tempsurf = NULL;
GAME_combattxt *nou = new GAME_combattxt,*p;
nou ->x = x;
nou ->y = y;
nou ->tFinish = 700 + SDL_GetTicks();


for( p = prim ->next; p != ultim; p = p-> next )
{
    if( nou->tFinish - p->tFinish < 300 )
    {
        nou->tFinish += 300;
        break;
    }
}


nou -> next = prim ->next;
prim ->next = nou;
tempsurf = TTF_RenderText_Blended(Font14,sText.c_str(),txtcolor);
nou ->txText = NULL;
nou ->txText = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );

}
void Combat::AttackAsPLAYER(){ //<<!>> ADD ITM DEF/DMG
int attack_x,attack_y,attack_w,attack_h,damage;///,revtraj
stringstream ssaux;

//Calculate Damage
/** add: dam variation ,item bonus damage **/
damage = PLAYER.dmg_base + PLAYER.dmg_weapon + PLAYER.dmg_var;


//Define attack range
if( PLAYER.look_direct == LEFT )
{
    attack_x = PLAYER._PhysicalBody.x - PLAYER._PhysicalBody.h;
    attack_y = PLAYER._PhysicalBody.y - PLAYER._PhysicalBody.w/2;
    attack_w = PLAYER._PhysicalBody.h;
    attack_h = PLAYER._PhysicalBody.w*2;
    ///revtraj  = RIGHT;
}
else if( PLAYER.look_direct == RIGHT )
{
    attack_x = PLAYER._PhysicalBody.x + PLAYER._PhysicalBody.h;
    attack_y = PLAYER._PhysicalBody.y - PLAYER._PhysicalBody.w/2;
    attack_w = PLAYER._PhysicalBody.h;
    attack_h = PLAYER._PhysicalBody.w*2;
    ///revtraj  = LEFT;
}
else if( PLAYER.look_direct == DOWN )
{
    attack_x = PLAYER._PhysicalBody.x - PLAYER._PhysicalBody.w/2;
    attack_y = PLAYER._PhysicalBody.y + PLAYER._PhysicalBody.h;
    attack_w = PLAYER._PhysicalBody.w*2;
    attack_h = PLAYER._PhysicalBody.h;
    ///revtraj  = UP;
}
else if( PLAYER.look_direct == UP )
{
    attack_x = PLAYER._PhysicalBody.x - PLAYER._PhysicalBody.w/2;
    attack_y = PLAYER._PhysicalBody.y - PLAYER._PhysicalBody.h;
    attack_w = PLAYER._PhysicalBody.w*2;
    attack_h = PLAYER._PhysicalBody.h;
    ///revtraj  = DOWN;
}

for( NPCList *cnpc = FirstEnemy->next; cnpc != LastEnemy; cnpc = cnpc->next )
{
    static NotPLayerCreature *inpc;
    inpc = cnpc->npc;
    if( inpc->rFrame.x + inpc->rBody.x + inpc->rBody.w > attack_x &&
        inpc->rFrame.x + inpc->rBody.x < attack_x + attack_w       &&
        inpc->rFrame.y + inpc->rBody.y + inpc->rBody.h > attack_y &&
        inpc->rFrame.y + inpc->rBody.y < attack_y + attack_h &&
        !inpc->Dead && inpc->Spawned )
        {

            if( inpc->comb == false ){
            inpc->comb = true;
            PLAYER.NrNpcInCombatWith++;
            }


            inpc->health -= damage;


            if( inpc->health <= 0 )
            {
                inpc->Dead = true;

                if( inpc->comb == true ){
                inpc->comb = false;
                PLAYER.NrNpcInCombatWith--;
                }

                AddInterSpot( inpc->rFrame.x,inpc->rFrame.y,inpc->rFrame.w,inpc->rFrame.h,3,//type 3 loot
                              1,0,0,0,0 );

                newGEvent = Gevent_NULL;
                newGEvent.name = inpc->Name;
                newGEvent.nr = 1;
                if( inpc->boss ) newGEvent.nr += 100;
                newGEvent.type = Event_EnemyDefeated;
                GAME_EVENTS.add( newGEvent );
            }


            ssaux << damage;
            CONSOLE.out( ssaux.str().c_str() );
            Combat::addtext(inpc->rFrame.x + inpc->rBody.x,inpc->rFrame.y + inpc->rBody.y - 35
                            ,ssaux.str().c_str(),RED);
            break;
        }
}
}
void Combat::AttackAsNPC(NotPLayerCreature *cnpc){
int attack_x,attack_y,attack_w,attack_h,revtraj;
stringstream ssaux;
//Define attack range

if( cnpc->anime_categ == 0 || cnpc->anime_categ == 4 )
{
    attack_x = cnpc->rFrame.x + cnpc->rBody.x - cnpc->rBody.h;
    attack_y = cnpc->rFrame.y + cnpc->rBody.y - cnpc->rBody.w/2;
    attack_w = cnpc->rBody.h;
    attack_h = cnpc->rBody.w*2;
    revtraj  = RIGHT;
}
else if( cnpc->anime_categ == 1 || cnpc->anime_categ == 5 )
{
    attack_x = cnpc->rFrame.x + cnpc->rBody.x + cnpc->rBody.h;
    attack_y = cnpc->rFrame.y + cnpc->rBody.y - cnpc->rBody.w/2;
    attack_w = cnpc->rBody.h;
    attack_h = cnpc->rBody.w*2;
    revtraj  = LEFT;
}
else if( cnpc->anime_categ == 2 || cnpc->anime_categ == 6 )
{
    attack_x = cnpc->rFrame.x + cnpc->rBody.x - cnpc->rBody.w/2;
    attack_y = cnpc->rFrame.y + cnpc->rBody.y + cnpc->rBody.h;
    attack_w = cnpc->rBody.w*2;
    attack_h = cnpc->rBody.h;
    revtraj  = UP;
}
else if( cnpc->anime_categ == 3 || cnpc->anime_categ == 7 )
{
    attack_x = cnpc->rFrame.x + cnpc->rBody.x - cnpc->rBody.w/2;
    attack_y = cnpc->rFrame.y + cnpc->rBody.y - cnpc->rBody.h;
    attack_w = cnpc->rBody.w*2;
    attack_h = cnpc->rBody.h;
    revtraj  = DOWN;
}
//nu in relatii bune cu PLAYER
if( PLAYER._PhysicalBody.x + PLAYER._PhysicalBody.w > attack_x &&
    PLAYER._PhysicalBody.x < attack_x    +    attack_w   &&
    PLAYER._PhysicalBody.y + PLAYER._PhysicalBody.h > attack_y &&
    PLAYER._PhysicalBody.y < attack_y    +    attack_h )
    {
        //BLOCKING
        if( revtraj == PLAYER.look_direct && PLAYER.SHIELD_UP )
        {
            Combat::addtext(PLAYER._PhysicalBody.x  - 40,PLAYER._PhysicalBody.y - 40,"BLOCKED",BLUE);
        }
        else
        {
            PLAYER._health_c -= (cnpc->dmg - (PLAYER.def_base+PLAYER.def_itm_chest)) > 0?(cnpc->dmg - (PLAYER.def_base+PLAYER.def_itm_chest)):0;
            if( cnpc->dmg - (PLAYER.def_base+PLAYER.def_itm_chest) > 0 )
            ssaux << (cnpc->dmg - PLAYER.def_base-PLAYER.def_itm_chest);
            else
            ssaux << '0';

            Combat::addtext(PLAYER._PhysicalBody.x - 20,PLAYER._PhysicalBody.y - 40,ssaux.str().c_str(),RED);
        }

    }


}
void Combat::showtexts(void){
for( GAME_combattxt *p = prim ->next;p != ultim;p = p->next)
{
    if( p->tFinish - SDL_GetTicks() <= 700  )
    ApplyTex( GAME_MAP.DisplayLocation_x + p->x,
              GAME_MAP.DisplayLocation_y + p->y + (p->tFinish - SDL_GetTicks())/50,
              p->txText);
}
}
void Combat::handletexts(void){
GAME_combattxt *delaux;
for( GAME_combattxt *p = prim;p->next != ultim&&p != ultim;p = p->next)
{
    if( p-> next-> tFinish < SDL_GetTicks() )
    {
        delaux = p->next;
        p ->next = p ->next->next;
        SDL_DestroyTexture(delaux->txText);
        delete delaux;
    }
    else if(p-> next-> tFinish - 300  < SDL_GetTicks() )
    {
        if( p->next->txText != NULL)
        SDL_SetTextureAlphaMod( p->next->txText,100 );
    }
}
}
int  Combat::Getx(){
return ix;
}
int  Combat::Gety(){
return iy;
}
Combat::~Combat(){
SDL_FreeSurface( sAbilityObj[0] );
}
