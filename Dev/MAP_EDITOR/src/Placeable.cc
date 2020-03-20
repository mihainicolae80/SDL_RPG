#include "Placeable.h"




CHUNK chunk_nullptr,chunk_C;

FreeChunk *FirstFCLib = nullptr,*FirstFreeChunk_fore = nullptr,*LastFreeChunk_fore = nullptr,*FirstFreeChunk_back= nullptr,
*LastFreeChunk_back = nullptr ,*FirstFreeChunk_dynam = nullptr,*LastFreeChunk_dynam = nullptr ,*C_FreeChunk = nullptr,
*nullptr_FreeChunk = nullptr,*FreeCTarget_Move = nullptr ,*LastChunkFore= nullptr ,*LastChunkBack = nullptr,*LastChunkDynam= nullptr;

DayNightChunk *FirstDNChunk = nullptr ,*LastDNChunk= nullptr,*CDNChunk= nullptr;
DayNightTool DNTool[2],*CDNTool;

ColideChunk *FirstCOLChunk = nullptr,*LastCOLChunk = nullptr,*CCOLChunk = nullptr;

ColideTool COLTool[2],*CCOLTool;
InterSpot *FirstItr,*LastItr;


list<NPCchunk *> npc_list;
list<NPCchunk *> npc_map;
