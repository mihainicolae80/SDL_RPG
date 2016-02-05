#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED



#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "cmath"
#include "Placeable.h"
#include "PixelManipulation.h"



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

extern SDL_Surface *screen,*NULL_img;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern const int BLOCK_WIDTH;
extern const int BLOCK_HEIGHT;
extern const int SCREEN_BPP;
extern const int MAXNRMAP;


extern int cDayTime,iMoveMap_x,iMoveMap_y,iInitial_x,iInitial_y,FCMove_x,FCMove_y,maxx,maxy,cInterItm,InterInfo[3];
extern int m_x,m_y;
extern bool bCtrlDown,bMouseDrag, bCSkinOrigin,FCmoveing,LightMap_Changed[6],LibLock[3];

extern TTF_Font *font14,*font;
extern SDL_Event event;
extern bool DEFINE_MAP;
extern SDL_Color BLUE ;
extern SDL_Color RED;
extern SDL_Color BYellow;
extern SDL_Color BLACK;

void ctos(char carray[],string &str );


bool Init();
void AddChunk( FreeChunk *modelchunk,int x, int y,int list );
DayNightChunk *AddLight( int x, int y,int nr, int range );
ColideChunk *AddColider( int x,int y, int w,int h,int id );
InterSpot *AddInterSpot( int x,int y ,int w,int h,int type,int i1,int i2,int i3,int i4,int id);
InterSpot *AddInterSpot( int x,int y );
NPCchunk *AddNPCchunk( NPCchunk *modelchunk );
SDL_Surface *load_image(string filename);
void apply_Surface(int x,int y,SDL_Surface *source,SDL_Rect *clip);
void cleanup();

#endif // BASE_H_INCLUDED
