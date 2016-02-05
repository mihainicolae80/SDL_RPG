#include "Placeable.h"




CHUNK chunk_NULL,chunk_C;

FreeChunk *FirstFCLib,*FirstFreeChunk_fore,*LastFreeChunk_fore,*FirstFreeChunk_back,
*LastFreeChunk_back,*FirstFreeChunk_dynam,*LastFreeChunk_dynam,*C_FreeChunk,
*NULL_FreeChunk,*FreeCTarget_Move,*LastChunkFore,*LastChunkBack,*LastChunkDynam;

DayNightChunk *FirstDNChunk,*LastDNChunk,*CDNChunk;
DayNightTool DNTool[2],*CDNTool;

ColideChunk *FirstCOLChunk = NULL,*LastCOLChunk = NULL,*CCOLChunk = NULL;

ColideTool COLTool[2],*CCOLTool;
InterSpot *FirstItr,*LastItr;
NPCchunk *FirstNPCList,*LastNPCList,*FirstNPCMap,*LastNPCMap;
