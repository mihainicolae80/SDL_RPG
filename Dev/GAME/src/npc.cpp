#include "npc.h"


void NPCs::init(){
int i,j;
healthbar   = LoadTex( "textures//combat//healthbar.png" );
healthgreen = LoadTex( "textures//combat//healthgreen.png" );
healthboss     = LoadTex( "textures//interface//bosshealth_empty.png" );
healthbossfill = LoadTex( "textures//interface//bosshealth_full.png" );
rHealthBar.x = 0;
rHealthBar.y = 0;
SDL_QueryTexture( healthgreen,0,0,&rHealthBar.w,&rHealthBar.h );


FirstNPC = new NotPLayerCreature;
LastNPC = new NotPLayerCreature;
FirstNPC->next = LastNPC;
LastNPC->next = NULL;

for( i = 0; i < 13; i ++ )
    for( j = 0; j < 8; j ++ )
    {
        NPC_BodyFrames_NORMAL[i][j].x = BLOCK_WIDTH*j;
        NPC_BodyFrames_NORMAL[i][j].y = BLOCK_HEIGHT*i;
        NPC_BodyFrames_NORMAL[i][j].w = BLOCK_WIDTH;
        NPC_BodyFrames_NORMAL[i][j].h = BLOCK_HEIGHT;
    }

}
void NPCs::manipulate( int action,int act_info,int iTarget,string sTarget ){
NotPLayerCreature *inpc;
switch( action )
{
        case SPAWN:
        if( sTarget != "" )
        {
            for( inpc = FirstNPC->next; inpc != LastNPC; inpc = inpc->next )
            {
                if( inpc->Name == sTarget )
                inpc->Spawned = true;
            }

        }
        break;
        case UNSPAWN:
        if( sTarget != "" )
        {
            for( inpc = FirstNPC->next; inpc != LastNPC; inpc = inpc->next )
            {
                if( inpc->Name == sTarget )
                inpc->Spawned = false;
            }

        }
        break;
}
}
void NPCs::behave(){/*Independent Bahavior*/

}
void NPCs::handle_interaction(){
NotPLayerCreature *inpc;
stringstream ssaux;
SDL_Surface *tempsurf = NULL;
StillTalkNPC = false;
for( inpc = FirstNPC->next; inpc != LastNPC; inpc = inpc->next )
{
    /// If in melee/talk range
    if( PLAYER._PhysicalBody.x + PLAYER._PhysicalBody.w > inpc->rFrame.x + inpc->rBody.x - inpc->rBody.w/2 &&
        PLAYER._PhysicalBody.x < inpc->rFrame.x + inpc->rBody.x + inpc->rBody.w + inpc->rBody.w/2 &&
        PLAYER._PhysicalBody.y + PLAYER._PhysicalBody.h > inpc->rFrame.y + inpc->rBody.y - inpc->rBody.h/2&&
        PLAYER._PhysicalBody.y < inpc->rFrame.y + inpc->rBody.y + inpc->rBody.h + inpc->rBody.h/2
        && inpc->Spawned )
    {
        /**Talk To the player

        *Near NPC -> Talk(E): NAME
        av_action = 3
        **/
        if( inpc->behaviors & FLAG_TALK_TO_PLAYER )
        {
        if( PLAYER.AboveHeadTip != NULL )
        {
            SDL_DestroyTexture( PLAYER.AboveHeadTip );
            PLAYER.AboveHeadTip = NULL;
        }
        ssaux.str("");
        ssaux<<"Talk(E):"<<inpc->Name;

        tempsurf = TTF_RenderText_Blended(Font14,ssaux.str().c_str(),BLACK);
        PLAYER.AboveHeadTip = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf  );
        SDL_FreeSurface( tempsurf );

        PLAYER.SHOW_TIP = true;
        PLAYER.InNPCRange = true;
        PLAYER.av_action = 3;

        if( inpc == cNPC_talk )
        StillTalkNPC = true;

        cNPC_talk = inpc;
        }


        /** ABLE FOR COMBAT **/
        if( inpc->behaviors & FLAG_COMBAT_ABLE && !inpc->Dead )
        {
            if( inpc->comb )
                {

                    //spre hero

                    if( ( inpc->rFrame.x + inpc->rBody.x > PLAYER._PhysicalBody.x ) &&
                          BAS_abs( inpc->rFrame.x + inpc->rBody.x - PLAYER._PhysicalBody.x ) >
                          BAS_abs( inpc->rFrame.y + inpc->rBody.y - PLAYER._PhysicalBody.y ) )
                          {
                              inpc->anime_categ = 4;
                          }
                    else if( ( inpc->rFrame.x + inpc->rBody.x < PLAYER._PhysicalBody.x ) &&
                          BAS_abs( inpc->rFrame.x + inpc->rBody.x - PLAYER._PhysicalBody.x ) >
                          BAS_abs( inpc->rFrame.y + inpc->rBody.y - PLAYER._PhysicalBody.y ) )
                          {
                              inpc->anime_categ = 5;
                          }
                    else if( ( inpc->rFrame.y + inpc->rBody.y < PLAYER._PhysicalBody.y ) &&
                          BAS_abs( inpc->rFrame.x + inpc->rBody.x - PLAYER._PhysicalBody.x ) <
                          BAS_abs( inpc->rFrame.y + inpc->rBody.y - PLAYER._PhysicalBody.y ) )
                          {
                              inpc->anime_categ = 6;
                          }
                    else if( ( inpc->rFrame.y + inpc->rBody.y > PLAYER._PhysicalBody.y ) &&
                          BAS_abs( inpc->rFrame.x + inpc->rBody.x - PLAYER._PhysicalBody.x ) <
                          BAS_abs( inpc->rFrame.y + inpc->rBody.y - PLAYER._PhysicalBody.y ) )
                          {
                              inpc->anime_categ = 7;
                          }

                        //Incepe atacul
                        if( SDL_GetTicks() - inpc->tAttack >= 2000/inpc->attackspeed )
                        {
                            //Reset the timer
                            inpc->WillAttack = true;
                            inpc->tAttack = SDL_GetTicks();

                            inpc->attackstyle = !inpc->attackstyle;
                            if( inpc->attackstyle == 0 )
                            inpc->anime_frame = 0;
                            else
                            inpc->anime_frame = 4;

                        }

                    //Attacking....
                    if( (inpc->anime_frame != 1 && inpc->anime_frame != 5 ) &&
                        SDL_GetTicks() - inpc->tAttack >= (2000/inpc->attackspeed*10/100 ) &&
                        SDL_GetTicks() - inpc->tAttack <= (2000/inpc->attackspeed*35/100 ) )
                    {
                        if( inpc->attackstyle == 0 )
                        inpc->anime_frame = 1;
                        else
                        inpc->anime_frame = 5;
                    }
                    else
                    if( inpc->anime_frame != 2 && inpc->anime_frame != 6 &&
                             SDL_GetTicks() - inpc->tAttack >= ((Uint32)2000/inpc->attackspeed*35/100 ) &&
                             SDL_GetTicks() - inpc->tAttack <= ((Uint32)2000/inpc->attackspeed*60/100 ) )
                    {
                        if( inpc->attackstyle == 0 )
                        inpc->anime_frame = 2;
                        else
                        inpc->anime_frame = 6;
                    }
                    ///ATTACK THE PLAYER
                    else if( inpc->anime_frame != 3 && inpc->anime_frame != 7 &&
                             SDL_GetTicks() - inpc->tAttack >= ((Uint32)2000/inpc->attackspeed*60/100 ) &&
                             SDL_GetTicks() - inpc->tAttack <= ((Uint32)2000/inpc->attackspeed*100/100 ) )
                    {
                        if( inpc->attackstyle == 0 )
                        inpc->anime_frame = 3;
                        else
                        inpc->anime_frame = 7;
                        COMBAT.AttackAsNPC( inpc );
                    }
                }
        }
    }
    else if( !inpc->Dead )
    {
        PLAYER.InNPCRange = false;

        ///Combat Able
        if( inpc->behaviors & FLAG_COMBAT_ABLE && inpc->comb )
        {

            ///Movement Script
            if( inpc->tLastMove < SDL_GetTicks() - 30 )
            {
                int move_dir = -1,bad_dir = -1;
                int px,py,ex,ey,velx,vely,move_speed;

                px = PLAYER._PhysicalBody.x + PLAYER._PhysicalBody.w/2 ;
                py = PLAYER._PhysicalBody.y + PLAYER._PhysicalBody.h/2 ;
                ex = inpc->rFrame.x + inpc->rFrame.w/2 ;
                ey = inpc->rFrame.y + inpc->rFrame.h/2 ;
                move_speed = 3;

                ///DECIDE WAY
                //W == 0
                if(  px-ex  == 0 )
                {
                    if( py-ey > 0 ) move_dir = DOWN;
                    else move_dir = UP;
                }
                else
                //H == 0
                if(  py-ey  == 0 )
                {
                    if( px-ex > 0 ) move_dir = RIGHT;
                    else move_dir = LEFT;
                }
                else
                //W < H => Go H
                if( BAS_abs( px-ex ) < BAS_abs( py-ey ) )
                {
                    if( py-ey > 0 )
                    move_dir = DOWN;
                    else
                    move_dir = UP;
                }
                //W > H => Go W
                else
                {
                    if( px-ex > 0 )
                    move_dir = RIGHT;
                    else
                    move_dir = LEFT;
                }


                if( move_dir == UP )   vely =  -move_speed,velx = 0,inpc->anime_categ = 3;
                else
                if( move_dir == DOWN ) vely =   move_speed,velx = 0,inpc->anime_categ = 2;
                else
                if( move_dir == LEFT )  velx =  -move_speed,vely = 0,inpc->anime_categ = 0;
                else
                if( move_dir == RIGHT ) velx =   move_speed,vely = 0,inpc->anime_categ = 1;

                //Check Collision


                    inpc->rFrame.x += velx;
                    inpc->rFrame.y += vely;

                ///Avoid Collision
                if( detect_colision( inpc ) == true )
                {
                    bad_dir = move_dir;
                    move_dir = -1;

                    inpc->rFrame.x -= velx;
                    inpc->rFrame.y -= vely;

                    if( bad_dir == UP || bad_dir == DOWN )
                    {

                        if( ( inpc->rFrame.x + inpc->rFrame.w ) - inpc->ColidedWith_x <
                            ( inpc->ColidedWith_x + inpc->ColidedWith_w ) - inpc->rFrame.x )
                        move_dir = LEFT;
                        else
                        move_dir = RIGHT;

                    }
                    else
                    if( bad_dir == LEFT || bad_dir == RIGHT )
                    {
                        if( ( inpc->rFrame.y + inpc->rFrame.h ) - inpc->ColidedWith_y <
                            ( inpc->ColidedWith_y + inpc->ColidedWith_h - inpc->rFrame.y ) )
                        move_dir = UP;
                        else
                        move_dir = DOWN;
                    }

                    if( move_dir == UP )   vely =  -move_speed,velx = 0,inpc->anime_categ = 3;
                    else
                    if( move_dir == DOWN ) vely =   move_speed,velx = 0,inpc->anime_categ = 2;
                    else
                    if( move_dir == LEFT )  velx =  -move_speed,vely = 0,inpc->anime_categ = 0;
                    else
                    if( move_dir == RIGHT ) velx =   move_speed,vely = 0,inpc->anime_categ = 1;

                    inpc->rFrame.x += velx;
                    inpc->rFrame.y += vely;

                    if( detect_colision( inpc ) == true )
                    {
                        inpc->rFrame.x -= velx;
                        inpc->rFrame.y -= vely;
                    }
                }



                inpc->tLastMove = SDL_GetTicks();
            }

            //Movement Animation
            if( inpc->tMove < SDL_GetTicks() - 150 )
            {
                inpc->anime_frame ++;
                if( inpc->anime_frame > 7 )
                inpc->anime_frame = 0;
                inpc->tMove = SDL_GetTicks();
            }

        }
    }
}

if( !StillTalkNPC )
PLAYER.SPEAK_NPC = false;
}
void NPCs::show(int layer){
NotPLayerCreature *inpc;
/** layer 0 BG ; layer 1 FG **/
for( inpc = FirstNPC->next; inpc != LastNPC; inpc = inpc->next )
if(( ( layer == 0 && inpc->rFrame.y + inpc->rBody.y + inpc->rBody.h < PLAYER._PhysicalBody.y + PLAYER._PhysicalBody.h ) ||
    ( layer == 1 && inpc->rFrame.y + inpc->rBody.y + inpc->rBody.h >= PLAYER._PhysicalBody.y + PLAYER._PhysicalBody.h ) )&&
     !inpc->Dead && inpc->Spawned )//Above hero condition
{
    ApplyTex( GAME_MAP.DisplayLocation_x +inpc->rFrame.x,//Mapx
              GAME_MAP.DisplayLocation_y+inpc->rFrame.y,
              inpc->tex,&NPC_BodyFrames_NORMAL[inpc->anime_categ][inpc->anime_frame] ) ;

    if( inpc->behaviors & FLAG_COMBAT_ABLE )
    {


        if( inpc->boss == false )
        {
            SDL_QueryTexture( healthgreen,0,0,&rHealthBar.w,&rHealthBar.h );
            rHealthBar.w = rHealthBar.w*inpc->health/inpc->maxhealth;
            ApplyTex(GAME_MAP.DisplayLocation_x+inpc->rFrame.x + inpc->rBody.x -5,
                      GAME_MAP.DisplayLocation_y+inpc->rFrame.y + inpc->rBody.y - 40,healthbar,NULL );
            ApplyTex(GAME_MAP.DisplayLocation_x+inpc->rFrame.x + inpc->rBody.x -5,
                      GAME_MAP.DisplayLocation_y+inpc->rFrame.y + inpc->rBody.y - 40,healthgreen,&rHealthBar );
        }
        else
        {
            rHealthBar.w = 1000*inpc->health/inpc->maxhealth;
            rHealthBar.h = 60;
            ApplyTex( (SCREEN_WIDTH-1000)/2,50,healthbossfill,&rHealthBar );
            ApplyTex( (SCREEN_WIDTH-1000)/2,50,healthboss );
        }

    }
//DEBUG_outf << "NPC found on " <<Mapx+inpc->rFrame.x<<','<<Mapy+inpc->rFrame.y<<" name:"<<inpc->Name<<'\n';

}
}
DIALOG_TOPIC *NPCs::GetNPC_talk(){
return cNPC_talk->Dialog_maintopic;
}
void NPCs::cleanup_allnpc(){
NotPLayerCreature *delaux;
while( FirstNPC->next != LastNPC )
{
    delaux = FirstNPC->next;
    FirstNPC->next = FirstNPC->next->next;
    delete delaux;
}
}
NPCs::~NPCs(){
NPCs::cleanup_allnpc();
}
