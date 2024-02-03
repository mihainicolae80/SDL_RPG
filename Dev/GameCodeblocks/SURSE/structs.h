#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "SDL2/SDL.h"
#include <iostream>

using namespace std;

extern struct FreeChunk{
int x,y,orient,nr,frames,cframe,w,h;
bool animated;
int info[3];
SDL_Surface *surf;
SDL_Texture *tex;
FreeChunk *next;
}*FirstFreeChunk_back,*LastFreeChunk_back,*FirstFreeChunk_dynam,*LastFreeChunk_dynam,*FirstFreeChunk_fore,*LastFreeChunk_fore;
extern struct Armor{
    char name[40];
    int def,nr,price,type,statsnr;
    SDL_Texture *tex,*skin_player;
    string description;
    struct sts
    {
        int value;
        string type;
    }stats[4];
    bool equipted;
}armors_NULL;
extern struct Shield{
    char name[40];
    int def,protect,nr,price,type,statsnr;
    SDL_Texture *tex,*skin_player;

    string description;
    struct
    {
        int value;
        string type;
    }stats[4];
    bool equipted;
}shield_NULL;
extern struct Weapon{
    char name[40];
    int dmg,sharp,nr,price,itm_class,FullCooldown;
    SDL_Texture *tex;
    string description;
    struct sts
    {
        int value;
        string type;
    }stats[4];
    bool equipted;
}weapons_NULL;
struct BAG{
SDL_Texture *tex;
char name[40];
int nr,skinnr,slots;
string description;
bool equipted;};
struct Misc{
    char name[40];
    int nr,price,quant,maxstack,effect_val;
    bool usable;
    string description,effect_type;
    SDL_Texture *tex;
};
struct Inventory_item{
int type,nr;
SDL_Texture *tex;
Armor  *armor;
Weapon *weapon;
BAG    *bag;
Shield *shield;
Misc   *misc;
Inventory_item *next;
};
extern struct newQuest{
string name,desc[10];
int nr,cprog,goalprog,rew_gold,rew_exp,
    rew_itm_nrof,rew_itm_take,desc_lines ;
Inventory_item *RewItems[10];
}quest_NULL,ACTIVE_QUEST[10];
extern struct GAME_event{
int type,nr,quant;
string name;
}Gevent_NULL,newGEvent;
struct ANIME{
int cfrap,goalfrap,x,y,w,h;
char type;
Uint32 time_when_change ,interval;
SDL_Texture *skin;
bool loaded,loop;
ANIME *next;
};
extern struct LIGHT{
int nr,range,x,y;
LIGHT *next;
}*FirstLight,*LastLight;
struct COLLIDER{
int w,h,x,y,id;
bool ON;
COLLIDER *next;
};
extern COLLIDER *FirstCol,*LastCol;
extern struct InterSpot{
int x,y,w,h,type,id;
int info[4];
InterSpot *next;
}*FirstItr,*LastItr;
extern struct ListEvent{
GAME_event event;
ListEvent *next,*pre;

}*FirstListEvent,*LastListEvent;
struct DIALOG_TOPIC{
string npctext[10],TopicName[4];
int instructions[4],val[4],val2[4],val3[4];//Instr si val sunt pentru topicele urmatoare!
int action_nr[3],action[3],act_info[3],act_id[3],lines_nr;
string action_name[3];
DIALOG_TOPIC *NextTopic[4];
};
extern struct NotPLayerCreature{
string Name;
int filenr;
int anime_categ,anime_frame;
bool WillAttack,Dead,Spawned,boss;
SDL_Rect rBody;
SDL_Rect rFrame;
Uint32 behaviors,tAttack;
SDL_Surface *skin;
SDL_Texture *tex;
int ColidedWith_w,ColidedWith_h,
    ColidedWith_x,ColidedWith_y;

DIALOG_TOPIC *Dialog_maintopic;

///Comabat stuff
int health,maxhealth,attackstyle,dmg;
bool comb;
Uint32 tMoveSpeed,tLastMove,tMove;
double attackspeed;
SDL_Rect Body;


NotPLayerCreature *next;
}*FirstNPC,*LastNPC;
extern struct NPCList{
NotPLayerCreature *npc;
NPCList *next;
}*FirstEnemy,*LastEnemy;


#endif // STRUCTS_H_INCLUDED
