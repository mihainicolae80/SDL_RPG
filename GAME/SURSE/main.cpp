#include "main_headers.h"
#include "run.h"
#include "base.h"
#include "settings.h"



int main( int argc,char **argv ){
int ERROR;
///LOAD Settings

SETTINGS.load();

///Init
ERROR = ENGINE_Init();
if( ERROR != 0 ) return ERROR;

RUN_GAME();

ENGINE_Cleanup();

return 0;
}
