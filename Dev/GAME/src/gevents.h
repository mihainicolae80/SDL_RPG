#ifndef GEVENTS_H_INCLUDED
#define GEVENTS_H_INCLUDED

#include "structs.h"
#include "main_headers.h"
#include "quests.h"
#include "scripts.h"
#include "interface.h"

extern class GEvents{
private:
ListEvent *FirstListEvent,*LastListEvent;
public:
void init();
void add( GAME_event tempGEvent );
void add( const int cEvent,int nr = -999 );
int get( GAME_event &LEvent);
}GAME_EVENTS;

void handle_GAMEevents();

#endif // GEVENTS_H_INCLUDED
