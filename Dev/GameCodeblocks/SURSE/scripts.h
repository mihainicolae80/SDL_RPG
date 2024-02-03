#ifndef SCRIPTS_H_INCLUDED
#define SCRIPTS_H_INCLUDED

#include <iostream>
#include "main_headers.h"
#include "anim.h"
#include "data.h"
#include "map.h"


using namespace std;

extern class Script{
private:
struct GAME_script{
string command;
int info[5];
GAME_script *next;
}*FirstScript,*LastScript,*LastAddScript;

bool WaitForDelay;
Uint32 tDelay;

public:
Script(void);
void add( string command,int info1 = -999,int info2 = -999,int info3 = -999,int info4 = -999,
          int info5 = -999 );
void handle(void);
}SCRIPT;

#endif // SCRIPTS_H_INCLUDED
