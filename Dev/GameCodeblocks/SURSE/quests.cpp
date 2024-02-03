#include "quests.h"



void Quest::init(){
int i;
txLeftbackground = LoadTex("textures//interface//quests//leftbackground.png");
txLeftside = LoadTex("textures//interface//quests//leftside.png");
txBackground = LoadTex("textures//interface//quests//background.png");
//txClose = LoadTex("textures//interface//quests//close.png");
txLogo = LoadTex("textures//interface//quests//logo.png");
txAbbandon = LoadTex("textures//interface//quests//abbandon.png");
txUp1 = LoadTex("textures//interface//quests//upfull.png");
txUp2 = LoadTex("textures//interface//quests//uphalf.png");
txToList = LoadTex( "textures//interface//quests//ButtonToList.png" );
txCoverLeft = LoadTex( "textures//interface//quests//Cover_left.png" );

SDL_SetTextureAlphaMod( txToList, 50 );
SDL_SetTextureBlendMode( txToList, SDL_BLENDMODE_BLEND );

for( i = 0 ; i< QUEST_CAP;i++)
{
    ACTIVE_QUEST[i] = quest_NULL;
}
for(i = 0 ; i < 10;i++)
txQuestdesc[i] = NULL;

QuestTitle_w = 0;

Ccol = Crow  = -1;

tooltip_statsnr = tooltip_name_nr = tooltip_desc_nr = 0;

SHOW_QUEST_DETAIL = false;
SHOW_ABBANDON_BUTTON = false;
DRAG_ABBANDON = false;
SHOW_UP = false;
OnButtonToList = false;

txRewGold = txRewExp = NULL;

SDL_QueryTexture( txBackground,0,0,&Background_w,&Background_h );
SDL_QueryTexture( txLeftbackground,0,0,&LeftBackground_w,0 );
SDL_QueryTexture( txLeftside,0,0,&Leftside_w,0 );
//SDL_QueryTexture( txClose,0,0,&CloseB_w,&CloseB_h );
SDL_QueryTexture( txAbbandon,0,0,&abox.w,&abox.h );
abox.x = SCREEN_WIDTH/2+Background_w/2 + Leftside_w - 4;
abox.y = SCREEN_HEIGHT/4;

cbox.w = 30;
cbox.h = 30;
cbox.x = SCREEN_WIDTH/2+Background_w/2;
cbox.y = SCREEN_HEIGHT/4;

cbox2.w = 30;
cbox2.h = 30;
cbox2.x = SCREEN_WIDTH/2+Background_w/2+LeftBackground_w-cbox2.w;
cbox2.y = SCREEN_HEIGHT/4;

tooltip_canvas = NULL;
for( i = 0; i < 3; i ++ )
tooltip_lines_name[i] = NULL;
for( i = 0; i < 6; i ++ )
tooltip_stats[i] = NULL;
for( i = 0; i < 6; i ++ )
tooltip_lines_desc[i] = NULL;

SHOW_QUEST = false;

///ADD TEST QUESTS
//add( 1 );

}
void Quest::show(){
int i;
//Render bg si close button


if( !SHOW_QUEST_DETAIL )
{
    ApplyTex(SCREEN_WIDTH/2+Background_w/2,SCREEN_HEIGHT/4,txLeftbackground);
    ApplyTex(SCREEN_WIDTH/2+Background_w/3+60,SCREEN_HEIGHT/4+ 20,txLogo);
    //ApplyTex(SCREEN_WIDTH/2+Background_w/2+LeftBackground_w-CloseB_w,SCREEN_HEIGHT/4,txClose);
    //Actual quests
    for(i = 0;i < QUEST_CAP;i++)
    if(ACTIVE_QUEST[i].nr != 0)
    {
        ApplyTex(SCREEN_WIDTH/2+Background_w/2 + 10,
                 SCREEN_HEIGHT/4+70+i*15,txQuestname[i]);
        ApplyTex(SCREEN_WIDTH/2+Background_w*2-Background_w/2,
                 SCREEN_HEIGHT/4+70+i*15,txQuestprog[i]);
    }
}
else
{
    ApplyTex(SCREEN_WIDTH/2+Background_w/2,SCREEN_HEIGHT/4,txCoverLeft );
    ApplyTex(SCREEN_WIDTH/2+Background_w/2,SCREEN_HEIGHT/4,txLeftside);
    ApplyTex(SCREEN_WIDTH/2+Background_w/2,SCREEN_HEIGHT/4,txToList);
    if( DRAG_ABBANDON == false || (DRAG_ABBANDON == true && bgbox.y < 10) )
    ApplyTex(SCREEN_WIDTH/2+Background_w/2 + Leftside_w - 4,SCREEN_HEIGHT/4,txBackground);
    else
    ApplyTex(SCREEN_WIDTH/2+Background_w/2 + Leftside_w - 4,SCREEN_HEIGHT/4 + bgbox.y,txBackground,&bgbox);

    if( !(DRAG_ABBANDON && mouse_y > SCREEN_WIDTH/4-60) )
    {
        if( SHOW_UP )
        ApplyTex(SCREEN_WIDTH/2+Background_w/2 + Leftside_w - 4,SCREEN_HEIGHT/4,txUp2);
        else
        ApplyTex(SCREEN_WIDTH/2+Background_w/2 + Leftside_w - 4,SCREEN_HEIGHT/4,txUp1);
    }

    if( !(DRAG_ABBANDON && mouse_y > SCREEN_HEIGHT/4+30) )
    ApplyTex(SCREEN_WIDTH/2+Background_w/2+55+( Background_w-QuestTitle_w )/2  ,
             SCREEN_HEIGHT/4+30,txcQuestname);
    for(i = 0  ;i < 10;i++)
    if( !(DRAG_ABBANDON && mouse_y > SCREEN_HEIGHT/4+70+15*i) )
    ApplyTex(SCREEN_WIDTH/2+Background_w/2+55,SCREEN_HEIGHT/4+70+15*i,txQuestdesc[i]);

    ApplyTex(SCREEN_WIDTH/2+Background_w/2+138,SCREEN_HEIGHT/4 + 233,txRewGold );
    ApplyTex(SCREEN_WIDTH/2+Background_w/2+49 ,SCREEN_HEIGHT/4 + 252,txRewExp );

    if( DRAG_ABBANDON && mouse_y > SCREEN_WIDTH/4-60 )
    ApplyTex(SCREEN_WIDTH/2+Background_w/2 + Leftside_w - 4,mouse_y-abox.h/2+5,txAbbandon);

    for( i = 0; i < ACTIVE_QUEST[ cquest ].rew_itm_nrof && i < 5;i++ )
    ApplyTex(SCREEN_WIDTH/2+Background_w/2 + i*53+49 ,
             SCREEN_HEIGHT/4 + 280,ACTIVE_QUEST[ cquest ].RewItems[i]->tex );
    if( ACTIVE_QUEST[ cquest ].rew_itm_nrof > 5 )
    for( i = 5; i < ACTIVE_QUEST[ cquest ].rew_itm_nrof ;i++ )
    ApplyTex(SCREEN_WIDTH/2+Background_w/2 + (i-5)*53+49 ,
             SCREEN_HEIGHT/4 + 334,ACTIVE_QUEST[ cquest ].RewItems[i]->tex );


    ApplyTex( SCREEN_WIDTH/2+Background_w/2 + 49 + Ccol*53 - ( tooltip_canvas_w - 50 )/2 ,
              SCREEN_HEIGHT/4 + 330 + Crow*54 ,
              tooltip_canvas );

            for( i = 0; i < tooltip_name_nr; i++ )
            if( tooltip_lines_name[i] != NULL )
            ApplyTex( SCREEN_WIDTH/2+Background_w/2 + 49 + Ccol*53 - ( tooltip_canvas_w - 50 )/2 +
                      ( tooltip_canvas_w - tooltip_ln_w[i] )/2 ,
                      i*(20 - 6) + SCREEN_HEIGHT/4 + 330 + Crow*54 ,
                      tooltip_lines_name[ i ], NULL  );
            //STats I : Chest/cloak .. etc
            if( tooltip_stats[0] != NULL )
                ApplyTex( SCREEN_WIDTH/2+Background_w/2 + 52 + Ccol*53 - ( tooltip_canvas_w - 50 )/2,
                           tooltip_name_nr*(20 - 6) + SCREEN_HEIGHT/4 + 330 + Crow*54 ,
                           tooltip_stats[ 0 ], NULL  );

            //Stats II -> N
            for( i = 1; i < tooltip_statsnr; i++ )
            {
                if( tooltip_stats[i] != NULL )
                ApplyTex( SCREEN_WIDTH/2+Background_w/2 + 52 + Ccol*53 - ( tooltip_canvas_w - 50 )/2,
                          tooltip_name_nr*(20 - 6) + SCREEN_HEIGHT/4 + 330 + 4 + Crow*54 + i*11,
                          tooltip_stats[ i ], NULL  );
            }

            for( i = 0; i < tooltip_desc_nr; i++ )
            {
                if( tooltip_lines_desc[i] != NULL )
                ApplyTex( SCREEN_WIDTH/2+Background_w/2 + 52 + Ccol*53 - ( tooltip_canvas_w - 50 )/2 ,//+( tooltip_canvas_w - tooltip_ld_w[i] )/2
                          tooltip_name_nr*(20 - 6) + SCREEN_HEIGHT/4 + 330 + 4 + Crow*54 + tooltip_statsnr*12 +
                          i*13  ,
                          tooltip_lines_desc[ i ] );
            }


}


}
void Quest::add(int nr){
stringstream ssaux;
bool added = false;
SDL_Surface *tempsurf = NULL;
int i;
for(i = 0; i < QUEST_CAP;i++){
box[i].w = Background_w;
box[i].h = 20;
box[i].x = SCREEN_WIDTH/2+Background_w/2;
box[i].y = SCREEN_HEIGHT/4+70+i*15;
if( ACTIVE_QUEST[i].nr == nr ){
    added = false;
    break;
}
else if( ACTIVE_QUEST[i].nr == 0 )
{
    //Add quest
    SHOW_QUEST_DETAIL = false;
    ACTIVE_QUEST[i] = DATABASE.GetQuest( nr );
    if( tempsurf != NULL ) SDL_FreeSurface( tempsurf );
    tempsurf = TTF_RenderText_Blended(Font20,ACTIVE_QUEST[i].name.c_str() ,BLACK);
    txQuestname[i] =  SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
    ssaux.str("");
    ssaux <<'['<< ACTIVE_QUEST[i].cprog <<'/'<< ACTIVE_QUEST[i].goalprog<<']';
    SDL_FreeSurface( tempsurf );
    tempsurf = TTF_RenderText_Blended(Font20,ssaux.str().c_str(),BLACK);
    txQuestprog[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
    added = true;
    break;
}
}
if( added == false )
PLAYER.SPEAK_NPC = false;

}
void Quest::handle_events(SDL_Event newevent){
Armor tempArmor;
Weapon tempWeapon;
stringstream ssqtxt;
bool foundrect = false;
SDL_Surface *tempsurf  = NULL;
int j,i,x,y,q;
if( newevent.type == SDL_MOUSEBUTTONDOWN )
{
x = newevent.button.x;
y = newevent.button.y;

if( !SHOW_QUEST_DETAIL )
{
    if(x > cbox.x && x <(cbox.x + cbox.w)&&y > cbox.y &&y < (cbox.y+ cbox.h))
    {
        QUEST.SHOW_QUEST = false;
    }
    else
    {
    for(i = 0 ; i<= QUEST_CAP;i++)
    {
        if(x > box[i].x && x <(box[i].x + box[i].w)&&y > box[i].y &&y < (box[i].y+ box[i].h)&&ACTIVE_QUEST[i].nr != 0)
        {
            stringstream ssaux;
            ssaux.str("");
            ssaux<<"desc line: "<<ACTIVE_QUEST[i].desc_lines;
            CONS.out( ssaux.str().c_str() );
            cquest = i;

            if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
            tempsurf = TTF_RenderText_Blended( Font24,ACTIVE_QUEST[i].name.c_str(),BLACK );

            txcQuestname = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );

            SDL_QueryTexture( txcQuestname,0,0,&QuestTitle_w,0 );
            SHOW_QUEST_DETAIL = true;
            for(j = 0 ; j < ACTIVE_QUEST[i].desc_lines ;j++)
            {
                if( tempsurf != NULL ) SDL_FreeSurface( tempsurf );
                tempsurf = TTF_RenderText_Blended(Font20,ACTIVE_QUEST[i].desc[j].c_str() ,BLACK);
                txQuestdesc[j] = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf  );
            }

            ssaux.str("");
            if( txRewGold != NULL ) SDL_DestroyTexture( txRewGold );
            if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
            ssaux<<"Gold: "<<ACTIVE_QUEST[i].rew_gold;
            tempsurf = TTF_RenderText_Blended( Font20,ssaux.str().c_str(),BLACK );
            txRewGold = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );
            ssaux.str("");
            if( txRewExp != NULL ) SDL_DestroyTexture( txRewExp );
            if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
            ssaux<<"Experience: "<<ACTIVE_QUEST[i].rew_exp;
            tempsurf = TTF_RenderText_Blended( Font20,ssaux.str().c_str(),BLACK );
            txRewExp = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );

            break;
        }
    }
    }
}
else if( SHOW_QUEST_DETAIL )
{
    if(x > cbox2.x && x <(cbox2.x + cbox2.w)&&y > cbox2.y &&y < (cbox2.y+ cbox2.h))
    {
        QUEST.SHOW_QUEST = false;
    }
    else
    if(x > abox.x && x <(abox.x + abox.w)&&y > abox.y &&y < (abox.y+ abox.h))
    {
        DRAG_ABBANDON = true;
        mouse_x = newevent.button.x;
        mouse_y = newevent.button.y;
        bgbox.w = 280;
        bgbox.h = 410;
        bgbox.y = mouse_y - SCREEN_HEIGHT/4;
        bgbox.h = Background_h - bgbox.y;
    }
    else
    if( mouse_x > SCREEN_WIDTH/2+Background_w/2 && mouse_x < SCREEN_WIDTH/2+Background_w/2 + 50 &&
        mouse_y > SCREEN_HEIGHT/4 && mouse_y < SCREEN_HEIGHT/4 + 410  )
    {
        SHOW_QUEST_DETAIL = false;
    }
}
}
else
if( newevent.type == SDL_MOUSEBUTTONUP )
{
    if( DRAG_ABBANDON == true )
    DRAG_ABBANDON = false;
}
else
if( newevent.type == SDL_MOUSEMOTION )
{
    mouse_x = newevent.motion.x;
    mouse_y = newevent.motion.y;

    if(mouse_x > abox.x && mouse_x <(abox.x + abox.w)&&mouse_y > abox.y &&mouse_y < (abox.y+ abox.h))
    {
        if( SHOW_UP == false )
        SHOW_UP = true;
    }
    else
    {
        if( SHOW_UP == true )
        SHOW_UP = false;
    }

    if( SHOW_QUEST_DETAIL )
    for( i = 0; i < 5 ;i++ )
        for( j = 0;j < 2;j++ )
    if( i + j*5 < ACTIVE_QUEST[ cquest ].rew_itm_nrof )
    {
        if( mouse_x > SCREEN_WIDTH/2+Background_w/2 + i*53+49 &&
        mouse_x < SCREEN_WIDTH/2+Background_w/2 + i*53+49 + 50 &&
        mouse_y > SCREEN_HEIGHT/4 + 280 + j*54 &&
        mouse_y < SCREEN_HEIGHT/4 + 280 + j*54 + 50 )
        {
            foundrect = true;
            if( i != Ccol || j != Crow )
            {
                Ccol = i;
                Crow = j;
                stringstream ssaux;
                ssaux <<"Mouse on Block "<<i<<','<<j;
                CONS.out(ssaux.str().c_str());

                if( tooltip_canvas != NULL )
                SDL_DestroyTexture( tooltip_canvas ); tooltip_canvas = NULL;
                for( q = 0; q < 3; q ++ )
                    if( tooltip_lines_name[q] != NULL )
                    SDL_DestroyTexture( tooltip_lines_name[q] ),tooltip_lines_name[q] = NULL;
                for( q = 0; q < 6; q ++ )
                    if( tooltip_stats[q] != NULL )
                    SDL_DestroyTexture( tooltip_stats[q] ) ,tooltip_stats[q]      = NULL;
                for( q = 0; q < 6; q ++ )
                    if( tooltip_lines_desc[q] != NULL )
                    SDL_DestroyTexture( tooltip_lines_desc[q] ),tooltip_lines_desc[q] = NULL;


                //Render Tooltip
                RenderTooltip( ACTIVE_QUEST[cquest].RewItems[ i + j*5 ] , &tooltip_canvas ,
                                  &tooltip_lines_name[0],
                                  &tooltip_lines_name[1],
                                  &tooltip_lines_name[2],
                                  &tooltip_stats[0],
                                  &tooltip_stats[1],
                                  &tooltip_stats[2],
                                  &tooltip_stats[3],
                                  &tooltip_stats[4],
                                  &tooltip_stats[5],
                                  &tooltip_lines_desc[0],
                                  &tooltip_lines_desc[1],
                                  &tooltip_lines_desc[2],
                                  &tooltip_lines_desc[3],
                                  &tooltip_lines_desc[4],
                                  &tooltip_lines_desc[5],
                                  tooltip_name_nr,tooltip_statsnr,tooltip_desc_nr );
                SDL_QueryTexture( tooltip_canvas,0,0,&tooltip_canvas_w,0 );
                for( q = 0; q < tooltip_name_nr; q ++ )
                if( tooltip_lines_name[q] != NULL )
                SDL_QueryTexture( tooltip_lines_name[q] ,0,0,&tooltip_ln_w[q] ,0 );
                for( q = 0; q < tooltip_desc_nr;q++ )
                if( tooltip_lines_desc[q] != NULL )
                SDL_QueryTexture( tooltip_lines_desc[q] ,0,0,&tooltip_ld_w[q] ,0 );
            }



        }
        else if( i == 4 && j == 1 && foundrect == false )
        {
            //Hide Tooltip(cleanup)
            if( tooltip_canvas != NULL )
            SDL_DestroyTexture( tooltip_canvas ); tooltip_canvas = NULL;
            for( q = 0; q < 3; q ++ )
                if( tooltip_lines_name[q] != NULL )
                SDL_DestroyTexture( tooltip_lines_name[q] ),tooltip_lines_name[q] = NULL;
            for( q = 0; q < 6; q ++ )
                if( tooltip_stats[q] != NULL )
                SDL_DestroyTexture( tooltip_stats[q] ) ,tooltip_stats[q]      = NULL;
            for( q = 0; q < 6; q ++ )
                if( tooltip_lines_desc[q] != NULL )
                SDL_DestroyTexture( tooltip_lines_desc[q] ),tooltip_lines_desc[q] = NULL;
                Ccol = -1;
                Crow = -1;
        }
    }
    else if( foundrect == false )
    {
        //Hide Tooltip(cleanup)
        if( tooltip_canvas != NULL )
        SDL_DestroyTexture( tooltip_canvas ); tooltip_canvas = NULL;
        for( q = 0; q < 3; q ++ )
            if( tooltip_lines_name[q] != NULL )
            SDL_DestroyTexture( tooltip_lines_name[q] ),tooltip_lines_name[q] = NULL;
        for( q = 0; q < 6; q ++ )
            if( tooltip_stats[q] != NULL )
            SDL_DestroyTexture( tooltip_stats[q] ) ,tooltip_stats[q]      = NULL;
        for( q = 0; q < 6; q ++ )
            if( tooltip_lines_desc[q] != NULL )
            SDL_DestroyTexture( tooltip_lines_desc[q] ),tooltip_lines_desc[q] = NULL;
            Ccol = -1;
            Crow = -1;
    }


    if( mouse_x > SCREEN_WIDTH/2+Background_w/2 && mouse_x < SCREEN_WIDTH/2+Background_w/2 + 50 &&
        mouse_y > SCREEN_HEIGHT/4 && mouse_y < SCREEN_HEIGHT/4 + 410  )
    {
        if( OnButtonToList == false )
        {
            OnButtonToList = true;
            SDL_SetTextureAlphaMod( txToList, 150 );
        }

    }
    else if( OnButtonToList )
    {
        OnButtonToList = false;
        SDL_SetTextureAlphaMod( txToList, 50 );
    }


    if( DRAG_ABBANDON == true && mouse_y >= SCREEN_HEIGHT/4+230 )
    {
        DRAG_ABBANDON = false;
        SHOW_QUEST_DETAIL = false;
        //abbandon quest
        ACTIVE_QUEST[cquest] = quest_NULL;
        SDL_FreeSurface( csQuestname );
    }
    else if( DRAG_ABBANDON == true )
    {
        bgbox.y = mouse_y - SCREEN_HEIGHT/4;
        bgbox.h = Background_h - bgbox.y;
    }

}
}
void Quest::setrects(){
if(SHOW_QUEST_DETAIL)
{
    //Abandon quest
}
else
{
    box[0].w = Background_w;
    box[0].h = 20;
    box[0].x = SCREEN_WIDTH/2+Background_w/2;
    box[0].y = SCREEN_HEIGHT/4+70;
}


}
bool Quest::is_finished( int x){
int i;
for(i = 0;i < QUEST_CAP;i++)
if(ACTIVE_QUEST[i].nr == x && ACTIVE_QUEST[i].cprog == ACTIVE_QUEST[i].goalprog )
{
    return true;
}
return false;
}
bool Quest::is_active(int x ){
int i;
if( bQUESTDONE[x] == true )
return false;
for(i = 0;i < QUEST_CAP;i++)
if(ACTIVE_QUEST[i].nr == x && x != 0 )
{
    return true;
}
return false;
}
void Quest::end(int nr){

for(int i = 0; i < QUEST_CAP;i++ )
if(ACTIVE_QUEST[i].nr == nr)
{
    if( i == cquest ) SHOW_QUEST_DETAIL = false;

    ACTIVE_QUEST[i] = quest_NULL;
}

}
void Quest::advance( int nr ){
stringstream ssaux;
SDL_Surface *tempsurf = NULL;
for(int i = 0 ; i < QUEST_CAP ;i++ )
if( ACTIVE_QUEST[i].nr == nr &&ACTIVE_QUEST[i].cprog < ACTIVE_QUEST[i].goalprog)
{
    ACTIVE_QUEST[i].cprog++;
    ssaux.str("");
    ssaux <<'['<< ACTIVE_QUEST[i].cprog <<'/'<< ACTIVE_QUEST[i].goalprog<<']';
    SDL_DestroyTexture( txQuestprog[i] );
    tempsurf = TTF_RenderText_Solid(Font20,ssaux.str().c_str(),BLACK);
    txQuestprog[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
    SDL_FreeSurface( tempsurf );
    break;
}

}
Quest::~Quest(){
int i;
SDL_DestroyTexture( txBackground );
SDL_DestroyTexture( txLogo       );
SDL_DestroyTexture( txcQuestname );
for(i = 0 ; i < QUEST_CAP;i++)
if( sQuestname[i] != NULL )
SDL_DestroyTexture( txQuestname[i] );
for(i = 0 ; i < 10;i++)
if( txQuestdesc[i] != NULL )
SDL_DestroyTexture( txQuestdesc[i] );
}
