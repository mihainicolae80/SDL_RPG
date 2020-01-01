#include "console.h"


#define CONSOLE_HEIGHT 400
#define CONSOLE_LINE_HEIGHT 30

//Class
CLS_CONSOLE CONS;

//Colors
static SDL_Color console_back_color = {0, 0, 0, 100};
static SDL_Color console_line_color = {0, 0, 0, 175};

void CLS_CONSOLE::init(){

	// -- INIT GENERAL --
	_DISPLAY = false;


	//  -- INIT LIST --
	_First_node = new node;
	_Last_node  = new node;
	_First_node->next = _Last_node;
	_Last_node->next  = nullptr;

	//Init Font
	CONS_FONT = TTF_OpenFont( "Data//Fonts//Amble-Regular.ttf",24 );

	INPUT_LINE_TEX = nullptr;
	INPUT_LINE_TEXT[0] = '\0';

	firstline    = 0;
	item_counter = 0;

}


void CLS_CONSOLE::draw(){
	//Close if needed
	if( !_DISPLAY ) return;
	int i,shown = 0;

	//Render the background
	SDL_SetRenderDrawBlendMode( RENDER_MAIN, SDL_BLENDMODE_BLEND );
	//Console background
	FillRect( 0,0,SCREEN_WIDTH,CONSOLE_HEIGHT - CONSOLE_LINE_HEIGHT, &console_back_color );
	//Console input line
	FillRect( 0,CONSOLE_HEIGHT-CONSOLE_LINE_HEIGHT, SCREEN_WIDTH,CONSOLE_HEIGHT,&console_line_color  );

	//Render CONTENT
	shown = 0;
	i     = 1;
	for( node *inode = _First_node->next; inode != _Last_node && shown < 20 ;inode = inode->next , i++ )
	{
		if( i > firstline )
		{
			shown++;

			ApplyTex( 5, CONSOLE_HEIGHT - shown * 25 - 35,inode->tx_text );
		}
	}


	//RENDER INPUT LINE TEXT
	ApplyTex( 10,CONSOLE_HEIGHT - CONSOLE_LINE_HEIGHT , INPUT_LINE_TEX );

}


void CLS_CONSOLE::switch_if_display(){

	/*
-When shown:
	*the game is paused
	*all the imput goes the the console
-When close
	*the game is resumed
*/

	_DISPLAY = !_DISPLAY;

	if( _DISPLAY )
		SDL_StartTextInput();
	else
		SDL_StopTextInput();


}


bool CLS_CONSOLE::is_shown(){
	return _DISPLAY;
}


void CLS_CONSOLE::out( std::string text ){
	item_counter ++;
	//Create New Item
	node* newnode = new node;
	newnode->next = _First_node->next;
	_First_node->next = newnode;
	newnode->tx_text = BAS_RenderText( CONS_FONT, text.c_str() , WHITE );

}


void CLS_CONSOLE::sendEvent( SDL_Event *event )
{
	// only handle if console visible
	if (!_DISPLAY) {
		return;
	}

	//reset update checker
	bool INPUT_LINE_UPDATE = false;

	//Add @Input to @String
	if( event->type == SDL_TEXTINPUT && event->text.text[0] != '`' )
	{
		BAS_strcat( INPUT_LINE_TEXT,event->text.text );

		INPUT_LINE_UPDATE = true;
	}
	else
		if( event->type == SDL_KEYDOWN )
		{
			if( event->key.keysym.scancode == SDL_SCANCODE_RETURN )
			{
#define SET_UNKNOWN BAS_strcat( INPUT_LINE_TEXT," (X)" );
				string CTStr[10];
				int cstr = 0;
				for( unsigned int i = 0; i < SDL_strlen( INPUT_LINE_TEXT );i++ )
					if( INPUT_LINE_TEXT[i] == ' ' ) cstr++;
					else
						CTStr[cstr] += INPUT_LINE_TEXT[i];




				///Switch Command
				if(CTStr[0] == "exit")
				{

				}
				else
					if(CTStr[0] == "czone")
					{
						PLAYER.teleportZone( atoi( CTStr[1].c_str() ) );
					}
					else
						if(CTStr[0] == "x"){

							PLAYER._PhysicalBody.x = atoi(CTStr[1].c_str());
						}
						else
							if(CTStr[0] == "y"){

								PLAYER._PhysicalBody.y = atoi(CTStr[1].c_str());
							}
							else {
								SET_UNKNOWN
							}
				//Oputput
				out( INPUT_LINE_TEXT );

				//Erase String
				INPUT_LINE_TEXT[0] = '\0';
			}
			else {
				if( event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE ) {
					INPUT_LINE_TEXT[ SDL_strlen( INPUT_LINE_TEXT ) - 1 ] = '\0';
				}
				else if( event->key.keysym.scancode == SDL_SCANCODE_END ) {
					firstline = 0;
				}
			}
			if( event->key.keysym.scancode == SDL_SCANCODE_HOME ) {
				firstline = item_counter - 12;
				if( firstline < 0 )  firstline = 0;
			}

			INPUT_LINE_UPDATE = true;
		}
		else if(event->type == SDL_MOUSEWHEEL) {
			if(event->wheel.y > 0) {
				firstline ++;
			}
			else if( event->wheel.y < 0 ) {
				firstline --;
				if( firstline < 0 ) firstline = 0;
			}
		}

	if( INPUT_LINE_UPDATE ) {
		BAS_DestroyTex( &INPUT_LINE_TEX );
		INPUT_LINE_TEX = BAS_RenderText( CONS_FONT,INPUT_LINE_TEXT,WHITE );
		INPUT_LINE_UPDATE = false;
	}
}















