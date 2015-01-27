#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <fstream>
#define FLIP_VERT 0x01
#define FLIP_HOR 0x02
#define ROTATE_LEFT 1
#define ROTATE_RIGHT 2
#define FreeBG 0
#define FreeFG 1
#define FreeDynam 2
#define DOOR_NR 1
#define CHANGE_INT 1
#define CHANGE_STR 2

#define itoa SDL_itoa
#define ltoa SDL_ltoa

using namespace std;

SDL_Surface *screen = NULL,*NULL_img;
int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 480;
const int BLOCK_WIDTH = 100;
const int BLOCK_HEIGHT = 100;
const int SCREEN_BPP = 32;
const int MAXNRMAP = 100;



//Classes
class PixelManipulation;

int cDayTime,iMoveMap_x,iMoveMap_y = 50,iInitial_x,iInitial_y,FCMove_x,FCMove_y,maxx,maxy,cInterItm,InterInfo[3];
int m_x,m_y;
bool bCtrlDown,bMouseDrag, bCSkinOrigin = false,FCmoveing = false,LightMap_Changed[6],LibLock[3];

struct CHUNK{
SDL_Surface *skin;
int nr,orient,frames,cframe;
int nr1,nr2,nr3;//door:zone,tox,toy;enemy:script;
bool rotate, animated;;
}chunk_NULL,chunk_C;

struct DECOR{
int nr,x,y,orient;
bool rotate,colide;
};
struct FreeChunk{
int x,y,orient,nr,frames,cframe;
bool animated;
int info[3];
SDL_Surface *skin;
FreeChunk *next;
}*FirstFCLib,*FirstFreeChunk_fore,*LastFreeChunk_fore,*FirstFreeChunk_back,
*LastFreeChunk_back,*FirstFreeChunk_dynam,*LastFreeChunk_dynam,*C_FreeChunk,
*NULL_FreeChunk,*FreeCTarget_Move,*LastChunkFore,*LastChunkBack,*LastChunkDynam;
struct DayNightChunk{
int nr,x,y,range;
DayNightChunk *next;
}*FirstDNChunk,*LastDNChunk,*CDNChunk;
struct DayNightTool{
int nr;
SDL_Surface *skin;
}DNTool[2],*CDNTool;
struct ColideChunk{
int w,h,x,y,g1x,g1y,g2x,g2y,id;
SDL_Surface *skin;
ColideChunk *next;
}*FirstCOLChunk,*LastCOLChunk,*CCOLChunk;
struct ColideTool{
int nr;
SDL_Surface *skin;
}COLTool[2],*CCOLTool;
struct InterSpot{
int x,y,w,h,type;
int info[4],id;
SDL_Surface *skin;
InterSpot *next;
}*FirstItr,*LastItr;
struct NPCchunk{
SDL_Surface *skin;
int x,y,w,h,Relationship,filenr,skinnr,nr;
string Name,Faction,Behavior;
bool CanTalk;
NPCchunk *next;
}*FirstNPCList,*LastNPCList,*FirstNPCMap,*LastNPCMap;

TTF_Font *font14,*font;
SDL_Event event;
bool DEFINE_MAP;
SDL_Color BLUE = {0,0,255};
SDL_Color RED = {255,0,0};
SDL_Color BYellow = {200,200,130};
SDL_Color BLACK = {0,0,0};


bool init(void);
void apply_Surface(int x,int y,SDL_Surface *source,SDL_Rect *clip);
SDL_Surface *load_image(string filename);
void AddChunk( FreeChunk *modelchunk,int x, int y,int list );
DayNightChunk* AddLight( int x, int y,int nr, int range );
InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id);
InterSpot *AddInterSpot( int x,int y );
NPCchunk *AddNPCchunk( NPCchunk *modelchunk );
ColideChunk *AddColider( int x,int y, int w,int h,int id );
void cleanup(void);
void AddNPC( string filelink );

void ctos(char carray[],string &str ){
str.erase(0, str.length());
for( int i = 0; carray[i] != 0; i++ )
str += carray[i];
}

class PixelManipulation{
private:
SDL_Surface *Palette;
public:
void init();
Uint32 getpix( SDL_Surface *surface,int x,int y );
void putpix( SDL_Surface *surface,int x,int y,Uint32 pixel );
SDL_Surface *flip_img( SDL_Surface *surface,int direction );
SDL_Surface *rotate_img( SDL_Surface *surface,int direction ,bool animated,int frames );
SDL_Surface *RenderCorner( SDL_Surface *surface );
SDL_Surface *RenderRect(int w,int h, int color);
SDL_Surface *RenderLightMap( int nr );
SDL_Surface *RenderGrid();
void RenderMouseLines(SDL_Surface *&l1 ,SDL_Surface *&l2 );
void cleanup();
}Pixels;
void PixelManipulation::init(){

Palette = load_image( "textures//environmental//DayNight_palette.png" );
}
Uint32 PixelManipulation::getpix( SDL_Surface *surface,int x,int y ){

    Uint32* PixVec = (Uint32*)surface->pixels;
    return PixVec[y * surface->w + x];
}
void PixelManipulation::putpix( SDL_Surface *surface,int x,int y,Uint32 pixel ){

    Uint32 *PixVec = (Uint32*)surface->pixels;
    PixVec[ (y*surface->w) + x ] = pixel;
}
SDL_Surface *PixelManipulation::flip_img( SDL_Surface *surface,int flags ){

    SDL_Surface *newsurf;
    int x,y;
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,0 );
    }
    else
    {
        newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );
    }

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    Uint32 pix;

    for( x = 0; x < surface->w ;x ++)
        for( y = 0; y < surface->h; y++ )
        {
            pix = getpix( surface, x ,y );

            if( ( flags & FLIP_VERT ) && ( flags & FLIP_HOR ) )
            {
                putpix( newsurf, surface->w - x -1 , surface->h - y -1, pix );
            }
            else if( flags & FLIP_VERT )
            {
                putpix( newsurf, surface->w - x -1, y, pix );
            }
            else if( flags & FLIP_HOR )
            {
                putpix( newsurf,x ,surface->h - y -1 , pix );
            }
        }

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }
    return newsurf;
}
SDL_Surface *PixelManipulation::rotate_img( SDL_Surface *surface,int direction ,bool animated,int frames ){

    SDL_Surface *newsurf;
    int x,y,z;


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    Uint32 pix;




if( !animated )
{
    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->h, surface->w,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,0 );
    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y < surface->h; y++ )
    {
        pix = getpix( surface, x,y );

        if( direction == ROTATE_LEFT )
        {
            putpix( newsurf, y,newsurf ->h - x -1,pix );
        }
        else if( direction == ROTATE_RIGHT )
        {
            putpix( newsurf, newsurf->w - y -1,x ,pix );
        }
        else
        putpix( newsurf, x,y,pix );
    }
}
else
{
    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->h*frames , surface->w/frames+1 ,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,0 );
    for( z = 0; z < frames ; z ++ )
    {
        for( x = 0; x < surface->w/frames; x ++ )
            for( y = 0; y < surface->h; y++ )
            {
                pix = getpix( surface, x + z*surface->w/frames,y );

                if( direction == ROTATE_LEFT )
                {
                    putpix( newsurf, y + z*surface->h , surface->w/frames - x ,pix );
                }
                else if( direction == ROTATE_RIGHT )
                {
                    putpix( newsurf,surface->h - y + z*surface->h ,x ,pix );
                }
            }
    }
    for( x = 0; x < newsurf->w;x++ )
    if( direction == ROTATE_LEFT )
    {
        putpix( newsurf, x, 0, getpix( Palette, 4,0 ) );
    }
    else if( direction == ROTATE_RIGHT )
    {
        putpix( newsurf, x, newsurf->h -1, getpix( Palette, 4,0 ) );
    }
}


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }

    return newsurf;

}
SDL_Surface *PixelManipulation::RenderCorner( SDL_Surface *surface ){

    SDL_Surface *newsurf;
    int x,y;
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,0 );
    }
    else
    {
        newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );
    }

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y < surface->h - x-1; y++ )
    {
        putpix( newsurf, x , y, getpix( surface, x ,y ) );
    }

    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y <= x; y++ )
    {
        putpix( newsurf,surface->w - y -1, x , getpix( surface, x ,y ) );
    }

    //for( x = 0; x < surface->w; x++ )
    //putpix( newsurf, y , surface->h - x , getpix( Palette, 7 ,0 ) );

    //for( x = 0; x < surface->w; x++ )
    //putpix( newsurf, x ,surface->h - x , getpix( Palette, 7 ,0 ) );

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }
    return newsurf;
}
SDL_Surface *PixelManipulation::RenderLightMap( int nr ){
int i,j,color,alpha;
SDL_Surface *newsurf;
DayNightChunk *p;
switch( nr + 1 )
{
    case 1://Zi
    color = 4;
    alpha = 20;
    break;
    case 2://Seara -> portocaliu
    color = 1;
    alpha = 20;
    break;
    case 3://Seara Bleo
    color = 2;
    alpha = 120;
    break;
    case 4://Noapte
    color = 3;
    alpha = 235;
    break;
    case 5://Dimineata Bleo
    color = 2;
    alpha = 120;
    break;
    case 6://Dimineata galben
    color = 0;
    alpha = 20;
    break;
}

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, BLOCK_WIDTH * maxy, BLOCK_HEIGHT * maxx,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

    for( i = 0; i < BLOCK_HEIGHT * maxx;i++ )
        for( j = 0; j < BLOCK_WIDTH * maxy ;j++ )
        {
            Pixels.putpix(newsurf ,j , i, Pixels.getpix( Palette, color,0 ) );
        }

        for( p = FirstDNChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
        {
            if( p ->nr == 1 )//Round light
            {
                if( nr == 3 )//Night
                {
                    for( i = p -> y - p -> range ; i < p -> y + p -> range; i++ )
                    for( j = p -> x - p ->range; j < p ->x + p ->range; j++ )
                    if( i > 0 && i < BLOCK_HEIGHT * maxx  && j > 0 && j < BLOCK_WIDTH * maxy )
                    {
                        if( sqrt( pow( i - p -> y, 2 ) + pow( j - p -> x, 2 ) ) < p -> range )
                        {
                            Pixels.putpix( newsurf, j, i, Pixels.getpix( Palette, 4, 0) );
                        }
                    }
                }
                else if( nr == 2 || nr == 4 )
                {
                    for( i = p -> y - p -> range ; i < p -> y + p -> range; i++ )
                    for( j = p -> x - p ->range; j < p ->x + p ->range; j++ )
                    if( i > 0 && i < 50 * maxx  && j > 0 && j < 50 * maxy )
                    {
                        if( sqrt( pow( i - p -> y, 2 ) + pow( j - p -> x, 2 ) ) < p -> range/2 )
                        {
                            Pixels.putpix( newsurf, j, i, Pixels.getpix( Palette, 4, 0) );
                        }
                    }
                }
            }
        }

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

SDL_SetAlpha( newsurf, SDL_SRCALPHA,alpha );

if( Palette->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, Palette->format->colorkey );
    }

return newsurf;
}
SDL_Surface *PixelManipulation::RenderRect(int w,int h,int color ){

int i,j,col;
SDL_Surface *newsurf;

switch( color ){
case 0:
col = 1;//blue
break;
case 1:
col = 5;//light blue
break;
case 2:
col = 1;//orange
break;
}

if( w > 0 && h > 0 )
{
if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

    for( i = 0; i < h;i++ )
        for( j = 0; j < w ;j++ )
        {
            Pixels.putpix(newsurf ,j , i, Pixels.getpix( Palette, col,0 ) );
        }

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

SDL_SetAlpha( newsurf, SDL_SRCALPHA,100 );

if( Palette->flags & SDL_SRCCOLORKEY )
{
    SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, Palette->format->colorkey );
}

}
else
newsurf = NULL;

return newsurf;
}
SDL_Surface *PixelManipulation::RenderGrid(){
int i,j,k,l;
SDL_Surface *newsurf;

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, maxy*BLOCK_WIDTH , maxx*BLOCK_HEIGHT,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

    for( i = 0; i < maxx*BLOCK_HEIGHT ;i++ )
        for( j = 0; j < maxy*BLOCK_WIDTH ;j++ )
        {
            Pixels.putpix(newsurf ,j , i, Pixels.getpix( Palette, 4,0 ) );
        }

    for( i = 0 ; i < maxx ; i++ )
        for( j = 0; j < maxy ; j++ )
        {
            for( l = 0; l < BLOCK_WIDTH; l++ )
            Pixels.putpix(newsurf , j*BLOCK_WIDTH + l, i*BLOCK_HEIGHT , Pixels.getpix( Palette, 6,0 ) );
            for( k = 0; k < BLOCK_HEIGHT; k++ )
            Pixels.putpix(newsurf , j*BLOCK_WIDTH ,i*BLOCK_HEIGHT + k, Pixels.getpix( Palette, 6,0 ) );

        }


if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, Palette->format->colorkey );

return newsurf;
}
void PixelManipulation::RenderMouseLines( SDL_Surface *&l1 ,SDL_Surface *&l2 ){
int i;
if( l1 == NULL ) l1 = SDL_CreateRGBSurface(SDL_SWSURFACE, maxy*BLOCK_WIDTH, 1,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );
if( l2 == NULL ) l2 = SDL_CreateRGBSurface(SDL_SWSURFACE, 1,maxx*BLOCK_HEIGHT,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

//l1
//for( i = 0; i < m_x - 50 - iMoveMap_x ; i++ )
//putpix( l1,i,0,getpix( Palette, 6,0) );
//for( i = m_x - 50 - iMoveMap_x; i < maxy*BLOCK_WIDTH ; i++ )
//putpix( l1,i,0,getpix( Palette, 4,0) );
for( i = 0; i < maxy*BLOCK_WIDTH ; i++ )
putpix( l1,i,0,getpix( Palette, 6,0) );
for( i = 0; i < maxx*BLOCK_HEIGHT; i++ )
putpix( l2,0,i,getpix( Palette, 6,0) );
//l2
/*
for( i = 0; i < m_y; i++ )
putpix( l2,0,i,getpix( Palette, 6,0) );
for( i = m_y; i < maxx*BLOCK_HEIGHT ; i++ )
putpix( l2,0,i,getpix( Palette, 4,0) );
*/
}

class Interface{
private:
/* UI Surfaces */
SDL_Surface *sInterface,*sOptions,*sSave;
SDL_Surface *sTxt1,*sTxt2,*sTxt3,*sTxt4,*sTxt5;


SDL_Surface *sDayNightCicle[6],*sInteractSkin,*sGrid,*sMousePoz,*sLine1,*sLine2;
SDL_Rect box_NewMap,box_Build,box_Inter,box_Light;
SDL_Rect box_NormalMap[100][100];
SDL_Rect box_Chunk[10],box_ROTATE[4],rInterfaceMenus[4],rInterfaceMenus_toggle[2] ;
SDL_Rect Button_WidthPlus,Button_WidthMinus,Button_HeightPlus,Button_HeightMinus;
SDL_Surface *sNormalNULL,*sWidth,*sHeight,*ModeLibHighlight,*sLight_CENTRE,*sLight_EDGE;
SDL_Surface *RedSquare,*YellowSquare,*GreenSquare,*sLibLock;
SDL_Surface *sColId;
int max_BUILD,max_INTER,max_DayNight,max_COLIDE;
CHUNK chunk_BUILD[200],chunk_INTER[200],NormalMap[100][100],ForeMap[100][100];
int nr1,nr2,nr3,nr4,cmode,clib,cInterTool,g_x,g_y,dif_w,dif_h;
bool DRAG_DRAW,SHOW_OPTIONS,SHOW_LOADMAP,SHOW_SAVE,DragLight_Round,DragColide,RescaleLight_Round,RescaleColide_width,RescaleColide_height;
bool DragColide_g1,DragColide_g2,DragInter,DragInter_g1,DragInter_g2,ConsoleInput;
bool ShowGrid,ShowMousePos,FULLSCREEN_On;
SDL_Rect rColId;
FreeChunk *FChunks[10];
InterSpot *cInterChunk;
//Interact Atributes window
SDL_Surface *sWindow_Inter_bg,*sWindow_InterButton_Door,*sWindow_InterButton_Custom;
SDL_Surface *sWindow_Inter_Text[5],*sWindow_Inter_Data[5],*sWindow_InterButton_TextAboveHead,
            *sWindow_InterButton_Loot;
int Window_Inter_x,Window_Inter_y,Window_Inter_Drag_x,Window_Inter_Drag_y,
    Window_Inter_MouseX,Window_Inter_MouseY;
string *StrChange;
bool Window_Inter_show,Window_Inter_Drag,ShowCChunk,Window_Inter_RenderVal;
SDL_Rect Window_InterRect_Move,Window_InterRect_Button[5],Window_InterRect_ButtonType ;

int MenuRow[2],NPCcframe;

Uint32 tAnime;
SDL_Rect rAnime;

/* Mouse-over/Selected chunk */
SDL_Surface *sCurrentChunk,*sSelectedChunk;
int CurrentChunk_x,CurrentChunk_y,SelectedChunk_x,SelectedChunk_y;

/* Console Input Variables */
int InputChange,*ValChange,CurrentSurface;
string InputString;
SDL_Surface *SurfaceChange;

/* NPC Window */
SDL_Surface *Window_NPCMAIN_bg,*Window_NPCMAIN_name,*Window_NPCMAIN_relationship,*Window_NPCMAIN_faction,*Window_NPCMAIN_behavior;
SDL_Surface *Window_NPCMAIN_talk,*Window_NPCMAIN_filenr;
int nrNPC;
NPCchunk *CMouseNPC,*CSelectedNPC,*AuxNPCchunk,*LastNPC_HL;
Uint32 tNPC;
SDL_Rect rNPC;
SDL_Surface *NPCSkins[100];
/*Chunks Surfaces */
SDL_Surface *BuildSurf[200][8];

public:
void init(void);
void AddNPC(int,int w,int h );
void showbg(void);
void RenderNPCInfo(void);
void handle_events( SDL_Event newevent );
void handle_logics();
void showNormaMap(void);
void showMouse(void);
void SaveMap(void);
void LoadMap(void);
void MapCleanup(void);
void cleanup();
}INTERFACE;
void Interface::AddNPC( int skinnr ,int w,int h ){
NPCchunk *newchunk = new NPCchunk,*inpc ;
stringstream ssaux;

if( skinnr == 0 )
newchunk->skin = NULL;
else//Add attributes
{
    if( !NPCSkins[skinnr] )
    {
        ssaux.str("");
        ssaux << "textures//NPC//" << skinnr << ".png";
        NPCSkins[skinnr] = load_image( ssaux.str().c_str() );
    }

    newchunk->skin = NPCSkins[skinnr];
}

    newchunk->w = w;
    newchunk->h = h;
    newchunk->skinnr = skinnr;


for( inpc = FirstNPCList; inpc -> next != LastNPCList; )
inpc = inpc->next;

newchunk->next = LastNPCList ;
inpc->next = newchunk;
}
void Interface::init(){
int i,j;
SHOW_OPTIONS = false;
DRAG_DRAW = false;
SHOW_SAVE = false;
SHOW_LOADMAP = false;
DragLight_Round = false;
DragColide = false;
RescaleLight_Round = false;
RescaleColide_width = false;
RescaleColide_height = false;
DragColide_g1 = false;
DragColide_g2 = false;
cInterItm = 0;
Window_Inter_show = false;
ConsoleInput = false;
ShowGrid = false;
ShowMousePos = true;
Window_Inter_RenderVal = false;

FULLSCREEN_On = 0;

tAnime = SDL_GetTicks();

sColId = NULL;

rColId.x = 550;
rColId.y = 100;
rColId.w = 100;
rColId.h = 30;

clib = 0;
max_BUILD = 7;
max_INTER = 14;
max_DayNight = 2;
max_COLIDE = 2;
cDayTime = 1;
cmode = clib = 0;
cInterItm = 0;
InterInfo[0] = InterInfo[1] = InterInfo[2] = 0;
nrNPC = 0;
NPCcframe = 0;
CMouseNPC = NULL;
SurfaceChange = NULL;

for( i = 0; i < 200; i ++ )
{
    BuildSurf[i][0] = BuildSurf[i][1] = BuildSurf[i][2] = BuildSurf[i][3] = NULL;
    BuildSurf[i][4] = BuildSurf[i][5] = BuildSurf[i][6] = BuildSurf[i][7] = NULL;
}

LibLock[0] = false;
LibLock[2] = LibLock[1] = true;
MenuRow[0] = MenuRow[1] = 0;

rAnime.w = BLOCK_WIDTH;
rAnime.h = BLOCK_HEIGHT;
rAnime.x = BLOCK_WIDTH;
rAnime.y = 0;
tNPC = SDL_GetTicks();
AuxNPCchunk = new NPCchunk;

FirstNPCList = new NPCchunk;
LastNPCList = new NPCchunk;
FirstNPCList->next = LastNPCList;
LastNPCList->next = NULL;

FirstNPCMap = new NPCchunk;
LastNPCMap = new NPCchunk;
FirstNPCMap->next = LastNPCMap;
LastNPCMap->next = NULL;

dif_w = dif_h = 0;

sCurrentChunk = NULL;
sSelectedChunk = NULL;

ShowCChunk = false;
FreeCTarget_Move = NULL;
DragInter = DragInter_g1 = DragInter_g2 = false;
LastNPC_HL = NULL;
Window_NPCMAIN_name = NULL;

sGrid = NULL;
sLine1 = sLine2 = NULL;

sWidth = sHeight = NULL;
sDayNightCicle[0] = NULL;
sDayNightCicle[1] = NULL;
sDayNightCicle[2] = NULL;
sDayNightCicle[3] = NULL;
sDayNightCicle[4] = NULL;
sDayNightCicle[5] = NULL;

rInterfaceMenus[0].x = 0;
rInterfaceMenus[1].x = 144;
rInterfaceMenus[2].x = 289;
rInterfaceMenus[3].x = 462;

rInterfaceMenus[0].y = 0;
rInterfaceMenus[1].y = 0;
rInterfaceMenus[2].y = 0;
rInterfaceMenus[3].y = 50;

rInterfaceMenus[0].w = 145;
rInterfaceMenus[1].w = 143;
rInterfaceMenus[2].w = 735;
rInterfaceMenus[3].w = 271;

rInterfaceMenus[0].h = 800;
rInterfaceMenus[1].h = 165;
rInterfaceMenus[2].h = 115;
rInterfaceMenus[3].h = 56;

Window_Inter_Drag = 0;
Window_InterRect_Move.x = 0;
Window_InterRect_Move.y = 0;
Window_InterRect_Move.w = 145;
Window_InterRect_Move.h = 30;

Window_InterRect_Button[0].x = 0;
Window_InterRect_Button[1].x = 0;
Window_InterRect_Button[2].x = 0;
Window_InterRect_Button[3].x = 0;
Window_InterRect_Button[4].x = 0;


Window_InterRect_Button[0].y = 62;
Window_InterRect_Button[1].y = 80;
Window_InterRect_Button[2].y = 97;
Window_InterRect_Button[3].y = 115;
Window_InterRect_Button[4].y = 130;

Window_InterRect_ButtonType.x = 0;
Window_InterRect_ButtonType.y = 45;
Window_InterRect_ButtonType.w = 145;
Window_InterRect_ButtonType.h = 15;

for( i = 0; i < 5; i++ )
{
    Window_InterRect_Button[i].w = 145;
    Window_InterRect_Button[i].h = 15;
    sWindow_Inter_Data[i] = sWindow_Inter_Text[i] = NULL;
}


maxx = 0;
maxy = 0;

nr1 = nr2 = nr3 = nr4 = 0;
sTxt1 = sTxt2 = sTxt3 = sTxt4 = sTxt5 = NULL;
sInterface = load_image( "Editor//Textures//Interface.png" );
sOptions = load_image( "Editor//Textures//window_door.png" );
sSave = load_image( "Editor//Textures//window_save.png" );
sNormalNULL = load_image( "Editor//Textures//NormalMapNULL.png" );
chunk_NULL.skin = NULL;
ModeLibHighlight = load_image( "Editor//Textures//ModeLibHighlight.png" );
cInterTool = 0;

sLibLock = load_image( "Editor//Textures//LibLock.png" );
SDL_SetAlpha( sLibLock,SDL_SRCALPHA,210);

sInteractSkin = load_image( "Editor//Textures//enviroment//colide_rectangle.png" );

RedSquare = load_image( "Editor//Textures//enviroment//RedSquare.png" );
YellowSquare = load_image( "Editor//Textures//enviroment//YellowSquare.png" );
GreenSquare = load_image( "Editor//Textures//enviroment//GreenSquare.png" );

sWindow_Inter_bg = load_image( "Editor//Textures//window_InteractionAtributes.png" );
sWindow_InterButton_Door = load_image( "Editor//Textures//window_IAButton_Door.png" );
sWindow_InterButton_Custom = load_image( "Editor//Textures//window_IAButton_Custom.png" );
sWindow_InterButton_TextAboveHead = load_image( "Editor//Textures//window_IAButton_TextAboveHead.png" );
sWindow_InterButton_Loot = load_image( "Editor//Textures//window_IAButton_Loot.png" );

Window_NPCMAIN_bg = load_image( "Editor//Textures//NPCwindow//window_NPCSETUP.png" );
Window_NPCMAIN_relationship = NULL;
Window_NPCMAIN_faction = NULL;
Window_NPCMAIN_behavior = NULL;
Window_NPCMAIN_talk = NULL;
Window_NPCMAIN_filenr = NULL;

{//Load CHUNKS

//chunk_C = chunk_NULL;

chunk_BUILD[0].nr = -1;
chunk_BUILD[0].skin = load_image( "Editor//Textures//NormalMapNULL.png" );

chunk_C = chunk_BUILD[0];

chunk_BUILD[1].nr = 0;
chunk_BUILD[1].skin = load_image( "textures//BUILDING//build//0.png" );

chunk_BUILD[2].nr = 1;
chunk_BUILD[2].skin = load_image( "textures//BUILDING//build//1.png" );

chunk_BUILD[3].nr = 2;
chunk_BUILD[3].skin = load_image( "textures//BUILDING//build//2.png" );

chunk_BUILD[4].nr = 3;
chunk_BUILD[4].skin = load_image( "textures//BUILDING//build//3.png" );

chunk_BUILD[5].nr = 4;
chunk_BUILD[5].skin = load_image( "textures//BUILDING//build//4.png" );

chunk_BUILD[6].nr = 5;
chunk_BUILD[6].skin = load_image( "textures//BUILDING//build//5.png" );

 //Old Interactive chunks ( foreground )

for( i = 0; i < max_INTER; i++ )
chunk_INTER[i].animated = false;

chunk_INTER[0].nr = -1;
chunk_INTER[0].skin = NULL;

chunk_INTER[1].nr = 205;
chunk_INTER[1].skin = load_image( "textures//BUILDING//decoration//205.png" );

chunk_INTER[2].nr = 200;
chunk_INTER[2].skin = load_image( "textures//BUILDING//decoration//200.png" );

chunk_INTER[3].nr = 201;
chunk_INTER[3].skin = load_image( "textures//BUILDING//decoration//201.png" );

chunk_INTER[4].nr = 202;
chunk_INTER[4].skin = load_image( "textures//BUILDING//decoration//202.png" );

chunk_INTER[5].nr = 203;
chunk_INTER[5].skin = load_image( "textures//BUILDING//decoration//203.png" );

chunk_INTER[6].nr = 204;
chunk_INTER[6].skin = load_image( "textures//BUILDING//decoration//204.png" );
chunk_INTER[6].animated = true;
chunk_INTER[6].frames = 4;

chunk_INTER[7].nr = 206;
chunk_INTER[7].skin = load_image( "textures//BUILDING//decoration//206.png" );

chunk_INTER[8].nr = 207;
chunk_INTER[8].skin = load_image( "textures//BUILDING//decoration//207.png" );


chunk_INTER[9].nr = 208;
chunk_INTER[9].skin = load_image( "textures//BUILDING//decoration//208.png" );

chunk_INTER[10].nr = 209;
chunk_INTER[10].skin = load_image( "textures//BUILDING//decoration//209.png" );
SDL_SetAlpha( chunk_INTER[10].skin, SDL_SRCALPHA, 190 );
chunk_INTER[10].animated = true;
chunk_INTER[10].frames = 4;

chunk_INTER[11].nr = 210;
chunk_INTER[11].skin = load_image( "textures//BUILDING//decoration//210.png" );
SDL_SetAlpha( chunk_INTER[11].skin, SDL_SRCALPHA, 190 );
chunk_INTER[11].animated = true;
chunk_INTER[11].frames = 4;

chunk_INTER[12].nr = 211;
chunk_INTER[12].skin = load_image( "textures//BUILDING//decoration//211.png" );

chunk_INTER[13].nr = 212;
chunk_INTER[13].skin = load_image( "textures//BUILDING//decoration//212.png" );

for( i = 0; i < max_BUILD; i++ )
chunk_BUILD[i].orient = 0;
for( i = 0; i < max_INTER; i++ )
{
    chunk_INTER[i].orient = 0;
    if(  chunk_INTER[i].animated )
    chunk_INTER[i].cframe = 0;
}


DNTool[0].nr = -1;
DNTool[0].skin = NULL;

DNTool[1].nr = 1;
DNTool[1].skin = load_image( "Editor//Textures//enviroment//lighteffect_round.png" );

COLTool[0].nr = -1;
COLTool[0].skin = NULL;

COLTool[1].nr = 1;
COLTool[1].skin = load_image( "Editor//Textures//enviroment//colide_rectangle.png" );

CDNTool = &DNTool[1];
for( i = 0 ; i < 4;i ++ )
{
    box_ROTATE[i].w = BLOCK_WIDTH;
    box_ROTATE[i].h = BLOCK_HEIGHT;
}
box_ROTATE[0].x = box_ROTATE[2].x = 0;
box_ROTATE[1].x = box_ROTATE[3].x = BLOCK_WIDTH;
box_ROTATE[0].y = box_ROTATE[1].y = 0;
box_ROTATE[2].y = box_ROTATE[3].y = BLOCK_HEIGHT;


for(i = 0; i < 6; i++)
{
    box_Chunk[i].x = 10;
    box_Chunk[i].y = 110 + ( BLOCK_HEIGHT + 10 )*i;
    box_Chunk[i].w = BLOCK_WIDTH;
    box_Chunk[i].h = BLOCK_HEIGHT;
}

for( i = 0;i < 100;i++)
    for( j = 0 ; j < 100;j++ )
    {
        ForeMap[i][j].skin = NULL;
        NormalMap[i][j].skin = NULL;
        box_NormalMap[i][j].h = BLOCK_HEIGHT;
        box_NormalMap[i][j].w = BLOCK_WIDTH;
        box_NormalMap[i][j].x = 280+box_NormalMap[i][j].w*j;
        box_NormalMap[i][j].y = 50+box_NormalMap[i][j].h*i;
    }

box_NewMap.x = 275;
box_NewMap.y = 0;
box_NewMap.w = 65;
box_NewMap.h = 50;

chunk_INTER[1].nr1 = chunk_INTER[1].nr2 = chunk_INTER[1].nr3 = 0;
}

for( i = 0; i < 100; i++ )
NPCSkins[i] = NULL;

AddNPC( 0,0,0);
AddNPC( 1,100,100 );
AddNPC( 2,100,100 );
AddNPC( 3,100,100 );
AddNPC( 4,100,100 );


Button_WidthMinus.y = Button_WidthPlus.y = Button_HeightMinus.y = Button_HeightPlus.y = 53;
Button_WidthMinus.h = Button_WidthPlus.h = Button_HeightMinus.h = Button_HeightPlus.h = 48;
Button_WidthMinus.w = Button_WidthPlus.w = Button_HeightMinus.w = Button_HeightPlus.w = 33;

Button_WidthMinus.x = 478;
Button_WidthPlus.x = 562;
Button_HeightPlus.x = 690;
Button_HeightMinus.x = 598;

//Load UI Rect
rInterfaceMenus_toggle[0].x = 342;
rInterfaceMenus_toggle[0].y = 47;
rInterfaceMenus_toggle[0].w = sSave->w;
rInterfaceMenus_toggle[0].h = sSave->h;

//Save UI Rect
rInterfaceMenus_toggle[1].x = 745;
rInterfaceMenus_toggle[1].y = 47;
rInterfaceMenus_toggle[1].w = rInterfaceMenus_toggle[0].w;
rInterfaceMenus_toggle[1].h = rInterfaceMenus_toggle[0].h;

}
void Interface::showbg(){
int i;
apply_Surface(0,0,sInterface,NULL);
if( clib == 0 )
    for( i = 6*MenuRow[0] ;i < 6*(MenuRow[0] + 1) && i < max_BUILD;i++ )
    {
        apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*(i - 6*MenuRow[0]),chunk_BUILD[i].skin,NULL);
    }
else if( clib == 1 )
{
    for( i = 6*MenuRow[1] ;i < 6*(MenuRow[1] + 1) && i < max_INTER;i++ )
    {
        if( chunk_INTER[i].animated )
        {
            rAnime.x = chunk_INTER[i].cframe*( chunk_INTER[i].skin->w/chunk_INTER[i].frames );
            rAnime.w = chunk_INTER[i].skin->w/chunk_INTER[i].frames;
            rAnime.h = chunk_INTER[i].skin->h;
            apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*(i - 6*MenuRow[1]) ,chunk_INTER[i].skin,&rAnime );
        }
        else
        apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*(i - 6*MenuRow[1]) ,chunk_INTER[i].skin,NULL);
    }
}
else if( clib == 2 )
    for( i = 0;i < max_DayNight;i++ )
    {
        apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*i,DNTool[i].skin,NULL);
    }
else if( clib == 3 )
{
    for( i = 0;i < max_COLIDE;i++ )
    {
        apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*i,COLTool[i].skin ,NULL);
    }

    SDL_FillRect(screen,&rColId,SDL_MapRGB(screen->format,0xFF,0xFF,0xFF));
    apply_Surface( rColId.x,rColId.y,sColId,NULL );
}
else if( clib == 4 )
{
    apply_Surface(10,110 + BLOCK_HEIGHT + 10 ,sInteractSkin ,NULL);
}
else if( clib == 5 )
{
    if( SDL_GetTicks() - tNPC > 500 )
    {
        NPCcframe ++;
        if( NPCcframe >= 4 )
        NPCcframe = 0;

        tNPC = SDL_GetTicks();
    }

    int nrnpcc = 0;
    for( NPCchunk *nc = FirstNPCList->next; nc != LastNPCList; nc = nc->next )
    {
        if( nc->skinnr == 4 )
          apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*( nrnpcc - 6*MenuRow[1] ) , nc->skin ,NULL);
        else
        if( nrnpcc >= 6*MenuRow[1] && nrnpcc <= 6*( MenuRow[1] + 1 )  )
        {
            rNPC.x = 0;
            rNPC.y = BLOCK_HEIGHT*NPCcframe;
            rNPC.w = BLOCK_WIDTH;
            rNPC.h = BLOCK_HEIGHT;

            apply_Surface(10,110 + ( BLOCK_HEIGHT + 10 )*( nrnpcc - 6*MenuRow[1] ) , nc->skin ,&rNPC );
        }
        nrnpcc ++;
    }
}

    apply_Surface( Button_WidthMinus.x + Button_WidthMinus.w + 5,Button_WidthMinus.y + 5, sWidth , NULL );
    apply_Surface( Button_HeightMinus.x + Button_HeightMinus.w+ 5,Button_HeightMinus.y + 5, sHeight , NULL );

    if( clib <= 2 )
    apply_Surface( 0,1 + clib*34,ModeLibHighlight ,NULL);

    if( cmode <= 4 )
    apply_Surface( 145,1 + cmode*33,ModeLibHighlight ,NULL);
    else
    apply_Surface( 290,50 + (cmode - 5)*33,ModeLibHighlight ,NULL);

    if( LibLock[0] )
    apply_Surface( 0,1 ,sLibLock ,NULL);
    if( LibLock[1] )
    apply_Surface( 0,34,sLibLock ,NULL);
    if( LibLock[2] )
    apply_Surface( 0,68,sLibLock ,NULL);

    if( SHOW_OPTIONS )
    {
        apply_Surface(145,165,sOptions,NULL);
        apply_Surface(223,205,sTxt1,NULL);
        apply_Surface(223,224,sTxt2,NULL);
        apply_Surface(223,245,sTxt3,NULL);
    }

    if( SHOW_SAVE )
    {
        apply_Surface(745,47,sSave,NULL);
        apply_Surface(815,97,sTxt4,NULL);
    }
    if( SHOW_LOADMAP )
    {
        apply_Surface(342,47,sSave,NULL);
        apply_Surface(408,91,sTxt5,NULL);
    }
    if( cmode == 6 )
    {
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w - 5 , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2     ,Window_NPCMAIN_bg,NULL );
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w     , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 5 ,Window_NPCMAIN_name,NULL );
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w + 55    , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 38 ,Window_NPCMAIN_relationship,NULL );
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w + 55    , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 52 ,Window_NPCMAIN_faction,NULL );
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w + 58    , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 80 ,Window_NPCMAIN_behavior,NULL );
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w + 36    , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 111 ,Window_NPCMAIN_talk,NULL );
        apply_Surface( SCREEN_WIDTH - Window_NPCMAIN_bg->w + 42    , ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 129 ,Window_NPCMAIN_filenr,NULL );
    }

}
void Interface::handle_events( SDL_Event newevent ){
int x,y,i,j;//,k;
SDL_Surface* auxsurf = NULL;
char auxchar[3];
bool OnUi = false;

if( newevent.type == SDL_MOUSEBUTTONDOWN )
{
    x = event.button.x;
    y = event.button.y;
    for( i = 0; i < 4; i++ )
    if( x > rInterfaceMenus[i].x && x < rInterfaceMenus[i].x + rInterfaceMenus[i].w && y > rInterfaceMenus[i].y && y < rInterfaceMenus[i].y + rInterfaceMenus[i].h )
    {
        OnUi = true;
        break;
    }
    for( i = 0; i < 2 && !OnUi; i++ )
    if( x > rInterfaceMenus_toggle[i].x && x < rInterfaceMenus_toggle[i].x + rInterfaceMenus_toggle[i].w &&
        y > rInterfaceMenus_toggle[i].y && y < rInterfaceMenus_toggle[i].y + rInterfaceMenus_toggle[i].h )
    {
        if( (i == 0 && SHOW_LOADMAP) || (i == 1 && SHOW_SAVE) )
        {
            OnUi = true;
            break;
        }

    }



    if( x > box_NewMap.x && (x < box_NewMap.x + box_NewMap.w) && y > box_NewMap.y && (y < box_NewMap.y + box_NewMap.h) )
    {
        DEFINE_MAP = true;
        iMoveMap_x = iMoveMap_y = 0;
        MapCleanup();
    }
    else if( x > 0 && x < 145 && y > 0 &&y < 35 && !LibLock[0] )//Build Lib button
    {
        //show_categ = 1;
        clib = 0;
    }
    else if( x > 0 && x < 145 && y > 35 &&y < 70 && !LibLock[1])//Foreground Lib button
    {
        //show_categ = 2;
        clib = 1;
    }
    else if( x > 0 && x < 145 && y > 70 &&y < 100 && !LibLock[2])//Light Lib button
    {
        //show_categ = 3;
        clib = 2;
    }
    else if( x > 342 && x < 405 && y > 0 && y < 48 )//load Map tab
    {
        SHOW_LOADMAP = !SHOW_LOADMAP;
        if( SHOW_LOADMAP )
        {
            if( sTxt5 != NULL )
            {
                SDL_FreeSurface( sTxt5 );
                sTxt5 = NULL;
            }
            itoa(nr4 ,auxchar,10);
            sTxt5 = TTF_RenderText_Solid(font14,auxchar,BLUE);
        }
    }
    else if( x > 387 && x < 402 && y > 93 &&y < 108  &&SHOW_LOADMAP)
    {
        nr4--;
        if( sTxt5 != NULL )
            {
                SDL_FreeSurface( sTxt5 );
                sTxt5 = NULL;
            }
            ltoa(nr4,auxchar,10);
            sTxt5 = TTF_RenderText_Solid(font14,auxchar,BLUE);
    }
    else if( x > 427 && x < 442 && y > 93 &&y < 108  &&SHOW_LOADMAP)
    {
        nr4++;
        if( sTxt5 != NULL )
            {
                SDL_FreeSurface( sTxt5 );
                sTxt5 = NULL;
            }
            ltoa(nr4,auxchar,10);
            sTxt5 = TTF_RenderText_Solid(font14,auxchar,BLUE);
    }
    else if( x > 382 && x < 450 && y > 165 &&y < 195 &&SHOW_LOADMAP )
    {
        //Load Map
        Interface::LoadMap();
        SHOW_LOADMAP = false;
    }
    else if( x > 745 && x < 885 && y > 0 &&y < 47 )//Save Map tab
    {
        SHOW_SAVE = !SHOW_SAVE;
        if( SHOW_SAVE )
        {
            itoa( nr4,auxchar,10 );
            sTxt4 = TTF_RenderText_Solid(font14,auxchar,BLUE);
        }
        else
        {
            SDL_FreeSurface( sTxt4 );
            sTxt4 = NULL;
        }

    }
    else if( x > 790 && x < 805 && y > 90 &&y < 105 && SHOW_SAVE )
    {
        nr4--;
        itoa( nr4,auxchar,10 );
        if( sTxt4 != NULL )
        SDL_FreeSurface( sTxt4 );
        sTxt4 = TTF_RenderText_Solid(font14,auxchar,BLUE);
    }
    else if( x > 830 && x < 845 && y > 90 &&y < 105 && SHOW_SAVE )
    {
        nr4++;
        itoa( nr4,auxchar,10 );
        if( sTxt4 != NULL )
        SDL_FreeSurface( sTxt4 );
        sTxt4 = TTF_RenderText_Solid(font14,auxchar,BLUE);
    }
    else if( x > 785 && x < 852 && y > 163 && y < 193 && SHOW_SAVE )
    {
        Interface::SaveMap();
    }
    else if( x > 145 && x < 288 && y > 0 &&y < 34 )//Locked BG Mode 0
    {
        cmode = 0;
        if( clib != 0 )
        clib = 0;
        LibLock[0] = false;
        LibLock[1] = true;
        LibLock[2] = true;
    }
    else if( x > 145 && x < 253 && y > 34 &&y < 67 )//Free BG Mode 1
    {
        cmode = 1;
        clib = 1;
        LibLock[0] = false;
        LibLock[1] = false;
        LibLock[2] = true;
        C_FreeChunk->skin = chunk_INTER[0].skin ;
        C_FreeChunk->nr = chunk_INTER[0].nr;
        C_FreeChunk->orient = chunk_INTER[0].orient;
        C_FreeChunk->animated = chunk_INTER[0].animated;
        C_FreeChunk->frames = chunk_INTER[0].frames;
        C_FreeChunk->cframe = chunk_INTER[0].cframe;
    }
    else if( x > 145 && x < 253 && y > 67 &&y < 100 )//Free FG Mode 2
    {
        cmode = 2;
        clib = 1;
        LibLock[0] = false;
        LibLock[1] = false;
        LibLock[2] = true;
        C_FreeChunk->skin = chunk_INTER[0].skin ;
        C_FreeChunk->nr = chunk_INTER[0].nr;
        C_FreeChunk->orient = chunk_INTER[0].orient;
        C_FreeChunk->animated = chunk_INTER[0].animated;
        C_FreeChunk->frames = chunk_INTER[0].frames;
        C_FreeChunk->cframe = chunk_INTER[0].cframe;
    }
    else if( x > 253 && x < 288 && y > 34 &&y < 100 )//Free Dynamic Mode 7
    {
        cmode = 7;
        clib = 1;
        LibLock[0] = false;
        LibLock[1] = false;
        LibLock[2] = true;
        C_FreeChunk->skin = chunk_INTER[0].skin ;
        C_FreeChunk->nr = chunk_INTER[0].nr;
        C_FreeChunk->orient = chunk_INTER[0].orient;
        C_FreeChunk->animated = chunk_INTER[0].animated;
        C_FreeChunk->frames = chunk_INTER[0].frames;
        C_FreeChunk->cframe = chunk_INTER[0].cframe;
    }
    else if( x > 145 && x < 288 && y > 100 &&y < 133 )//Free Light Mode 3
    {
        cmode = 3;
        if( clib != 2 )
        clib = 2;
        LibLock[0] = true;
        LibLock[1] = true;
        LibLock[2] = false;
        CDNTool = &DNTool[0];
    }
    else if( x > 145 && x < 288 && y > 133 &&y < 166 )//Free Colide Mode 4
    {
        cmode = 4;
        CCOLTool = &COLTool[0];
        clib = 3;
        LibLock[0] = true;
        LibLock[1] = true;
        LibLock[2] = true;
    }
    else if( x > 290 && x < 432 && y > 50 && y < 81 )//Interactive Spots Mode 5
    {
        cmode = 5;
        clib = 4;
        LibLock[0] = true;
        LibLock[1] = true;
        LibLock[2] = true;

        cInterChunk = NULL;
    }
    else if( x > 290 && x < 432 && y > 81 && y < 112 )//NPC Mode 6
    {
        cmode = 6;
        clib = 5;
        LibLock[0] = true;
        LibLock[1] = true;
        LibLock[2] = true;
        CMouseNPC = FirstNPCList->next;
        RenderNPCInfo();
    }
    else if( x > 405 && x < 462 && y > 35 &&y < 50 )//Day time buttons
    {
        cDayTime = 1;
    }
    else if( x > 462 && x < 520 && y > 35 &&y < 50 )//Day time buttons
    {
        cDayTime = 2;
    }
    else if( x > 520 && x < 580 && y > 35 &&y < 50 )//Day time buttons
    {
        cDayTime = 3;
    }
    else if( x > 580 && x < 637 && y > 35 &&y < 50 )//Day time buttons
    {
        cDayTime = 4;
    }
    else if( x > 637 && x < 694 && y > 35 &&y < 50 )//Day time buttons
    {
        cDayTime = 5;
    }
    else if( x > 694 && x < 745 && y > 35 &&y < 50 )//Day time buttons
    {
        cDayTime = 6;
    }
    else if( x > 407 && x < 445 && y > 0 && y < 35 )//Grid Button
    {
        ShowGrid = !ShowGrid;
    }
    else if( x > 445 && x < 494 && y > 0 && y < 35 )//Mouse Position Button
    {
        ShowMousePos = !ShowMousePos;
    }
    else if( x > 199 && x < 345 && y > 203 &&y < 215 && SHOW_OPTIONS)
    {
        InterInfo[0]--;
    }
    else if( x > 199 && x < 245 && y > 203 &&y < 215 && SHOW_OPTIONS)
    {
        InterInfo[0]++;
    }
    else if( x > 199 && x < 345 && y > 223 &&y < 233 && SHOW_OPTIONS)
    {
        InterInfo[1]--;
    }
    else if( x > 199 && x < 245 && y > 223 &&y < 233 && SHOW_OPTIONS)
    {
       InterInfo[1]++;
    }
    else if( x > 199 && x < 345 && y > 244 &&y < 255 && SHOW_OPTIONS)
    {
        InterInfo[2]--;
    }
    else if( x > 199 && x < 245 && y > 244 &&y < 255 && SHOW_OPTIONS)
    {
        InterInfo[2]++;
    }
    else if( x > 885 && x < 1024 && y > 0 && y < 50  )
    {
        FULLSCREEN_On = !FULLSCREEN_On;
        if( FULLSCREEN_On )  screen = SDL_SetVideoMode(0,0,32, SDL_SWSURFACE | SDL_FULLSCREEN );
        if( !FULLSCREEN_On ) screen = SDL_SetVideoMode(0,0,32, SDL_SWSURFACE );
    }
    else if( x > Button_WidthMinus.x && x < Button_WidthMinus.x + Button_WidthMinus.w && y > Button_WidthMinus.y && y <  Button_WidthMinus.y + Button_WidthMinus.h )//Widht minus Button
    {
        LightMap_Changed[0] = true;
        LightMap_Changed[1] = true;
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
        LightMap_Changed[5] = true;
        maxy--;
        if( sWidth != NULL )
        {
            SDL_FreeSurface( sWidth );
            sWidth = NULL;
        }
        itoa(maxy,auxchar,10);
        sWidth = TTF_RenderText_Solid( font,auxchar,BYellow ) ;

        for( i = 0; i < maxx;i++ )
        {
            NormalMap[i][maxy] = chunk_NULL;
            ForeMap[i][ maxy ] = chunk_NULL;
        }

    }
    else if( x > Button_WidthPlus.x && x < Button_WidthPlus.x + Button_WidthPlus.w && y > Button_WidthPlus.y && y <  Button_WidthPlus.y + Button_WidthPlus.h )//Widht minus Button
    {
        LightMap_Changed[0] = true;
        LightMap_Changed[1] = true;
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
        LightMap_Changed[5] = true;
        maxy++;
        if( sWidth != NULL )
        {
            SDL_FreeSurface( sWidth );
            sWidth = NULL;
        }
        itoa(maxy,auxchar,10);
        sWidth = TTF_RenderText_Solid( font,auxchar,BYellow ) ;
        for( i = 0; i < maxx;i++ )
        {
            NormalMap[i][ maxy -1 ] = chunk_NULL;
            NormalMap[i][ maxy -1 ].skin = sNormalNULL;
            ForeMap[i][ maxy -1 ] = chunk_NULL;
        }

    }
    else if( x > Button_HeightMinus.x && x < Button_HeightMinus.x + Button_HeightMinus.w && y > Button_HeightMinus.y && y <  Button_HeightMinus.y + Button_HeightMinus.h )//Widht minus Button
    {
        LightMap_Changed[0] = true;
        LightMap_Changed[1] = true;
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
        LightMap_Changed[5] = true;
        maxx--;
        if( sHeight != NULL )
        {
            SDL_FreeSurface( sHeight );
            sHeight = NULL;
        }
        itoa(maxx,auxchar,10);
        sHeight = TTF_RenderText_Solid( font,auxchar,BYellow ) ;
        for( i = 0; i < maxy;i++ )
        {
            NormalMap[ maxx ][ i ] = chunk_NULL;
            ForeMap[ maxx ][ i ] = chunk_NULL;
        }
    }
    else if( x > Button_HeightPlus.x && x < Button_HeightPlus.x + Button_HeightPlus.w && y > Button_HeightPlus.y && y <  Button_HeightPlus.y + Button_HeightPlus.h )//Widht minus Button
    {
        LightMap_Changed[0] = true;
        LightMap_Changed[1] = true;
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
        LightMap_Changed[5] = true;
        maxx++;
        if( sHeight != NULL )
        {
            SDL_FreeSurface( sHeight );
            sHeight = NULL;
        }
        itoa(maxx,auxchar,10);
        sHeight = TTF_RenderText_Solid( font,auxchar,BYellow ) ;

        for( i = 0; i < maxy;i++ )
        {
            NormalMap[ maxx -1 ][ i ] = chunk_NULL;
            NormalMap[ maxx -1 ][ i ].skin = sNormalNULL;
            ForeMap[ maxx -1 ][ i ] = chunk_NULL;
        }
    }
    else if( x > 0 && x < 72 && y > 768 && y < 800 ) // - minus Menu Row
    {
        if( clib == 0 && MenuRow[0] > 0 )
        MenuRow[0] --;
        else if( clib == 1 && MenuRow[1] > 0 )
        MenuRow[1] --;
    }
    else if( x > 74 && x < 145 && y > 768 && y < 800 ) // - minus Menu Row
    {
        if( clib == 0 && MenuRow[0] < max_BUILD/6 )
        MenuRow[0] ++;
        else if( clib == 1 && MenuRow[1] < max_INTER/6 )
        MenuRow[1] ++;
    }
    else if(  DEFINE_MAP == true && !bCtrlDown )
    {

        DEFINE_MAP = false;
        for( i = 0 ; i < 100 ; i++ )
        if( NormalMap[0][i].skin == NULL )
        {
            maxy = i;
            break;
        }
        for( i = 0 ; i < 100 ; i++ )
        if( NormalMap[i][0].skin == NULL )
        {
            maxx = i;
            break;
        }
        LightMap_Changed[0] = true;
        LightMap_Changed[1] = true;
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
        LightMap_Changed[5] = true;
        if( sGrid != NULL )
        SDL_FreeSurface( sGrid );
        Pixels.RenderGrid();
        Pixels.RenderMouseLines( sLine1,sLine2 );
    }
    else if( C_FreeChunk ->nr == chunk_INTER[0].nr && !bCtrlDown ) //Check click on foreground item
    {
        FreeChunk *p;
        if( cmode == 1 )
        {
            for( p = FirstFreeChunk_back -> next ;p != LastFreeChunk_back ; p = p->next )
            {
                if(( ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) && (!p->animated) ) ||
                    ( p->animated && ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w/p->frames ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) ) )
                {
                    FreeCTarget_Move = p;
                    FCMove_x = x + iMoveMap_x + 280 ;
                    FCMove_y = y + iMoveMap_y + 50 ;
                    dif_w = p->x + 280 + iMoveMap_x - x;
                    dif_h = p->y + 50 + iMoveMap_y - y;
                    FCmoveing = true;
                    if( sSelectedChunk != NULL )
                    SDL_FreeSurface( sSelectedChunk );
                    if( p->animated )
                    sSelectedChunk = Pixels.RenderRect( p->skin->w/p->frames,p->skin->h,2 );
                    else
                    sSelectedChunk = Pixels.RenderRect( p->skin->w,p->skin->h,2 );
                    break;
                }
                else if( p->next == LastFreeChunk_back )
                {
                    FreeCTarget_Move = NULL;
                }
            }
        }
        else if( cmode == 7  )
        {
            for( p = FirstFreeChunk_dynam -> next ; p != LastFreeChunk_dynam ; p = p->next )
            {
                if(( ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) && (!p->animated) ) ||
                    ( p->animated && ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w/p->frames ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) ) )
                {
                    FreeCTarget_Move = p;
                    FCMove_x = x + iMoveMap_x + 280 ;
                    FCMove_y = y + iMoveMap_y + 50 ;
                    dif_w = p->x + 280 + iMoveMap_x - x;
                    dif_h = p->y + 50 + iMoveMap_y - y;
                    FCmoveing = true;
                    if( sSelectedChunk != NULL )
                    SDL_FreeSurface( sSelectedChunk );
                    if( p->animated )
                    sSelectedChunk = Pixels.RenderRect( p->skin->w/p->frames,p->skin->h,2 );
                    else
                    sSelectedChunk = Pixels.RenderRect( p->skin->w,p->skin->h,2 );
                    break;
                }
                else if( p->next == LastFreeChunk_dynam )
                {
                    FreeCTarget_Move = NULL;
                }
            }
        }
        else if( cmode == 2  )
        {
            for( p = FirstFreeChunk_fore -> next ; p != LastFreeChunk_fore ; p = p->next )
            {
                if(( ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) && (!p->animated) ) ||
                    ( p->animated && ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w/p->frames ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) ) )
                {
                    FreeCTarget_Move = p;
                    FCMove_x = x + iMoveMap_x + 280 ;
                    FCMove_y = y + iMoveMap_y + 50 ;
                    dif_w = p->x + 280 + iMoveMap_x - x;
                    dif_h = p->y + 50 + iMoveMap_y - y;
                    FCmoveing = true;
                    if( sSelectedChunk != NULL )
                    SDL_FreeSurface( sSelectedChunk );
                    if( p->animated )
                    sSelectedChunk = Pixels.RenderRect( p->skin->w/p->frames,p->skin->h,2 );
                    else
                    sSelectedChunk = Pixels.RenderRect( p->skin->w,p->skin->h,2 );
                    break;
                }
                else if( p->next == LastFreeChunk_fore )
                {
                    FreeCTarget_Move = NULL;
                }
            }
        }
    }

    if( SHOW_OPTIONS )
    {
        if( sTxt1 != NULL )
        {
            SDL_FreeSurface( sTxt1 );
            sTxt1 = NULL;
        }
        if( sTxt2 != NULL )
        {
            SDL_FreeSurface( sTxt2 );
            sTxt2 = NULL;
        }
        if( sTxt3 != NULL )
        {
            SDL_FreeSurface( sTxt3 );
            sTxt3 = NULL;
        }

        itoa(chunk_INTER[1].nr1,auxchar,10);
        sTxt1 = TTF_RenderText_Solid(font14,auxchar,BLUE);
        itoa(chunk_INTER[1].nr2,auxchar,10);
        sTxt2 = TTF_RenderText_Solid(font14,auxchar,BLUE);
        itoa(chunk_INTER[1].nr3,auxchar,10);
        sTxt3 = TTF_RenderText_Solid(font14,auxchar,BLUE);
    }

    if( DEFINE_MAP == false )//Select item from Left
    {
        if( event.button.button == SDL_BUTTON_LEFT )//Rotate
        {
            if( cmode == 0 )
            DRAG_DRAW = true;

        if( clib == 0 )
        {
            for( i = 6*MenuRow[0] ;i < 6*(MenuRow[0] + 1 ) && i < max_BUILD; i++ )
            if( x > box_Chunk[i - 6*MenuRow[0] ].x && (x < box_Chunk[i - 6*MenuRow[0]].x + box_Chunk[i - 6*MenuRow[0]].w ) && y > box_Chunk[i - 6*MenuRow[0]].y && (y < box_Chunk[ i-6*MenuRow[0] ].y + box_Chunk[i - 6*MenuRow[0]].h) )
            {
                cInterItm = i;
                if( chunk_C.nr != chunk_BUILD[i].nr )
                chunk_C = chunk_BUILD[i];
                else if( chunk_C.nr != -1 )
                {
                    chunk_C.orient++;

                    if( chunk_C.orient > 3  )
                    {
                        chunk_C.orient = 0;
                    }

                    chunk_C.skin = NULL;

                    switch( chunk_C.orient ) //chose / render skin
                    {
                        case 0:
                        chunk_C.skin = chunk_BUILD[i].skin;
                        break;
                        case 1:
                        chunk_C.skin = Pixels.rotate_img( chunk_BUILD[i].skin,ROTATE_RIGHT,chunk_C.animated,chunk_C.frames ) ;
                        break;
                        case 2:
                        chunk_C.skin = Pixels.flip_img( chunk_BUILD[i].skin, FLIP_HOR ) ;
                        break;
                        case 3:
                        chunk_C.skin = Pixels.rotate_img( chunk_BUILD[i].skin, ROTATE_LEFT,chunk_C.animated,chunk_C.frames ) ;
                        break;
                    }
                }

            }
        }
        else if( clib == 1 )
        {
            for( i = 6*MenuRow[1];i < 6*(MenuRow[1] + 1 ) && i < max_INTER; i++ )
            if( x > box_Chunk[ i-6*MenuRow[1] ].x && (x < box_Chunk[ i-6*MenuRow[1] ].x + box_Chunk[ i-6*MenuRow[1] ].w) && y > box_Chunk[ i-6*MenuRow[1] ].y && (y < box_Chunk[ i-6*MenuRow[1] ].y + box_Chunk[ i-6*MenuRow[1] ].h) )
            {
                cInterItm = i;
                if( C_FreeChunk -> nr != chunk_INTER[i].nr && newevent.button.button == SDL_BUTTON_LEFT)
                {
                    C_FreeChunk->skin = chunk_INTER[i].skin ;
                    C_FreeChunk->nr = chunk_INTER[i].nr;
                    C_FreeChunk->orient = chunk_INTER[i].orient;
                    C_FreeChunk->animated = chunk_INTER[i].animated;
                    C_FreeChunk->frames = chunk_INTER[i].frames;
                    C_FreeChunk->cframe = chunk_INTER[i].cframe;
                    bCSkinOrigin = true;
                }
                else if( newevent.button.button == SDL_BUTTON_LEFT && chunk_INTER[i].skin != NULL )
                {
                    C_FreeChunk -> orient++;

                    if( C_FreeChunk -> orient > 3  )
                    {
                        C_FreeChunk -> orient = 0;
                        bCSkinOrigin = true;
                    }

                    if( C_FreeChunk -> skin != NULL && bCSkinOrigin == false )
                    {
                        SDL_FreeSurface( C_FreeChunk -> skin );
                        C_FreeChunk -> skin = NULL;
                    }

                    switch( C_FreeChunk -> orient ) //chose / render skin
                    {
                        case 0:
                        C_FreeChunk -> skin = chunk_INTER[i].skin;
                        bCSkinOrigin = true;
                        break;
                        case 1:
                        C_FreeChunk -> skin = Pixels.rotate_img( chunk_INTER[i].skin ,ROTATE_RIGHT,C_FreeChunk->animated, C_FreeChunk->frames ) ;
                        bCSkinOrigin = false;
                        break;
                        case 2:
                        C_FreeChunk -> skin = Pixels.flip_img( chunk_INTER[i].skin , FLIP_HOR ) ;
                        bCSkinOrigin = false;
                        break;
                        case 3:
                        C_FreeChunk -> skin = Pixels.rotate_img( chunk_INTER[i].skin, ROTATE_LEFT,C_FreeChunk->animated, C_FreeChunk->frames ) ;
                        bCSkinOrigin = false;
                        break;
                    }

                }
                else if( newevent.button.button == SDL_BUTTON_RIGHT ) // Neadaugat inca
                {
                    if( chunk_INTER[i].nr == chunk_INTER[1].nr )
                    {
                        if( SHOW_OPTIONS == false )
                        {
                            SHOW_OPTIONS = true;

                            if( sTxt1 != NULL )
                            {
                                SDL_FreeSurface( sTxt1 );
                                sTxt1 = NULL;
                            }
                            if( sTxt2 != NULL )
                            {
                                SDL_FreeSurface( sTxt2 );
                                sTxt2 = NULL;
                            }
                            if( sTxt3 != NULL )
                            {
                                SDL_FreeSurface( sTxt3 );
                                sTxt3 = NULL;
                            }
                            itoa(chunk_INTER[i].nr1,auxchar,10);
                            sTxt1 = TTF_RenderText_Solid(font14,auxchar,BLUE);
                            itoa(chunk_INTER[i].nr2,auxchar,10);
                            sTxt2 = TTF_RenderText_Solid(font14,auxchar,BLUE);
                            itoa(chunk_INTER[i].nr3,auxchar,10);
                            sTxt3 = TTF_RenderText_Solid(font14,auxchar,BLUE);
                        }
                        else
                        SHOW_OPTIONS = false;
                    }
                }
            }
        }
        else if( clib == 2 )
        {
            for( i = 0;i < max_DayNight; i++ )
            if( x > box_Chunk[i].x && (x < box_Chunk[i].x + box_Chunk[i].w) && y > box_Chunk[i].y && (y < box_Chunk[i].y + box_Chunk[i].h) )
            {
                CDNTool = &DNTool[i];
            }
        }
        else if( cmode == 4 )
        {
            for( i = 0;i < max_COLIDE; i++ )
            if( x > box_Chunk[i].x && (x < box_Chunk[i].x + box_Chunk[i].w) && y > box_Chunk[i].y && (y < box_Chunk[i].y + box_Chunk[i].h) )
            {
                CCOLTool = &COLTool[i];
            }
        }
        else if( clib == 4 )
        {
            if( x > 10 && x < 10 + BLOCK_WIDTH && y > 110 && y < 110 + BLOCK_HEIGHT )//10 , 110
            {
                cInterTool = 0;
            }
            else if( x > 10 && x < 10 + BLOCK_WIDTH && y > 110 + 10 + BLOCK_HEIGHT && y < 110 + 10 + BLOCK_HEIGHT*2 )
            {
                cInterTool = 1;
            }
        }
        else if( clib == 5 )
        {

            {
                int nrnpcc = 0;
                for( NPCchunk *nc = FirstNPCList->next; nc != LastNPCList; nc = nc->next )
                {
                if( nrnpcc >= 6*MenuRow[1] && nrnpcc <= 6*( MenuRow[1] + 1 ) &&
                   (  x > 10 && x < 10 + BLOCK_WIDTH  ) && ( y > 110 + ( BLOCK_HEIGHT + 10 )*( nrnpcc - 6*MenuRow[1] ) &&
                                                             y < 110 + ( BLOCK_HEIGHT + 10 )*( nrnpcc - 6*MenuRow[1] + 1 ) ) )
                {
                    CMouseNPC = nc;
                    break;
                }
                nrnpcc ++;
                }
            }

        }

        }
        else if( event.button.button == SDL_BUTTON_RIGHT )//Corner Rotate
        {

            if( cmode == 0 )
            DRAG_DRAW = true;

        if( clib == 0 )
        {
            for( i = 6*MenuRow[0] ;i < 6*(MenuRow[0] + 1 ) && i < max_BUILD; i++ )
            if( x > box_Chunk[i - 6*MenuRow[0] ].x && (x < box_Chunk[i - 6*MenuRow[0]].x + box_Chunk[i - 6*MenuRow[0]].w ) && y > box_Chunk[i - 6*MenuRow[0]].y && (y < box_Chunk[ i-6*MenuRow[0] ].y + box_Chunk[i - 6*MenuRow[0]].h) )
            {
                cInterItm = i;
                if( chunk_C.nr != chunk_BUILD[i].nr )
                chunk_C = chunk_BUILD[i];
                else if( chunk_C.nr != -1 )
                {
                    chunk_C.orient++;

                    if( chunk_C.orient < 4 || chunk_C.orient > 7 ) chunk_C.orient = 4;

                    chunk_C.skin = NULL;

                    switch( chunk_C.orient ) //chose / render skin
                    {
                        case 4:
                        chunk_C.skin = Pixels.RenderCorner( chunk_BUILD[i].skin );
                        break;
                        case 5:
                        chunk_C.skin = Pixels.rotate_img( Pixels.RenderCorner( chunk_BUILD[i].skin ),
                                                         ROTATE_RIGHT,chunk_C.animated,chunk_C.frames ) ;
                        break;
                        case 6:
                        chunk_C.skin = Pixels.rotate_img( Pixels.rotate_img( Pixels.RenderCorner( chunk_BUILD[i].skin ),
                                                          ROTATE_LEFT,chunk_C.animated,chunk_C.frames ),
                                                          ROTATE_LEFT,chunk_C.animated,chunk_C.frames )  ;
                        break;
                        case 7:
                        chunk_C.skin = Pixels.rotate_img( Pixels.RenderCorner( chunk_BUILD[i].skin ),
                                                          ROTATE_LEFT,chunk_C.animated,chunk_C.frames ) ;
                        break;
                    }
                }

            }
        }
        else if( clib == 1 )
        {
            for( i = 6*MenuRow[1];i < 6*(MenuRow[1] + 1 ) && i < max_INTER; i++ )
            if( x > box_Chunk[ i-6*MenuRow[1] ].x && (x < box_Chunk[ i-6*MenuRow[1] ].x + box_Chunk[ i-6*MenuRow[1] ].w) && y > box_Chunk[ i-6*MenuRow[1] ].y && (y < box_Chunk[ i-6*MenuRow[1] ].y + box_Chunk[ i-6*MenuRow[1] ].h) )
            {
                cInterItm = i;
                if( C_FreeChunk -> nr != chunk_INTER[i].nr && newevent.button.button == SDL_BUTTON_LEFT)
                {
                    C_FreeChunk->skin = chunk_INTER[i].skin ;
                    C_FreeChunk->nr = chunk_INTER[i].nr;
                    C_FreeChunk->orient = chunk_INTER[i].orient;
                    C_FreeChunk->animated = chunk_INTER[i].animated;
                    C_FreeChunk->frames = chunk_INTER[i].frames;
                    C_FreeChunk->cframe = chunk_INTER[i].cframe;
                    bCSkinOrigin = true;
                }
                else if( newevent.button.button == SDL_BUTTON_LEFT && chunk_INTER[i].skin != NULL )
                {
                    C_FreeChunk -> orient++;

                    if( C_FreeChunk -> orient > 3  )
                    {
                        C_FreeChunk -> orient = 0;
                        bCSkinOrigin = true;
                    }

                    if( C_FreeChunk -> skin != NULL && bCSkinOrigin == false )
                    {
                        SDL_FreeSurface( C_FreeChunk -> skin );
                        C_FreeChunk -> skin = NULL;
                    }

                    switch( C_FreeChunk -> orient ) //chose / render skin
                    {
                        case 0:
                        C_FreeChunk -> skin = chunk_INTER[i].skin;
                        bCSkinOrigin = true;
                        break;
                        case 1:
                        C_FreeChunk -> skin = Pixels.rotate_img( chunk_INTER[i].skin ,ROTATE_RIGHT,C_FreeChunk->animated, C_FreeChunk->frames ) ;
                        bCSkinOrigin = false;
                        break;
                        case 2:
                        C_FreeChunk -> skin = Pixels.flip_img( chunk_INTER[i].skin , FLIP_HOR ) ;
                        bCSkinOrigin = false;
                        break;
                        case 3:
                        C_FreeChunk -> skin = Pixels.rotate_img( chunk_INTER[i].skin, ROTATE_LEFT,C_FreeChunk->animated, C_FreeChunk->frames ) ;
                        bCSkinOrigin = false;
                        break;
                    }

                }
                else if( newevent.button.button == SDL_BUTTON_RIGHT ) // Neadaugat inca
                {
                    if( chunk_INTER[i].nr == chunk_INTER[1].nr )
                    {
                        if( SHOW_OPTIONS == false )
                        {
                            SHOW_OPTIONS = true;

                            if( sTxt1 != NULL )
                            {
                                SDL_FreeSurface( sTxt1 );
                                sTxt1 = NULL;
                            }
                            if( sTxt2 != NULL )
                            {
                                SDL_FreeSurface( sTxt2 );
                                sTxt2 = NULL;
                            }
                            if( sTxt3 != NULL )
                            {
                                SDL_FreeSurface( sTxt3 );
                                sTxt3 = NULL;
                            }
                            itoa(chunk_INTER[i].nr1,auxchar,10);
                            sTxt1 = TTF_RenderText_Solid(font14,auxchar,BLUE);
                            itoa(chunk_INTER[i].nr2,auxchar,10);
                            sTxt2 = TTF_RenderText_Solid(font14,auxchar,BLUE);
                            itoa(chunk_INTER[i].nr3,auxchar,10);
                            sTxt3 = TTF_RenderText_Solid(font14,auxchar,BLUE);
                        }
                        else
                        SHOW_OPTIONS = false;
                    }
                }
            }
        }
        else if( clib == 2 )
        {
            for( i = 0;i < max_DayNight; i++ )
            if( x > box_Chunk[i].x && (x < box_Chunk[i].x + box_Chunk[i].w) && y > box_Chunk[i].y && (y < box_Chunk[i].y + box_Chunk[i].h) )
            {
                CDNTool = &DNTool[i];
            }
        }
        else if( cmode == 4 )
        {
            for( i = 0;i < max_COLIDE; i++ )
            if( x > box_Chunk[i].x && (x < box_Chunk[i].x + box_Chunk[i].w) && y > box_Chunk[i].y && (y < box_Chunk[i].y + box_Chunk[i].h) )
            {
                CCOLTool = &COLTool[i];
            }
        }
        else if( clib == 4 )
        {
            if( x > 10 && x < 10 + BLOCK_WIDTH && y > 110 && y < 110 + BLOCK_HEIGHT )//10 , 110
            {
                cInterTool = 0;
            }
            else if( x > 10 && x < 10 + BLOCK_WIDTH && y > 110 + 10 + BLOCK_HEIGHT && y < 110 + 10 + BLOCK_HEIGHT*2 )
            {
                cInterTool = 1;
            }
        }
        else if( clib == 5 )
        {

            {
                int nrnpcc = 0;
                for( NPCchunk *nc = FirstNPCList->next; nc != LastNPCList; nc = nc->next )
                {
                if( nrnpcc >= 6*MenuRow[1] && nrnpcc <= 6*( MenuRow[1] + 1 ) &&
                   (  x > 10 && x < 10 + BLOCK_WIDTH  ) && ( y > 110 + ( BLOCK_HEIGHT + 10 )*( nrnpcc - 6*MenuRow[1] ) &&
                                                             y < 110 + ( BLOCK_HEIGHT + 10 )*( nrnpcc - 6*MenuRow[1] + 1 ) ) )
                {
                    CMouseNPC = nc;
                    break;
                }
                nrnpcc ++;
                }
            }

        }

        }

    }

    if( !OnUi && !bCtrlDown )//Add/Select CHUNKS TO/FROM MAP
    {
    if( cmode == 0 )//Locked BG
    {
        //if( chunk_C.nr != chunk_NULL.nr )
        //{
            for( i = 0; i <= maxx;i++ )
            for(j = 0;j <= maxy;j++)
            if( x > box_NormalMap[i][j].x + iMoveMap_x && (x < box_NormalMap[i][j].x + box_NormalMap[i][j].w + iMoveMap_x ) && y > box_NormalMap[i][j].y  + iMoveMap_y && (y < box_NormalMap[i][j].y + box_NormalMap[i][j].h + iMoveMap_y ) )
            {
                //add block
                NormalMap[i][j] = chunk_C;
                break;
            }
        //}
    }
    else if( cmode == 1 )//Free BG
    {
        switch( clib )
        {
            case 0:
            C_FreeChunk -> skin = chunk_C.skin ;
            C_FreeChunk -> nr = chunk_C.nr;
            C_FreeChunk -> orient = chunk_C.orient;
            if( C_FreeChunk -> skin != NULL )
            {
                if( auxsurf != NULL)
                SDL_FreeSurface( auxsurf );
                auxsurf = C_FreeChunk ->skin;
                C_FreeChunk ->skin = chunk_BUILD[ cInterItm ].skin;
                AddChunk( C_FreeChunk,x - iMoveMap_x - 280, y - iMoveMap_y - 50, FreeBG );
                C_FreeChunk ->skin = auxsurf;
            }
            break;
            case 1:
            if( C_FreeChunk -> skin != NULL )
            {
                if( auxsurf != NULL)
                SDL_FreeSurface( auxsurf );
                auxsurf = C_FreeChunk ->skin;
                C_FreeChunk ->skin = chunk_INTER[ cInterItm ].skin;
                AddChunk( C_FreeChunk,x - iMoveMap_x - 280, y - iMoveMap_y - 50, FreeBG );
                C_FreeChunk ->skin = auxsurf;
            }
            break;
        }
    }
    else if( cmode == 2 )//Free FG
    {
        switch( clib )
        {
            case 0:
            C_FreeChunk -> skin = chunk_C.skin ;
            C_FreeChunk -> nr = chunk_C.nr;
            C_FreeChunk -> orient = chunk_C.orient;
            if( C_FreeChunk -> skin != NULL )
            {
                if( auxsurf != NULL)
                SDL_FreeSurface( auxsurf );
                auxsurf = C_FreeChunk ->skin;
                C_FreeChunk ->skin = chunk_BUILD[ cInterItm ].skin;
                AddChunk( C_FreeChunk,x - iMoveMap_x - 280, y - iMoveMap_y - 50, FreeFG );
                C_FreeChunk ->skin = auxsurf;
            }
            break;
            case 1:
            if( C_FreeChunk -> skin != NULL )
            {
                if( auxsurf != NULL)
                SDL_FreeSurface( auxsurf );
                auxsurf = C_FreeChunk ->skin;
                C_FreeChunk ->skin = chunk_INTER[ cInterItm ].skin;
                AddChunk( C_FreeChunk,x - iMoveMap_x - 280, y - iMoveMap_y - 50, FreeFG );
                C_FreeChunk ->skin = auxsurf;
            }
            break;
        }

    }
    else if( cmode == 7 )//Free Dynamic
    {
        switch( clib )
        {
            case 0:
            C_FreeChunk -> skin = chunk_C.skin ;
            C_FreeChunk -> nr = chunk_C.nr;
            C_FreeChunk -> orient = chunk_C.orient;
            if( C_FreeChunk -> skin != NULL )
            {
                if( auxsurf != NULL)
                SDL_FreeSurface( auxsurf );
                auxsurf = C_FreeChunk ->skin;
                C_FreeChunk ->skin = chunk_BUILD[ cInterItm ].skin;
                AddChunk( C_FreeChunk,x - iMoveMap_x - 280, y - iMoveMap_y - 50, FreeDynam );
                C_FreeChunk ->skin = auxsurf;
            }
            break;
            case 1:
            if( C_FreeChunk -> skin != NULL )
            {
                if( auxsurf != NULL)
                SDL_FreeSurface( auxsurf );
                auxsurf = C_FreeChunk ->skin;
                C_FreeChunk ->skin = chunk_INTER[ cInterItm ].skin;
                AddChunk( C_FreeChunk,x - iMoveMap_x - 280, y - iMoveMap_y - 50, FreeDynam );
                C_FreeChunk ->skin = auxsurf;
            }
            break;
        }

    }
    else if( cmode == 3 )//Light Map
    {
        if( clib == 2 )
        {
            if( CDNTool ->nr == DNTool[1].nr )
            {
                CDNChunk = AddLight( x - iMoveMap_x - 280, y - iMoveMap_y - 50,1,20);
                LightMap_Changed[2] = true;
                LightMap_Changed[3] = true;
                LightMap_Changed[4] = true;
            }
            else if( CDNTool ->nr == DNTool[0].nr )
            {
                bool edge = false,center = false;

                for( DayNightChunk* p = FirstDNChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                {
                    if( x > (p -> x + 280 + iMoveMap_x - 5 ) && x < (p -> x + 280 + iMoveMap_x + 5 ) && y > (p -> y + 50 + iMoveMap_y - 5 ) && y < (p -> y + 50 + iMoveMap_y + 5 ) )
                    {
                        DragLight_Round = true;
                        center = true;
                    }
                }
                if( !center )
                {
                    for( DayNightChunk* p = FirstDNChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                    {
                        if( x > (p -> x + 280 + iMoveMap_x - 5 - p -> range) && x < (p -> x + 280 + iMoveMap_x + 5 - p -> range) && y > (p -> y + 50 + iMoveMap_y - 5) && y < (p -> y + 50 + iMoveMap_y + 5 ) )
                        {
                            RescaleLight_Round = true;
                            edge = true;
                        }
                    }
                }
                if( !edge && !center )
                {
                    for( DayNightChunk* p = FirstDNChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                    {
                        if( sqrt( pow( (p -> y + 50 + iMoveMap_y) - y , 2 ) + pow( (p -> x + 280 + iMoveMap_x ) - x , 2 ) ) < p -> range )
                        {
                            CDNChunk = p;
                        }
                    }
                }

            }
        }

    }
    else if( cmode == 4 )//Colide Map
    {
        if( x > rColId.x && x < rColId.x + rColId.w && y > rColId.y && y < rColId.y + rColId.h )
        {
            ValChange = &CCOLChunk->id;
            InputString = ' ';
            InputChange = CHANGE_INT;
            ConsoleInput = true;
        }
        else
        {
        if( CCOLTool -> nr == COLTool[1].nr )
        CCOLChunk = AddColider(x - iMoveMap_x - 280 ,y - iMoveMap_y - 50 ,20,20,-1);
        if( CCOLTool->nr == COLTool[0].nr )
        {
            bool center = false;
            ColideChunk* p;

            for( p = FirstCOLChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
            {
                if( x > (p -> x + 280 + iMoveMap_x - 5 ) && x < (p -> x + 280 + iMoveMap_x + 5 ) &&
                    y > (p -> y + 50  + iMoveMap_y - 5 ) && y < (p -> y + 50  + iMoveMap_y + 5 ) )
                {
                    DragColide = true;
                    center = true;
                    if( CCOLChunk  != p  )
                    CCOLChunk = p;


                }
            }

            if( !center )//Rescale w
            {
                for( p = FirstCOLChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                {
                    if( x > (p -> x + 280 + iMoveMap_x - 5 + p->w/2 ) && x < (p -> x + 280 + iMoveMap_x + 5 + p->w/2 ) && y > (p -> y + 50 + iMoveMap_y - 5 ) && y < (p -> y + 50 + iMoveMap_y + 5 ) )
                    {
                        RescaleColide_width = true;
                    }
                }
            }
            if( !center && !RescaleColide_width )//Rescale H
            {
                for( p = FirstCOLChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                {
                    if( x > (p -> x + 280 + iMoveMap_x - 5 ) && x < (p -> x + 280 + iMoveMap_x + 5 ) && y > (p -> y + 50 + iMoveMap_y - 5 - p->h/2 ) && y < (p -> y + 50 + iMoveMap_y + 5 - p->h/2 ) )
                    {
                        RescaleColide_height = true;
                    }
                }
            }

            if( !center && !RescaleColide_width && !RescaleColide_height )//G1 Rescale
            {
                for( p = FirstCOLChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                {
                    if( x > (p -> x + 280 + iMoveMap_x - 5 - p->w/2 ) && x < (p -> x + 280 + iMoveMap_x + 5 - p->w/2 ) && y > (p -> y + 50 + iMoveMap_y - 5 - p->h/2 ) && y < (p -> y + 50 + iMoveMap_y + 5 - p->h/2 ) )
                    {
                        DragColide_g1 = true;
                    }
                }
            }
            if( !center && !RescaleColide_width && !RescaleColide_height && !DragColide_g1 )//G2 Rescale
            {
                for( p = FirstCOLChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
                {
                    if( x > (p -> x + 280 + iMoveMap_x - 5 + p->w/2 ) && x < (p -> x + 280 + iMoveMap_x + 5 + p->w/2 ) && y > (p -> y + 50 + iMoveMap_y - 5 + p->h/2 ) && y < (p -> y + 50 + iMoveMap_y + 5 + p->h/2 ) )
                    {
                        DragColide_g2 = true;
                    }
                }
            }

            if( !center )
            {
                for( p = FirstCOLChunk -> next; p != LastCOLChunk; p = p-> next )
                {
                    if( x > (p -> x + 280 + iMoveMap_x - p -> w/2 ) && x < (p -> x + 280 + iMoveMap_x + p -> w/2 ) && y > (p -> y + 50 + iMoveMap_y - p ->h/2 ) && y < (p -> y + 50 + iMoveMap_y + p ->h/2 ) )
                    {
                        CCOLChunk = p;
                        stringstream ssaux;
                        ssaux<<CCOLChunk->id;
                        if( sColId != NULL ) SDL_FreeSurface( sColId ),sColId = NULL;
                        sColId = TTF_RenderText_Blended( font14,ssaux.str().c_str() ,BLUE );
                    }
                }
            }
        }
        }





    }
    else if( cmode == 5 )//Interact
    {
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            if( cInterTool == 0 )//Select tool
            {
                //Red dot( center )
                if( cInterChunk != NULL &&
                    x > cInterChunk -> x + 280 + iMoveMap_x + cInterChunk -> w/2 - 5 &&
                    x < cInterChunk -> x + 280 + iMoveMap_x + cInterChunk -> w/2 + 5 &&
                    y > cInterChunk -> y + 50 + iMoveMap_y + cInterChunk -> h/2 - 5 &&
                    y < cInterChunk -> y + 50 + iMoveMap_y + cInterChunk -> h/2 + 5 )
                {
                    DragInter = true;
                }
                //Green dot top-left
                else if( cInterChunk != NULL &&
                         x > cInterChunk -> x + 280 + iMoveMap_x - 5 &&
                         x < cInterChunk -> x + 280 + iMoveMap_x + 5 &&
                         y > cInterChunk -> y + 50 + iMoveMap_y - 5 &&
                         y < cInterChunk -> y + 50 + iMoveMap_y + 5 )
                {
                    g_x = cInterChunk -> x;
                    g_y = cInterChunk -> y;
                    DragInter_g1 = true;
                }
                //Green dot bot-right
                else if( cInterChunk != NULL &&
                         x > cInterChunk -> x + 280 + iMoveMap_x + cInterChunk -> w - 5 &&
                         x < cInterChunk -> x + 280 + iMoveMap_x + cInterChunk -> w + 5 &&
                         y > cInterChunk -> y + 50 + iMoveMap_y + cInterChunk -> h - 5 &&
                         y < cInterChunk -> y + 50 + iMoveMap_y + cInterChunk -> h + 5 )
                {
                    g_x = cInterChunk -> x + cInterChunk -> w;
                    g_y = cInterChunk -> y + cInterChunk -> h;
                    DragInter_g2 = true;
                }
                else//Select Chunk
                {
                    /*
                    for( InterSpot *z = FirstItr -> next; z != LastItr; z = z -> next )
                    {


                        if( x > z -> x + 280 + iMoveMap_x && x < z -> x + z -> w + 280 + iMoveMap_x && y > z -> y + 50 + iMoveMap_y && y < z -> y + z -> h + 50 + iMoveMap_y )
                        {
                            cInterChunk = z;
                            switch( cInterChunk->type )
                            {

                                case 0:
                                sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
                                sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Locked:",BLUE );
                                sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"To X:",BLUE );
                                sWindow_Inter_Text[3] = TTF_RenderText_Solid( font14,"To Y:",BLUE );
                                sWindow_Inter_Text[4] = TTF_RenderText_Solid( font14,"To Zone:",BLUE );
                                itoa(cInterChunk ->id,NrAux,10);
                                sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[0],NrAux,10);
                                sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[0],NrAux,10);
                                sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[1],NrAux,10);
                                sWindow_Inter_Data[3] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[2],NrAux,10);
                                sWindow_Inter_Data[4] = TTF_RenderText_Solid( font14,NrAux,BLUE );

                                break;

                                case 1:
                                sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
                                sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Val 0:",BLUE );
                                sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"Val 1:",BLUE );
                                sWindow_Inter_Text[3] = TTF_RenderText_Solid( font14,"Val 2:",BLUE );
                                itoa(cInterChunk ->info[0],NrAux,10);
                                sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->id,NrAux,10);
                                sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[1],NrAux,10);
                                sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[2],NrAux,10);
                                sWindow_Inter_Data[3] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                break;
                            }
                        }
                    }
                    */

                    Window_Inter_MouseX = x - iMoveMap_x - 280;
                    Window_Inter_MouseY = y - iMoveMap_y - 50;
                    Window_Inter_RenderVal = true;

                }

            }
            else if( cInterTool == 1 )//Add tool
            {
                cInterChunk = AddInterSpot( x - iMoveMap_x - 280 ,y - iMoveMap_y - 50 );

                Window_Inter_MouseX = x - iMoveMap_x - 280;
                Window_Inter_MouseY = y - iMoveMap_y - 50;
                Window_Inter_RenderVal = true;
                //Window_Inter_show = true;

                /*
                switch( cInterChunk->type )
                {

                                case 0:
                                sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
                                sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Locked:",BLUE );
                                sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"To X:",BLUE );
                                sWindow_Inter_Text[3] = TTF_RenderText_Solid( font14,"To Y:",BLUE );
                                sWindow_Inter_Text[4] = TTF_RenderText_Solid( font14,"To Zone:",BLUE );
                                itoa(cInterChunk ->id,NrAux,10);
                                sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[0],NrAux,10);
                                sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[0],NrAux,10);
                                sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[1],NrAux,10);
                                sWindow_Inter_Data[3] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[2],NrAux,10);
                                sWindow_Inter_Data[4] = TTF_RenderText_Solid( font14,NrAux,BLUE );

                                break;

                                case 1:
                                sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
                                sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Val 0:",BLUE );
                                sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"Val 1:",BLUE );
                                sWindow_Inter_Text[3] = TTF_RenderText_Solid( font14,"Val 2:",BLUE );
                                itoa(cInterChunk ->info[0],NrAux,10);
                                sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->id,NrAux,10);
                                sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[1],NrAux,10);
                                sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                itoa(cInterChunk ->info[2],NrAux,10);
                                sWindow_Inter_Data[3] = TTF_RenderText_Solid( font14,NrAux,BLUE );
                                break;
                }
                */

            }

            //Butoane de pe Window
            if( Window_Inter_show )
            {
                //Drag Arround
                if( x > Window_Inter_x && x < Window_Inter_x + Window_InterRect_Move.w &&
                    y > Window_Inter_y && y < Window_Inter_y + Window_InterRect_Move.h )
                {
                    Window_Inter_Drag_x = x - Window_Inter_x;
                    Window_Inter_Drag_y = y - Window_Inter_y;
                    Window_Inter_Drag = true;
                }
                //Change type
                else if( x > Window_Inter_x + Window_InterRect_ButtonType.x &&
                         x < Window_Inter_x + Window_InterRect_ButtonType.x + Window_InterRect_ButtonType.w &&
                         y > Window_Inter_y + Window_InterRect_ButtonType.y &&
                         y < Window_Inter_y + Window_InterRect_ButtonType.y + Window_InterRect_ButtonType.h )
                {
                cInterChunk->type ++;
                if( cInterChunk->type > 3 )
                cInterChunk->type = 0;

                Window_Inter_RenderVal = true;

                }
                else
                {
                    int t;
                    for( t = 0; t < 5; t++ )
                    if( x > Window_Inter_x + Window_InterRect_Button[t].x && x < Window_Inter_x + Window_InterRect_Button[t].x + Window_InterRect_Button[t].w &&
                        y > Window_Inter_y + Window_InterRect_Button[t].y && y < Window_Inter_y + Window_InterRect_Button[t].y + Window_InterRect_Button[t].h )
                    {

                        //id
                        if( t == 0 )
                        {
                            ValChange = &cInterChunk->id;
                            SurfaceChange = sWindow_Inter_Data[ 0 ];
                        }
                        //val 0,1,2,3
                        else
                        {
                            ValChange     = &cInterChunk->info[ t-1 ];
                            SurfaceChange = sWindow_Inter_Data[ t ];
                        }


                        InputChange = CHANGE_INT;
                        ConsoleInput = true;
                        InputString = ' ' ;
                        char caux[7];
                        itoa( *ValChange,caux,10 );
                        InputString = (string)caux;

                    }
                }

            }
        }
        else if( event.button.button == SDL_BUTTON_RIGHT )
        {
            Window_Inter_show = !Window_Inter_show;

            if( Window_Inter_show )
            {
                Window_Inter_x = x;
                Window_Inter_y = y;
            }
        }
    }
    else if( cmode == 6 )//NPC
    {
        NPCchunk *inpc;
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            if( CMouseNPC == FirstNPCList->next )//Select
            {
                //ON NPC MainWindow
                if( x > ( SCREEN_WIDTH - Window_NPCMAIN_bg->w - 10 ) && x < ( SCREEN_WIDTH - 10 ) &&
                    y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + Window_NPCMAIN_bg->h ))
                {
                    ///Button 1 Name
                    if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 21) )
                    {
                        StrChange = &CSelectedNPC->Name;
                        SurfaceChange = Window_NPCMAIN_name;
                        CurrentSurface = 1;
                        InputString = ' ';
                        InputChange = CHANGE_STR;
                        ConsoleInput = true;
                    }///Button 2 Relationship
                    else if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 +38 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 51 ) )
                    {
                        CSelectedNPC->Relationship ++;
                        if( CSelectedNPC->Relationship > 3 )
                        CSelectedNPC->Relationship = 1;
                        RenderNPCInfo();
                    }///Button 3 Faction
                    else if( CSelectedNPC != NULL && CSelectedNPC->Relationship == 3 &&
                             y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 53 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 65 )  )
                    {
                        StrChange = &CSelectedNPC->Faction;
                        CurrentSurface = 2;
                        InputString = ' ';
                        InputChange = CHANGE_STR;
                        ConsoleInput = true;
                    }///Button 4 Hate List
                    else if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 66 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 79 )  )
                    {

                    }///Button 5 Behavior
                    else if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 80 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 93 )  )
                    {
                        StrChange = &CSelectedNPC->Behavior;
                        CurrentSurface = 3;
                        InputString = ' ';
                        InputChange = CHANGE_STR;
                        ConsoleInput = true;
                    }///Button 6 Define path
                    else if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 95 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 109 ) )
                    {

                    }///Buton 7 Talk-Interact ON/OFF
                    else if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 111 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 123 ) )
                    {
                        CSelectedNPC->CanTalk = !CSelectedNPC->CanTalk;
                        if( Window_NPCMAIN_talk )
                        {
                            SDL_FreeSurface( Window_NPCMAIN_talk );
                            Window_NPCMAIN_talk = NULL;
                        }
                        if( CSelectedNPC->CanTalk )
                        Window_NPCMAIN_talk = TTF_RenderText_Solid( font14, "YES" ,BLACK );
                        else
                        Window_NPCMAIN_talk = TTF_RenderText_Solid( font14, "NO" ,BLACK );
                    }///Button 8 File nr
                    else if( y > ( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 129 && y < (( SCREEN_HEIGHT - Window_NPCMAIN_bg->h )/2 + 141 ) )
                    {
                        ValChange = &CSelectedNPC->filenr;
                        CurrentSurface = 4;
                        InputString = ' ';
                        InputChange = CHANGE_INT;
                        ConsoleInput = true;

                    }
                }
                else
                {
                    for( inpc = FirstNPCMap->next ;inpc != LastNPCMap ; inpc = inpc->next )
                    {
                        if( x > inpc->x + iMoveMap_x + 280 && x < ( inpc->x + iMoveMap_x + 280 + inpc->w ) &&
                            y > inpc->y + iMoveMap_y + 50  && y < ( inpc->y + iMoveMap_y + 50  + inpc->h ) )
                            {
                                CSelectedNPC = inpc;
                                FCMove_x = x + iMoveMap_x + 280 ;
                                FCMove_y = y + iMoveMap_y + 50 ;
                                dif_w = inpc->x + 280 + iMoveMap_x - x;
                                dif_h = inpc->y + 50 + iMoveMap_y - y;
                                FCmoveing = true;
                                if( sSelectedChunk != NULL )
                                SDL_FreeSurface( sSelectedChunk );
                                sSelectedChunk = Pixels.RenderRect( inpc->w,inpc->h,2 );
                                RenderNPCInfo();
                                break;
                            }
                        else if( inpc->next == LastNPCMap )
                        {
                            CSelectedNPC = NULL;
                            RenderNPCInfo();
                        }
                    }
                }
            }
            else
            {
                AuxNPCchunk->x = x - 280 - iMoveMap_x;
                AuxNPCchunk->y = y - 50 - iMoveMap_y;
                AuxNPCchunk->w = CMouseNPC->w;
                AuxNPCchunk->h = CMouseNPC->h;
                AuxNPCchunk->skin = CMouseNPC->skin;
                AuxNPCchunk->skinnr = CMouseNPC->skinnr;
                CSelectedNPC = AddNPCchunk( AuxNPCchunk );
                CMouseNPC = FirstNPCList->next;

                FCMove_x = x + iMoveMap_x + 280 ;
                FCMove_y = y + iMoveMap_y + 50 ;
                dif_w = CSelectedNPC->x + 280 + iMoveMap_x - x;
                dif_h = CSelectedNPC->y + 50 + iMoveMap_y - y;
                FCmoveing = true;
                if( sSelectedChunk != NULL )
                SDL_FreeSurface( sSelectedChunk );
                sSelectedChunk = Pixels.RenderRect( CSelectedNPC->w,CSelectedNPC->h,2 );

                RenderNPCInfo();
            }
        }
    }

    }
}
else if( newevent.type == SDL_MOUSEBUTTONUP )
{
    if( DRAG_DRAW )
    DRAG_DRAW = false;
    if( FCmoveing )
    FCmoveing = false;
    if( DragLight_Round )
    DragLight_Round = false;
    if( RescaleLight_Round )
    RescaleLight_Round = false;
    if( DragColide )
    DragColide = false;
    if( RescaleColide_width )
    RescaleColide_width = false;
    if( RescaleColide_height )
    RescaleColide_height = false;

    if( DragColide_g1 )
    {
        DragColide_g1 = false;
    }
    if( DragColide_g2 )
    {
        DragColide_g2 = false;
    }

    if( DragInter )
    DragInter = false;
    if( DragInter_g1 )
    DragInter_g1 = false;
    if( DragInter_g2 )
    DragInter_g2 = false;

    if( Window_Inter_Drag )
    Window_Inter_Drag = false;

    x = event.button.x;
    y = event.button.y;

}
else if( newevent.type == SDL_MOUSEMOTION )
{
    FreeChunk *p,*Lastfc = NULL;
    NPCchunk *inpc;
    stringstream ssaux;
    x = event.motion.x;
    y = event.motion.y;
    m_x = x;
    m_y = y;

    if( ShowMousePos )
    {
        //Render nrs
        if( sMousePoz != NULL ) SDL_FreeSurface( sMousePoz );

        if( m_x- 280 - iMoveMap_x < 0 )
        ssaux<<'-';
        ssaux<<m_x- 280 - iMoveMap_x <<',';
        if( m_y- 50 - iMoveMap_y < 0 )
        ssaux<<'-';
        ssaux<<m_y - 50 - iMoveMap_y ;
        sMousePoz = TTF_RenderText_Blended( font14,ssaux.str().c_str() ,RED );
    }

    if( bMouseDrag )//Map drag
    {
        if( iInitial_y != y ) iMoveMap_y -= (iInitial_y - y),iInitial_y = y;
        if( iInitial_x != x ) iMoveMap_x -= (iInitial_x - x),iInitial_x = x;
    }
    else if( !bCtrlDown )//daca ctrl nu este apasat
    {
    if( DEFINE_MAP )
    {
        for(i = 0; i < 100;i++)
            for( j = 0; j < 100;j++ )
            if( x > (box_NormalMap[i][j].x + iMoveMap_x ) && y > (box_NormalMap[i][j].y + iMoveMap_y ) )
            {
                NormalMap[i][j].skin = sNormalNULL;
                maxx = j;
                maxy = i;

                if( sWidth != NULL )
                {
                    SDL_FreeSurface( sWidth );
                    sWidth = NULL;
                }
                if( sHeight != NULL )
                {
                    SDL_FreeSurface( sHeight );
                    sHeight = NULL;
                }
                itoa(maxy,auxchar,10);
                sWidth = TTF_RenderText_Solid( font,auxchar,BYellow ) ;

                itoa(maxx,auxchar,10);
                sHeight = TTF_RenderText_Solid( font,auxchar,BYellow ) ;

            }
            else
            NormalMap[i][j].skin = NULL;
    }
    else if( DRAG_DRAW  )//&& chunk_C.nr != chunk_NULL.nr
    {
        if( clib == 0 )
        {
            for( i = 0; i <= maxx;i++ )
            for(j = 0;j <= maxy;j++)
            if( x > box_NormalMap[i][j].x + iMoveMap_x && (x < box_NormalMap[i][j].x + box_NormalMap[i][j].w + iMoveMap_x ) && y > box_NormalMap[i][j].y  + iMoveMap_y && (y < box_NormalMap[i][j].y + box_NormalMap[i][j].h + iMoveMap_y ) )
            {
                //add block
                NormalMap[i][j] = chunk_C;
                break;
            }
        }
    }
    }

    if( FCmoveing )
    {
        if( ( cmode >= 0 && cmode <= 5 ) || ( cmode == 7 ) )
        {
            FreeCTarget_Move -> x = x - iMoveMap_x - 280 + dif_w ;
            FreeCTarget_Move -> y = y - iMoveMap_y - 50 + dif_h ;
        }
        else if( cmode == 6 )
        {
            CSelectedNPC->x = x - iMoveMap_x - 280 + dif_w;
            CSelectedNPC->y = y - iMoveMap_y - 50 + dif_h;
        }

    }

    if( DragLight_Round )
    {
        CDNChunk ->x = x - 280 - iMoveMap_x;
        CDNChunk ->y = y - 50 - iMoveMap_y;
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
    }
    else if( RescaleLight_Round )
    {
        CDNChunk -> range = CDNChunk -> x - (x - 280 - iMoveMap_x);
        LightMap_Changed[2] = true;
        LightMap_Changed[3] = true;
        LightMap_Changed[4] = true;
    }

    if( DragColide )
    {
        CCOLChunk -> x = x - 280 - iMoveMap_x;
        CCOLChunk -> y = y - 50 - iMoveMap_y;
        CCOLChunk ->g1x = CCOLChunk ->x - CCOLChunk ->w/2;
        CCOLChunk ->g2x = CCOLChunk ->x + CCOLChunk ->w/2;
        CCOLChunk ->g1y = CCOLChunk ->y - CCOLChunk ->h/2;
        CCOLChunk ->g2y = CCOLChunk ->y + CCOLChunk ->h/2;
    }

    if( RescaleColide_width )
    {
        CCOLChunk -> w = ( x - ( CCOLChunk -> x + iMoveMap_x + 280 ) )*2;
        SDL_FreeSurface( CCOLChunk ->skin );
        CCOLChunk ->skin = Pixels.RenderRect( CCOLChunk -> w ,CCOLChunk ->h ,0);
        CCOLChunk ->g1x = CCOLChunk ->x - CCOLChunk ->w/2;
        CCOLChunk ->g2x = CCOLChunk ->x + CCOLChunk ->w/2;
    }
    if( RescaleColide_height )
    {
        CCOLChunk -> h = ( ( CCOLChunk -> y + iMoveMap_y + 50 ) - y )*2;
        SDL_FreeSurface( CCOLChunk ->skin );
        CCOLChunk ->skin = Pixels.RenderRect( CCOLChunk -> w ,CCOLChunk ->h, 0 );
        CCOLChunk ->g1y = CCOLChunk ->y - CCOLChunk ->h/2;
        CCOLChunk ->g2y = CCOLChunk ->y + CCOLChunk ->h/2;
    }
    if( DragColide_g1 )
    {
        CCOLChunk -> g1x = x - 280 - iMoveMap_x;
        CCOLChunk -> g1y = y - 50 - iMoveMap_y;
        CCOLChunk -> w = CCOLChunk -> g2x - CCOLChunk -> g1x;
        CCOLChunk -> h = CCOLChunk -> g2y - CCOLChunk -> g1y;
        SDL_FreeSurface( CCOLChunk ->skin );
        CCOLChunk ->skin = Pixels.RenderRect( CCOLChunk -> w,CCOLChunk -> h, 0 );
        CCOLChunk ->x = CCOLChunk ->g1x + ( CCOLChunk ->g2x - CCOLChunk ->g1x )/2;
        CCOLChunk ->y = CCOLChunk ->g1y + ( CCOLChunk ->g2y - CCOLChunk ->g1y )/2;
    }
    if( DragColide_g2 )
    {
        CCOLChunk -> g2x = x - 280 - iMoveMap_x;
        CCOLChunk -> g2y = y - 50 - iMoveMap_y;
        CCOLChunk -> w = CCOLChunk -> g2x - CCOLChunk -> g1x;
        CCOLChunk -> h = CCOLChunk -> g2y - CCOLChunk -> g1y;
        SDL_FreeSurface( CCOLChunk ->skin );
        CCOLChunk ->skin = Pixels.RenderRect( CCOLChunk -> w,CCOLChunk -> h,0 );
        CCOLChunk ->x = CCOLChunk ->g1x + ( CCOLChunk ->g2x - CCOLChunk ->g1x )/2;
        CCOLChunk ->y = CCOLChunk ->g1y + ( CCOLChunk ->g2y - CCOLChunk ->g1y )/2;
    }

    if( DragInter )
    {
        cInterChunk -> x = x - 280 - iMoveMap_x - cInterChunk -> w/2;
        cInterChunk -> y = y - 50 - iMoveMap_y - cInterChunk -> h/2;
    }
    else if( DragInter_g1 )
    {
        SDL_FreeSurface( cInterChunk ->skin );
        cInterChunk -> w = ( cInterChunk ->x + cInterChunk ->w + 280 + iMoveMap_x ) - x;
        cInterChunk -> h = ( cInterChunk ->y + cInterChunk ->h + 50 + iMoveMap_y ) - y;
        cInterChunk -> x = x - 280 - iMoveMap_x;
        cInterChunk -> y = y - 50 - iMoveMap_y;
        cInterChunk ->skin = Pixels.RenderRect(cInterChunk ->w ,cInterChunk ->h,0 );
    }
    else if( DragInter_g2 )
    {
        SDL_FreeSurface( cInterChunk ->skin );
        cInterChunk -> w = x - ( cInterChunk -> x + 280 + iMoveMap_x );
        cInterChunk -> h = y - ( cInterChunk -> y + 50 + iMoveMap_y );

        cInterChunk ->skin = Pixels.RenderRect(cInterChunk ->w ,cInterChunk ->h,0 );
    }

    if( Window_Inter_Drag )
    {
        Window_Inter_x = x - Window_Inter_Drag_x;
        Window_Inter_y = y - Window_Inter_Drag_y;
    }

    if( cmode == 1 )//Current chunk on BG
    {
        for( p = FirstFreeChunk_back->next ; p != LastFreeChunk_back ;p = p->next )
        if(( ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) && (!p->animated) ) ||
                    ( p->animated && ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w/p->frames ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) ) )
        {
            CurrentChunk_x = p->x +280 +iMoveMap_x ;
            CurrentChunk_y = p->y +50 + iMoveMap_y ;

            if( Lastfc != p )
            {
                if( sCurrentChunk != NULL )
                SDL_FreeSurface( sCurrentChunk );
                if( p->animated )
                sCurrentChunk = Pixels.RenderRect( p->skin->w/p->frames,p->skin->h, 1 );
                else
                sCurrentChunk = Pixels.RenderRect( p->skin->w,p->skin->h, 1 );
            }

            ShowCChunk = true;
            Lastfc = p;
            break;
        }
        else if( p->next == LastFreeChunk_back )
        ShowCChunk = false;
    }
    else if( cmode == 7 )//Current chunk on Dynamic
    {
        for( p = FirstFreeChunk_dynam->next ; p != LastFreeChunk_dynam ;p = p->next )
        if(( ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) && (!p->animated) ) ||
            ( p->animated && ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w/p->frames ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) ) )
        {
            CurrentChunk_x = p->x +280 +iMoveMap_x ;
            CurrentChunk_y = p->y +50 + iMoveMap_y ;

            if( Lastfc != p )
            {
                if( sCurrentChunk != NULL )
                SDL_FreeSurface( sCurrentChunk );
                if( p->animated )
                sCurrentChunk = Pixels.RenderRect( p->skin->w/p->frames ,p->skin->h, 1 );
                else
                sCurrentChunk = Pixels.RenderRect( p->skin->w,p->skin->h, 1 );
            }

            ShowCChunk = true;
            Lastfc = p;
            break;
        }
        else if( p->next == LastFreeChunk_dynam )
        ShowCChunk = false;
    }
    else if( cmode == 2 )//Current chunk on FG
    {
        for( p = FirstFreeChunk_fore->next ; p != LastFreeChunk_fore ;p = p->next )
        if(( ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) && (!p->animated) ) ||
            ( p->animated && ( x > p -> x + iMoveMap_x + 280 && x < ( p ->x + iMoveMap_x + 280 + p -> skin ->w/p->frames ) && y > p-> y  + iMoveMap_y + 50&& y < ( p -> y + iMoveMap_y + 50 + p -> skin -> h ) ) ) )
        {
            CurrentChunk_x = p->x +280 +iMoveMap_x ;
            CurrentChunk_y = p->y +50 + iMoveMap_y ;

            if( Lastfc != p )
            {
                if( sCurrentChunk != NULL )
                SDL_FreeSurface( sCurrentChunk );
                if( p->animated )
                sCurrentChunk = Pixels.RenderRect( p->skin->w/p->frames ,p->skin->h, 1 );
                else
                sCurrentChunk = Pixels.RenderRect( p->skin->w,p->skin->h, 1 );
            }

            ShowCChunk = true;
            Lastfc = p;
            break;
        }
        else if( p->next == LastFreeChunk_fore )
        ShowCChunk = false;
    }
    else if( cmode == 6 )//Current NPC cnhunk
    {
        for( inpc = FirstNPCMap->next ; inpc != LastNPCMap ;inpc = inpc->next )
        if( x > inpc->x + iMoveMap_x + 280 && x < ( inpc->x + iMoveMap_x + 280 + inpc->w ) &&
            y > inpc->y  + iMoveMap_y + 50 && y < ( inpc->y + iMoveMap_y + 50 + inpc-> h ) )
        {
            CurrentChunk_x = inpc->x +280 +iMoveMap_x ;
            CurrentChunk_y = inpc->y +50 + iMoveMap_y ;

            if( LastNPC_HL != inpc )
            {
                if( sCurrentChunk != NULL )
                SDL_FreeSurface( sCurrentChunk );

                sCurrentChunk = Pixels.RenderRect( inpc->w,inpc->h, 1 );
            }

            ShowCChunk = true;
            LastNPC_HL = inpc;
            break;
        }
        else if( inpc->next == LastNPCMap )
        ShowCChunk = false;
    }
}
else if( newevent.type == SDL_KEYDOWN )
{

    if( ConsoleInput )
    {
        stringstream ssaux;
        string saux;

        if( InputChange == CHANGE_STR )
        {
            InputString = *StrChange;

            if( newevent.key.keysym.unicode >= (Uint16)'a' && newevent.key.keysym.unicode <= (Uint16)'z'  )
            {
                InputString += (char)newevent.key.keysym.unicode;
            }
            else if( newevent.key.keysym.unicode >= (Uint16)'A' && newevent.key.keysym.unicode <= (Uint16)'Z'  )
            {
                InputString += (char)newevent.key.keysym.unicode;
            }
            else if( newevent.key.keysym.unicode == (Uint16)' ' )
            {
                InputString += (char)newevent.key.keysym.unicode;
            }
            else if( newevent.key.keysym.unicode >= (Uint16)'[' && newevent.key.keysym.unicode <= (Uint16)'`'  )
            {
                InputString += (char)newevent.key.keysym.unicode;
            }
        }

        if( newevent.key.keysym.unicode >= (Uint16)'0' && newevent.key.keysym.unicode <= (Uint16)'9'  )
        {
            InputString += (char)newevent.key.keysym.unicode;
        }
        else if( newevent.key.keysym.sym == SDLK_BACKSPACE && InputString.length() > 0 )
        {
            InputString.erase( InputString.length() - 1 );
        }
        else if( newevent.key.keysym.sym == SDLK_RETURN )
        {
            ConsoleInput = false;
        }

        if( InputChange == CHANGE_INT )
        {
            if( InputString.length() == 0 )
            {
                *ValChange = 0;
                saux = '0';
            }
            else
            *ValChange = atoi( InputString.c_str() );

            if( cmode == 4 )
            {
                if( sColId != NULL )
                SDL_FreeSurface( sColId );

                if( InputString.length() > 0 )
                sColId = TTF_RenderText_Solid( font14,InputString.c_str() ,BLUE );
                else
                sColId = TTF_RenderText_Solid( font14,saux.c_str() ,BLUE );
            }
            else
            if( cmode == 5 )
            {
                if( SurfaceChange != NULL )
                SDL_FreeSurface( SurfaceChange ),SurfaceChange = NULL ;

                if( InputString.length() > 0 )
                SurfaceChange = TTF_RenderText_Solid( font14,InputString.c_str() ,BLUE );
                else
                SurfaceChange = TTF_RenderText_Solid( font14,saux.c_str() ,BLUE );
            }
            else
            if( cmode == 6 )
            {
                if( CurrentSurface == 4 )
                {
                    if( Window_NPCMAIN_filenr != NULL )
                    {
                        SDL_FreeSurface( Window_NPCMAIN_filenr );
                        Window_NPCMAIN_filenr = NULL;
                    }
                    Window_NPCMAIN_filenr = TTF_RenderText_Solid( font14, InputString.c_str() , BLACK );
                }
            }

        }
        else if( InputChange == CHANGE_STR )
        {
            *StrChange = InputString;

            if( cmode == 6 )
            {
                if( CurrentSurface == 1 )
                {
                    if( Window_NPCMAIN_name != NULL )
                    {
                        SDL_FreeSurface( Window_NPCMAIN_name );
                        Window_NPCMAIN_name = NULL;
                    }
                    Window_NPCMAIN_name = TTF_RenderText_Solid( font14,InputString.c_str(),BLACK );
                }
                else if( CurrentSurface == 2 )
                {
                    if( Window_NPCMAIN_faction != NULL )
                    {
                        SDL_FreeSurface( Window_NPCMAIN_faction );
                        Window_NPCMAIN_faction = NULL;
                    }
                    Window_NPCMAIN_faction = TTF_RenderText_Solid( font14,InputString.c_str(),BLACK );
                }
                else if( CurrentSurface == 3 )
                {
                    if( Window_NPCMAIN_behavior != NULL )
                    {
                        SDL_FreeSurface( Window_NPCMAIN_behavior );
                        Window_NPCMAIN_behavior = NULL;
                    }
                    Window_NPCMAIN_behavior = TTF_RenderText_Solid( font14,InputString.c_str(),BLACK );
                }
            }
        }
    }
    else
    {
    if( newevent.key.keysym.sym == SDLK_DELETE )
    {
        FreeChunk *p;

        if( cmode == 1 || cmode == 2 || cmode == 7 )
        {
                 if( cmode == 1 ) p = FirstFreeChunk_back;
            else if( cmode == 7 ) p = FirstFreeChunk_dynam;
            else if( cmode == 2 ) p = FirstFreeChunk_fore;

            for(; p ->next != NULL && p != NULL ; p = p->next )
            if( FreeCTarget_Move == p -> next )
            {
                if( cmode == 1 && LastChunkBack == p->next ) LastChunkBack = p;
                if( cmode == 7 && LastChunkDynam == p->next ) LastChunkDynam = p;
                if( cmode == 2 && LastChunkFore == p->next ) LastChunkFore = p;

                p -> next = p-> next -> next;
                delete FreeCTarget_Move;

                FreeCTarget_Move = NULL;
            }

            if( sCurrentChunk )
            {
                SDL_FreeSurface( sCurrentChunk );
                sCurrentChunk = NULL;
            }

            if( sSelectedChunk )
            {
                SDL_FreeSurface( sSelectedChunk );
                sSelectedChunk = NULL;
            }


        }
        if( cmode == 3 )
        {
            DayNightChunk *q;
            for( q = FirstDNChunk  ; q ->next != NULL && q != NULL; q = q->next )
            if( CDNChunk == q -> next )
            {
                q -> next = q -> next -> next;
                delete CDNChunk;
                CDNChunk = NULL;
                LightMap_Changed[2] = true;
                LightMap_Changed[3] = true;
                LightMap_Changed[4] = true;
                break;
            }
        }
        else if( cmode == 4 )
        {
            ColideChunk *q;
            for( q = FirstCOLChunk; q ->next != NULL && q != NULL; q = q->next  )
            if( q ->next == CCOLChunk )
            {
                q -> next = q ->next ->next;
                delete CCOLChunk;
                CCOLChunk = NULL;
                break;
            }

        }
        else if( cmode == 5 )
        {
            InterSpot *z;
            for( z = FirstItr; z ->next != NULL && z != NULL; z = z ->next  )
            if( z ->next == cInterChunk )
            {
                z -> next = z ->next ->next;
                delete cInterChunk;
                cInterChunk = NULL;
                break;
            }
        }
        else if( cmode == 6 )
        {
            NPCchunk *inpc;
            for( inpc = FirstNPCMap; inpc ->next != LastNPCMap; inpc = inpc->next  )
            if( inpc->next == CSelectedNPC )
            {
                inpc->next = inpc->next->next;
                delete CSelectedNPC;
                CSelectedNPC = NULL;
                RenderNPCInfo();
                break;
            }
        }
    }
    }

}

}
void Interface::handle_logics(){
int i;
FreeChunk *p;
for( i = 0; i < 6; i++ )
    if( LightMap_Changed[i] )
    {
        LightMap_Changed[i] = false;
        if( sDayNightCicle[i] != NULL )
        {
            SDL_FreeSurface( sDayNightCicle[i] );
        }
        sDayNightCicle[i] = Pixels.RenderLightMap( i );
    }

    if( SDL_GetTicks() - tAnime > 250 )
    {
        tAnime = SDL_GetTicks();
        for( i = 0; i < max_INTER;i++  )
        {
            chunk_INTER[i].cframe ++;
            if( chunk_INTER[i].cframe >= chunk_INTER[i].frames )
            {
                chunk_INTER[i].cframe = 0;
            }
        }
        for( p = FirstFreeChunk_back->next ; p != LastFreeChunk_back; p = p->next )
        if( p->animated )
        {
            p->cframe++;
            if( p->cframe >= p->frames )
            p->cframe = 0;
        }
        for( p = FirstFreeChunk_fore->next ; p != LastFreeChunk_fore; p = p->next )
        if( p->animated )
        {
            p->cframe++;
            if( p->cframe >= p->frames )
            p->cframe = 0;
        }
    }

if( Window_Inter_RenderVal )
{
    char NrAux[20];

    for( InterSpot *z = FirstItr -> next; z != LastItr; z = z -> next )
    {
        if( Window_Inter_MouseX > z->x  &&
            Window_Inter_MouseX < z->x + z->w  &&
            Window_Inter_MouseY > z->y  &&
            Window_Inter_MouseY < z->y + z->h  )
        {
            cInterChunk = z;
            break;
        }
    }

    for( int u = 0; u < 5; u++ )
    {
        if( sWindow_Inter_Data[u] != NULL )
        {
            SDL_FreeSurface( sWindow_Inter_Data[u] );
            sWindow_Inter_Data[u] = NULL;
        }

        if( sWindow_Inter_Text[u] != NULL )
        {
            SDL_FreeSurface( sWindow_Inter_Text[u] );
            sWindow_Inter_Text[u] = NULL;
        }

    }

    if( cInterChunk != NULL )
    switch( cInterChunk->type )
    {

        //Custom
        case 0:
        sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
        sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Val 0:",BLUE );
        sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"Val 1:",BLUE );
        sWindow_Inter_Text[3] = TTF_RenderText_Solid( font14,"Val 2:",BLUE );
        itoa(cInterChunk ->id,NrAux,10);
        sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[0] ,NrAux,10);
        sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[1],NrAux,10);
        sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[2],NrAux,10);
        sWindow_Inter_Data[3] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        break;
        //Door
        case 1:
        sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
        sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Locked:",BLUE );
        sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"To X:",BLUE );
        sWindow_Inter_Text[3] = TTF_RenderText_Solid( font14,"To Y:",BLUE );
        sWindow_Inter_Text[4] = TTF_RenderText_Solid( font14,"To Zone:",BLUE );
        itoa(cInterChunk ->id,NrAux,10);
        sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[0] ,NrAux,10);
        sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[1],NrAux,10);
        sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[2],NrAux,10);
        sWindow_Inter_Data[3] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[3],NrAux,10);
        sWindow_Inter_Data[4] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        break;
        //TEXTS ABOVE HERO HEAD
        case 2:
        sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"ID:",BLUE );
        sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"Complexe(0/1):",BLUE );
        sWindow_Inter_Text[2] = TTF_RenderText_Solid( font14,"File(nr):",BLUE );
        itoa(cInterChunk ->id,NrAux,10);
        sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[0] ,NrAux,10);
        sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[1],NrAux,10);
        sWindow_Inter_Data[2] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        break;
        //LOOT
        case 3:
        sWindow_Inter_Text[0] = TTF_RenderText_Solid( font14,"id(AOU ATM):",BLUE );
        sWindow_Inter_Text[1] = TTF_RenderText_Solid( font14,"File(nr):",BLUE );
        itoa(cInterChunk ->id,NrAux,10);
        sWindow_Inter_Data[0] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        itoa(cInterChunk->info[0],NrAux,10);
        sWindow_Inter_Data[1] = TTF_RenderText_Solid( font14,NrAux,BLUE );
        break;
    }
}

}
void Interface::RenderNPCInfo(){
stringstream ssaux;
if( Window_NPCMAIN_name != NULL )
{
    SDL_FreeSurface( Window_NPCMAIN_name );
    Window_NPCMAIN_name = NULL;
}
if( Window_NPCMAIN_relationship != NULL )
{
    SDL_FreeSurface( Window_NPCMAIN_relationship );
    Window_NPCMAIN_relationship = NULL;
}
if( Window_NPCMAIN_faction != NULL )
{
    SDL_FreeSurface( Window_NPCMAIN_faction );
    Window_NPCMAIN_faction = NULL;
}
if( Window_NPCMAIN_behavior != NULL )
{
    SDL_FreeSurface( Window_NPCMAIN_behavior );
    Window_NPCMAIN_behavior = NULL;
}
if( Window_NPCMAIN_talk )
{
    SDL_FreeSurface( Window_NPCMAIN_talk );
    Window_NPCMAIN_talk = NULL;
}
if( Window_NPCMAIN_filenr != NULL )
{
    SDL_FreeSurface( Window_NPCMAIN_filenr );
    Window_NPCMAIN_filenr = NULL;
}

if( CSelectedNPC != NULL )
{
Window_NPCMAIN_name = TTF_RenderText_Solid( font14,CSelectedNPC->Name.c_str() ,BLACK );

if(  CSelectedNPC->Relationship == 1 )
Window_NPCMAIN_relationship = TTF_RenderText_Solid( font14, "All Friendly" ,BLACK );
else if( CSelectedNPC->Relationship == 2 )
Window_NPCMAIN_relationship = TTF_RenderText_Solid( font14, "All Enemy" ,BLACK );
else if( CSelectedNPC->Relationship == 3 )
Window_NPCMAIN_relationship = TTF_RenderText_Solid( font14, "Faction" ,BLACK );


Window_NPCMAIN_faction = TTF_RenderText_Solid( font14, CSelectedNPC->Faction.c_str() ,BLACK );

Window_NPCMAIN_behavior = TTF_RenderText_Solid( font14, CSelectedNPC->Behavior.c_str() ,BLACK );

if( CSelectedNPC->CanTalk )
Window_NPCMAIN_talk = TTF_RenderText_Solid( font14, "YES" ,BLACK );
else
Window_NPCMAIN_talk = TTF_RenderText_Solid( font14, "NO" ,BLACK );


ssaux << CSelectedNPC->filenr;
Window_NPCMAIN_filenr = TTF_RenderText_Solid( font14, ssaux.str().c_str() ,BLACK );
}

}
void Interface::showNormaMap(){
int i,j;
FreeChunk *p;
ColideChunk *q;
InterSpot *z;
SDL_Rect  rShowAnime;
rShowAnime.y = 0;

if( DEFINE_MAP )
{
    for( i = 0; i < 100; i++ )
        for( j = 0; j < 100;j ++ )
        {
            apply_Surface(280+BLOCK_WIDTH*j + iMoveMap_x ,50+BLOCK_HEIGHT*i + iMoveMap_y ,NormalMap[i][j].skin,NULL);//&box_ROTATE[NormalMap[i][j].orient]);
        }
}
else //Render Layers
{
    for( i = 0; i < maxx; i++ )
        for( j = 0; j < maxy;j ++ )
        {
            if( NormalMap[i][j].skin != NULL && NormalMap[i][j].skin->w == BLOCK_WIDTH && NormalMap[i][j].skin->h == BLOCK_HEIGHT)
            apply_Surface(280+BLOCK_WIDTH*j + iMoveMap_x ,50+BLOCK_HEIGHT*i  + iMoveMap_y,NormalMap[i][j].skin,NULL);//&box_ROTATE[NormalMap[i][j].orient]
            else
            apply_Surface(280+BLOCK_WIDTH*j + iMoveMap_x ,50+BLOCK_HEIGHT*i - ( NormalMap[i][j].skin->h - 50 ) + iMoveMap_y,NormalMap[i][j].skin,NULL);
        }
            //Render Free BG(mode)
            for( p = FirstFreeChunk_back -> next ; p != LastFreeChunk_back; p = p->next )
            if( p -> skin != NULL )
            {
                if( p->animated )
                {
                    rShowAnime.x = p->cframe*( p->skin->w/p->frames );
                    rShowAnime.w = p->skin->w/p->frames;
                    rShowAnime.h = p->skin->h;
                    apply_Surface(p -> x + iMoveMap_x + 280,iMoveMap_y + p -> y + 50,p -> skin, &rShowAnime);
                }
                else
                apply_Surface(p -> x + iMoveMap_x + 280,iMoveMap_y + p -> y + 50,p -> skin, NULL);
            }
            //Render NPCs
            for( NPCchunk *inpc = FirstNPCMap->next; inpc != LastNPCMap; inpc = inpc->next )
            {
                rShowAnime.x = 0;
                rShowAnime.w = inpc->w;
                rShowAnime.h = inpc->h;
                apply_Surface( inpc->x + 280 + iMoveMap_x, inpc->y + 50 + iMoveMap_y, inpc->skin,&rShowAnime );
            }

            //Render Free Dynamic(mode)
            for( p = FirstFreeChunk_dynam -> next ; p ->next != NULL && p != NULL; p = p->next )
            if( p -> skin != NULL )
            {
                if( p->animated )
                {
                    rShowAnime.x = p->cframe*( p->skin->w/p->frames );
                    rShowAnime.w = p->skin->w/p->frames;
                    rShowAnime.h = p->skin->h;
                    apply_Surface(p -> x + iMoveMap_x + 280,iMoveMap_y + p -> y + 50,p -> skin, &rShowAnime);
                }
                else
                apply_Surface(p -> x + iMoveMap_x + 280,iMoveMap_y + p -> y + 50,p -> skin, NULL);
            }

            //Render Free FG(mode)
            for( p = FirstFreeChunk_fore -> next ; p ->next != NULL && p != NULL; p = p->next )
            if( p -> skin != NULL )
            {
                if( p->animated )
                {
                    rShowAnime.x = p->cframe*( p->skin->w/p->frames );
                    rShowAnime.w = p->skin->w/p->frames;
                    rShowAnime.h = p->skin->h;
                    apply_Surface(p -> x + iMoveMap_x + 280,iMoveMap_y + p -> y + 50,p -> skin, &rShowAnime);
                }
                else
                apply_Surface(p -> x + iMoveMap_x + 280,iMoveMap_y + p -> y + 50,p -> skin, NULL);
            }

            //Render Day & Night
            if( cmode == 3 )
            {
                apply_Surface( 280 + iMoveMap_x, 50 + iMoveMap_y, sDayNightCicle[ cDayTime -1  ] , NULL );
                if( CDNChunk != NULL )//&& CDNChunk -> nr == 1 )
                {
                    apply_Surface( CDNChunk ->x + 280 + iMoveMap_x - 5, CDNChunk ->y + 50 + iMoveMap_y - 5, RedSquare , NULL  );
                    apply_Surface( CDNChunk ->x + 280 + iMoveMap_x - CDNChunk -> range - 5, CDNChunk ->y + 50 + iMoveMap_y - 5, YellowSquare , NULL  );
                }
            }
            else if( cmode == 4 )//Render colliders
            {
                //de add for loop q
                for( q = FirstCOLChunk; q != LastCOLChunk;q = q ->next )
                apply_Surface( q -> x + iMoveMap_x + 280 - q -> w/2 , q -> y + iMoveMap_y + 50 - q -> h/2,q -> skin , NULL );
                if( CCOLChunk != NULL )
                {
                    apply_Surface( CCOLChunk ->x + 280 + iMoveMap_x - 5 , CCOLChunk ->y + 50 + iMoveMap_y - 5, RedSquare , NULL  );
                    apply_Surface( CCOLChunk ->x + 280 + iMoveMap_x - 5 , CCOLChunk ->y + 50 + iMoveMap_y - 5 - CCOLChunk ->h/2 , YellowSquare , NULL  );
                    apply_Surface( CCOLChunk ->x + 280 + iMoveMap_x - 5 + CCOLChunk ->w/2, CCOLChunk ->y + 50 + iMoveMap_y - 5, YellowSquare , NULL  );
                    apply_Surface( CCOLChunk -> g1x - 5 + 280 + iMoveMap_x, CCOLChunk -> g1y - 5 + 50 + iMoveMap_y, GreenSquare , NULL  );
                    apply_Surface( CCOLChunk -> g2x - 5 + 280 + iMoveMap_x, CCOLChunk -> g2y - 5 + 50 + iMoveMap_y, GreenSquare , NULL  );
                }
            }
            else if( cmode == 5 )//Inter Spots
            {
                for( z = FirstItr -> next; z != LastItr; z = z -> next )
                {
                    apply_Surface( z -> x + 280 + iMoveMap_x, z -> y + 50 + iMoveMap_y, z -> skin, NULL );
                }
                if( cInterChunk != NULL )
                {
                    apply_Surface( cInterChunk -> x + 280 + iMoveMap_x + cInterChunk -> w/2 - 5,cInterChunk -> y + 50 + iMoveMap_y + cInterChunk -> h/2 - 5, RedSquare, NULL  );
                    apply_Surface( cInterChunk -> x + 280 + iMoveMap_x - 5,cInterChunk -> y + 50 + iMoveMap_y - 5, GreenSquare, NULL  );
                    apply_Surface( cInterChunk -> x + 280 + iMoveMap_x + cInterChunk -> w - 5,cInterChunk -> y + 50 + iMoveMap_y + cInterChunk -> h - 5, GreenSquare, NULL  );
                }
            }
}

if( ShowCChunk && (cmode == 1 || cmode == 2 || cmode == 7 || cmode == 6 ) )
apply_Surface( CurrentChunk_x, CurrentChunk_y,sCurrentChunk ,NULL );

if(( C_FreeChunk->nr == chunk_INTER[0].nr && FreeCTarget_Move != NULL && (cmode == 1 || cmode == 2 || cmode == 7 ) ) )
apply_Surface( FreeCTarget_Move->x + 280 + iMoveMap_x , FreeCTarget_Move->y + 50 + iMoveMap_y ,sSelectedChunk ,NULL );
else if( CMouseNPC == FirstNPCList->next && CSelectedNPC != NULL  &&  cmode == 6 )
apply_Surface( CSelectedNPC->x + 280 + iMoveMap_x , CSelectedNPC->y + 50 + iMoveMap_y ,sSelectedChunk ,NULL );

if( ShowGrid )
apply_Surface( 280 + iMoveMap_x, 50 + iMoveMap_y,sGrid ,NULL );

if( ShowMousePos )
{
    apply_Surface( m_x+10,m_y,sMousePoz,NULL );

    if( m_y - 50 > iMoveMap_y &&  m_y - 50 < iMoveMap_y + maxx*BLOCK_HEIGHT )
    apply_Surface( 280 + iMoveMap_x,m_y, sLine1, NULL );

    if( m_x - 280 > iMoveMap_x &&  m_x - 280 < iMoveMap_x + maxy*BLOCK_WIDTH )
    apply_Surface( m_x, 50 + iMoveMap_y, sLine2, NULL );
}

if( Window_Inter_show )
{
    apply_Surface( Window_Inter_x,Window_Inter_y, sWindow_Inter_bg ,NULL );
    if( cInterChunk -> type == 0 )
    apply_Surface( Window_Inter_x + 45,Window_Inter_y + 45, sWindow_InterButton_Custom ,NULL );
    else if( cInterChunk -> type == 1 )
    apply_Surface( Window_Inter_x + 45,Window_Inter_y + 45, sWindow_InterButton_Door ,NULL );
    else if( cInterChunk -> type == 2 )
    apply_Surface( Window_Inter_x + 45,Window_Inter_y + 45, sWindow_InterButton_TextAboveHead ,NULL );
    else if( cInterChunk -> type == 3 )
    apply_Surface( Window_Inter_x + 45,Window_Inter_y + 45, sWindow_InterButton_Loot ,NULL );

    for( i = 0; i < 5; i++ )
    if( sWindow_Inter_Text[i] != NULL )
    {
        apply_Surface( Window_Inter_x + 5, Window_Inter_y + Window_InterRect_Button[i].y, sWindow_Inter_Text[i] ,NULL );
        apply_Surface( Window_Inter_x + sWindow_Inter_Text[i] -> w + 10, Window_Inter_y + Window_InterRect_Button[i].y, sWindow_Inter_Data[i] ,NULL );
    }

}

}
void Interface::showMouse(){
SDL_Rect rShowAnime;
switch( clib )
{
    case 0:
    if( chunk_C.skin != NULL )//chunk_C.nr != chunk_NULL.nr &&
    {
        apply_Surface(m_x,m_y,chunk_C.skin,NULL );
    }
    break;
    case 1:
    if( C_FreeChunk ->nr != NULL_FreeChunk ->nr && C_FreeChunk -> skin != NULL )
    {
        if( C_FreeChunk->animated )
        {
            rShowAnime.x = C_FreeChunk->cframe*( C_FreeChunk->skin->w/C_FreeChunk->frames );
            rShowAnime.w = C_FreeChunk->skin->w/C_FreeChunk->frames;
            rShowAnime.h = C_FreeChunk->skin->h;
            apply_Surface(m_x,m_y,C_FreeChunk -> skin,&rShowAnime );
        }
        else
        apply_Surface(m_x,m_y,C_FreeChunk -> skin,NULL );
    }
    break;
    case 5:
    if( CMouseNPC != NULL && CMouseNPC->skin != NULL )
    {
        rShowAnime.x = 0;
        rShowAnime.y = 0;
        rShowAnime.w = CMouseNPC->w;
        rShowAnime.h = CMouseNPC->h;
        apply_Surface(m_x,m_y,CMouseNPC->skin,&rShowAnime );
    }
    break;
}
}
void Interface::SaveMap(){
int i,j;
stringstream ssaux;
FreeChunk *p;
DayNightChunk *q;
ColideChunk *r;
InterSpot *t;
NPCchunk *inpc;
ofstream file;

ssaux.str("");
ssaux << "maps//map"<<nr4<<".mp";
file.open( ssaux.str().c_str() );

file << "New_Map\n";
file << maxy <<" "<< maxx<<'\n';

//Matricea elemente
for( i = 0;i < maxx;i++ )
{
    file <<'\n';
    for( j = 0;j < maxy;j++ )
    {
        if( NormalMap[i][j].nr < 10 )
        file <<"  ";
        file << NormalMap[i][j].nr <<" ";
    }
}

file << "\n\n";

//Mat orient
for( i = 0;i < maxx;i++ )
{
    file <<'\n';
    for( j = 0;j < maxy;j++ )
    {
        if( NormalMap[i][j].orient < 10 )
        file <<"  ";
        file << NormalMap[i][j].orient <<" ";
    }
}

file << "\n\n";


file << "OPEN_BG\n\n";
for( p = FirstFreeChunk_back -> next; p != NULL && p -> next != NULL;p = p -> next )
{
    file <<"Add: " << "nr: " << p ->nr << '\n';

    file << "x: " << p->x << '\n';
    file << "y: " << p->y << '\n';
    file << "orient: " << p->orient << '\n';
    file << "animated: " << p->animated << '\n';
    file << "frames: " << p->frames << '\n';

    cout << '\n';
}
file << "CLOSE_BG\n\n";

file << "OPEN_FG\n\n";
for( p = FirstFreeChunk_fore -> next; p != NULL && p -> next != NULL;p = p -> next )
{
    file <<"Add: " << "nr: " << p ->nr << '\n';

    file << "x: " << p ->x << '\n';
    file << "y: " << p ->y << '\n';
    file << "orient: " << p -> orient << '\n';
    file << "animated: " << p -> animated << '\n';
    file << "frames: " << p -> frames << '\n';

    file << '\n';
}
file << "CLOSE_FG\n\n";

file << "OPEN_DYNAMIC\n\n";
for( p = FirstFreeChunk_dynam -> next; p != NULL && p -> next != NULL;p = p -> next )
{
    file <<"Add: " << "nr: " << p ->nr << '\n';

    file << "x: " << p ->x << '\n';
    file << "y: " << p ->y << '\n';
    file << "orient: " << p -> orient << '\n';
    file << "animated: " << p -> animated << '\n';
    file << "frames: " << p -> frames << '\n';

    file << '\n';
}
file << "CLOSE_DYNAMIC\n\n";


file << "OPEN_LIGHT\n\n";
for( q = FirstDNChunk -> next; q != NULL && q -> next != NULL; q = q ->next )
{
    file << "Add: \n" << "nr: " << q -> nr << '\n';
    file << "x: " << q -> x << '\n';
    file << "y: " << q -> y << '\n';
    file << "range: " << q -> range << '\n';
    file << '\n';
}
file << "CLOSE_LIGHT\n\n";

file << "OPEN_COLIDER\n\n";
for( r = FirstCOLChunk -> next; r != NULL && r -> next != NULL; r = r ->next )
{
    file << "Add: \n";
    file << "id: "<<r  -> id << '\n';
    file << "x: " << r -> x << '\n';
    file << "y: " << r -> y << '\n';
    file << "w: " << r -> w << '\n';
    file << "h: " << r -> h << '\n';
    file << '\n';
}
file << "CLOSE_COLIDER\n\n";

file << "OPEN_INTERACTION\n\n";
for( t = FirstItr -> next; t != LastItr ; t = t ->next )
{
    file << "Add: \n";
    file << "x: " << t -> x << '\n';
    file << "y: " << t -> y << '\n';
    file << "w: " << t -> w << '\n';
    file << "h: " << t -> h << '\n';
    file << "type: " << t -> type << '\n';
    file << "ID: " << t->id << '\n';
    file << "info_1: " << t -> info[0] << '\n';
    file << "info_2: " << t -> info[1] << '\n';
    file << "info_3: " << t -> info[2] << '\n';
    file << "info_4: " << t -> info[3] << '\n';
    file << '\n';

}
file << "CLOSE_INTERACTION\n\n";

file << "OPEN_NOTPLAYERCREATURE\n";
for( inpc = FirstNPCMap->next;inpc != LastNPCMap; inpc = inpc->next )
{
    file << "Add:\n";
    file << "Nume: " << inpc->Name<<'\n';
    file << "x: " << inpc->x<<'\n';
    file << "y: " << inpc->y<<'\n';
    file << "w: " << inpc->w<<'\n';
    file << "h: " << inpc->h<<'\n';
    file << "skinnr: " << inpc->skinnr<<'\n';
    file << "filenr: " << inpc->filenr << '\n';
}

file << "CLOSE_NOTPLAYERCREATURE\n";

file<<"file_end";
file.close();
}
void Interface::LoadMap(){
stringstream ssaux;
string saux;
int i,j,info[9];
char caux[30];
FreeChunk *TempFreeChunk = new FreeChunk;
NPCchunk *modelnpcchunk = new NPCchunk;
ifstream file;

//Cleanup pt vechea harta
MapCleanup();

ssaux.str("");
ssaux << "maps//map" << nr4 << ".mp";
file.open( ssaux.str().c_str() );

if( file.is_open() )
{

file >> saux;

file >> maxy >> maxx;

//Matricea elemente
for( i = 0;i < maxx;i++ )
    for( j = 0;j < maxy;j++ )
    {
        file >> NormalMap[i][j].nr;


        if( BuildSurf[ NormalMap[i][j].nr ][0] == NULL && NormalMap[i][j].nr != -1 )
        {
            ssaux.str("");
            ssaux<<"textures//BUILDING//build//";
            ssaux<<NormalMap[i][j].nr<<".png";
            BuildSurf[ NormalMap[i][j].nr ][0] = load_image( ssaux.str() );
        }

            //NormalMap[i][j].skin = BuildSurf[ NormalMap[i][j].nr ][0];

    }


//Mat orient
for( i = 0;i < maxx;i++ )
for( j = 0;j < maxy;j++ )
{
    file >> NormalMap[i][j].orient;

    if( NormalMap[i][j].nr != -1 )
    {
        if( BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] != NULL )
        NormalMap[i][j].skin = BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ];
        else
        {
            switch( NormalMap[i][j].orient ) //chose / render skin
            {
                case 1:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.rotate_img( BuildSurf[ NormalMap[i][j].nr ][0],ROTATE_RIGHT,0,0 ) ;

                break;
                case 2:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.flip_img( BuildSurf[ NormalMap[i][j].nr ][0], FLIP_HOR ) ;

                break;
                case 3:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.rotate_img( BuildSurf[ NormalMap[i][j].nr ][0], ROTATE_LEFT,0,0 ) ;

                break;
                case 4:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.RenderCorner( BuildSurf[ NormalMap[i][j].nr ][0] );

                break;
                case 5:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.rotate_img( Pixels.RenderCorner( BuildSurf[ NormalMap[i][j].nr ][0] ),
                                          ROTATE_RIGHT,chunk_C.animated,chunk_C.frames ) ;
                break;
                case 6:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.rotate_img( Pixels.rotate_img( Pixels.RenderCorner( BuildSurf[ NormalMap[i][j].nr ][0] ),
                                          ROTATE_LEFT,chunk_C.animated,chunk_C.frames ),
                                          ROTATE_LEFT,chunk_C.animated,chunk_C.frames )  ;
                break;
                case 7:

                BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] =
                Pixels.rotate_img( Pixels.RenderCorner( BuildSurf[ NormalMap[i][j].nr ][0] ),
                                          ROTATE_LEFT,chunk_C.animated,chunk_C.frames ) ;
                break;
                }
            if( BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ] != NULL )
            NormalMap[i][j].skin = BuildSurf[ NormalMap[i][j].nr ][ NormalMap[i][j].orient ];
            else
            NormalMap[i][j].skin = sNormalNULL;
        }
    }
    else
    NormalMap[i][j].skin = sNormalNULL;

}

file >> saux;
file >> saux;
while( saux != "CLOSE_BG" )
{
    file >> saux;
    file >> TempFreeChunk ->nr;

    file >> saux >> TempFreeChunk ->x;
    file >> saux >> TempFreeChunk ->y;
    file >> saux >> TempFreeChunk ->orient;
    file >> saux >> TempFreeChunk ->animated;
    file >> saux >> TempFreeChunk ->frames;

    for( i = 0; i < max_INTER; i++ )
    if( chunk_INTER[i].nr == TempFreeChunk ->nr )
    {
        TempFreeChunk ->skin = chunk_INTER[i].skin;
        break;
    }
    else if( i == max_INTER -1 )
    {
        TempFreeChunk ->skin = sNormalNULL;
    }

    AddChunk(TempFreeChunk,TempFreeChunk ->x,TempFreeChunk ->y,FreeBG);

    file >> saux;
}


file >> saux;
file >> saux;
while( saux != "CLOSE_FG" )
{
    file >> saux;
    file >> TempFreeChunk ->nr;

    file >> saux >> TempFreeChunk ->x;
    file >> saux >> TempFreeChunk ->y;
    file >> saux >> TempFreeChunk ->orient;
    file >> saux >> TempFreeChunk ->animated;
    file >> saux >> TempFreeChunk ->frames;

    for( i = 0; i < max_INTER; i++ )
    if( chunk_INTER[i].nr == TempFreeChunk ->nr )
    {
        TempFreeChunk ->skin = chunk_INTER[i].skin;
        break;
    }
    else if( i == max_INTER -1 )
    {
        TempFreeChunk ->skin = sNormalNULL;
    }

    AddChunk(TempFreeChunk,TempFreeChunk ->x,TempFreeChunk ->y,FreeFG);

    file >> saux;
}

file >> saux;
file >> saux;
while( saux != "CLOSE_DYNAMIC" )
{
    file >> saux;
    file >> TempFreeChunk ->nr;

    file >> saux >> TempFreeChunk ->x;
    file >> saux >> TempFreeChunk ->y;
    file >> saux >> TempFreeChunk ->orient;
    file >> saux >> TempFreeChunk ->animated;
    file >> saux >> TempFreeChunk ->frames;

    for( i = 0; i < max_INTER; i++ )
    if( chunk_INTER[i].nr == TempFreeChunk ->nr )
    {
        TempFreeChunk ->skin = chunk_INTER[i].skin;
        break;
    }
    else if( i == max_INTER -1 )
    {
        TempFreeChunk ->skin = sNormalNULL;
    }

    AddChunk(TempFreeChunk,TempFreeChunk ->x,TempFreeChunk ->y,FreeDynam );

    file >> saux;
}


file >> saux;
file >> saux;

while( saux != "CLOSE_LIGHT" )
{
    file >> saux >> info[0];
    file >> saux >> info[1];
    file >> saux >> info[2];
    file >> saux >> info[3];
    AddLight(info[1],info[2],info[0],info[3]);
    file >> saux;

}
LightMap_Changed[0] = true;
LightMap_Changed[1] = true;
LightMap_Changed[2] = true;
LightMap_Changed[3] = true;
LightMap_Changed[4] = true;
LightMap_Changed[5] = true;


file >> saux;
file >> saux;

while( saux != "CLOSE_COLIDER" )
{
    static int id;
    file >> saux >> id;
    file >> saux >> info[0];
    file >> saux >> info[1];
    file >> saux >> info[2];
    file >> saux >> info[3];
    AddColider( info[0],info[1],info[2],info[3],id );
    file >> saux;
}

file >> saux;
file >> saux;

while( saux != "CLOSE_INTERACTION" )
{
    int idaux = 0;
    file >> saux >> info[0];
    file >> saux >> info[1];
    file >> saux >> info[2];
    file >> saux >> info[3];
    file >> saux >> info[4];//type
    file >> saux >> idaux;
    file >> saux >> info[5];
    file >> saux >> info[6];
    file >> saux >> info[7];
    file >> saux >> info[8];

    cInterChunk = AddInterSpot( info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],idaux );

    file >> saux;
}

file >> saux;
file >> saux;

while( saux != "CLOSE_NOTPLAYERCREATURE" )
{
file >> saux;

file.getline( caux,30,'\n' );
ctos( caux,modelnpcchunk->Name );

file >> saux >> modelnpcchunk->x;
file >> saux >> modelnpcchunk->y;
file >> saux >> modelnpcchunk->w;
file >> saux >> modelnpcchunk->h;
file >> saux >> modelnpcchunk->skinnr;
modelnpcchunk->skin = NPCSkins[modelnpcchunk->skinnr];

file >> saux >> modelnpcchunk->filenr;
file >> saux;

AddNPCchunk(modelnpcchunk);
}


}

file.close();
if( sGrid != NULL )
SDL_FreeSurface( sGrid );
sGrid = Pixels.RenderGrid();

delete TempFreeChunk;
delete modelnpcchunk;


//Render lines
//if( ShowMousePos )<<!>>
Pixels.RenderMouseLines( sLine1,sLine2 );

}
void Interface::MapCleanup(){
FreeChunk *fc,*fcaux;
DayNightChunk *dn,*dnaux;
ColideChunk *cc,*ccaux;
InterSpot *t,*taux;
NPCchunk *inpc,*npcaux;


for( fc = FirstFreeChunk_back; fc -> next != LastFreeChunk_back; )
{
    fcaux = fc ->next;
    fc -> next = fc ->next -> next;
    delete fcaux;
}
LastChunkBack = FirstFreeChunk_back;

for( fc = FirstFreeChunk_fore; fc -> next != LastFreeChunk_fore; )
{
    fcaux = fc ->next;
    fc -> next = fc ->next -> next;
    delete fcaux;
}
LastChunkFore = FirstFreeChunk_fore;

for( fc = FirstFreeChunk_dynam; fc -> next != LastFreeChunk_dynam; )
{
    fcaux = fc ->next;
    fc -> next = fc ->next -> next;
    delete fcaux;
}
LastChunkDynam = FirstFreeChunk_dynam;

for( dn = FirstDNChunk; dn -> next != LastDNChunk;  )
{
    dnaux = dn ->next;
    dn -> next = dn -> next ->next;
    delete dnaux;
}

for( cc = FirstCOLChunk; cc -> next != LastCOLChunk ;  )
{
    ccaux = cc -> next;
    cc -> next = cc -> next -> next;
    delete ccaux;
}

for( t = FirstItr; t -> next != LastItr; )
{
    taux = t -> next;
    t->next = t->next->next;
    delete taux;
}

for( inpc = FirstNPCMap;inpc->next != LastNPCMap; )
{
    npcaux = inpc->next;
    inpc->next = inpc->next->next;
    delete npcaux;
}

}
void Interface::cleanup(){

MapCleanup();

SDL_FreeSurface( sInterface );
SDL_FreeSurface( sOptions );

}

int main(int argc, char *args[]){
bool end = false;

if( !init() )
return 5;
INTERFACE.init();

SDL_EnableUNICODE( SDL_ENABLE );

while( !end )
{
    //Events
    while( SDL_PollEvent( &event ) )
    {
        if(  event.type == SDL_QUIT || ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) )
        end = true;
        else if( event.type == SDL_KEYDOWN )
        {
            if( event.key.keysym.sym == SDLK_LCTRL )
            bCtrlDown = true;
        }
        else if( event.type == SDL_KEYUP )
        {
            bCtrlDown = false;
        }
        else if( event.type == SDL_MOUSEBUTTONDOWN )
        {
            if( bCtrlDown )
            {
                bMouseDrag = true;
                iInitial_x = event.button.x;
                iInitial_y = event.button.y;
            }

        }
        else if( event.type == SDL_MOUSEBUTTONUP )
        {
            bMouseDrag = false;
        }

        //External events
        INTERFACE.handle_events( event );
    }
//Logics
INTERFACE.handle_logics();
//Render
//SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0));//0xFF
//INTERFACE.showNormaMap();
//INTERFACE.showbg();
INTERFACE.showMouse();

SDL_Flip( screen );
}

void cleanup();

SDL_EnableUNICODE( SDL_DISABLE );

return 0;
}
bool init(){

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

Pixels.init();

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
