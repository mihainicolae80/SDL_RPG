#include "pixel_manip.h"

void PixelManipulation::init(){
Palette = LoadSurf( "textures//environmental//DayNight_palette.png" );
///Daca nu sunt necesare la compilarea completa se sterg
//model_cloak = LoadSurf( "textures//items//cloak//model.PNG" );
//CLOAKS[0] = LoadSurf( "textures//items//cloak//1.PNG" );

}
Uint32 PixelManipulation::getpix( SDL_Surface *surface,int x,int y ){
int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
void PixelManipulation::putpix( SDL_Surface *surface,int x,int y,Uint32 pixel ){
int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
SDL_Surface *PixelManipulation::flip_img( SDL_Surface *surface,int flags ){

    SDL_Surface *newsurf;
    int x,y;

    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_LockSurface( surface );
    }

    Uint32 pix;

    for( x = 0; x < surface->w ;x ++)
        for( y = 0; y < surface->h; y++ )
        {
            pix = getpix( surface, x ,y );

            if( ( flags & VERT ) && ( flags & HOR ) )
            {
                putpix( newsurf, surface->w - x -1 , surface->h - y -1, pix );
            }
            else if( flags & VERT )
            {
                putpix( newsurf, surface->w - x -1, y, pix );
            }
            else if( flags & HOR )
            {
                putpix( newsurf,x ,surface->h - y -1 , pix );
            }
        }

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

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
    newsurf = SDL_CreateRGBSurface(0, surface->h, surface->w,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );
    for( x = 0; x < surface->w ;x ++)
    for( y = 0; y < surface->h; y++ )
    {
        pix = getpix( surface, x,y );

        if( direction == LEFT )
        {
            putpix( newsurf, y,newsurf ->h - x -1,pix );
        }
        else if( direction == RIGHT )
        {
            putpix( newsurf, newsurf->w - y -1,x ,pix );
        }
        else
        putpix( newsurf, x,y,pix );
    }
}
else
{
    newsurf = SDL_CreateRGBSurface(0, surface->h*frames , surface->w/frames ,surface->format->BitsPerPixel ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );
    for( z = 0; z < frames ; z ++ )
    {
        for( x = 0; x < surface->w/frames; x ++ )
            for( y = 0; y < surface->h; y++ )
            {
                pix = getpix( surface, x + z*surface->w/frames,y );

                if( direction == LEFT )
                {
                    putpix( newsurf, y + z*surface->h , surface->w/frames - x ,pix );
                }
                else if( direction == RIGHT )
                {
                    putpix( newsurf,surface->h - y + z*surface->h ,x ,pix );
                }
            }
    }
}


    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }


    return newsurf;

}
SDL_Surface *PixelManipulation::CreatePixAnime( SDL_Surface* sample, SDL_Surface *model){
int x,y,i,j,nrx,nry,fx,fy;
bool found;

nrx = model->w/50;
nry = model->h/50;

         SDL_Surface *newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, model->w, model->h,model->format->BitsPerPixel ,model->format->Rmask, model->format->Gmask, model->format->Bmask,0 );

         if( SDL_MUSTLOCK( sample ) )
         {
             SDL_LockSurface( sample );
         }
         if( SDL_MUSTLOCK( model ) )
         {
             SDL_LockSurface( model );
         }
        //Copiaza primele imagini
        for( i = 0; i < nry;i++ )
        {
            for( x = 0; x < 50; x ++ )
            for( y = 0; y < 50; y ++ )
            {
                putpix( newsurf,x,y + 50*i,getpix(sample,x,y+50*i) );
            }
        }
        for( i = 0; i < nry;i++ )//parcurgere toate imagini acre trebuie create
            for( j = 1;j < nrx; j++ )//exceptand imaginile din extrema stanga
            {
                for( x = 0 ; x < 50; x++ )//parcurgere img care trebuie creata
                    for( y = 0; y < 50;y++ )
                    {
                        found = false;
                        if( getpix(model,x + 50*j ,y + 50 *i) == getpix(model,x ,y + 50 *i) )
                        {
                            putpix( newsurf ,x + 50*j ,y + 50 *i,getpix( newsurf,x ,y + 50*i ) );
                            found = true;
                        }

                        else
                        {
                            for( fy = 0; fy < 50 && found == false; fy++ )
                                for( fx = 0 ; fx < 50 && found == false; fx++ )
                                if( getpix( model, x + 50*j,y + 50*i ) == getpix( model, fx ,fy + 50*i ) )
                                {
                                    putpix(newsurf,x + 50*j,y + 50 *i,getpix( newsurf,fx,fy + 50*i ));
                                    found = true;
                                }
                        }
                    }
            }

        if( SDL_MUSTLOCK( sample ) )
         {
             SDL_UnlockSurface( sample );
         }
         if( SDL_MUSTLOCK( model ) )
         {
             SDL_UnlockSurface( model );
         }


return newsurf;
}
SDL_Surface *PixelManipulation::CreatePixAnime2( SDL_Surface* sample, SDL_Surface *model){
int x,y,i,j,nrx,nry,fx,fy;
bool found;

nrx = model->w/50;
nry = model->h/50;

         SDL_Surface *newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, model->w, model->h,model->format->BitsPerPixel ,model->format->Rmask, model->format->Gmask, model->format->Bmask,0 );

         if( SDL_MUSTLOCK( sample ) )
         {
             SDL_LockSurface( sample );
         }
         if( SDL_MUSTLOCK( model ) )
         {
             SDL_LockSurface( model );
         }
         if( SDL_MUSTLOCK( model_univers ) )
         {
             SDL_LockSurface( model_univers );
         }

        //Copiaza primele imagini
        /*
        for( i = 0; i < nry;i++ )
        {
            for( x = 0; x < 50; x ++ )
            for( y = 0; y < 50; y ++ )
            {
                putpix( newsurf,x,y + 50*i,getpix(sample,x,y+50*i) );
            }
        }
        */
        for( i = 0; i < nry;i++ )//parcurgere toate imagini care trebuie create
            for( j = 0;j < nrx; j++ )//exceptand imaginile din extrema stanga
            {
                for( x = 0 ; x < 50; x++ )//parcurgere img care trebuie creata
                    for( y = 0; y < 50;y++ )
                    {
                            found = false;
                            for( fy = 0; fy < 50 && found == false; fy++ )
                                for( fx = 0 ; fx < 50 && found == false; fx++ )
                                if( getpix( model, x + 50*j,y + 50*i ) == getpix( model_univers, fx ,fy) )
                                {
                                    putpix(newsurf,x + 50*j,y + 50 *i,getpix( sample,fx,fy ));
                                    found = true;
                                }
                                if( !found )
                                putpix(newsurf,x + 50*j,y + 50 *i,getpix( sample,1,0 ));

                    }
            }

        if( SDL_MUSTLOCK( sample ) )
         {
             SDL_UnlockSurface( sample );
         }
         if( SDL_MUSTLOCK( model ) )
         {
             SDL_UnlockSurface( model );
         }
         if( SDL_MUSTLOCK( model_univers ) )
         {
             SDL_UnlockSurface( model_univers );
         }



return newsurf;
}
SDL_Surface *PixelManipulation::RenderLightMap( int nr ){
int i,j,color,alpha;
SDL_Surface *newsurf;
LIGHT *p;
switch( nr + 1 )
{
    case 1://Dimineata galben
    color = 0;
    alpha = 40;
    break;
    case 2://Seara -> portocaliu
    color = 1;
    alpha = 20;
    break;
    case 3://Seara/dimineata Bleo
    color = 2;
    alpha = 120;
    break;
    case 4://Noapte
    color = 3;
    alpha = 253;
    break;

}

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, BLOCK_WIDTH * NrBlockX, BLOCK_HEIGHT * NrBlockY,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,Palette->format->Amask );

    for( i = 0; i < BLOCK_HEIGHT * NrBlockY;i++ )
        for( j = 0; j < BLOCK_WIDTH *NrBlockX ;j++ )
        {
            Pixels.putpix(newsurf ,j , i, Pixels.getpix( Palette, color,0 ) );
        }

        for( p = FirstLight -> next; p -> next != NULL && p != NULL; p = p-> next )
        {
            if( p ->nr == 1 )//Round light
            {
                if( nr == 3 )//Night
                {
                    for( i = p->y - p->range; i < p->y + p->range; i++ )
                    for( j = p->x - p->range; j < p->x + p->range; j++ )
                    if( i > 0 && i < BLOCK_HEIGHT * NrBlockY  && j > 0 && j < BLOCK_WIDTH * NrBlockX )
                    {
                        if( sqrt( pow( i - p -> y, 2 ) + pow( j - p -> x, 2 ) ) < p -> range )
                        {
                            Pixels.putpix( newsurf, j, i, Pixels.getpix( Palette, 7, 0) );
                        }
                    }
                }
                else if( nr == 2 || nr == 4 )
                {
                    for( i = p -> y - p -> range ; i < p -> y + p -> range; i++ )
                    for( j = p -> x - p ->range; j < p ->x + p ->range; j++ )
                    if( i > 0 && i < 50 * NrBlockY  && j > 0 && j < 50 * NrBlockX )
                    {
                        if( sqrt( pow( i - p -> y, 2 ) + pow( j - p -> x, 2 ) ) < p -> range/2 )
                        {
                            Pixels.putpix( newsurf, j, i, Pixels.getpix( Palette, 7, 0) );
                        }
                    }
                }
            }
        }

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

SDL_SetSurfaceAlphaMod(newsurf,alpha);

return newsurf;
}
SDL_Surface *PixelManipulation::RenderCorner( SDL_Surface *surface ){

    SDL_Surface *newsurf;
    int x,y;

    newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h,32 ,surface->format->Rmask, surface->format->Gmask, surface->format->Bmask,surface->format->Amask );


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

    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

    return newsurf;
}
SDL_Surface *PixelManipulation::RenderNormalMap(){
int i,j,k,l;
FreeChunk *fcc,*delaux;
SDL_Surface *newsurf;

if( SDL_MUSTLOCK( Palette ) ) SDL_LockSurface( Palette );

newsurf = SDL_CreateRGBSurface(0, BLOCK_WIDTH * NrBlockX , BLOCK_HEIGHT * NrBlockY ,32 ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,Palette->format->Amask );
//Render Locked Back Layer
for( i = 0; i < NrBlockY ;i++ )
    for( j = 0; j < NrBlockX ;j++ )
    {
        if( SDL_MUSTLOCK( GAME_MAP.Normal_Map[i][j] ) )
        {
            SDL_LockSurface( GAME_MAP.Normal_Map[i][j] );
        }

        for( k = 0; k < BLOCK_HEIGHT; k++ )
            for( l = 0; l < BLOCK_WIDTH; l++ )
            putpix( newsurf,l + BLOCK_WIDTH*j,k + BLOCK_HEIGHT*i, getpix( GAME_MAP.Normal_Map[i][j],l,k ) );

        if( SDL_MUSTLOCK( GAME_MAP.Normal_Map[i][j] ) )
        {
            SDL_UnlockSurface( GAME_MAP.Normal_Map[i][j] );
        }
    }

//Fusion Free Back Layer
fcc = FirstFreeChunk_back;
while( fcc->next != LastFreeChunk_back )
{
    if( !fcc->next->animated )
    {
        if( fcc->next->surf != NULL  )
        for( i = 0; i < fcc->next->surf->w; i++ )
        for( j = 0; j < fcc->next->surf->h; j++ )
        if( i <= BLOCK_WIDTH*NrBlockX &&
            j <= BLOCK_HEIGHT*NrBlockY &&
            ( getpix( fcc->next->surf,i,j ) != getpix( Palette,4 ,0 ) ) &&
            ( getpix( fcc->next->surf,i,j ) != getpix( Palette,7 ,0 ) ) )
        putpix( newsurf,fcc->next->x + i ,fcc->next->y + j ,getpix( fcc->next->surf,i,j ) );

        //Delete chunk
        delaux = fcc->next;
        fcc->next = fcc->next->next;
        delete delaux;
    }
    else//In anime
    fcc = fcc->next;


}

if( SDL_MUSTLOCK( Palette ) ) SDL_UnlockSurface( Palette );

return newsurf;
}
SDL_Surface *PixelManipulation::RenderForeLayerMap(){
int i,j;
FreeChunk *fcc,*delaux;
SDL_Surface *newsurf;

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, BLOCK_WIDTH * NrBlockX, BLOCK_HEIGHT * NrBlockY,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,Palette->format->Amask );

//Fill with Transparency
for( i = 0; i < NrBlockY * BLOCK_HEIGHT ;i++ )
    for( j = 0; j < NrBlockX * BLOCK_WIDTH;j++ )
    {
        putpix( newsurf,j,i, getpix( Palette,7 ,0 ) );
    }


//Fusio Render Free Back Layer
fcc = FirstFreeChunk_fore;
while( fcc->next != LastFreeChunk_fore )
{
    if( !fcc->next->animated )
    {
        if( fcc->next->tex != NULL )
        for( i = 0; i < fcc->next->surf->w; i++ )
        for( j = 0; j < fcc->next->surf->h; j++ )
        if( i <= BLOCK_WIDTH*NrBlockX && j <= BLOCK_HEIGHT*NrBlockY && ( getpix( fcc->next->surf,i,j ) != getpix( Palette,4 ,0 ) ) )
        putpix( newsurf,fcc->next->x + i ,fcc->next->y + j ,getpix( fcc->next->surf,i,j ) );

        //Delete chunk
        delaux = fcc->next;
        fcc->next = fcc->next->next;
        delete delaux;
    }
    else
    fcc = fcc->next;

}


if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}
return newsurf;
}
SDL_Surface *PixelManipulation::RenderMisc(int nr){
int i,j;
SDL_Surface *newsurf;

newsurf = SDL_CreateRGBSurface(SDL_SWSURFACE, SCREEN_WIDTH, SCREEN_HEIGHT/30,Palette->format->BitsPerPixel ,Palette->format->Rmask, Palette->format->Gmask, Palette->format->Bmask,0 );

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_LockSurface( Palette );
}

if( nr == 1 )//Code bot line
{
    for( i = 0 ; i < SCREEN_HEIGHT/30; i++ )
    for( j = 0; j < SCREEN_WIDTH; j++ )
    putpix( newsurf,j,i,getpix(Palette,6,0) );
    SDL_SetSurfaceAlphaMod( newsurf,100  );
}

if( SDL_MUSTLOCK( Palette ) )
{
    SDL_UnlockSurface( Palette );
}

return newsurf;
}


void RenderTooltip( Inventory_item *CurrentInvItm,
                    SDL_Texture **tt_canvas,
                    SDL_Texture **tt_lines_name1,SDL_Texture **tt_lines_name2,SDL_Texture **tt_lines_name3,
                    SDL_Texture **tt_stats1,SDL_Texture **tt_stats2,SDL_Texture **tt_stats3,SDL_Texture **tt_stats4,
                    SDL_Texture **tt_stats5,SDL_Texture **tt_stats6,
                    SDL_Texture **tt_lines_desc1, SDL_Texture **tt_lines_desc2, SDL_Texture **tt_lines_desc3,
                    SDL_Texture **tt_lines_desc4, SDL_Texture **tt_lines_desc5, SDL_Texture **tt_lines_desc6,
                    int &tt_name_nr,int &tt_statsnr,int &tt_desc_nr
                    ){


            int i,rendi,a,b,length,q,tooltip_statsnr,tooltip_name_nr,tooltip_desc_nr ;
            char *ptoChar,lines_name[5][20],auxchar[100] ;
            bool run_splitname;
            SDL_Surface *tooltip_lines_name[3], *tooltip_stats[6],*tooltip_lines_desc[6],
                        *tooltip_canvas,*tooltip_pal ;

            ///INIT
            for( i = 0; i < 3; i ++ )
            tooltip_lines_name[i] = NULL;

            for( i = 0; i < 6; i ++ )
            {
                tooltip_stats[i] = NULL;
                tooltip_lines_desc[i] = NULL;
            }


            tooltip_pal = LoadSurf("textures//inventory//pal.PNG");

            run_splitname  = true;

            for( int tempi = 0; tempi < 5 ; tempi ++ )
            for( int tempj = 0; tempj < 20; tempj ++ )
            lines_name[tempi][tempj] = '\0';

            for( rendi = 0; rendi < 100 && auxchar[rendi] != '\0' ; rendi++ )
            auxchar[rendi] = '\0';

            switch( CurrentInvItm->type )
            {
                case INV_TYPE_ARMOR:
                ptoChar = CurrentInvItm->armor->name;
                BAS_strcpy( auxchar,CurrentInvItm->armor->description.c_str() );
                tooltip_statsnr = 1;
                break;
                case INV_TYPE_WEAPON:
                ptoChar = CurrentInvItm->weapon->name;
                BAS_strcpy( auxchar,CurrentInvItm->weapon->description.c_str() );
                tooltip_statsnr = 2;
                break;
                case INV_TYPE_BAG:
                ptoChar = CurrentInvItm->bag->name;
                BAS_strcpy( auxchar,CurrentInvItm->bag->description.c_str() );
                tooltip_statsnr = 1;
                break;
                case INV_TYPE_SHIELD:
                ptoChar = CurrentInvItm->shield->name;
                BAS_strcpy( auxchar,CurrentInvItm->shield->description.c_str() );
                tooltip_statsnr = 2;
                break;
                case INV_TYPE_MISC:
                ptoChar = CurrentInvItm->misc->name;
                BAS_strcpy( auxchar,CurrentInvItm->misc->description.c_str() );
                tooltip_statsnr = 0;
                break;
                default:
                ptoChar    = NULL;
                auxchar[0] = '\0';
                tooltip_statsnr = 0;
            }

            tooltip_name_nr  = 0;
            length = 0;
            while( ptoChar[ length ] != '\0' ) length++;

            //Requires Split
            if( length > 15 )
            {
                rendi   = 0;
                length  = 0;
                a = b   = 0;


                while( run_splitname )
                {
                    while(  ptoChar[ rendi ] != ' ' &&
                            ptoChar[ rendi ] != '\0' )
                    rendi++;

                    if( rendi - a < 18 )
                    {
                        b = rendi -1;//Set safe point
                        //safe a -> b
                        rendi++;

                        //Daca este ultimul rand
                        if( ptoChar[ rendi ] == '\0' )
                        {
                            for( q = a; q <= rendi; q ++ )
                            lines_name[tooltip_name_nr][ q - a ] = ptoChar[ q ];
                            tooltip_name_nr++;
                        }
                    }
                    else
                    {
                        //render a->b
                        for( q = a; q <= b; q ++ )
                        lines_name[tooltip_name_nr][ q - a ] = ptoChar[ q ];
                        lines_name[tooltip_name_nr][ b-a+1 ] = '\0';
                        tooltip_name_nr++;
                        //b <- rendi rendi se intoarce la b+1
                        rendi = b+2;
                        a = rendi;
                        b = a;
                    }
                    if( ptoChar[ rendi ] == '\0' )
                    run_splitname = false;
                }
            }
            else
            {
                tooltip_name_nr = 1;
                BAS_strcpy( lines_name[0],ptoChar  );
            }

            ///<-END

            ///*Rende name

            //Render
            for( rendi = 0; rendi < tooltip_name_nr; rendi++ )
            if( lines_name[ rendi ][0] != '\0' )
            {
                tooltip_lines_name[ rendi ] = TTF_RenderText_Blended( Font14,lines_name[ rendi ] ,WHITE );
            }
            ///<-END

            ///*Add Stats
            stringstream ssaux;

            //Find , Render: Subtype, Stat I

            ssaux.str("");
            switch( CurrentInvItm->type )
            {
                int tempi;

                //*Armor
                case INV_TYPE_ARMOR:
                tooltip_statsnr = 2;
                switch( CurrentInvItm->armor->type )
                {
                    case ARMOR_TYPE_CHEST:
                    ssaux<<"Chest";
                    break;
                    case ARMOR_TYPE_CLOAK:
                    ssaux<<"Cloak";
                    break;
                }
                tooltip_stats[0] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                ssaux.str( "" );
                ssaux<<"Defense: ";
                ssaux<<CurrentInvItm->armor->def;
                tooltip_stats[1] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );

                for( tempi = 0; tempi < CurrentInvItm->armor->statsnr; tempi++ )
                {
                    ssaux.str( "" );
                    //Health bonux
                    if( CurrentInvItm->armor->stats[ tempi ].type == "H"  )
                    ssaux<<"Health bonus: "<< CurrentInvItm->armor->stats[ tempi ].value;

                    tooltip_stats[ tooltip_statsnr + tempi ] = TTF_RenderText_Blended( Font14,
                                                               ssaux.str().c_str(),WHITE );
                }
                tooltip_statsnr += CurrentInvItm->armor->statsnr;
                break;
                //*Weapon
                case INV_TYPE_WEAPON:
                tooltip_statsnr = 2;
                ssaux.str( "" );
                ssaux<<"Damage: ";
                ssaux<<CurrentInvItm->weapon->dmg;
                tooltip_stats[0] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                ssaux.str( "" );
                ssaux<<"Sharp: ";
                ssaux<<CurrentInvItm->weapon->sharp;
                tooltip_stats[1] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                break;
                case INV_TYPE_BAG:
                tooltip_statsnr = 1;
                ssaux.str( "" );
                ssaux<<"Slots: ";
                ssaux<<CurrentInvItm->bag->slots;
                tooltip_stats[0] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                break;
                case INV_TYPE_SHIELD:
                tooltip_statsnr = 3;
                ssaux.str( "" );
                ssaux<<"Shield";
                tooltip_stats[0] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                ssaux.str( "" );
                ssaux<<"Defense: ";
                ssaux<<CurrentInvItm->shield->def;
                tooltip_stats[1] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                ssaux.str( "" );
                ssaux<<"Protection: ";
                ssaux<<CurrentInvItm->shield->protect;
                tooltip_stats[2] = TTF_RenderText_Blended( Font14,
                                   ssaux.str().c_str() ,WHITE );
                break;
                default:
                ssaux.str( "" );
                ssaux<<"!Unr Itm Type!";
                tooltip_statsnr = 1;
            }

            ///Description

            tooltip_desc_nr  = 0;
            length = 0;

            for( int tempi = 0; tempi < 5 ; tempi ++ )
            for( int tempj = 0; tempj < 20; tempj ++ )
            lines_name[tempi][tempj] = '\0';

            while( auxchar[ length ] != '\0' ) length++;

            //Requires Split
            if( length > 15 )
            {
                rendi   = 0;
                length  = 0;
                a = b   = 0;


                while( run_splitname )
                {
                    while(  auxchar[ rendi ] != ' ' &&
                            auxchar[ rendi ] != '\0' )
                    rendi++;

                    if( rendi - a < 18 )
                    {
                        b = rendi -1;//Set safe point
                        //safe a -> b
                        rendi++;

                        //Daca este ultimul rand
                        if(auxchar[ rendi ] == '\0' )
                        {
                            for( q = a; q <= rendi; q ++ )
                            lines_name[tooltip_desc_nr][ q - a ] = auxchar[ q ];
                            tooltip_desc_nr++;
                        }
                    }
                    else
                    {
                        //render a->b
                        for( q = a; q <= b; q ++ )
                        lines_name[tooltip_desc_nr][ q - a ] = auxchar[ q ];
                        lines_name[tooltip_desc_nr][b-a+1] = '\0';
                        tooltip_desc_nr++;
                        //b <- rendi rendi se intoarce la b+1
                        rendi = b+2;
                        a = rendi;
                        b = a;
                    }
                    if( auxchar[ rendi ] == '\0' )
                    run_splitname = false;
                }
            }
            else if( length != 0 )//Daca exista descriere
            {

                tooltip_desc_nr = 1;
                BAS_strcpy( lines_name[0],auxchar  );
            }


            //Render
            for( rendi = 0; rendi < tooltip_desc_nr; rendi++ )
            if( lines_name[ rendi ][0] != '\0' )
            {
                tooltip_lines_desc[ rendi ] = TTF_RenderText_Blended( Font14,lines_name[ rendi ] ,
                                              YELLOW_LIGHT );
            }

            ///*Render Canvas

            if( SDL_MUSTLOCK( tooltip_pal ) ) SDL_LockSurface( tooltip_pal );

            tooltip_canvas = SDL_CreateRGBSurface(0,
                             117,
                             14*(tooltip_name_nr + tooltip_statsnr + tooltip_desc_nr ) ,
                             32 ,
                             tooltip_pal->format->Rmask,
                             tooltip_pal->format->Gmask,
                             tooltip_pal->format->Bmask,0 );


            for( a = 0; a < tooltip_canvas->w;a++ )
            for( b = 0; b < tooltip_canvas->h;b++ )
            Pixels.putpix( tooltip_canvas,a , b,Pixels.getpix( tooltip_pal ,0 ,0 ) );

            for( a = 0; a < tooltip_canvas->w;a++ ){
                Pixels.putpix( tooltip_canvas,a , 0,Pixels.getpix( tooltip_pal ,1 ,0 ) );
                Pixels.putpix( tooltip_canvas,a , tooltip_canvas->h-1,Pixels.getpix( tooltip_pal ,1 ,0 ) );
            }
            for( b = 0; b < tooltip_canvas->h;b++ ){
                Pixels.putpix( tooltip_canvas,0 , b,Pixels.getpix( tooltip_pal ,1 ,0 ) );
                Pixels.putpix( tooltip_canvas,tooltip_canvas->w-1 , b,Pixels.getpix( tooltip_pal ,1 ,0 ) );
            }

            if( SDL_MUSTLOCK( tooltip_pal ) ) SDL_UnlockSurface( tooltip_pal );

            SDL_FreeSurface(tooltip_pal);

            *tt_canvas = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_canvas );
            if( tooltip_lines_name[0] != NULL )
            *tt_lines_name1 =  SDL_CreateTextureFromSurface( RENDER_MAIN,
                                                             tooltip_lines_name[0] );
            if( tooltip_lines_name[1] != NULL )
            *tt_lines_name2 =  SDL_CreateTextureFromSurface( RENDER_MAIN,
                                                             tooltip_lines_name[1] );
            if( tooltip_lines_name[2] != NULL )
            *tt_lines_name3 =  SDL_CreateTextureFromSurface( RENDER_MAIN,
                                                             tooltip_lines_name[2] );

            *tt_stats1 = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_stats[0] );
            *tt_stats2 = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_stats[1] );
            *tt_stats3 = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_stats[2] );
            *tt_stats4 = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_stats[3] );
            *tt_stats5 = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_stats[4] );
            *tt_stats6 = SDL_CreateTextureFromSurface( RENDER_MAIN, tooltip_stats[5] );

            *tt_lines_desc1 = SDL_CreateTextureFromSurface( RENDER_MAIN,tooltip_lines_desc[0] );
            *tt_lines_desc2 = SDL_CreateTextureFromSurface( RENDER_MAIN,tooltip_lines_desc[1] );
            *tt_lines_desc3 = SDL_CreateTextureFromSurface( RENDER_MAIN,tooltip_lines_desc[2] );
            *tt_lines_desc4 = SDL_CreateTextureFromSurface( RENDER_MAIN,tooltip_lines_desc[3] );
            *tt_lines_desc5 = SDL_CreateTextureFromSurface( RENDER_MAIN,tooltip_lines_desc[4] );
            *tt_lines_desc6 = SDL_CreateTextureFromSurface( RENDER_MAIN,tooltip_lines_desc[5] );

            tt_name_nr = tooltip_name_nr;
            tt_statsnr = tooltip_statsnr;
            tt_desc_nr = tooltip_desc_nr;
}





