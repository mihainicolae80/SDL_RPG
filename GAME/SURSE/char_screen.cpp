#include "main_headers.h"


Character char_screen(){
SDL_Surface *background = LoadSurf("textures//charscreen//charscreen_background.png");
///SDL_Surface *logo = LoadSurf("textures//charscreen//KOTLogo.png");
SDL_Surface *icon[4];
//Surfaces pt create char
SDL_Surface *sCover1,*sClessBook,*sPageLeft,*sCover1Trans[6],*sTxtOnPage[6],*sPagePic[8],*sCorners[4],*sBox[4],*sShadows[5];
sBox[3] = NULL;
icon[0] = LoadSurf("textures//charscreen//charscreen_icon0.png");
icon[1] = LoadSurf("textures//charscreen//charscreen_iconW.png");
icon[2] = LoadSurf("textures//charscreen//charscreen_iconR.png");
icon[3] = LoadSurf("textures//charscreen//charscreen_iconL.png");
SDL_Surface *frame = LoadSurf("textures//charscreen//charscreen_frame.png");
SDL_Surface *frame2 = LoadSurf("textures//charscreen//charscreen_frame_g.png");
SDL_Surface *Cframe[5];
///SDL_Surface *txtName[5];
///SDL_Surface *txtLvl[5];
bool exit_char = false,make_new_char = false;
SDL_Rect Rframe[5];
ifstream savef;
///SDL_Surface *txtFPS = NULL;
int fraps = 1,BG_X = 0,i,hero_origin = 0,hero_reason = 0,hero_perspective = 0;
Uint32 secund_counter = SDL_GetTicks();
Uint32 fps_show;
string hero_name = "";

for(i = 0; i<5; i++)
{
    SDL_SetSurfaceAlphaMod( frame2,50 );
    SDL_SetSurfaceAlphaMod( frame,200 );
}
struct BUTTON {
int x,y,h,w;
bool render;
SDL_Surface *txtButton;
SDL_Surface *frame;
}Button[5];

Rframe[0].h = Rframe[1].h = Rframe[2].h = Rframe[3].h = Rframe[4].h = 403;
Rframe[0].w = Rframe[1].w = Rframe[2].w = Rframe[3].w = Rframe[4].w = 191;


for( i = 0 ; i < 5;i++ )
{
    Rframe[i].h = 403;
    Rframe[i].w = 191;
    Rframe[i].y = SCREEN_HEIGHT/2.56;//400;
    Rframe[i].x = 30 + ((SCREEN_WIDTH-30)/5)*i;  // 30 + 250*i;
}


//Init buttons
for(i = 0; i<5; i++){
Button[i].x = (Rframe[i].x + 10);
Button[i].y = (Rframe[i].y + 350);
Button[i].w = 136;
Button[i].h = 40;
Button[i].frame = LoadSurf("textures//charscreen//charscreen_buttonframe.png");
}


Character Player[5];
for(i=0;i<5;i++){


switch(i){
case 0:
savef.open("saves//save1.sav");
break;
case 1:
savef.open("saves//save2.sav");
break;
case 2:
savef.open("saves//save3.sav");
break;
case 3:
savef.open("saves//save4.sav");
break;
case 4:
savef.open("saves//save5.sav");
break;
}

savef>>Player[i].created;
//get info from files
if(Player[i].created){
savef>>Player[i].name>>Player[i].lvl>>Player[i].Class;

//Render name and lvl
///txtName[i] = TTF_RenderText_Solid(font,Player[i].name.c_str(),textColor);
stringstream lvl;
lvl <<Player[i].lvl;
///txtLvl[i] = TTF_RenderText_Solid(font,lvl.str().c_str(),BLACK);
///Button[i].txtButton = TTF_RenderText_Solid(font,"Chose",textColor);
}
else{
///txtName[i] = TTF_RenderText_Solid(font,"New",textColor);
///txtLvl[i] = TTF_RenderText_Solid(font,"Character",textColor);
///Button[i].txtButton = TTF_RenderText_Solid(font,"Create",textColor);
}
savef.close();
}
//LOOP!
while(!exit_char){
BG_X -= 2;
//Get time
fps_show = SDL_GetTicks();
fraps++;
    //Handle events
    while(SDL_PollEvent(&event))
    {
        int x,y;
        if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) )
        exit_char = true;
        else if( event.type == SDL_MOUSEMOTION ){

        x = event.motion.x;
        y = event.motion.y;
        for(i = 0 ; i<5 ; i++){
        //Verifica daca cursorul este pe un frame
            if( (x>Rframe[i].x)&&(x<Rframe[i].x+Rframe[i].w)&&(y>Rframe[i].y)&&(y<Rframe[i].y+Rframe[i].h) )
            {
                Cframe[i] = frame;
                Button[i].render = true;
            }
            else
            {
                Cframe[i] = frame2;
                Button[i].render = false;
            }

        }
        }
        //Click pe button
        else if( event.type == SDL_MOUSEBUTTONDOWN )
        {
            x = event.button.x;
            y = event.button.y;
            for(i = 0 ; i < 5; i++ )
            {
                if(Button[i].render&&(x>Button[i].x)&&(x<Button[i].x+Button[i].w)&&(y>Button[i].y) && ( y<Button[i].y+Button[i].h ))
                {
                    if(Player[i].created)
                    {
                        exit_char = true;

                        //Load player
                        stringstream filename;
                        filename<<"saves//save"<<i+1<<".sav";
                        savef.open(filename.str().c_str());

                        //Loading din fisier pana la spells exclusiv
                        Player[i].file_nr = i;
                        savef>>Player[i].created>>Player[i].name>>Player[i].lvl>>Player[i].Class>>Player[i].rBody.x>>Player[i].rBody.y>>Player[i].czone>>Player[i].health_current >>Player[i].health_max ;
                        savef>>Player[i].def_base>>Player[i].cpower>>Player[i].power>>Player[i].exp>>Player[i].exp_ding>>Player[i].dmg_base >>Player[i].gold;
                        savef.close();

                        return Player[i];
                    }
                    else//Create char
                    {
                        //init
                        bool CreateChar = true,drag = false,input = false,reveal[4],fade_img[3];
                        int stage = 0,page = 0,fade_val[3];
                        fade_val[0] = fade_val[1] = fade_val[2] = 255;
                        fade_img[0] = fade_img[1] = fade_img[2] = false;
                        reveal[0] = reveal[1] = reveal[2] = false;
                        SDL_Rect rNext,rLeft,rRight,rLeft2,rRight2,rBox[4],rSecBox[4];

                        rNext.w = 40;
                        rNext.h = 50;
                        rNext.x = SCREEN_WIDTH/2 + 430; //+ sClessBook->w - 100;
                        rNext.y = SCREEN_HEIGHT/2 - 75;// - sClessBook->h/2 - 72;
                        rLeft.x = rLeft.y = rRight.x = rRight.y = rLeft2.x = rLeft2.y = rRight2.x = rRight2.y = 0;
                        rLeft.w = rRight.w = 511;
                        rLeft.h = rRight.h = rLeft2.h = rRight2.h = 625;
                        rLeft2.w = rRight2.w = 0;

                        //load
                        sCover1 = LoadSurf( "textures//charscreen//Create Char//Cover.png" );
                        sClessBook = LoadSurf( "textures//charscreen//Create Char//CoverlessBook.png" );
                        sPageLeft = LoadSurf( "textures//charscreen//Create Char//PageLeft.png" );
                        sCover1Trans[0] = LoadSurf( "textures//charscreen//Create Char//CoverTrans1.png" );
                        sCover1Trans[1] = LoadSurf( "textures//charscreen//Create Char//CoverTrans2.png" );
                        sCover1Trans[2] = LoadSurf( "textures//charscreen//Create Char//CoverTrans3.png" );
                        sCover1Trans[3] = LoadSurf( "textures//charscreen//Create Char//CoverTrans4.png" );
                        sCover1Trans[4] = LoadSurf( "textures//charscreen//Create Char//CoverTrans5.png" );
                        sCover1Trans[5] = LoadSurf( "textures//charscreen//Create Char//CoverTrans6.png" );
                        sTxtOnPage[0] = LoadSurf( "textures//charscreen//Create Char//texts left//1_1.png" );
                        sTxtOnPage[1] = LoadSurf( "textures//charscreen//Create Char//texts left//1_2.png" );
                        sTxtOnPage[2] = LoadSurf( "textures//charscreen//Create Char//texts left//1_3.png" );
                        sTxtOnPage[3] = LoadSurf( "textures//charscreen//Create Char//texts left//1_4.png" );
                        sTxtOnPage[4] = LoadSurf( "textures//charscreen//Create Char//texts right//2_1.png" );
                        sTxtOnPage[5] = LoadSurf( "textures//charscreen//Create Char//texts right//3_4.png" );
                        sCorners[0] = LoadSurf( "textures//charscreen//Create Char//texts right//c_1.png" );
                        sCorners[1] = LoadSurf( "textures//charscreen//Create Char//texts right//c_2.png" );
                        sCorners[2] = LoadSurf( "textures//charscreen//Create Char//texts right//c_3.png" );

                        sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_0.png" );
                        sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_1.png" );
                        sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_2.png" );
                        sPagePic[3] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_3.png" );
                        sPagePic[4] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_2_0.png" );
                        sShadows[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5.png" );
                        sShadows[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_1.png" );
                        sShadows[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_2.png" );
                        sShadows[3] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_3.png" );
                        sShadows[4] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_4.png" );
                        while(CreateChar)
                        {
                            //events
                            while( SDL_PollEvent(&event) )
                            {
                                if( event.type == SDL_QUIT || ( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) )
                                {
                                    CreateChar = false;
                                    exit_char = true;
                                }
                                else if( event.type == SDL_MOUSEBUTTONDOWN )
                                {
                                    x = event.button.x;
                                    y = event.button.y;

                                    if( ( x > rNext.x && x < (rNext.x + rNext.w) && y > rNext.y && y < ( rNext.y + rNext.h ) ) )
                                    {
                                        drag = true;
                                    }
                                    if( page == 2 )
                                    {
                                        if( ( x > rSecBox[0].x && x < (rSecBox[0].x + rSecBox[0].w) && y > rSecBox[0].y && y < ( rSecBox[0].y + rSecBox[0].h ) ) )
                                        {
                                            if( stage == 1 )
                                            {
                                                reveal[0] = true;
                                                stage = 2;
                                                rSecBox[0].x = SCREEN_WIDTH/2 +10;
                                                rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 30;
                                                rSecBox[0].w = 90 ;
                                                rSecBox[0].h = 160 ;
                                                rSecBox[1].x = SCREEN_WIDTH/2 + 150;
                                                rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[1].w = 121 ;
                                                rSecBox[1].h = 226 ;
                                                rSecBox[2].x = SCREEN_WIDTH/2 + 300;
                                                rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[2].w = 170 ;
                                                rSecBox[2].h = 147 ;
                                                rSecBox[3].x = SCREEN_WIDTH/2 + 170;
                                                rSecBox[3].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 370;
                                                rSecBox[3].w = 170 ;
                                                rSecBox[3].h = 238 ;
                                                if( sPagePic[0] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[0] = NULL;
                                                }
                                                if( sPagePic[1] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[1] = NULL;
                                                }
                                                if( sPagePic[2] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[2] = NULL;
                                                }
                                                if( sPagePic[3] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[3] = NULL;
                                                }
                                                if( sPagePic[4] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[4] = NULL;
                                                }
                                                sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_0.png" );
                                                sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_1.png" );
                                                sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_2.png" );
                                                sPagePic[3] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_3.png" );
                                                sPagePic[4] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_2_0.png" );
                                            }
                                            else if( stage == 2 )
                                            {
                                                hero_origin = 1;
                                                reveal[1] = true;
                                                reveal[2] = false;
                                                if( sTxtOnPage[2] != NULL )
                                                SDL_FreeSurface( sTxtOnPage[2] );
                                                sTxtOnPage[2] = LoadSurf( "textures//charscreen//Create Char//texts left//3_2_1.png" );
                                                SDL_FreeSurface ( sBox[1] );
                                                ///sBox[1] = TTF_RenderText_Solid( font14,"PEASANT",RED );
                                                if( sBox[2] != NULL )
                                                SDL_FreeSurface( sBox[2] );
                                                ///sBox[2] = TTF_RenderText_Solid( font14,"<DECISION>",RED );
                                                if( sBox[3] != NULL )
                                                {
                                                    SDL_FreeSurface( sBox[3] );
                                                }
                                                ///sBox[3] = TTF_RenderText_Solid( font14,"<PERSPECTIVE>",RED );
                                                fade_img[2] = true;
                                                fade_img[0] = fade_img[1] = false;
                                                fade_val[2] = 55;
                                                fade_val[0] = fade_val[1] = 255;
                                                SDL_SetSurfaceAlphaMod( sShadows[1],fade_val[0] );
                                                SDL_SetSurfaceAlphaMod( sShadows[2],fade_val[1] );
                                                SDL_SetSurfaceAlphaMod( sShadows[3],fade_val[2] );

                                                hero_perspective = 0;
                                            }
                                            else if( stage == 3 )
                                            {

                                                hero_reason = 1;
                                                stage = 4;
                                                if( reveal[2] == false )
                                                reveal[2] = true;
                                                if( sBox[2] != NULL )
                                                SDL_FreeSurface( sBox[2] );
                                                /**
                                                if( hero_origin == 1 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Peasant I",RED );
                                                else if( hero_origin == 2 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Noble I",RED );
                                                else if( hero_origin == 3 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Merchant I",RED );
                                                **/
                                                rSecBox[0].x = SCREEN_WIDTH/2 +10;
                                                rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 30;
                                                rSecBox[0].w = 155 ;
                                                rSecBox[0].h = 185 ;
                                                rSecBox[1].x = SCREEN_WIDTH/2 + 150;
                                                rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[1].w = 155 ;
                                                rSecBox[1].h = 185 ;
                                                rSecBox[2].x = SCREEN_WIDTH/2 + 300;
                                                rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[2].w = 155 ;
                                                rSecBox[2].h = 185 ;
                                                rSecBox[3].x = SCREEN_WIDTH/2 + 120;
                                                rSecBox[3].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 260;
                                                rSecBox[3].w = 235 ;
                                                rSecBox[3].h = 215 ;

                                            }
                                            else if( stage == 4 )
                                            {
                                                if( hero_origin == 1 || hero_origin == 3 )
                                                {
                                                    hero_perspective = 1;
                                                    fade_img[1] = fade_img[2] = true;
                                                    fade_img[0] = false;
                                                    SDL_FreeSurface( sBox[3] );sBox[3] = NULL;
                                                    ///sBox[3] = TTF_RenderText_Solid( font14,"Warrior",RED );
                                                }
                                            }

                                        }
                                        else if( ( x > rSecBox[1].x && x < (rSecBox[1].x + rSecBox[1].w) && y > rSecBox[1].y && y < ( rSecBox[1].y + rSecBox[1].h ) ) )
                                        {
                                            if( stage == 2 )
                                            {
                                                hero_perspective = 0;
                                                hero_origin = 2;
                                                reveal[1] = true;
                                                reveal[2] = false;
                                                if( sTxtOnPage[2] != NULL )
                                                SDL_FreeSurface( sTxtOnPage[2] );
                                                sTxtOnPage[2] = LoadSurf( "textures//charscreen//Create Char//texts left//3_2_2.png" );
                                                SDL_FreeSurface ( sBox[1] );
                                                //sBox[1] = TTF_RenderText_Solid( font14,"NOBLE",RED );
                                                ///if( sBox[2] != NULL )
                                                ///SDL_FreeSurface( sBox[2] );
                                                ///sBox[2] = TTF_RenderText_Solid( font14,"<DECISION>",RED );
                                                if( sBox[3] != NULL )
                                                {
                                                    SDL_FreeSurface( sBox[3] );sBox[3] = NULL;
                                                }
                                                ///sBox[3] = TTF_RenderText_Solid( font14,"<PERSPECTIVE>",RED );
                                                fade_img[0] = true;
                                                fade_img[1] = fade_img[2] = false;
                                                fade_val[0] = 50;
                                                fade_val[1] = fade_val[2] = 255;
                                                SDL_SetSurfaceAlphaMod( sShadows[1],fade_val[0] );
                                                SDL_SetSurfaceAlphaMod( sShadows[2],fade_val[1] );
                                                SDL_SetSurfaceAlphaMod( sShadows[3],fade_val[2] );
                                            }
                                            else if( stage == 3 )
                                            {
                                                hero_reason = 1;
                                                stage = 4;
                                                if( reveal[2] == false )
                                                reveal[2] = true;
                                                if( sBox[2] != NULL )
                                                SDL_FreeSurface( sBox[2] );
                                                /**
                                                if( hero_origin == 1 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Peasant II",RED );
                                                else if( hero_origin == 2 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Noble II",RED );
                                                else if( hero_origin == 3 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Merchant II",RED );
                                                **/
                                                if( sPagePic[0] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                }
                                                if( sPagePic[1] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[1] );
                                                }
                                                if( sPagePic[2] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[2] );
                                                }
                                                if( sPagePic[3] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[3] );
                                                }
                                                sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5.png" );
                                                sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_1.png" );
                                                sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_2.png" );
                                                sPagePic[3] = LoadSurf( "textures//charscreen//Create Char//texts right//3_5_3.png" );

                                                rSecBox[0].x = SCREEN_WIDTH/2 +10;
                                                rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 30;
                                                rSecBox[0].w = 155 ;
                                                rSecBox[0].h = 185 ;
                                                rSecBox[1].x = SCREEN_WIDTH/2 + 150;
                                                rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[1].w = 155 ;
                                                rSecBox[1].h = 185 ;
                                                rSecBox[2].x = SCREEN_WIDTH/2 + 300;
                                                rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[2].w = 155 ;
                                                rSecBox[2].h = 185 ;
                                            }
                                            else if( stage == 4 )
                                            {
                                                if( hero_origin == 1 || hero_origin == 2 )
                                                {
                                                    hero_perspective = 2;
                                                    fade_img[0] = fade_img[2] = true;
                                                    fade_img[1] = false;
                                                    SDL_FreeSurface( sBox[3] );sBox[3] = NULL;
                                                    ///sBox[3] = TTF_RenderText_Solid( font14,"Shadow",RED );
                                                }
                                            }
                                        }
                                        else if( ( x > rSecBox[2].x && x < (rSecBox[2].x + rSecBox[2].w) && y > rSecBox[2].y && y < ( rSecBox[2].y + rSecBox[2].h ) ) )
                                        {
                                            if( stage == 2 )
                                            {
                                                hero_origin = 3;
                                                hero_perspective = 0;
                                                reveal[1] = true;
                                                reveal[2] = false;
                                                if( sTxtOnPage[2] != NULL )
                                                SDL_FreeSurface( sTxtOnPage[2] );
                                                sTxtOnPage[2] = LoadSurf( "textures//charscreen//Create Char//texts left//3_2_3.png" );
                                                SDL_FreeSurface ( sBox[1] );sBox[1] = NULL;
                                                ///sBox[1] = TTF_RenderText_Solid( font14,"MERCHANT",RED );
                                                if( sBox[2] != NULL )
                                                SDL_FreeSurface( sBox[2] );sBox[2] = NULL;
                                                ///sBox[2] = TTF_RenderText_Solid( font14,"<DECISION>",RED );
                                                fade_img[1] = true;
                                                fade_img[0] = fade_img[2] = false;
                                                fade_val[1] = 50;
                                                fade_val[0] = fade_val[2] = 255;
                                                SDL_SetSurfaceAlphaMod( sShadows[1],fade_val[0] );
                                                SDL_SetSurfaceAlphaMod( sShadows[2],fade_val[1] );
                                                SDL_SetSurfaceAlphaMod( sShadows[2],fade_val[2] );
                                            }
                                            else if( stage == 3 )
                                            {
                                                hero_reason = 1;
                                                stage = 4;
                                                if( reveal[2] == false )
                                                reveal[2] = true;
                                                if( sBox[2] != NULL )
                                                SDL_FreeSurface( sBox[2] );sBox[2] = NULL;
                                                /**
                                                if( hero_origin == 1 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Peasant III",RED );
                                                else if( hero_origin == 2 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Noble III",RED );
                                                else if( hero_origin == 3 )
                                                sBox[2] = TTF_RenderText_Solid( font14,"Merchant III",RED );
                                                **/
                                                //SDL_FreeSurface( sBox[3] );
                                                //sBox[3] = TTF_RenderText_Solid(font14,"<PERSPECTIVE>",RED);
                                                rSecBox[0].x = SCREEN_WIDTH/2 +10;
                                                rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 30;
                                                rSecBox[0].w = 155 ;
                                                rSecBox[0].h = 185 ;
                                                rSecBox[1].x = SCREEN_WIDTH/2 + 150;
                                                rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[1].w = 155 ;
                                                rSecBox[1].h = 185 ;
                                                rSecBox[2].x = SCREEN_WIDTH/2 + 300;
                                                rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[2].w = 155 ;
                                                rSecBox[2].h = 185 ;
                                            }
                                            else if( stage == 4 )
                                            {
                                                if( hero_origin == 2 || hero_origin == 3 )
                                                {
                                                    hero_perspective = 3;
                                                    fade_img[0] = fade_img[1] = true;
                                                    fade_img[2] = false;
                                                    SDL_FreeSurface( sBox[3] );sBox[3] = NULL;
                                                    ///sBox[3] = TTF_RenderText_Solid( font14,"Battlewizard",RED );
                                                }
                                            }
                                        }
                                        else if( x > rBox[0].x && x < (rBox[0].x + rBox[0].w) && y > rBox[0].y && y < ( rBox[0].y + rBox[0].h ) )
                                        {
                                            stage = 1;
                                            rSecBox[0].x = SCREEN_WIDTH /2 + 185 ;
                                            rSecBox[0].y = SCREEN_HEIGHT/2 - 91 ;
                                            rSecBox[0].w = 100;
                                            rSecBox[0].h = 35;
                                        }
                                        else if( x > rBox[1].x && x < (rBox[1].x + rBox[1].w) && y > rBox[1].y && y < ( rBox[1].y + rBox[1].h ) && reveal[0] )
                                        {
                                            stage = 2;
                                            //initialize
                                            rSecBox[0].x = SCREEN_WIDTH/2 +10;
                                            rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 30;
                                            rSecBox[0].w = 90 ;
                                            rSecBox[0].h = 160 ;
                                            rSecBox[1].x = SCREEN_WIDTH/2 + 150;
                                            rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                            rSecBox[1].w = 121 ;
                                            rSecBox[1].h = 226;
                                            rSecBox[2].x = SCREEN_WIDTH/2 + 300;
                                            rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                            rSecBox[2].w = 170 ;
                                            rSecBox[2].h = 147 ;
                                            rSecBox[3].x = SCREEN_WIDTH/2 + 170;
                                            rSecBox[3].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 370;
                                            rSecBox[3].w = 170 ;
                                            rSecBox[3].h = 238 ;

                                            if( sPagePic[0] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[0] = NULL;
                                                }
                                                if( sPagePic[1] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[1] = NULL;
                                                }
                                                if( sPagePic[2] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[2] = NULL;
                                                }
                                                if( sPagePic[3] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[3] = NULL;
                                                }
                                                if( sPagePic[4] != NULL )
                                                {
                                                    SDL_FreeSurface( sPagePic[0] );
                                                    sPagePic[4] = NULL;
                                                }

                                                sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_0.png" );
                                                sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_1.png" );
                                                sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_2.png" );
                                                sPagePic[3] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_1_3.png" );
                                                sPagePic[4] = LoadSurf( "textures//charscreen//Create Char//texts right//3_2_2_0.png" );

                                        }
                                        else if( ( x > rBox[2].x && x < (rBox[2].x + rBox[2].w) && y > rBox[2].y && y < ( rBox[2].y + rBox[2].h ) && reveal[1] ) || ( (( x > rSecBox[3].x && x < (rSecBox[3].x + rSecBox[3].w) && y > rSecBox[3].y && y < ( rSecBox[3].y + rSecBox[3].h ) ) && stage == 2 && reveal[1] ) ) )
                                        {
                                            stage = 3;

                                                rSecBox[0].w = rSecBox[1].w = rSecBox[2].w = 400;
                                                rSecBox[0].h = rSecBox[1].h = rSecBox[2].h = 140;
                                                rSecBox[0].x = SCREEN_WIDTH/2 + 30;
                                                rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 130;
                                                rSecBox[1].x = SCREEN_WIDTH/2 + 40;
                                                rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 290;
                                                rSecBox[2].x = SCREEN_WIDTH/2 + 50;
                                                rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 450;

                                            if( hero_origin == 1 )
                                            {
                                                sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_1_1.png" );
                                                sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_1_2.png" );
                                                sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_1_3.png" );
                                            }
                                            else if( hero_origin == 2 )
                                            {
                                                sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_2_1.png" );
                                                sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_2_2.png" );
                                                sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_2_3.png" );
                                            }
                                            else if( hero_origin == 3 )
                                            {
                                                sPagePic[0] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_3_1.png" );
                                                sPagePic[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_3_2.png" );
                                                sPagePic[2] = LoadSurf( "textures//charscreen//Create Char//texts right//3_3_3_3.png" );
                                            }
                                        }
                                        else if(  x > rSecBox[3].x && x < (rSecBox[3].x + rSecBox[3].w) && y > rSecBox[3].y && y < ( rSecBox[3].y + rSecBox[3].h ) && page ==2 && stage == 4 && hero_perspective != 0 )
                                        {
                                            CreateChar = false;
                                            make_new_char = true;
                                        }
                                        else if( x > rBox[3].x && x < (rBox[3].x + rBox[3].w) && y > rBox[3].y && y < ( rBox[3].y + rBox[3].h ) && reveal[2]  )
                                        {
                                            stage = 4;
                                                rSecBox[0].x = SCREEN_WIDTH/2 +10;
                                                rSecBox[0].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 30;
                                                rSecBox[0].w = 155 ;
                                                rSecBox[0].h = 185 ;
                                                rSecBox[1].x = SCREEN_WIDTH/2 + 150;
                                                rSecBox[1].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[1].w = 155 ;
                                                rSecBox[1].h = 185 ;
                                                rSecBox[2].x = SCREEN_WIDTH/2 + 300;
                                                rSecBox[2].y = SCREEN_HEIGHT/2 - sClessBook->h/2 + 40;
                                                rSecBox[2].w = 155 ;
                                                rSecBox[2].h = 185 ;

                                                if( hero_perspective == 0 )
                                                {
                                                    if( hero_origin == 1 )
                                                    {
                                                        fade_img[2] = true;
                                                        fade_val[2] = 55;
                                                        fade_img[0] = fade_img[1] = false;
                                                        fade_val[0] = fade_val[1] = 255;
                                                    }
                                                    else if( hero_origin == 2 )
                                                    {
                                                        fade_img[0] = true;
                                                        fade_val[0] = 55;
                                                        fade_img[2] = fade_img[1] = false;
                                                        fade_val[2] = fade_val[1] = 255;
                                                    }
                                                    else if( hero_origin == 3 )
                                                    {
                                                        fade_img[1] = true;
                                                        fade_val[1] = 55;
                                                        fade_img[0] = fade_img[2] = false;
                                                        fade_val[0] = fade_val[2] = 255;
                                                    }
                                                }
                                                else
                                                {
                                                    if( hero_perspective == 1 )
                                                    {
                                                        fade_img[0] = false;
                                                        fade_val[0] = 255;
                                                        fade_img[1] = fade_img[2] = true;
                                                        fade_val[1] = fade_val[2] = 55;
                                                    }
                                                    else if( hero_perspective == 2 )
                                                    {
                                                        fade_img[1] = false;
                                                        fade_val[1] = 255;
                                                        fade_img[0] = fade_img[2] = true;
                                                        fade_val[0] = fade_val[2] = 55;
                                                    }
                                                    else if( hero_perspective == 3 )
                                                    {
                                                        fade_img[2] = false;
                                                        fade_val[2] = 255;
                                                        fade_img[0] = fade_img[1] = true;
                                                        fade_val[0] = fade_val[1] = 55;
                                                    }
                                                }


                                        }
                                    }
                                }
                                else if( event.type == SDL_MOUSEBUTTONUP )
                                {
                                    if( drag == true )
                                    {
                                        drag = false;
                                        if( page == 0 )
                                        {
                                            if( stage != 7 )
                                            stage = 0;
                                            else
                                            {
                                                stage = 0;
                                                page = 1;
                                                SDL_FreeSurface( sTxtOnPage[0] );
                                                sTxtOnPage[0] = LoadSurf( "textures//charscreen//Create Char//texts left//1_4.png" );
                                                SDL_FreeSurface( sTxtOnPage[1] );
                                                SDL_FreeSurface( sTxtOnPage[2] );
                                                SDL_FreeSurface( sTxtOnPage[3] );
                                                sTxtOnPage[1] = LoadSurf( "textures//charscreen//Create Char//texts right//2_1.png" );
                                                sTxtOnPage[2] = LoadSurf( "textures//charscreen//Create Char//texts left//3_1.png" );
                                                sTxtOnPage[3] = LoadSurf( "textures//charscreen//Create Char//texts right//3_1.png" );
                                                rNext.x = SCREEN_WIDTH/2 + 460; //+ sClessBook->w - 100;
                                                rNext.y = SCREEN_HEIGHT/2 + 200;
                                            }
                                        }
                                    }
                                }
                                else if( event.type == SDL_MOUSEMOTION )
                                {
                                    x = event.motion.x;
                                    y = event.motion.y;
                                    if( drag )
                                    {
                                        if( page == 0 )
                                        {
                                        if( (rNext.x - x) > 143 * 0 && (rNext.x - x) < 143 )
                                        {
                                            stage = 1;
                                        }
                                        else if( (rNext.x - x) > 143 * 1 && (rNext.x - x) < 143 * 2 )
                                        {
                                            stage = 2;
                                        }
                                        else if( (rNext.x - x) > 143 * 2 && (rNext.x - x) < 143 * 3 )
                                        {
                                            stage = 3;
                                        }
                                        else if( (rNext.x - x) > 143 * 3 && (rNext.x - x) < 143 * 4 )
                                        {
                                            stage = 4;
                                        }
                                        else if( (rNext.x - x) > 143 * 4 && (rNext.x - x) < 143 * 5 )
                                        {
                                            stage = 5;
                                        }
                                        else if( (rNext.x - x) > 143 * 5 && (rNext.x - x) < 143 * 6 )
                                        {
                                            stage = 6;
                                        }
                                        else if( (rNext.x - x) > 143 * 6 && (rNext.x - x) < 143 * 7 )
                                        {
                                            stage = 7;
                                        }
                                        }
                                        else if( page == 1 || page == 2 )
                                        {
                                            if( drag == true )
                                            {
                                                if( rNext.x - x > 0 )
                                                {
                                                    if( (rNext.x - x) < 420 )
                                                    {
                                                        rRight.w = 420 - (rNext.x - x);
                                                        rRight2.w = (rNext.x - x);// - rRight.w;
                                                        rRight2.x = x - ( SCREEN_WIDTH/2 - 50 ) ;
                                                    }
                                                    else
                                                    {
                                                        rRight.w = 1;
                                                        rRight2.w = 511;
                                                        rRight2.x = 0;
                                                        if( (rNext.x - x) < 760 )
                                                        {
                                                            rLeft.w = 340 - (rNext.x - x - 500);
                                                            rLeft2.w = (rNext.x - x - 470 );
                                                            rLeft2.x = 470 - rLeft2.w;
                                                        }
                                                        else if( page == 1 ) // urmatoarea pagina
                                                        {
                                                            rLeft.w = 1;
                                                            drag = false;
                                                            page = 2;
                                                            stage = 1;
                                                            SDL_FreeSurface( sTxtOnPage[0] );
                                                            SDL_FreeSurface( sTxtOnPage[1] );
                                                            sTxtOnPage[0] = LoadSurf( "textures//charscreen//Create Char//texts left//3_1.png" );
                                                            sTxtOnPage[1] = LoadSurf( "textures//charscreen//Create Char//texts right//3_1.png" );
                                                            SDL_FreeSurface( sTxtOnPage[2] );
                                                            sTxtOnPage[2] = NULL;
                                                            SDL_FreeSurface( sTxtOnPage[3] );
                                                            sTxtOnPage[3] = LoadSurf( "textures//charscreen//Create Char//texts left//3_3.png" );
                                                            /*
                                                            sBox[0] = TTF_RenderText_Solid(font14,"Enter your name",RED);
                                                            sBox[1] = TTF_RenderText_Solid(font14,"<ORIGIN>",RED);
                                                            sBox[2] = TTF_RenderText_Solid(font14,"<DECISION>",RED);
                                                            sBox[3] = TTF_RenderText_Solid(font14,"<PERSPECTIVE>",RED);
                                                            */
                                                            rBox[0].w = 92;
                                                            rBox[0].h = 15;
                                                            rBox[0].x = SCREEN_WIDTH/2 - 380;
                                                            rBox[0].y = SCREEN_HEIGHT/2 - 265;
                                                            rBox[1].w = 50;
                                                            rBox[1].h = 17;
                                                            rBox[1].x = SCREEN_WIDTH/2 - 450  ;
                                                            rBox[1].y = SCREEN_HEIGHT/2 - 247 ;
                                                            rBox[2].w = 50;//neinitializat inca
                                                            rBox[2].h = 17;
                                                            rBox[2].x = SCREEN_WIDTH/2 - 450  ;
                                                            rBox[2].y = SCREEN_HEIGHT/2 + 25 ;
                                                            rBox[3].x = SCREEN_WIDTH/2 - 340;
                                                            rBox[3].y = SCREEN_HEIGHT/2 + 105;
                                                            rBox[3].w = 115;
                                                            rBox[3].h = 20;
                                                            rSecBox[0].x = SCREEN_WIDTH /2 + 185 ;
                                                            rSecBox[0].y = SCREEN_HEIGHT/2 - 91 ;
                                                            rSecBox[0].w = 100;
                                                            rSecBox[0].h = 35;
                                                            rLeft.w = rRight.w = 511;
                                                            rLeft.h = rRight.h = 625;
                                                            rLeft.x = rRight.x = 0;
                                                            rLeft2.w = rRight2.w = 0;
                                                            rLeft2.h = rRight2.h = 0;
                                                            rLeft2.x = rRight2.x = 0;
                                                            ///SDL_EnableUNICODE( SDL_ENABLE );
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                else if( event.type == SDL_KEYDOWN )
                                {
                                    if( page == 2 && stage == 1 )
                                    {
                                        if( hero_name.length() <= 20 )
                                        {
                                            /*
                                            if( event.key.keysym.unicode >= (Uint16)'A' && event.key.keysym.unicode <= (Uint16)'Z' )
                                            {
                                                hero_name += (char)event.key.keysym.unicode;
                                                input = true;
                                            }
                                            else if( event.key.keysym.unicode >= (Uint16)'a' && event.key.keysym.unicode <= (Uint16)'z' )
                                            {
                                                hero_name += (char)event.key.keysym.unicode;
                                                input = true;
                                            }
                                            */

                                        }
                                        if( event.key.keysym.sym == SDLK_BACKSPACE && hero_name.length() != 0 )
                                        {
                                            hero_name.erase( hero_name.length() - 1 );
                                            input = true;
                                        }
                                    }
                                }
                            }

                            //logics
                            if( input )
                            {
                                SDL_FreeSurface( sBox[0] );
                                ///sBox[0] = TTF_RenderText_Solid( font14,hero_name.c_str(),RED);
                            }
                            if( stage == 4 )
                            {
                                if( fade_img[0] == true && fade_val[0] > 50 )
                                {
                                    fade_val[0] -= 5;
                                    SDL_SetSurfaceAlphaMod( sShadows[1],fade_val[0] );
                                }
                                else if( fade_img[0] == false && fade_val[0] < 255 )
                                {
                                    fade_val[0] += 5;
                                    SDL_SetSurfaceAlphaMod( sShadows[1],fade_val[0] );
                                }

                                if( fade_img[1] == true && fade_val[1] > 50 )
                                {
                                    fade_val[1] -= 5;
                                    SDL_SetSurfaceAlphaMod( sShadows[2],fade_val[1] );
                                }
                                else if( fade_img[1] == false && fade_val[1] < 255 )
                                {
                                    fade_val[1] += 5;
                                    SDL_SetSurfaceAlphaMod( sShadows[2],fade_val[1] );
                                }

                                if( fade_img[2] == true && fade_val[2] > 50 )
                                {
                                    fade_val[2] -= 5;
                                    SDL_SetSurfaceAlphaMod( sShadows[3],fade_val[2] );
                                }
                                else if( fade_img[2] == false && fade_val[2] < 255 )
                                {
                                    fade_val[2] += 5;
                                    SDL_SetSurfaceAlphaMod( sShadows[3],fade_val[2] );
                                }
                            }
                            //render
                            /**
                            if( page == 0 )
                            {
                                switch( stage )
                                {
                                case 0:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2 -30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 -40,SCREEN_HEIGHT/2 - sClessBook->h/2,sCover1,NULL );
                                break;
                                case 1:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2-15,SCREEN_HEIGHT/2 - sClessBook->h/2-80,sCover1Trans[0] ,NULL );
                                break;
                                case 2:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[4],NULL );
                                ApplySurf( SCREEN_WIDTH/2-33,SCREEN_HEIGHT/2 - sClessBook->h/2-215,sCover1Trans[1] ,NULL );
                                break;
                                case 3:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[4],NULL );
                                ApplySurf( SCREEN_WIDTH/2-10,SCREEN_HEIGHT/2 - sClessBook->h/2-110,sCover1Trans[2] ,NULL );
                                break;
                                case 4://apar texte de la credits
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 62,SCREEN_HEIGHT/2 - sClessBook->h/2-75,sCover1Trans[3] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 62,SCREEN_HEIGHT/2 - 100,sTxtOnPage[0] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[4],NULL );
                                break;
                                case 5:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 366,SCREEN_HEIGHT/2 - sClessBook->h/2-86,sCover1Trans[4] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 350,SCREEN_HEIGHT/2 - 100,sTxtOnPage[1] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[4],NULL );
                                break;
                                case 6:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 555,SCREEN_HEIGHT/2 - sClessBook->h/2-50,sCover1Trans[5] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 350,SCREEN_HEIGHT/2 - 100,sTxtOnPage[2] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[4],NULL );
                                break;
                                case 7:
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2 -30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - sPageLeft->w + 16,SCREEN_HEIGHT/2 - sClessBook->h/2+5,sPageLeft,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 350,SCREEN_HEIGHT/2 - 100,sTxtOnPage[3] ,NULL );
                                ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[4],NULL );
                                break;
                                }
                            }
                            else if( page == 1 )
                            {
                                    SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                    ApplySurf( SCREEN_WIDTH/2 -30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                    ApplySurf( SCREEN_WIDTH/2 - sPageLeft->w + 16,SCREEN_HEIGHT/2 - sClessBook->h/2+5,sPageLeft,NULL );

                                    ApplySurf( SCREEN_WIDTH/2 - 350,SCREEN_HEIGHT/2 - 100,sTxtOnPage[0] ,&rLeft );
                                    ApplySurf( SCREEN_WIDTH/2 +50,SCREEN_HEIGHT/2 -90,sTxtOnPage[1],&rRight );
                                    ApplySurf( SCREEN_WIDTH/2 - 470 + (511 - rLeft2.w) ,SCREEN_HEIGHT/2 - 270,sTxtOnPage[2] , &rLeft2);//&rLeft2
                                    ApplySurf( SCREEN_WIDTH/2 - 50 + (511 - rRight2.w),SCREEN_HEIGHT/2 - 360,sTxtOnPage[3], &rRight2);//&rRight2
                                    if( !(drag && x < (rNext.x + rNext.w )) )
                                    ApplySurf( rNext.x-2,rNext.y+10,sCorners[0],NULL );
                                    //SDL_FillRect(screen,&rNext,SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );

                                if( drag )
                                {
                                    if( (rNext.x + rNext.w ) - x < 250 )
                                    ApplySurf(x-40,rNext.y - 100,sCorners[1],NULL );
                                    else
                                    ApplySurf(x-170,rNext.y - 600,sCorners[2],NULL );
                                }
                            }
                            else if( page == 2 )
                            {
                                SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0,0,0) );
                                ApplySurf( SCREEN_WIDTH/2 -30,SCREEN_HEIGHT/2 - sClessBook->h/2,sClessBook,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - sPageLeft->w + 16,SCREEN_HEIGHT/2 - sClessBook->h/2+5,sPageLeft,NULL );
                                ApplySurf( SCREEN_WIDTH/2 - 470,SCREEN_HEIGHT/2 - 270,sTxtOnPage[0] ,&rLeft );//&rLeft2

                                if( reveal[0] )
                                {
                                    ApplySurf( rBox[1].x ,rBox[1].y ,sBox[1] ,NULL );
                                }
                                if( reveal[1] )
                                {
                                    ApplySurf( SCREEN_WIDTH/2 - 480 ,SCREEN_HEIGHT/2 - 200,sTxtOnPage[2] ,&rLeft);
                                    ApplySurf( SCREEN_WIDTH/2 - 480 ,SCREEN_HEIGHT/2 ,sTxtOnPage[3] ,&rLeft);
                                    ApplySurf( rBox[2].x ,rBox[2].y,sBox[2] ,&rRight);
                                }
                                if( reveal[2] )
                                {
                                    ApplySurf( rBox[3].x - 130,rBox[3].y - 9,sTxtOnPage[5] ,&rLeft);
                                    ApplySurf( rBox[3].x ,rBox[3].y ,sBox[3] ,&rLeft);
                                }
                                ApplySurf( rBox[0].x, rBox[0].y,sBox[0],NULL );
                                if( stage == 1 )
                                {
                                    ApplySurf( SCREEN_WIDTH/2 - 50 ,SCREEN_HEIGHT/2 - 360,sTxtOnPage[1] ,&rRight);//&rRight2
                                    ApplySurf( SCREEN_WIDTH/2 + 170 , SCREEN_HEIGHT/2 - 160,sBox[0],NULL );
                                    //SDL_FillRect(screen,&rSecBox[0],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                }
                                else if( stage == 2 )
                                {
                                    ApplySurf( SCREEN_WIDTH/2 -30,SCREEN_HEIGHT/2 - sClessBook->h/2,sPagePic[0],NULL );
                                    ApplySurf( rSecBox[0].x ,rSecBox[0].y ,sPagePic[1],NULL );
                                    //SDL_FillRect(screen,&rSecBox[0],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    ApplySurf( rSecBox[1].x ,rSecBox[1].y ,sPagePic[2],NULL );
                                    //SDL_FillRect(screen,&rSecBox[1],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    ApplySurf( rSecBox[2].x ,rSecBox[2].y ,sPagePic[3],NULL );
                                    //SDL_FillRect(screen,&rSecBox[2],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    ApplySurf( rSecBox[3].x,rSecBox[3].y,sPagePic[4],NULL );
                                    //SDL_FillRect(screen,&rSecBox[3],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    //ApplySurf( SCREEN_WIDTH/2 -30,SCREEN_HEIGHT/2 - sClessBook->h/2,sPagePic[0],NULL );
                                }
                                else if( stage == 3 )
                                {
                                    //SDL_FillRect(screen,&rSecBox[0],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    ApplySurf( rSecBox[0].x ,rSecBox[0].y ,sPagePic[0],NULL );//&rRight
                                    //SDL_FillRect(screen,&rSecBox[1],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    ApplySurf( rSecBox[1].x ,rSecBox[1].y ,sPagePic[1],NULL );
                                    //SDL_FillRect(screen,&rSecBox[2],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    ApplySurf( rSecBox[2].x ,rSecBox[2].y ,sPagePic[2],NULL );
                                }
                                else if( stage == 4 )
                                {
                                    ApplySurf( rSecBox[0].x, rSecBox[0].y, sShadows[1], NULL );
                                    ApplySurf( rSecBox[1].x, rSecBox[1].y, sShadows[2], NULL );
                                    ApplySurf( rSecBox[2].x, rSecBox[2].y, sShadows[3], NULL );
                                    ApplySurf( SCREEN_WIDTH/2 ,SCREEN_HEIGHT/2 - sClessBook->h/2,sShadows[0],NULL );
                                    //SDL_FillRect(screen,&rSecBox[3],SDL_MapRGB(screen->format,0xFF,0x4F,0x38) );
                                    if( hero_perspective != 0 )
                                    {
                                        ApplySurf( rSecBox[3].x + 115, rSecBox[3].y + 25, sShadows[4], NULL );
                                    }

                                }
                            }

                            **/
                            ///SDL_Flip( screen );

                        }
                        if( make_new_char ) //create character
                        {

                        }


                    }
                   break;
                }
            }
        }
    }
//LOGICS
if( BG_X < -background->w )
BG_X = 0;

//Timere
if( (SDL_GetTicks() - secund_counter) > 1000)
{
    secund_counter = SDL_GetTicks();
    stringstream time;
    time<<fraps;
    ///txtFPS = TTF_RenderText_Solid(font,time.str().c_str(),BLACK);
    fraps = 0;
}

    //Render background si logo
///ApplySurf(BG_X,SCREEN_HEIGHT/2 - background->h /2,background,NULL);
///ApplySurf(BG_X + background->w,SCREEN_HEIGHT/2 - background->h /2,background,NULL);

///ApplySurf( (SCREEN_WIDTH/2)-(logo->w/2),SCREEN_HEIGHT/10.24,logo,NULL);//10,24

//render frame & buttons
    for(i = 0 ; i<5 ; i++)
    {
        ///ApplySurf(Rframe[i].x,Rframe[i].y,Cframe[i],NULL);//Cframe[i] in loc de frame2
    }

//Render icon , nume , lvl
for(int a = 0; a < 5; a++){

if(Player[a].created == false)
{
    /**
    ApplySurf(  Rframe[a].x+4,Rframe[a].y+3,icon[0],NULL);
    ApplySurf( (Rframe[a].x+10),(Rframe[a].y+141),txtName[a],NULL);
    ApplySurf( (Rframe[a].x+10),(Rframe[a].y+141+30),txtLvl[a],NULL);
    **/
}
else if( Player[a].created == true )
{   int icn = 0;
    if(Player[a].Class == 'w')
    icn = 1;
    else if(Player[a].Class == 'r')
    icn = 2;
    else
    icn = 3;
    ///ApplySurf(  Rframe[a].x+6,Rframe[a].y+3,icon[icn],NULL);
    ///ApplySurf( (Rframe[a].x+10),(Rframe[a].y+141),txtName[a],NULL);
    ///ApplySurf( (Rframe[a].x+10),(Rframe[a].y+3),txtLvl[a],NULL);
}
}

    //render frame & buttons
    for(i = 0 ; i<5 ; i++)
    {
        if(Button[i].render)
        {
            ///ApplySurf(Button[i].x,Button[i].y,Button[i].frame,NULL);
            ///ApplySurf(Button[i].x+10,Button[i].y+5,Button[i].txtButton,NULL);
        }
    }


    ///ApplySurf(10,10,txtFPS,NULL);

    ///SDL_Flip( screen );
}
Player[0].created = false;

return Player[0];
}
