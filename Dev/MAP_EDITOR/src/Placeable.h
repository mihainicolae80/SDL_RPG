#ifndef PLACEABLE_H_INCLUDED
#define PLACEABLE_H_INCLUDED

#include <iostream>
#include "SDL2/SDL.h"
#include <list>

using namespace std;


struct CHUNK
{
	SDL_Surface *skin;
	int nr,orient,frames,cframe;
	int nr1,nr2,nr3;//door:zone,tox,toy;enemy:script;
	bool rotate, animated;;
};

struct DECOR
{
	int nr,x,y,orient;
	bool rotate,colide;
};


struct FreeChunk
{
	int x,y,orient,nr,frames,cframe;
	bool animated;
	int info[3];
	SDL_Surface *skin;
	FreeChunk *next;
};


struct DayNightChunk
{
	int nr,x,y,range;
	DayNightChunk *next;
};


struct DayNightTool
{
	int nr;
	SDL_Surface *skin;
};


struct ColideChunk
{
	int w,h,x,y,g1x,g1y,g2x,g2y,id;
	SDL_Surface *skin;
	ColideChunk *next;
};


struct ColideTool
{
	int nr;
	SDL_Surface *skin;
};


struct InterSpot
{
	int x,y,w,h,type;
	int info[4],id;
	SDL_Surface *skin;
	InterSpot *next;
};


struct NPCchunk
{
	SDL_Surface *skin;
	int x, y, w, h, Relationship, filenr, skinnr, nr;
	string Name,Faction,Behavior;
	bool CanTalk;
	NPCchunk *next;
};



extern CHUNK chunk_nullptr,chunk_C;
extern FreeChunk *FirstFCLib,*FirstFreeChunk_fore,*LastFreeChunk_fore,*FirstFreeChunk_back,
*LastFreeChunk_back,*FirstFreeChunk_dynam,*LastFreeChunk_dynam,*C_FreeChunk,
*nullptr_FreeChunk,*FreeCTarget_Move,*LastChunkFore,*LastChunkBack,*LastChunkDynam;
extern DayNightChunk *FirstDNChunk,*LastDNChunk,*CDNChunk;
extern DayNightTool DNTool[2],*CDNTool;
extern ColideChunk *FirstCOLChunk,*LastCOLChunk,*CCOLChunk;
extern ColideTool COLTool[2],*CCOLTool;
extern InterSpot *FirstItr,*LastItr;
//extern NPCchunk *FirstNPCList,*LastNPCList,*FirstNPCMap,*LastNPCMap;
//extern NPCchunk *FirstNPCMap,*LastNPCMap;

extern list<NPCchunk *> npc_list;
extern list<NPCchunk *> npc_map;

#endif // PLACEABLE_H_INCLUDED
