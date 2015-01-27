#include "scripts.h"



Script::Script(){
FirstScript = new GAME_script;
LastScript  = new GAME_script;
FirstScript->next = LastScript;
LastScript ->next = NULL;
LastAddScript = FirstScript;

WaitForDelay = false;
}
void Script::add(string command,int info1,int info2,int info3,int info4,int info5){
GAME_script* newscr = new GAME_script;

newscr->command = command;
if( info1 != -999 )
newscr->info[0] = info1;
if( info2 != -999 )
newscr->info[1] = info2;
if( info3 != -999 )
newscr->info[2] = info3;
if( info4 != -999 )
newscr->info[3] = info4;
if( info5 != -999 )
newscr->info[4] = info5;

newscr->next = LastAddScript->next;
LastAddScript->next = newscr;
LastAddScript = newscr;
}
void Script::handle(){

while( FirstScript->next != LastScript && WaitForDelay == false )
{
    ///Do Command
    #define cscr FirstScript->next

    if( cscr->command == "wait" )
    {
        tDelay = SDL_GetTicks() + cscr->info[0];
        CONSOLE.out("Waiting");
        WaitForDelay = true;
    }
    else
    if( cscr->command == "addanime:" )
    ALLANIMES.addanime( cscr->info[0],cscr->info[1],cscr->info[2] ),CONSOLE.out("Addanime");
    else
    if( cscr->command == "addfreechunk:" )
    {
        if( cscr->info[0] == DEFINED )
        {
            //Map 7 Add Door
            if( cscr->info[1] == 1 )
            {
                FreeChunk *tempfc = new FreeChunk;
                SDL_Surface *tempsurf = NULL;
                tempfc->animated = 1;
                tempfc->frames   = 1;
                tempfc->x = 0;
                tempfc->y = 100;
                tempfc->w = 150;
                tempfc->h = 150;

                //Door
                tempsurf = DATABASE.GetChunkSkin( 215, 3,0,0 );
                tempfc->tex = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
                GAME_MAP.UpdateLastChunk();
                GAME_MAP.AddFreeChunk( tempfc,BACK );
                delete tempfc;
                if(tempsurf != NULL ) SDL_FreeSurface( tempsurf );
            }
        }
    }
    else
    if( cscr->command == "intermanip:" )
    {
        GAME_MAP.manipulate_inter( cscr->info[0],cscr->info[1],cscr->info[2],cscr->info[3] );
    }
    ///Delete From List
    GAME_script *delaux;
    delaux = FirstScript->next;
    FirstScript->next = FirstScript->next->next;

    //Reset First Script
    if( delaux == LastAddScript )
        LastAddScript = FirstScript;

    delete delaux;

}

if( WaitForDelay == true )
{
    if( tDelay <= SDL_GetTicks() )
    {
        WaitForDelay = false;
        CONSOLE.out("Stop Waiting");
    }

}

}
