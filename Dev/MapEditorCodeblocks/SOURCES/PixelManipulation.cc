#include "PixelManipulation.h"




void PixelManipulation::init(){

Palette = loadSurf( "textures//environmental//DayNight_palette.png" );
}
Uint32 PixelManipulation::getpix( SDL_Surface *surface,int x,int y ){

    Uint32* PixVec = (Uint32*)surface->pixels;
    return PixVec[y * surface->w + x];
}
void PixelManipulation::putpix( SDL_Surface *surface,int x,int y,Uint32 pixel ){

    Uint32 *PixVec = (Uint32*)surface->pixels;
    PixVec[ (y*surface->w) + x ] = pixel;
}
SDL_Surface *PixelManipulation::flip_img( SDL_Surface *surface,int flags ){

    SDL_Surface *newsurf;
    int x,y;

    //if( surface->flags & SDL_SRCCOLORKEY )
    //{
    //    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,0 );
    //}
    //else
    //{
        newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );
    //}

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    Uint32 pix;

    for( x = 0; x < surface->w ;x ++)
        for( y = 0; y < surface->h; y++ )
        {
            pix = getpix( surface, x ,y );

            if( ( flags & FLIP_VERT ) && ( flags & FLIP_HOR ) )
            {
                putpix( newsurf, surface->w - x -1 , surface->h - y -1, pix );
            }
            else if( flags & FLIP_VERT )
            {
                putpix( newsurf, surface->w - x -1, y, pix );
            }
            else if( flags & FLIP_HOR )
            {
                putpix( newsurf,x ,surface->h - y -1 , pix );
            }
        }

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }
    //if( surface->flags & SDL_SRCCOLORKEY )
    //{
        //SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    //}
    return newsurf;
}
SDL_Surface *PixelManipulation::rotate_img( SDL_Surface *surface,int direction ,bool animated,int frames ){

    SDL_Surface *newsurf;
    int x,y,z;

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    Uint32 pix;

if( !animated )
{
    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->h, surface->w,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,0 );
    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y < surface->h; y++ )
    {
        pix = getpix( surface, x,y );

        if( direction == ROTATE_LEFT )
        {
            putpix( newsurf, y,newsurf ->h - x -1,pix );
        }
        else if( direction == ROTATE_RIGHT )
        {
            putpix( newsurf, newsurf->w - y -1,x ,pix );
        }
        else
        putpix( newsurf, x,y,pix );
    }
}
else
{
    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->h*frames
              ,surface->w/frames+1 ,surface->format->BitsPerPixel
              ,surface->format->Rmask, surface->format->Gmask
              ,surface->format->Bmask,0 );

    for( z = 0; z < frames ; z ++ )
    {
        for( x = 0; x < surface->w/frames; x ++ )
            for( y = 0; y < surface->h; y++ )
            {
                pix = getpix( surface, x + z*surface->w/frames,y );

                if( direction == ROTATE_LEFT )
                {
                    putpix( newsurf, y + z*surface->h , surface->w/frames - x ,pix );
                }
                else if( direction == ROTATE_RIGHT )
                {
                    putpix( newsurf,surface->h - y + z*surface->h ,x ,pix );
                }
            }
    }
    for( x = 0; x < newsurf->w;x++ )
    if( direction == ROTATE_LEFT )
    {
        putpix( newsurf, x, 0, getpix( Palette, 4,0 ) );
    }
    else if( direction == ROTATE_RIGHT )
    {
        putpix( newsurf, x, newsurf->h -1, getpix( Palette, 4,0 ) );
    }
}


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }
    /*
    if( surface->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, surface->format->colorkey );
    }
    */
    return newsurf;

}
SDL_Surface *PixelManipulation::RenderCorner( SDL_Surface *surface ){

    SDL_Surface *newsurf;
    int x,y;

    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y < surface->h - x-1; y++ )
    {
        putpix( newsurf, x , y, getpix( surface, x ,y ) );
    }

    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y <= x; y++ )
    {
        putpix( newsurf,surface->w - y -1, x , getpix( surface, x ,y ) );
    }

    //for( x = 0; x < surface->w; x++ )
    //putpix( newsurf, y , surface->h - x , getpix( Palette, 7 ,0 ) );

    //for( x = 0; x < surface->w; x++ )
    //putpix( newsurf, x ,surface->h - x , getpix( Palette, 7 ,0 ) );

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

    return newsurf;
}
SDL_Surface *PixelManipulation::RenderLightMap( int nr ){
int i,j,color,alpha;
SDL_Surface *newsurf;
DayNightChunk *p;
switch( nr + 1 )
{
    case 1://Zi
    color = 4;
    alpha = 20;
    break;
    case 2://Seara -> portocaliu
    color = 1;
    alpha = 20;
    break;
    case 3://Seara Bleo
    color = 2;
    alpha = 120;
    break;
    case 4://Noapte
    color = 3;
    alpha = 235;
    break;
    case 5://Dimineata Bleo
    color = 2;
    alpha = 120;
    break;
    case 6://Dimineata galben
    color = 0;
    alpha = 20;
    break;
}

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, BLOCK_WIDTH * maxy, BLOCK_HEIGHT * maxx,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

    for( i = 0; i < BLOCK_HEIGHT * maxx;i++ )
        for( j = 0; j < BLOCK_WIDTH * maxy ;j++ )
        {
            PIXELS.putpix(newsurf ,j , i, PIXELS.getpix( Palette, color,0 ) );
        }

        for( p = FirstDNChunk -> next; p -> next != NULL && p != NULL; p = p-> next )
        {
            if( p ->nr == 1 )//Round light
            {
                if( nr == 3 )//Night
                {
                    for( i = p -> y - p -> range ; i < p -> y + p -> range; i++ )
                    for( j = p -> x - p ->range; j < p ->x + p ->range; j++ )
                    if( i > 0 && i < BLOCK_HEIGHT * maxx  && j > 0 && j < BLOCK_WIDTH * maxy )
                    {
                        if( sqrt( pow( i - p -> y, 2 ) + pow( j - p -> x, 2 ) ) < p -> range )
                        {
                            PIXELS.putpix( newsurf, j, i, PIXELS.getpix( Palette, 4, 0) );
                        }
                    }
                }
                else if( nr == 2 || nr == 4 )
                {
                    for( i = p -> y - p -> range ; i < p -> y + p -> range; i++ )
                    for( j = p -> x - p ->range; j < p ->x + p ->range; j++ )
                    if( i > 0 && i < 50 * maxx  && j > 0 && j < 50 * maxy )
                    {
                        if( sqrt( pow( i - p -> y, 2 ) + pow( j - p -> x, 2 ) ) < p -> range/2 )
                        {
                            PIXELS.putpix( newsurf, j, i, PIXELS.getpix( Palette, 4, 0) );
                        }
                    }
                }
            }
        }

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

//SDL_SetAlpha( newsurf, SDL_SRCALPHA,alpha );

    /*
    if( Palette->flags & SDL_SRCCOLORKEY )
    {
        SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, Palette->format->colorkey );
    }
    */

return newsurf;
}
SDL_Surface *PixelManipulation::RenderRect(int w,int h,int color ){

int i,j,col;
SDL_Surface *newsurf;

switch( color ){
case 0:
col = 1;//blue
break;
case 1:
col = 5;//light blue
break;
case 2:
col = 1;//orange
break;
}

if( w > 0 && h > 0 )
{
if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

    for( i = 0; i < h;i++ )
        for( j = 0; j < w ;j++ )
        {
            PIXELS.putpix(newsurf ,j , i, PIXELS.getpix( Palette, col,0 ) );
        }

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

//SDL_SetAlpha( newsurf, SDL_SRCALPHA,100 );
/*
if( Palette->flags & SDL_SRCCOLORKEY )
{
    SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, Palette->format->colorkey );
}
*/

}
else
newsurf = NULL;

return newsurf;
}
SDL_Surface *PixelManipulation::RenderGrid(){
int i,j,k,l;
SDL_Surface *newsurf;

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, maxy*BLOCK_WIDTH , maxx*BLOCK_HEIGHT,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

    for( i = 0; i < maxx*BLOCK_HEIGHT ;i++ )
        for( j = 0; j < maxy*BLOCK_WIDTH ;j++ )
        {
            PIXELS.putpix(newsurf ,j , i, PIXELS.getpix( Palette, 4,0 ) );
        }

    for( i = 0 ; i < maxx ; i++ )
        for( j = 0; j < maxy ; j++ )
        {
            for( l = 0; l < BLOCK_WIDTH; l++ )
            PIXELS.putpix(newsurf , j*BLOCK_WIDTH + l, i*BLOCK_HEIGHT , PIXELS.getpix( Palette, 6,0 ) );
            for( k = 0; k < BLOCK_HEIGHT; k++ )
            PIXELS.putpix(newsurf , j*BLOCK_WIDTH ,i*BLOCK_HEIGHT + k, PIXELS.getpix( Palette, 6,0 ) );

        }


if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

//SDL_SetColorKey( newsurf, SDL_RLEACCEL | SDL_SRCCOLORKEY, Palette->format->colorkey );

return newsurf;
}
void PixelManipulation::RenderMouseLines( SDL_Surface *&l1 ,SDL_Surface *&l2 ){
int i;
if( l1 == NULL ) l1 = SDL_CreateRGBSurface(SDL_SWSURFACE, maxy*BLOCK_WIDTH, 1,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );
if( l2 == NULL ) l2 = SDL_CreateRGBSurface(SDL_SWSURFACE, 1,maxx*BLOCK_HEIGHT,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

//l1
//for( i = 0; i < m_x - 50 - iMoveMap_x ; i++ )
//putpix( l1,i,0,getpix( Palette, 6,0) );
//for( i = m_x - 50 - iMoveMap_x; i < maxy*BLOCK_WIDTH ; i++ )
//putpix( l1,i,0,getpix( Palette, 4,0) );
for( i = 0; i < maxy*BLOCK_WIDTH ; i++ )
putpix( l1,i,0,getpix( Palette, 6,0) );
for( i = 0; i < maxx*BLOCK_HEIGHT; i++ )
putpix( l2,0,i,getpix( Palette, 6,0) );
//l2
/*
for( i = 0; i < m_y; i++ )
putpix( l2,0,i,getpix( Palette, 6,0) );
for( i = m_y; i < maxx*BLOCK_HEIGHT ; i++ )
putpix( l2,0,i,getpix( Palette, 4,0) );
*/
}



///The instance
PixelManipulation PIXELS;

