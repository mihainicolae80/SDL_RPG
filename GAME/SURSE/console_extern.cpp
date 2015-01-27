#include "console_extern.h"
#include "player.h"
#include "basics.h"
#include "debug.h"


int CLS_CONSOLE::init(){

window = SDL_CreateWindow( "RPG CONSOLE",1000,SDL_WINDOWPOS_CENTERED,
                                400,500,0 );

if( window == NULL )
    return true;

render = SDL_CreateRenderer( window,-1, SDL_RENDERER_PRESENTVSYNC );

if( render == NULL )
    return true;

//Load TTF Font
font = NULL;
font = TTF_OpenFont("textures//fonts//Amble-Regular.ttf" ,14 );

if( font == NULL )
    return true;

return false;// --- NO ERROR --
}
void CLS_CONSOLE::sendEvents( SDL_Event *EVENT ){

if( event.window.windowID == WINDOW_CONSOLE_ID )
{
    static SDL_Rect BotRect;
    BotRect.x = 0;
    BotRect.w = SCREEN_WIDTH_CONS;
    BotRect.h = 20;
    BotRect.y = SCREEN_HEIGHT_CONS-BotRect.h;

    bool UpdateCons = false;
    if( event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED )
    {
        SDL_StartTextInput();
        ConsFocus = true;
        UpdateCons = true;
    }
    else
    if( event.window.event == SDL_WINDOWEVENT_FOCUS_LOST )
    {
        SDL_StopTextInput();
        ConsFocus = false;
    }
    else
    if( event.type == SDL_KEYDOWN )
    {
        ///HANDLE COMMANDS
        if( event.key.keysym.sym == SDLK_RETURN )
        {
            #define SET_UNKNOWN BAS_strcat( ConsText," (X)" );
            string CTStr[10];
            int cstr = 0;
            for( unsigned int i = 0; i < SDL_strlen( ConsText );i++ )
            if( ConsText[i] == ' ' ) cstr++;
            else
            CTStr[cstr] += ConsText[i];

            ///Switch Command
            //if( CTStr[0] == "exit" ) RUN = false;
            //else
            if( CTStr[0] == "set" )
            {
                if( CTStr[1] == "x" )
                PLAYER._PhysicalBody.x = atoi( CTStr[2].c_str() );
                else
                if( CTStr[1] == "y" )
                PLAYER._PhysicalBody.y = atoi( CTStr[2].c_str() );
                else
                if( CTStr[1] == "czone" )
                PLAYER.teleportZone( atoi( CTStr[2].c_str() ) );
                else
                SET_UNKNOWN;
            }
            else
            if( CTStr[0] == "show" )
            {
                if( CTStr[1] == "colrect" )
                {
                    if( CTStr[2] == "0" )
                        DEBUG.setDrawCollRects( false );
                    else
                    DEBUG.setDrawCollRects( true );
                }

            }
            else
            if( CTStr[0] == "turn" )
            {
                if( CTStr[1] == "left" )
                    PLAYER.TurnBody( LEFT );
                else
                if( CTStr[1] == "right" )
                    PLAYER.TurnBody( RIGHT );
                else
                if( CTStr[1] == "up" )
                    PLAYER.TurnBody( UP );
                else
                if( CTStr[1] == "down" )
                    PLAYER.TurnBody( DOWN );

            }
            else
            if( CTStr[0] == "addanim" )
            {
                ALLANIMES.addanime( atoi( CTStr[1].c_str() ) ,atoi( CTStr[2].c_str() ),
                                    atoi( CTStr[3].c_str() ) );
            }
            else
            if( CTStr[0] == "forcequit" ) return;
            //NO Known Command
            else
            SET_UNKNOWN;


            out( ConsText );
            ConsText[0] = '\0';
            UpdateCons = true;
        }
        else
        if( event.key.keysym.sym == SDLK_BACKSPACE )
        {
            ConsText[ SDL_strlen( ConsText ) -1 ] = '\0';
            UpdateCons = true;
        }
    }

    if( ConsFocus )
    {
        if( event.type == SDL_TEXTINPUT )
        {
            BAS_strcat(ConsText,event.text.text );
            UpdateCons = true;
        }
    }



    if( UpdateCons )
    {
        SDL_Surface *tempsurf = NULL;
        SDL_Texture *temptex  = NULL;

        tempsurf = TTF_RenderText_Blended( Font2_18,ConsText,BLACK );
        temptex  = SDL_CreateTextureFromSurface( RENDER_CONSOLE,tempsurf );

        SDL_SetRenderDrawColor( RENDER_CONSOLE,255,255,255,255);
        SDL_RenderFillRect( RENDER_CONSOLE,&BotRect );
        outTex( BotRect.x,BotRect.y,temptex );
        SDL_RenderPresent( RENDER_CONSOLE );

        SDL_FreeSurface( tempsurf );
        SDL_DestroyTexture( temptex );
    }

}

}
void CLS_CONSOLE::out( string text ){
SDL_Surface *tempsurf = NULL;
SDL_Texture *temptex  = NULL;
CONSOLE_TEXT *newct;
SDL_Rect BotRect;

//OUT TO FILE
DEBUG.fout( FILE_CONSOUT, text );


tempsurf = TTF_RenderText_Blended( Font2_14,text.c_str(),WHITE );

if( tempsurf == NULL )
{
    DEBUG.fout( FILE_GENERAL, "Failed to output to CONSOLE" );
    DEBUG.fout( FILE_GENERAL, "Reason: Could not render text\n" );
}


newct = new CONSOLE_TEXT;

newct->tex = SDL_CreateTextureFromSurface( RENDER_CONSOLE,tempsurf );
if( tempsurf != NULL )
SDL_FreeSurface( tempsurf );
//Add to the list
newct->next = FirstConsText->next;
FirstConsText->next = newct;

BotRect.x = 0;
BotRect.w = SCREEN_WIDTH_CONS;
BotRect.h = 20;
BotRect.y = SCREEN_HEIGHT_CONS-BotRect.h;

tempsurf = TTF_RenderText_Blended( Font2_18,ConsText,BLACK );
temptex  = SDL_CreateTextureFromSurface( RENDER_CONSOLE, tempsurf );


    static CONSOLE_TEXT *ict;
    static int consi;
    SDL_SetRenderDrawColor( RENDER_CONSOLE,0,0,0,255);
    SDL_RenderClear( RENDER_CONSOLE );
    SDL_SetRenderDrawColor( RENDER_CONSOLE,255,255,255,255);
    SDL_RenderFillRect( RENDER_CONSOLE,&BotRect );
    outTex( BotRect.x,BotRect.y,temptex );
    for( ict = FirstConsText->next,consi = 0;ict != LastConsText;ict = ict->next,consi++ )
    outTex( 10,SCREEN_HEIGHT_CONS-40-14*consi,ict->tex );

    SDL_FreeSurface( tempsurf );
    SDL_DestroyTexture( temptex );
    SDL_RenderPresent( RENDER_CONSOLE );
}
void CLS_CONSOLE::outTex( int x,int y,SDL_Texture *tex,SDL_Rect *clip ){
SDL_Rect pos;

pos.x = x;
pos.y = y;

    if( clip != NULL )
    {
        pos.w = clip->w;
        pos.h = clip->h;
    }
    else
    SDL_QueryTexture( tex,NULL,NULL,&pos.w,&pos.h );

SDL_RenderCopy( RENDER_CONSOLE,tex,clip,&pos );
}
Uint32 CLS_CONSOLE::getID(){
return SDL_GetWindowID( window );
}






