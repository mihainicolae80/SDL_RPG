#ifndef PIXEL_MANIP_H_INCLUDED
#define PIXEL_MANIP_H_INCLUDED


#include "main_headers.h"
#include "basics.h"


extern class PixelManipulation{
private:
//SDL_Surface *model_cloak,*CLOAKS[2],*c_cloak[3];
SDL_Surface *Palette,*model_univers;
Uint32 mat_cloak[3][50][50];
public:
void init();
Uint32 getpix( SDL_Surface *surface,int x,int y );
void   putpix( SDL_Surface *surface,int x,int y,Uint32 pixel );
SDL_Surface *flip_img( SDL_Surface *surface,int direction );
SDL_Surface *rotate_img( SDL_Surface *surface,int direction ,bool animated,int frames );
SDL_Surface *CreatePixAnime( SDL_Surface* sample, SDL_Surface *model);
SDL_Surface *CreatePixAnime2( SDL_Surface* sample, SDL_Surface *model);
SDL_Surface *RenderLightMap( int nr );
SDL_Surface *RenderCorner( SDL_Surface *surface );
SDL_Surface *RenderNormalMap();
SDL_Surface *RenderForeLayerMap();
SDL_Surface *RenderMisc(int nr);
}Pixels;

void RenderTooltip( Inventory_item *CurrentInvItm,
                    SDL_Texture **tt_canvas,
                    SDL_Texture **tt_lines_name1,SDL_Texture **tt_lines_name2,SDL_Texture **tt_lines_name3,
                    SDL_Texture **tt_stats1,SDL_Texture **tt_stats2,SDL_Texture **tt_stats3,SDL_Texture **tt_stats4,
                    SDL_Texture **tt_stats5,SDL_Texture **tt_stats6,
                    SDL_Texture **tt_lines_desc1, SDL_Texture **tt_lines_desc2, SDL_Texture **tt_lines_desc3,
                    SDL_Texture **tt_lines_desc4, SDL_Texture **tt_lines_desc5, SDL_Texture **tt_lines_desc6,
                    int &tt_name_nr,int &tt_statsnr,int &tt_desc_nr
                    );


#endif // PIXEL_MANIP_H_INCLUDED
