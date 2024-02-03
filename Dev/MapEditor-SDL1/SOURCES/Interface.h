#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <cmath>
#include <fstream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "base.h"
#include "Placeable.h"
#include "sstream"

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
};

extern Interface INTERFACE;

#endif // INTERFACE_H_INCLUDED
