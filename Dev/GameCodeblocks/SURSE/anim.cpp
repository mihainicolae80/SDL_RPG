#include "anim.h"
#include "basics.h"

void Anime::init(){

//Load imgs
txParticle[0] = LoadTex( "textures//environmental//particle_blue.png" );

//Animes
txAnimeSkin[0] = LoadTex( "textures//Animations//explosion1.png" );
txAnimeSkin[1] = LoadTex( "textures//Animations//explosion1_dynam.png" );


    //Anime init
    anime_prim = new ANIME;
    anime_ultim = new ANIME;
    anime_prim ->next = anime_ultim;
    anime_ultim ->next = NULL;

animebox.y = 0;
animebox.w = 50;
animebox.h = 50;
particlebox.y = 0;
particlebox.w = 3;
particlebox.h = 3;

txTextAboveHead[0] = NULL;
txTextAboveHead[1] = NULL;
txTextAboveHead[2] = NULL;
txTextAboveHead[3] = NULL;
txTextAboveHead[4] = NULL;

}
void Anime::addanime(int nr,int x,int y){
    ANIME *tempnew = new ANIME;
    if( nr == 0 )
    {
        tempnew->interval = 200;
        tempnew->goalfrap = 8;
        tempnew->w = 150;
        tempnew->h = 150;
    }
    else
    if( nr == 1 )
    {
        tempnew->interval = 2000;
        tempnew->goalfrap = 1;
        tempnew->w = 150;
        tempnew->h = 150;
    }
    else if( nr == 2 || nr == 3)
    {
        tempnew->interval = 150;
        tempnew->goalfrap = 2;
    }
    else if( nr == 4 || nr == 5 )
    {
        tempnew->interval = 150;
        tempnew->goalfrap = 3;
    }
    else if( nr == 6 || nr == 7 || nr == 8 || nr == 9 )
    {
        tempnew->interval = 100;
        tempnew->goalfrap = 4;
    }
    else if( nr == 10 )
    {
        tempnew->interval = 300;
        tempnew->goalfrap = 2;
    }

    tempnew->skin = txAnimeSkin[nr];
    tempnew->cfrap = 0;
    tempnew ->next = anime_prim ->next;
    anime_prim->next = tempnew;
    tempnew->time_when_change = SDL_GetTicks() + tempnew->interval;
    tempnew->x = x;
    tempnew->y = y;
}
void Anime::handle_anime(){
ANIME *p,*delaux;
for(p = anime_prim;p->next != anime_ultim&&p != anime_ultim;p = p->next)
{
    if( p->next->time_when_change <= SDL_GetTicks() )
    {
        if( p->next->cfrap < p->next->goalfrap )
        {
            p->next->time_when_change = SDL_GetTicks() + p->next->interval;
            p->next->cfrap ++;
        }
        else
        {
            //if loop then reset
            delaux = p->next;
            p->next = p->next ->next;
            delete delaux;
        }
    }
}
}
void Anime::showanimes(void){
ANIME *p;
for(p = anime_prim->next;p != anime_ultim;p = p->next)
{
    if( p->type == 'p'  )//Particle
    {
        particlebox.x = 3*p->cfrap;
        ApplyTex(p->x,p->y,p->skin,&particlebox);
    }
    else
    {
        animebox.x = p->w*p->cfrap;
        animebox.w = p->w;
        animebox.h = p->h;
        ApplyTex( GAME_MAP.DisplayLocation_x + p->x,
                  GAME_MAP.DisplayLocation_y + p->y,p->skin,&animebox);
    }
}
}
void Anime::addparticle( int x,int y,int nr,int range ){
ANIME *tempnew = new ANIME;
tempnew->skin = txParticle[nr];
tempnew->interval = 150;
tempnew->goalfrap = 3;
tempnew->cfrap = 0;
tempnew ->next = anime_prim ->next;
anime_prim->next = tempnew;
tempnew->time_when_change = SDL_GetTicks() + tempnew->interval;
tempnew->x = GAME_MAP.DisplayLocation_x + x - range + rand()%range ;
tempnew->y = GAME_MAP.DisplayLocation_y + y - range + rand()%range ;
tempnew->type = 'p';
}
void Anime::addfreeparticle( int x,int y,int nr,int range ){

ANIME *tempnew = new ANIME;


if( nr == 0 )
{
    tempnew->skin = txParticle[0];
    tempnew->interval = 100;
    tempnew->goalfrap = 3;
}
else if( nr == 1 )
{
    tempnew->skin = txParticle[0];
    tempnew->interval = 30;
    tempnew->goalfrap = 3;
}

tempnew->cfrap = 0;
tempnew ->next = anime_prim ->next;
anime_prim->next = tempnew;
tempnew->time_when_change = SDL_GetTicks() + tempnew->interval;
tempnew->x = x - range +rand()%range;
tempnew->y = y - range +rand()%range;
tempnew->type = 'p';
}
void Anime::GetInfoAHeroTxt( InterSpot *cis ){//int filenr, bool comp
AHT_filenr  = cis->info[1];
AHT_complex = cis->info[0];
cIntSpot = cis;
}
void Anime::ReadRenderAHeroTxt(){
stringstream link;
ifstream in;
int i;
char Texts[5][50];
string sRead;
SDL_Surface *tempsurf = NULL;

link << "Data//flashtexts//" << AHT_filenr << ".in";
in.open( link.str().c_str() );
in >> NrTextAboveHead;
in.get();

for( i = 0; i <= NrTextAboveHead; i++ )
{
    in.getline( Texts[i], 50);
}


    while( in.peek() != EOF )
    {
        in >> sRead;

        if( sRead == "change_type:" )
        {
            in >> PLAYER.AvailableInterSpot->type;
        }
        else if( sRead == "change_v0:" )
        {
            in >> PLAYER.AvailableInterSpot->info[0];
        }
        else if( sRead == "change_v1:" )
        {
            in >> PLAYER.AvailableInterSpot->info[1];
        }
        else if( sRead == "change_v2:" )
        {
            in >> PLAYER.AvailableInterSpot->info[2];
        }
        else if( sRead == "change_v3:" )
        {
            in >> PLAYER.AvailableInterSpot->info[3];
        }
        else
        if( sRead == "addquest:" )
        {
            int tempi;
            in >> tempi;

            QUEST.add( tempi );

        }
        else
        if( sRead == "advancequest:" )
        {
            int tempi;
            in >> tempi;

            QUEST.advance( tempi );
        }
        else
        if( sRead == "selfdestruct" )
        {
            cIntSpot->type = -1;
        }
    }


in.close();

for( i = 0; i < 5; i++ )
{
    if( txTextAboveHead[i] != NULL )
    {
        SDL_DestroyTexture( txTextAboveHead[i] );
        txTextAboveHead[i] = NULL;
    }

    if( i < NrTextAboveHead )
    {
        if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
        tempsurf = TTF_RenderText_Blended( Font14,Texts[i] ,BLACK );
        txTextAboveHead[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
    }


}

}
void Anime::cleanup(void){
int i;
for( i = 0 ; i < 10 ; i++ )
SDL_DestroyTexture( txAnimeSkin[i] );
for( i = 0 ; i < 1 ; i++ )
SDL_DestroyTexture( txParticle[i] );
}

