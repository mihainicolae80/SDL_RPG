#include "base.h"

 SDL_Surface *screen = NULL,*NULL_img;

 int SCREEN_WIDTH = 1280;
 int SCREEN_HEIGHT = 720;
 const int BLOCK_WIDTH = 100;
 const int BLOCK_HEIGHT = 100;
 const int SCREEN_BPP = 32;
 const int MAXNRMAP = 100;


 int cDayTime,iMoveMap_x,iMoveMap_y = 50,iInitial_x,iInitial_y,FCMove_x,FCMove_y,maxx,maxy,cInterItm,InterInfo[3];
 int m_x,m_y;
 bool bCtrlDown,bMouseDrag, bCSkinOrigin = false,FCmoveing = false,LightMap_Changed[6],LibLock[3];

 TTF_Font *font14,*font;
 SDL_Event event;
 bool DEFINE_MAP;
 SDL_Color BLUE = {0,0,255};
 SDL_Color RED = {255,0,0};
 SDL_Color BYellow = {200,200,130};
 SDL_Color BLACK = {0,0,0};






void ctos(char carray[],string &str ){

    str.erase(0, str.length());
    for( int i = 0; carray[i] != 0; i++ )
    str += carray[i];
}

bool Init(){

if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
return false;

const SDL_VideoInfo *vinfo = SDL_GetVideoInfo();

//SCREEN_HEIGHT = vinfo->current_h;
//SCREEN_WIDTH = vinfo->current_w;

screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
if( screen == NULL )
return false;

TTF_Init();
font14 = TTF_OpenFont("textures//fonts//font_knight.ttf",14);
font = TTF_OpenFont("textures//fonts//font_knight.ttf",25);
if( font14 == NULL || font == NULL )
return false;

SDL_WM_SetCaption("K.O.T Map Editor",NULL);



NULL_img = load_image( "Editor//NULL_img.png" );

chunk_NULL.nr = -1;
chunk_NULL.orient = 0;
chunk_NULL.rotate = false;
chunk_NULL.skin = NULL;

NULL_FreeChunk = new FreeChunk;
NULL_FreeChunk ->next = NULL;
NULL_FreeChunk ->skin = NULL;

FirstFreeChunk_fore = new FreeChunk;
LastFreeChunk_fore = new FreeChunk;

FirstFreeChunk_fore -> next = LastFreeChunk_fore;
LastFreeChunk_fore -> next = NULL;

FirstFreeChunk_back = new FreeChunk;
LastFreeChunk_back = new FreeChunk;

FirstFreeChunk_back -> next = LastFreeChunk_back;
LastFreeChunk_back -> next = NULL;

FirstFreeChunk_dynam = new FreeChunk;
LastFreeChunk_dynam = new FreeChunk;

FirstFreeChunk_dynam -> next = LastFreeChunk_dynam;
LastFreeChunk_dynam -> next = NULL;

LastChunkBack = FirstFreeChunk_back;
LastChunkFore = FirstFreeChunk_fore;
LastChunkDynam = FirstFreeChunk_dynam;

FirstDNChunk = new DayNightChunk;
LastDNChunk = new DayNightChunk;

FirstDNChunk ->next = LastDNChunk;
LastDNChunk -> next = NULL;

FirstFCLib = new FreeChunk;
C_FreeChunk = new FreeChunk;
C_FreeChunk -> skin = NULL_img;

FirstCOLChunk = new ColideChunk;
LastCOLChunk = new ColideChunk;
FirstCOLChunk -> next = LastCOLChunk;
LastCOLChunk -> next = NULL;
CCOLChunk = NULL;

CDNChunk = NULL;

FirstItr = new InterSpot;
LastItr = new InterSpot;
FirstItr -> next = LastItr;

return true;
}
void AddChunk( FreeChunk *modelchunk,int x, int y,int list ){
FreeChunk *newchunk = new FreeChunk;
newchunk -> nr = modelchunk ->nr;
newchunk -> orient = modelchunk -> orient;

newchunk -> x = x;
newchunk -> y = y;
newchunk->frames = modelchunk->frames;
newchunk->animated = modelchunk->animated;
newchunk->cframe = 0;

switch( modelchunk -> orient ) //chose / render skin
{
    case 0:
    newchunk -> skin = modelchunk -> skin;
    break;
    case 1:
    newchunk -> skin = Pixels.rotate_img( modelchunk -> skin ,ROTATE_RIGHT,newchunk->animated,newchunk->frames ) ;
    break;
    case 2:
    newchunk -> skin = Pixels.flip_img( modelchunk -> skin , FLIP_HOR ) ;
    break;
    case 3:
    newchunk -> skin = Pixels.rotate_img( modelchunk -> skin, ROTATE_LEFT,newchunk->animated,newchunk->frames ) ;
    break;
}


if( list == FreeBG )
{
    newchunk->next = LastChunkBack->next;
    LastChunkBack->next = newchunk;
    LastChunkBack = newchunk;
}
else if( list == FreeFG )
{
    newchunk->next = LastChunkFore->next;
    LastChunkFore->next = newchunk;
    LastChunkFore = newchunk;
}
else if( list == FreeDynam )
{
    newchunk->next = LastChunkDynam->next;
    LastChunkDynam->next = newchunk;
    LastChunkDynam = newchunk;
}


if( newchunk -> nr == DOOR_NR )
{
    newchunk ->info[0] = InterInfo[0];
    newchunk ->info[1] = InterInfo[1];
    newchunk ->info[2] = InterInfo[2];
}

}
DayNightChunk *AddLight( int x, int y,int nr, int range ){
DayNightChunk *newchunk = new DayNightChunk;
newchunk ->x = x;
newchunk ->y = y;
newchunk ->range = range;
newchunk ->nr = nr;

newchunk -> next = FirstDNChunk ->next;
FirstDNChunk -> next = newchunk;
return newchunk;
}
ColideChunk *AddColider( int x,int y, int w,int h,int id ){
ColideChunk *newchunk = new ColideChunk;
newchunk ->x = x;
newchunk ->y = y;
newchunk ->w = w;
newchunk ->h = h;
newchunk ->id = id;
newchunk ->g1x = x - w/2;
newchunk ->g1y = y - h/2;
newchunk ->g2x = x + w/2;
newchunk ->g2y = y + h/2;

newchunk ->skin = Pixels.RenderRect(w,h,0);

newchunk -> next = FirstCOLChunk ->next;
FirstCOLChunk -> next = newchunk;
return newchunk;
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
nou -> id      = id;
nou -> info[0] = i1;
nou -> info[1] = i2;
nou -> info[2] = i3;
nou -> info[3] = i4;
nou -> skin = Pixels.RenderRect( nou -> w,nou -> h ,0 );
return nou;
}
InterSpot *AddInterSpot( int x,int y ){
InterSpot *nou = new InterSpot;
nou -> next = FirstItr -> next;
FirstItr -> next = nou;
nou -> x = x;
nou -> y = y;
nou -> w = nou -> h = 60;
nou -> type = 0;
nou -> id = 0;
nou -> info[0] = 0;
nou -> info[1] = 0;
nou -> info[2] = 0;
nou -> info[3] = 0;
nou -> skin = Pixels.RenderRect( nou -> w,nou -> h ,0 );
return nou;
}
NPCchunk *AddNPCchunk( NPCchunk *modelchunk ){
NPCchunk *newchunk = new NPCchunk;

newchunk->skin = modelchunk->skin;
newchunk->x = modelchunk->x;
newchunk->y = modelchunk->y;
newchunk->w = modelchunk->w;
newchunk->h = modelchunk->h;
newchunk->Name = modelchunk->Name;
newchunk->Faction = "NONE";
newchunk->Behavior = "NONE";
newchunk->Relationship = 1;
newchunk->CanTalk = true;
newchunk->filenr = modelchunk->filenr;
newchunk->skinnr = modelchunk->skinnr;
newchunk->next = FirstNPCMap->next;
FirstNPCMap->next = newchunk;

return newchunk;
}
SDL_Surface *load_image(string filename){
SDL_Surface *loadedimg = NULL;
SDL_Surface *optimimg = NULL;

loadedimg = IMG_Load(filename.c_str());
if(loadedimg == NULL)
return NULL;

optimimg = SDL_DisplayFormat(loadedimg);
SDL_FreeSurface(loadedimg);
Uint32 colorkey = SDL_MapRGB(optimimg->format,0,0xFF,0xFF);
SDL_SetColorKey(optimimg,SDL_SRCCOLORKEY,colorkey);
return optimimg;
}
void apply_Surface(int x,int y,SDL_Surface *source,SDL_Rect *clip){
SDL_Rect offset;
offset.x = x;
offset.y = y;
if(source != NULL)
SDL_BlitSurface(source,clip,screen,&offset);
}
void cleanup(){
FreeChunk *delaux,*p;
for( p = FirstFreeChunk_back  ; p ->next != NULL && p != NULL; p = p->next )
if( p -> next != LastFreeChunk_back )
{
    delaux = p -> next;
    p -> next = p -> next -> next;
    delete delaux;
}
delete FirstFreeChunk_back;
delete FirstFreeChunk_back;

for( p = FirstFreeChunk_fore  ; p ->next != NULL && p != NULL ; p = p->next )
if( p -> next != LastFreeChunk_fore )
{
    delaux = p -> next;
    p -> next = p -> next -> next;
    delete delaux;
}
delete FirstFreeChunk_fore;
delete FirstFreeChunk_fore;

SDL_Quit();
}
