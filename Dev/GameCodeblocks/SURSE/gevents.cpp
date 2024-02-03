#include "gevents.h"



void GEvents::init(){
FirstListEvent = new ListEvent;
LastListEvent = new ListEvent;
FirstListEvent->next = LastListEvent;
FirstListEvent->pre = NULL;
LastListEvent->next = NULL;
LastListEvent->pre = FirstListEvent;
}
void GEvents::add( GAME_event tempGEvent ){//Add event via an event variable
ListEvent *newle = new ListEvent;
newle->event = tempGEvent;
LastListEvent->pre->next = newle;
newle->pre = LastListEvent->pre;
newle->next = LastListEvent;
LastListEvent->pre = newle;
}
void GEvents::add( const int cEvent,int nr ){//Add event via a constant
ListEvent *newle = new ListEvent;
newle->event.type = cEvent;
newle->event.nr   = nr;
LastListEvent->pre->next = newle;
newle->pre = LastListEvent->pre;
newle->next = LastListEvent;
LastListEvent->pre = newle;
}
int GEvents::get( GAME_event &LEvent ){
ListEvent *delaux;

if( FirstListEvent -> next == LastListEvent )
return 0;

delaux = FirstListEvent -> next;
FirstListEvent->next = FirstListEvent->next->next;
FirstListEvent->next->pre = FirstListEvent;

LEvent = delaux->event;
delete delaux;

return 1;
}

void handle_GAMEevents(){
GAME_event Gevent;

while( GAME_EVENTS.get( Gevent ) )
{
    if( Gevent.type == Event_EnemyDefeated )
    {
        if( Gevent.nr )
        {
            if( Gevent.nr == 1 ) //Daca mobul este cel cu nr 1
            {
                QUEST.advance( 1 );//avanseaza questul 1
            }
            else if( Gevent.nr == 101 )
                QUEST.advance( 4 );
        }

        if( Gevent.name != "" )
        {
            if( Gevent.name == "Injured Goblin" )
            {
                QUEST.advance( 1 );
            }
        }

    }
    else if( Gevent.type == Event_RequestMapLoad )
    {
        // !!! DISSABLED W?O CHECKING !!!
        //GAME_MAP.load();
    }
    else if( Gevent.type == Event_RequestAddQuest )
    {
        QUEST.add( Gevent.nr );
    }
    else if( Gevent.type == Event_RequestAdvanceQuest )
    {
        QUEST.advance( Gevent.nr );
    }
    else if( Gevent.type == Event_RequestTriggerScript )
    {
        //Trigger event 1 (Door explosion map 7)
        if( Gevent.nr == 1 )
        {
            SCRIPT.add( "addanime:",1,70,100 );
            SCRIPT.add( "wait",1900 );
            SCRIPT.add( "addanime:",0,70,100 );
            SCRIPT.add( "wait",800 );
            SCRIPT.add( "addfreechunk:",DEFINED,1 );
            SCRIPT.add( "intermanip:",1,-1,-1,1 );

        }
    }
    else if( Gevent.type == 72 ) //Weapon changed
    {
        cInterface.handle_events(event,Gevent);
    }
}

}
