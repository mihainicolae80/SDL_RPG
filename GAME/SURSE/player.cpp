#include "player.h"


void CLS_PLAYER::init(){
int i,j;
//Physhical Body
_PhysicalBody.x = 0;
_PhysicalBody.y = 0;
_PhysicalBody.w = 40;
_PhysicalBody.h = 40;
//Skin Slices
for( i = 0; i < 13; i ++ )
    for( j = 0; j < 8; j ++ )
    {
        _SkinSlices[i][j].x = BLOCK_WIDTH*j;
        _SkinSlices[i][j].y = BLOCK_HEIGHT*i;
        _SkinSlices[i][j].w = BLOCK_WIDTH;
        _SkinSlices[i][j].h = BLOCK_HEIGHT;
    }
//Zone,health..
_czone = 1;
_health_c = _health_max = 100;
_power_c  = _power_max  = 100;
dmg_base  = 10;

//Stas
tStart = tMove = tAnim_cloak = SDL_GetTicks();
NrNpcInCombatWith = 0;
SHIELD_UP = false;
SHOW_TIP  = false;
InNPCRange= false;
SPEAK_NPC = false;
HideTip = false;
ShowTAH = false;
CombatCounter = SDL_GetTicks();
_txSkin = LoadTex( "textures//hero//Default.png" );
}
/*
bool detectCollision(void){
COLLIDER coll;

#define cl coll.next

    #define x CLS_PLAYER::_PhysicalBody.x
    #define y CLS_PLAYER::_PhysicalBody.y
    #define w CLS_PLAYER::_PhysicalBody.w
    #define h CLS_PLAYER::_PhysicalBody.h



//COLLISION AREAS
for( cl = FirstCol -> next; cl -> next != NULL; cl = cl -> next )
if( cl->ON && (x + w) > cl->x && x < (cl->x + cl->w) && (y + h) > cl->y && y < (cl->y + cl->h) )
{
    return true;
}
//COLLISION WITH NPCs
for( NPCList *cnpc = FirstEnemy->next;cnpc != LastEnemy; cnpc = cnpc->next )
if( cnpc->npc->Spawned && cnpc->npc->Dead == false && cnpc->npc &&
    x + w > cnpc->npc->rFrame.x + cnpc->npc->rBody.x  &&
    x     < cnpc->npc->rFrame.x + cnpc->npc->rBody.x + cnpc->npc->rBody.w &&
    y + h > cnpc->npc->rFrame.y + cnpc->npc->rBody.y  &&
    y     < cnpc->npc->rFrame.y + cnpc->npc->rBody.y + cnpc->npc->rBody.h )
    {
        return true;
    }

//IF NO COLLISION OCCURED
return false;
}
*/
void CLS_PLAYER::TeleportXY(int x,int y){
_PhysicalBody.x = x;
_PhysicalBody.y = y;
}
void CLS_PLAYER::teleportZone(int zone){
_czone = zone;
GAME_MAP.load( zone );
}
int  CLS_PLAYER::GetZone(void){
return _czone;
}
void CLS_PLAYER::TurnBody(int direction){
look_direct = direction;
}
void CLS_PLAYER::handleMovement(){
/** BUGG , INTR UN CADRU, PLAYERUL NU APARE  **/
int velX,velY;
InterSpot *t;
SDL_Surface *tempsurf = NULL;

const unsigned char* KEYDOWN = SDL_GetKeyboardState(NULL);

if( KEYDOWN[ SDL_SCANCODE_UP ] )    velY = -hero_movement_speed ,velX = 0;
else
if( KEYDOWN[ SDL_SCANCODE_DOWN ] )  velY =  hero_movement_speed ,velX = 0;
else
if( KEYDOWN[ SDL_SCANCODE_LEFT ] )  velX = -hero_movement_speed ,velY = 0;
else
if( KEYDOWN[ SDL_SCANCODE_RIGHT ] ) velX =  hero_movement_speed ,velY = 0;
else
velX = velY = 0;


//UP
if     ( velY < 0 ){
    if( armor_mode   != 3 ) armor_mode   = 3;
    if( cloak_cmode  != 3 ) cloak_cmode  = 3;
    if( shield_cmode != 3 ) shield_cmode = 3;
}
//DOWN
else if( velY > 0 ){
    if( armor_mode   != 2 ) armor_mode   = 2;
    if( cloak_cmode  != 2 ) cloak_cmode  = 2;
    if( shield_cmode != 2 ) shield_cmode = 2;
}
//LEFT
else if( velX < 0 ){
    if( armor_mode   != 0 ) armor_mode    = 0;
    if( cloak_cmode  != 0 ) cloak_cmode   = 0;
    if( shield_cmode != 0 ) shield_cmode  = 0;
}
//RIGHT
else if( velX > 0 ){
    if( armor_mode   != 1 ) armor_mode    = 1;
    if( cloak_cmode  != 1 ) cloak_cmode   = 1;
    if( shield_cmode != 1 ) shield_cmode  = 1;
}

//IDLE - NO_MOVE
if( velX == 0 && velY == 0 )
{
    //NONCOMBAT
    if( NrNpcInCombatWith == 0 )
    {
        look_categ = 4;
        //!!!LOOK DIRECTION!
        if( look_direct == UP ){
            if( look_frame != 3 )
            {
                look_frame = 3;
            }
        }
        else
        if( look_direct == DOWN ){
            if( look_frame != 2 )
            {
                look_frame = 2;
            }
        }
        else
        if( look_direct == LEFT ){
            if( look_frame != 0 )
            {
                look_frame = 0;
            }
        }
        else
        if( look_direct == RIGHT ){
            if( look_frame != 1 )
            {
                look_frame = 1;
            }
        }
    }
    //COMBAT
    else
    {
        look_frame = 0;
        if( look_direct == UP ){
        if( look_categ != 8 ) look_categ = 8;
        }
        else
        if( look_direct == DOWN ){
        if( look_categ != 7 ) look_categ = 7;
        }
        else
        if( look_direct == LEFT ){
        if( look_categ != 5 ) look_categ = 5;
        }
        else
        if( look_direct == RIGHT ){
        if( look_categ != 6 ) look_categ = 6;
        }
    }
}
//MOVE - NO_IDLE
else
{
    //NONCOMBAT
        if( KEYDOWN[ SDL_SCANCODE_UP ] ){
            if( look_categ  != 3 ) look_categ = 3;
            if( look_direct != UP ) look_direct = UP;
        }
        else
        if( KEYDOWN[ SDL_SCANCODE_DOWN ] ){
            if( look_categ  != 2 ) look_categ = 2;
            if( look_direct != DOWN ) look_direct = DOWN;
        }
        else
        if( KEYDOWN[ SDL_SCANCODE_LEFT ] ){
            if( look_categ  != 0 ) look_categ = 0;
            if( look_direct != LEFT ) look_direct = LEFT;
        }
        else
        if( KEYDOWN[ SDL_SCANCODE_RIGHT ] ){
            if( look_categ  != 1 ) look_categ = 1;
            if( look_direct != RIGHT ) look_direct = RIGHT;
        }

    ///}
    //COMBAT
   /// else
   /// {

   /// }
}

//Moveing anime timer
if( SDL_GetTicks() - tStart > 150 && ( velX != 0 || velY != 0 ) ){
tStart = SDL_GetTicks();

if( (look_categ >= 0 && look_categ <= 3) )
{
    look_frame++;
    if( look_frame > 2 ) look_frame = 0;
}

}

if( SDL_GetTicks() - tAnim_cloak > 300 )//Cloak anime timer
{
    tAnim_cloak = SDL_GetTicks();
    if( cloak_cframe < cloak_frames -1 )
    cloak_cframe ++;
    else
    cloak_cframe = 0;

}


///Movement
if( SDL_GetTicks() - tMove > (unsigned)HERO_RUN_TWAIT )
{
    tMove = SDL_GetTicks();

    _PhysicalBody.x += velX;
    _PhysicalBody.y += velY;


    if( detect_colision() )
    {
        _PhysicalBody.x -= velX;
        _PhysicalBody.y -= velY;
    }
    else if( !InNPCRange )
    {
        SHOW_TIP = false;
    }

}



//Check InterSpot collision
InterSpot *delaux = NULL;

if( FirstItr->next!= LastItr && FirstItr->next->type == -1 )
{
    delaux = FirstItr->next;
    FirstItr->next = FirstItr->next->next;
    delete delaux;
}



for( t = FirstItr -> next; t != LastItr ; t = t -> next )
{

    if( _PhysicalBody.x + _PhysicalBody.w > t->x && _PhysicalBody.x < t->x + t->w &&
        _PhysicalBody.y + _PhysicalBody.h > t->y && _PhysicalBody.y < t->y + t->h )
    {

        if(t->type == 1 )///DOOR
        {
            if( AvailableInterSpot != t )
            {
                if( AboveHeadTip != NULL )
                {
                    SDL_DestroyTexture( AboveHeadTip );
                    AboveHeadTip = NULL;
                }
                if( tempsurf != NULL )
                {
                    SDL_FreeSurface( tempsurf );
                    tempsurf = NULL;
                }

                tempsurf = TTF_RenderText_Blended(Font14,"Enter(E)",BLACK);
                AboveHeadTip = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );

            }

            SHOW_TIP = true;
            PLAYER.av_action = 1;
        }
        else if( t->type == 2 )///Text Above Hero Head
        {

                //Complexe 0/1 : May have behaviors read from the text File
                //File nr: File's nr from wich we read


            if( AvailableInterSpot != t )
            {
                if( AboveHeadTip != NULL )
                {
                    SDL_DestroyTexture( AboveHeadTip );
                    AboveHeadTip = NULL;
                }
                if( tempsurf != NULL )
                {
                    SDL_FreeSurface( tempsurf );
                    tempsurf = NULL;
                }
                tempsurf = TTF_RenderText_Solid(Font14,"Use(E)",BLACK);
                AboveHeadTip = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
            }

            SHOW_TIP = true;
            ALLANIMES.GetInfoAHeroTxt( t );//t->info[1] ,t->info[0]
            PLAYER.av_action = 2;
        }
        else if( t->type == 0 )///CUSTOM
        {

        }
        else
        if( t->type == 3 )///LOOT
        {
            if( AvailableInterSpot != t )
            {
                if( AboveHeadTip != NULL )
                {
                    SDL_DestroyTexture( AboveHeadTip );
                    AboveHeadTip = NULL;
                }
                if( tempsurf != NULL )
                {
                    SDL_FreeSurface( tempsurf );
                    tempsurf = NULL;
                }

                tempsurf = TTF_RenderText_Blended(Font14,"Loot(E)",BLACK);
                AboveHeadTip = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );

            }

            SHOW_TIP = true;
            PLAYER.av_action = 4;
        }

        AvailableInterSpot = t;
        break;


    }
    else if( t->next == LastItr )
    {
        AvailableInterSpot = NULL;
    }

        if( t->next != LastItr && t->next->type == -1 )
        {
            delaux = t->next;
            t->next = t->next->next;
            delete delaux;
        }

}


}
void CLS_PLAYER::draw(void){

//Shielf BG
if( shield_cmode == 2 )
ApplyTex( GAME_MAP.DisplayLocation_x + _PhysicalBody.x - BLOCK_WIDTH/2  + _PhysicalBody.w/2 ,
          GAME_MAP.DisplayLocation_y + _PhysicalBody.y - BLOCK_HEIGHT/2 - 10 ,
          txEQUIPT_Shield,&_SkinSlices[shield_cmode][shield_cframe] );

//Cloak(BG)
if( cloak_cmode == 2 )
ApplyTex( GAME_MAP.DisplayLocation_x + _PhysicalBody.x - BLOCK_WIDTH/2  + _PhysicalBody.w/2 ,
          GAME_MAP.DisplayLocation_y + _PhysicalBody.y - BLOCK_HEIGHT/2 - 10 ,
          txEQUIPT_Cloak,&_SkinSlices[cloak_cmode][cloak_cframe] );

//Skin
ApplyTex( GAME_MAP.DisplayLocation_x + _PhysicalBody.x - BLOCK_WIDTH/2  + _PhysicalBody.w/2 ,
          GAME_MAP.DisplayLocation_y + _PhysicalBody.y - BLOCK_HEIGHT/2 - 10 ,
          _txSkin,&_SkinSlices[look_categ][look_frame] );
/**/
//Chest
ApplyTex( GAME_MAP.DisplayLocation_x + _PhysicalBody.x - BLOCK_WIDTH/2  + _PhysicalBody.w/2 ,
          GAME_MAP.DisplayLocation_y + _PhysicalBody.y - BLOCK_HEIGHT/2 - 10 ,
          txEQUIPT_Chest,&_SkinSlices[armor_mode][0] );

//Cloak(FG)
if( cloak_cmode != 2 )
ApplyTex( GAME_MAP.DisplayLocation_x + _PhysicalBody.x - BLOCK_WIDTH/2  + _PhysicalBody.w/2 ,
          GAME_MAP.DisplayLocation_y + _PhysicalBody.y - BLOCK_HEIGHT/2 - 10 ,
          txEQUIPT_Cloak,&_SkinSlices[cloak_cmode][cloak_cframe] );
//Shield(FG)
if( shield_cmode != 2 )
ApplyTex( GAME_MAP.DisplayLocation_x + _PhysicalBody.x - BLOCK_WIDTH/2  + _PhysicalBody.w/2 ,
          GAME_MAP.DisplayLocation_y + _PhysicalBody.y - BLOCK_HEIGHT/2 - 10 ,
          txEQUIPT_Shield,&_SkinSlices[shield_cmode][shield_cframe] );

// ABOVE HEAD TIP
if( PLAYER.SHOW_TIP && !HideTip )
ApplyTex( PLAYER._PhysicalBody.x - 25 + PLAYER._PhysicalBody.w/2 + GAME_MAP.DisplayLocation_x,
          PLAYER._PhysicalBody.y - BLOCK_HEIGHT/2 - 25 + GAME_MAP.DisplayLocation_y,
          PLAYER.AboveHeadTip );

for( int i = 0; i < 5; i++ )
ApplyTex( PLAYER._PhysicalBody.x - 25 + PLAYER._PhysicalBody.w/2 + GAME_MAP.DisplayLocation_x,
          PLAYER._PhysicalBody.y - BLOCK_HEIGHT/2 - 25 + GAME_MAP.DisplayLocation_y - 10 - 12*(ALLANIMES.NrTextAboveHead - i ) - 8,
          ALLANIMES.txTextAboveHead[i]);

}
void CLS_PLAYER::handleEvents( SDL_Event *EVENT ){


if( EVENT->type == SDL_KEYDOWN )
{
    if( event.key.keysym.sym == SDLK_e )
    {
        //ACTION
        if( !HideTip && PLAYER.SHOW_TIP )//If Show Tips
        {
            if( PLAYER.av_action == ACTION_OPENDOOR )//Open door
            {
                if( PLAYER.AvailableInterSpot->info[0] == true )//LOCKED message
                {
                    PLAYER.AboveHeadTip = BAS_RenderText_Texture( Font14,"Locked",BLACK );
                }
                else
                {
                    PLAYER._PhysicalBody.x = PLAYER.AvailableInterSpot->info[1];
                    PLAYER._PhysicalBody.y = PLAYER.AvailableInterSpot->info[2];
                    PLAYER.teleportZone( PLAYER.AvailableInterSpot->info[3] );
                    PLAYER.SHOW_TIP = false;
                }
            }
            else if( PLAYER.av_action == ACTION_TEXTABOVEHEAD )//Text Above Hero
            {
                ALLANIMES.ReadRenderAHeroTxt();
                ShowTAH = true;
                HideTip = true;
            }
            else if( PLAYER.av_action == ACTION_DIALOG )//Open dialog with npc
            {
                if( PLAYER.SPEAK_NPC == false )
                {
                    DIALOG.open( GAME_NPCS.GetNPC_talk() );
                }
            }
            else
            if( PLAYER.av_action == ACTION_LOOT )//LOOT
            {
                LOOT.loadtable( 1 );
                LOOT.SHOW_LOOT = true;
            }
        }
        else//If Show something else
        {
            HideTip = false;
            ShowTAH = false;
        }
    }
    else
    if( event.key.keysym.sym == SDLK_r && INVENTORY.Is_ShieldEquip() &&
        PLAYER.tAttack + 250 < SDL_GetTicks() )//SHIELD DEF
    {
        CombatCounter = SDL_GetTicks();
        PLAYER.SHIELD_UP = true;
        PLAYER.shield_cframe = 2;
    }
    else
    if( event.key.keysym.sym == SDLK_t && PLAYER.SHIELD_UP == false ){//ATTACK

        if( PLAYER.tAttack + 250 < SDL_GetTicks() ){//200 = attack duration
        PLAYER.tAttack = SDL_GetTicks();

        CombatCounter = SDL_GetTicks();

        COMBAT.AttackAsPLAYER();
        }
    }
}
else
if( event.type == SDL_KEYUP )
{

    if( event.key.keysym.sym == SDLK_r )
    {
        PLAYER.SHIELD_UP = false;
        PLAYER.shield_cframe = 1;
    }

}

}
void CLS_PLAYER::handleMisc(){


if( PLAYER.NrNpcInCombatWith == 0 && CombatCounter < SDL_GetTicks() - 4000 )
{
    PLAYER.shield_cframe = 0;
}


}

