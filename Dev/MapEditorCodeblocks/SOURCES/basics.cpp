#include "basics.h"
#include "base.h"

void   BAS_ctos( char carray[], string &strnew ){
int i;
strnew.erase();

while( carray[0] == ' ' )
{
    for( i = 1; (unsigned)i < SDL_strlen( carray ); i++ )
    carray[i-1] = carray[i];
    carray[i-1] = '\0';
}

for( int i = 0; carray[i] != '\0'; i++ )
strnew += carray[i];
}
double BAS_abs( double x ){
if( x < 0 )
return x * -1;
return x;
}
void   BAS_itos(int nr,string &newstr ){
std::stringstream ssaux;
newstr.erase();
ssaux<<nr;
ssaux>>newstr;
}
void   BAS_strcpy( char *dest,const char *sourc ){
int len_sourc,i;
len_sourc = SDL_strlen( sourc );

for( i = 0; i < len_sourc; i++ )
dest[i] = sourc[i];
dest[len_sourc] = '\0';
}
void   BAS_strcat( char *dest,const char *sourc ){
int len_dest,len_sourc,i;
len_dest  = SDL_strlen( dest );
len_sourc = SDL_strlen( sourc );

for( i = 0; i < len_sourc; i++ )
dest[i + len_dest ] = sourc[i];
dest[len_sourc + len_dest ] = '\0';
}


TTF_Font *BAS_LoadFont(string file,int fontsize){

    TTF_Font *temp = NULL;

    temp = TTF_OpenFont(file.c_str(), fontsize);

    if(temp == NULL)
    {
        DEBUG_OUT_FLF(file);
        //throw new exception();
    }

    return temp;
}
int BAS_LoadFonts(void){

try{

    Font14 = BAS_LoadFont("textures//fonts//Font20_knight.ttf" ,14 );
    Font18 = BAS_LoadFont("textures//fonts//Font20_knight.ttf" ,18 );
    Font20 = BAS_LoadFont("textures//fonts//Font20_knight.ttf" ,20 );
    Font24 = BAS_LoadFont("textures//fonts//Font20_knight.ttf" ,24 );

    Font2_14 = BAS_LoadFont("textures//fonts//Amble-Regular.ttf" ,14 );
    Font2_18 = BAS_LoadFont("textures//fonts//Amble-Regular.ttf" ,18 );
    Font2_20 = BAS_LoadFont("textures//fonts//Amble-Regular.ttf" ,20 );
    Font2_24 = BAS_LoadFont("textures//fonts//Amble-Regular.ttf" ,24 );
}
catch(exception){
    return false;
}

return true;
}

SDL_Texture* BAS_RenderText( TTF_Font *Font20,const char *text,SDL_Color color ){

SDL_Surface *aux_surf = NULL;
SDL_Texture *dest = NULL;

aux_surf = TTF_RenderText_Blended( Font20,text,color );
dest = SDL_CreateTextureFromSurface( WIN_MAIN.render, aux_surf );
return dest;
}
void         BAS_DestroyTex( SDL_Texture **tex ){

if( &tex != NULL )
{
    SDL_DestroyTexture( *tex );
    *tex = NULL;
}
}

