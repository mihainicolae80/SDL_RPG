#include "basics.h"
#include "base.h"
#include "player.h"

void   BAS_ctos( char carray[], string &strnew ){
int i;
strnew.erase();

while( carray[0] == ' ' )
{
    for( i = 1; (unsigned)i < SDL_strlen( carray ); i++ )
    carray[i-1] = carray[i];
    carray[i-1] = '\0';
}

for( int i = 0; carray[i] != '\0'; i++ )
strnew += carray[i];
}
double BAS_abs( double x ){
if( x < 0 )
return x * -1;
return x;
}
void   BAS_itos(int nr,string &newstr ){
std::stringstream ssaux;
newstr.erase();
ssaux<<nr;
ssaux>>newstr;
}
void   BAS_strcpy( char *dest,const char *sourc ){
int len_sourc,i;
len_sourc = SDL_strlen( sourc );

for( i = 0; i < len_sourc; i++ )
dest[i] = sourc[i];
dest[len_sourc] = '\0';
}
void   BAS_strcat( char *dest,const char *sourc ){
int len_dest,len_sourc,i;
len_dest  = SDL_strlen( dest );
len_sourc = SDL_strlen( sourc );

for( i = 0; i < len_sourc; i++ )
dest[i + len_dest ] = sourc[i];
dest[len_sourc + len_dest ] = '\0';
}
InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id){
InterSpot *nou = new InterSpot;
nou -> next = FirstItr -> next;
FirstItr -> next = nou;
nou -> x = x;
nou -> y = y;
nou -> w = w;
nou -> h = h;
nou -> type = type;
nou -> id = id;
nou -> info[0] = i1;
nou -> info[1] = i2;
nou -> info[2] = i3;
nou -> info[3] = i4;
return nou;
}


bool detect_colision(NotPLayerCreature *npc ){
COLLIDER *cl;
int x,y,w,h;

if( npc != NULL )
{
    x = npc->rFrame.x;
    y = npc->rFrame.y;
    w = npc->rFrame.w;
    h = npc->rFrame.h;
}
else
{
    x = PLAYER._PhysicalBody.x;
    y = PLAYER._PhysicalBody.y;
    w = PLAYER._PhysicalBody.w;
    h = PLAYER._PhysicalBody.h;
}


//COLLISION AREAS
for( cl = FirstCol -> next; cl -> next != NULL; cl = cl -> next )
if( cl->ON && (x + w) > cl->x && x < (cl->x + cl->w) && (y + h) > cl->y && y < (cl->y + cl->h) )
{
    if( npc != NULL )
    {
        npc->ColidedWith_x = cl->x,
        npc->ColidedWith_y = cl->y,
        npc->ColidedWith_w = cl->w,
        npc->ColidedWith_h = cl->h;
    }

    return true;
}

for( NPCList *cnpc = FirstEnemy->next;cnpc != LastEnemy; cnpc = cnpc->next )
if( cnpc->npc->Spawned && cnpc->npc->Dead == false && cnpc->npc != npc &&
    x + w > cnpc->npc->rFrame.x + cnpc->npc->rBody.x  &&
    x     < cnpc->npc->rFrame.x + cnpc->npc->rBody.x + cnpc->npc->rBody.w &&
    y + h > cnpc->npc->rFrame.y + cnpc->npc->rBody.y  &&
    y     < cnpc->npc->rFrame.y + cnpc->npc->rBody.y + cnpc->npc->rBody.h )
    {
        //If NPC
        if( npc != NULL )
        {
            npc->ColidedWith_x = cnpc->npc->rFrame.x,
            npc->ColidedWith_y = cnpc->npc->rFrame.y,
            npc->ColidedWith_w = cnpc->npc->rFrame.w,
            npc->ColidedWith_h = cnpc->npc->rFrame.h;
        }


        return true;
    }

return false;
}

int BAS_LoadFonts(void){

Font14 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,14 );
Font18 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,18 );
Font20 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,20 );
Font24 = TTF_OpenFont("textures//fonts//font_knight.ttf" ,24 );

Font2_14 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,14 );
Font2_18 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,18 );
Font2_20 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,20 );
Font2_24 = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,24 );

if( Font14 || Font18 || Font20 || Font24 ||
    Font2_14 || Font2_18 || Font2_20 || Font2_24 )
    return true;

return false;
}

SDL_Texture* BAS_RenderText( TTF_Font *font,const char *text,SDL_Color color ){

SDL_Surface *aux_surf = NULL;
SDL_Texture *dest = NULL;

aux_surf = TTF_RenderText_Blended( font,text,color );
dest = SDL_CreateTextureFromSurface( WIN_MAIN.render, aux_surf );
return dest;
}
void         BAS_DestroyTex( SDL_Texture **tex ){

if( &tex != NULL )
{
    SDL_DestroyTexture( *tex );
    *tex = NULL;
}
}

