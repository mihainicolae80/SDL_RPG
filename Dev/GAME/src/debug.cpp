#include "debug.h"

using namespace std;

//Declaration
cls_debug_t DEBUG;

void cls_debug_t::init_low()
{
	// open output files
	file_flf.open("DEBUG//FileLoadFails.txt", ios_base::trunc);
	file_general.open( "DEBUG//general_errors.txt", ios_base::trunc);
	file_cons_out.open( "DEBUG//CONSOLE OUTPUT.txt", ios_base::trunc);
	drawcolrects = true;
}

int  cls_debug_t::init_high()
{
	int ERROR = 0;

	//init picture
	ex_mark_sf = nullptr;
	ex_mark_tx = nullptr;

	//Surf
	ex_mark_sf = LoadSurf("textures//debug//ExcMark.png");
	if(ex_mark_sf == nullptr)
	{
		cls_debug_t::fout(FILE_FLF, "Couldn't load ExMark surface");
		ERROR =  true;
	}

	//Texture
	ex_mark_tx = LoadTex( "textures//debug//ExcMark.png" );
	if(ex_mark_tx == nullptr) {
		cls_debug_t::fout(FILE_FLF, "Couldn't load ExMark surface");
		ERROR = true;
	}

	return ERROR;
}


cls_debug_t::~cls_debug_t()
{
	//Close FILES
	if( file_flf.is_open() )
	file_flf.close();

	if( file_general.is_open() )
	file_general.close();

	SDL_FreeSurface   ( ex_mark_sf  );
	SDL_DestroyTexture( ex_mark_tx );
}

void cls_debug_t::fout( int file_out, string text )
{

	if(file_out == FILE_FLF) {
		file_flf.open( "DEBUG//FileLoadFails.txt", ios_base::app);
		if(file_flf.is_open()) {
			file_flf << text << '\n';
			file_flf.close();
		}
	} else if(file_out == FILE_GENERAL) {
		file_general.open( "DEBUG//general_errors.txt", ios_base::app );
		if( file_general.is_open()) {
			file_general << text << '\n';
			file_general.close();
		}
	} else if(file_out == FILE_CONSOUT) {
		file_cons_out.open( "DEBUG//CONSOLE OUTPUT.txt", ios_base::app );
		if( file_cons_out.is_open() )
		{
			file_cons_out << text << '\n';
			file_cons_out.close();
		}
	}
}

SDL_Surface *cls_debug_t::getNewsfExc(void)
{
	SDL_Surface *temp;
	temp = LoadSurf( "textures//debug//ExcMark.png" );
	return temp;
}

void cls_debug_t::DrawCollRects()
{
	SDL_Rect rAux;
	COLLIDER *icol;


	SDL_SetRenderDrawBlendMode( RENDER_MAIN, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( RENDER_MAIN,80,80,80,100 );

	rAux.x = PLAYER._PhysicalBody.x + GAME_MAP.DisplayLocation_x;
	rAux.y = PLAYER._PhysicalBody.y + GAME_MAP.DisplayLocation_y;
	rAux.w = PLAYER._PhysicalBody.w;
	rAux.h = PLAYER._PhysicalBody.h;
	SDL_RenderFillRect( RENDER_MAIN, &rAux );

	for(icol = FirstCol->next ;icol != LastCol;icol = icol->next) {
		rAux.x = icol->x + GAME_MAP.DisplayLocation_x;
		rAux.y = icol->y + GAME_MAP.DisplayLocation_y;
		rAux.w = icol->w;
		rAux.h = icol->h;

		if(icol->ON) {
			SDL_SetRenderDrawColor( RENDER_MAIN,150,230,255,100 );
		} else {
			SDL_SetRenderDrawColor( RENDER_MAIN,230,180,20,100 );
		}

		SDL_RenderFillRect( RENDER_MAIN, &rAux );
	}
}


void cls_debug_t::setDrawCollRects(bool x)
{
	drawcolrects = x;
}
