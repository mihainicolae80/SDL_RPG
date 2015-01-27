#include "dialog.h"


void Dialog::init(void){
int i;
finish_drag = false;
//New Dialog
bot_backslice      = LoadTex( "textures//interface//dialog//bot_backslice.png" );
back_fog           = LoadTex( "textures//interface//dialog//back_fog.png" );
text_backline      = LoadTex( "textures//interface//dialog//text_backline.png" );
text_backfog       = LoadTex( "textures//interface//dialog//text_backfog.png" );

slot_empty         = LoadTex( "textures//inventory//slot_empty.png" );
FQP_ItmSelect      = LoadTex( "textures//interface//dialog//itm_select.png" );
txFQP_ButtonFQ_up   = LoadTex( "textures//interface//dialog//button_FinishQuest_up.png" );
txFQP_ButtonFQ_down = LoadTex( "textures//interface//dialog//button_FinishQuest_down.png" );

BotEdge_Left  = LoadTex( "textures//interface//dialog//backsliceedge_left.png" );
BotEdge_Right = LoadTex( "textures//interface//dialog//backsliceedge_right.png" );

Tip_tx = NULL;
UPDATE_DIALOG = false;

SDL_QueryTexture( bot_backslice,0,0,&BotBackslice_w,&BotBackslice_h );

Percent_ShowBotBack = 85;
NrBotSlices = floor( (float)(SCREEN_WIDTH*Percent_ShowBotBack/100)/BotBackslice_w );

FQP_RewItmTaken = 0;
tooltip_canvas_w = 0;
bFinQuePanel = false;
FQP_RENDER_ONCE = false;
FQP_RENDER_TOOLTIP = false;
FQP_ButtonPressed = false;
FQP_CItm    = 0;
FQP_QuestNr = 0;
FQP_Text_w  = 0;
FQP_FinishStatus = 0;
FQP_FinishQuest  = false;
RecItm_ShowTip   = false;
AllowInvAcces    = false;

FQP_ButtonTimer = SDL_GetTicks();

SDL_SetTextureAlphaMod( back_fog,50 );
SDL_SetTextureAlphaMod( text_backfog,212 );
SDL_SetTextureAlphaMod( bot_backslice,200 );
SDL_SetTextureBlendMode( bot_backslice,SDL_BLENDMODE_BLEND );

SDL_SetTextureAlphaMod( BotEdge_Left,200 );
SDL_SetTextureAlphaMod( BotEdge_Right,200 );
SDL_SetTextureBlendMode( BotEdge_Left,SDL_BLENDMODE_BLEND );
SDL_SetTextureBlendMode( BotEdge_Right,SDL_BLENDMODE_BLEND );

FQP_Text = NULL;
FQP_ItmRewNr   = 0;
FQP_RewItmLeft = 0;

SDL_QueryTexture( txFQP_ButtonFQ_up,0,0,&FQP_ButtonFinish.w,&FQP_ButtonFinish.h );
SDL_QueryTexture( back_fog,0,0,&BackFog_w,&BackFog_h );


FQP_ButtonFinish.x = (SCREEN_WIDTH-FQP_ButtonFinish.w)/2;
FQP_ButtonFinish.y = SCREEN_HEIGHT-FQP_ButtonFinish.h - 5;

tooltip_canvas    = NULL;
tooltip_highlight = NULL;

for( i = 0; i < 3; i++ )
tooltip_lines_name[i] = NULL;
for( i = 0; i < 6; i ++ )
tooltip_lines_desc[i] = tooltip_stats[i] = NULL;

for( i = 0; i < 10; i ++ ){
    FQP_ItmBox[i].x = 0;//x este setat in functie de situatie
    FQP_ItmBox[i].y = SCREEN_HEIGHT - BotBackslice_h + 45;
    FQP_ItmBox[i].w = 50;
    FQP_ItmBox[i].h = 50;
}
for( i = 0; i < 10; i ++ ){
FQP_rew_items[i] = new Inventory_item;
}
}
void Dialog::show(){
int i,k,txt_w;//,j;
FQP_RENDER_ONCE = true;

    //Background

    for( i = 0; i < NrBotSlices; i++ )
    ApplyTex( SCREEN_WIDTH/2-NrBotSlices*BotBackslice_w/2+i*BotBackslice_w
             ,SCREEN_HEIGHT - BotBackslice_h,bot_backslice,NULL );
    ApplyTex( SCREEN_WIDTH/2-NrBotSlices*BotBackslice_w/2 - BotBackslice_w
             ,SCREEN_HEIGHT - BotBackslice_h,BotEdge_Left,NULL );
    ApplyTex( SCREEN_WIDTH/2+NrBotSlices*BotBackslice_w/2
             ,SCREEN_HEIGHT - BotBackslice_h,BotEdge_Right,NULL );

    //Text
    for( i = 0; i < 4; i++ )
    {
        if( txDialogtxt[i] != NULL )
        {
           SDL_QueryTexture( txDialogtxt[i],0,0,&txt_w,0 );
           ApplyTex( SCREEN_WIDTH /2 - txt_w/2 ,
                     SCREEN_HEIGHT - BotBackslice_h - 25 - CurrentDialogTopic->lines_nr*20 + i*20 ,
                     txDialogtxt[i] );
        }



    }



        //HERO
        //Finish quest
        if( bFinQuePanel ){
        ApplyTex( (SCREEN_WIDTH-FQP_Text_w)/2  , SCREEN_HEIGHT - BotBackslice_h + 10,FQP_Text , NULL );
        if( Tip_tx != NULL )
        ApplyTex( (SCREEN_WIDTH-Tip_tx_w)/2 , SCREEN_HEIGHT - BotBackslice_h + 25,Tip_tx , NULL );

        for( k = 0; k < FQP_ItmRewNr; k++ ){
        ApplyTex(FQP_ItmBox[k].x -1,FQP_ItmBox[k].y -1, slot_empty, NULL );
        ApplyTex(FQP_ItmBox[k].x ,FQP_ItmBox[k].y , FQP_rew_items[k]->tex, NULL );
        if( FQP_ItmSelected[k] )
        ApplyTex(FQP_ItmBox[k].x ,FQP_ItmBox[k].y ,FQP_ItmSelect , NULL );
        if( FQP_ButtonPressed == false )
        ApplyTex( FQP_ButtonFinish.x, FQP_ButtonFinish.y,txFQP_ButtonFQ_up ,NULL );
        else
        ApplyTex( FQP_ButtonFinish.x, FQP_ButtonFinish.y,txFQP_ButtonFQ_down ,NULL );
        }

        }
        //Dialog
        else{
        for( i = 0; i < 4; i ++ )
        if( !(box_topic[i].x == 0 && box_topic[i].y == 0) )
        ApplyTex( box_topic[i].x, box_topic[i].y ,txTopic[i] );
        }



    if( FQP_RENDER_TOOLTIP ){
    int difx = 0,dify = 0;

    if( FQP_ItmBox[FQP_CItm].x + FQP_ItmBox[FQP_CItm].w + tooltip_canvas_w > SCREEN_WIDTH )
    difx -= FQP_ItmBox[FQP_CItm].w + tooltip_canvas_w;

    if( FQP_ItmBox[FQP_CItm].y - (tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2
        + tooltip_canvas_h > SCREEN_HEIGHT )
    dify -= SCREEN_HEIGHT - FQP_ItmBox[FQP_CItm].y - (tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2 + tooltip_canvas_h;

    ApplyTex( FQP_ItmBox[FQP_CItm].x + FQP_ItmBox[FQP_CItm].w + difx ,
                   FQP_ItmBox[FQP_CItm].y + dify
                   -(tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2
                   ,tooltip_canvas, NULL );
    for( i = 0; i < tooltip_name_nr; i++ )
    if( tooltip_lines_name[i] != NULL )
    ApplyTex( FQP_ItmBox[FQP_CItm].x + FQP_ItmBox[FQP_CItm].w + difx
                   +(tooltip_canvas_w-tooltip_lines_name_w[i])/2 ,
                   i*(tooltip_lines_name_h[i] - 6) +
                   FQP_ItmBox[FQP_CItm].y + dify
                   -(tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2,
                   tooltip_lines_name[ i ], NULL  );
    //STats I : Chest/cloak .. etc
    if( tooltip_stats[0] != NULL )
        ApplyTex( FQP_ItmBox[FQP_CItm].x + FQP_ItmBox[FQP_CItm].w + difx +3,
                   tooltip_name_nr*(tooltip_lines_name_h[0] -2) + //Ajustari dist nume<->stats, stats<->stats
                   FQP_ItmBox[FQP_CItm].y + dify
                   -(tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2,
                   tooltip_stats[ 0 ], NULL  );
    //Stats II -> N
    for( i = 1; i < tooltip_statsnr; i++ )
    {
        if( tooltip_stats[i] != NULL )
        ApplyTex( FQP_ItmBox[FQP_CItm].x + FQP_ItmBox[FQP_CItm].w + difx +3,
                   tooltip_name_nr*(tooltip_lines_name_h[0] -2) + i*(tooltip_lines_name_h[0] -4 ) + //Ajustari
                   FQP_ItmBox[FQP_CItm].y + dify
                   -(tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2,
                   tooltip_stats[ i ], NULL  );
    }

    for( i = 0; i < tooltip_desc_nr; i++ )
    {
        if( tooltip_lines_desc[i] != NULL )
        ApplyTex( FQP_ItmBox[FQP_CItm].x + FQP_ItmBox[FQP_CItm].w + difx +3,
                       FQP_ItmBox[FQP_CItm].y + tooltip_name_nr*(tooltip_lines_name_h[0] -2) + tooltip_statsnr*(tooltip_lines_name_h[0] -4 ) +
                       i*( tooltip_lines_name_h[0] -2) + dify
                      -(tooltip_canvas_h-FQP_ItmBox[FQP_CItm].h)/2,
                       tooltip_lines_desc[ i ], NULL  );
    }
    }

    if( RecItm_ShowTip ){
    AllowInvAcces = true;
    ///ApplySurf( (SCREEN_WIDTH-FQP_Text->w)/2 ,SCREEN_HEIGHT - bot_backslice->h + 20,FQP_Text , NULL );
    ///SDL_Flip( screen );
    RecItm_ShowTip = false;
    }
}
void Dialog::open(DIALOG_TOPIC *CDialTopic){
PLAYER.SPEAK_NPC     = true;
UPDATE_DIALOG = true;
CurrentDialogTopic = CDialTopic;
}
void Dialog::handle_events(SDL_Event dialog_event){//Open Inv w/o timer
SDL_Surface *tempsurf = NULL;
int i,k,x,y;
stringstream ssaux;
if     ( event.type == SDL_KEYDOWN ){
if( event.key.keysym.sym == SDLK_i && (bFinQuePanel == true || AllowInvAcces == true )  )
{
    INVENTORY.open();
}
}
else if( event.type == SDL_MOUSEBUTTONDOWN ){
    x = event.button.x;
    y = event.button.y;

    if( bFinQuePanel == false ){//Standard Dialog
    //Click on a line( Legat de urmatorul topic )
    for( i = 0; i < 4; i++ )
    if( x > box_topic[i].x && x < box_topic[i].x + box_topic[i].w &&
        y > box_topic[i].y && y < box_topic[i].y + box_topic[i].h &&
        ( CTopicInstr[i] != HIDE && !( CurrentDialogTopic->instructions[i] == GIVE_ITEM && INVENTORY.GetFreeSpaces() == 0) )){

            //Close Dialog
            if( CTopicInstr[i] == CLOSE )
            PLAYER.SPEAK_NPC = false;

            else if( CurrentDialogTopic->NextTopic[i] != NULL )
            {
                UPDATE_DIALOG = true;

            newQuest qtemp;
            switch( CTopicInstr[i] )
            {
                case ADD_QUEST:
                QUEST.add( CurrentDialogTopic->val[ i ] );
                break;
                case FINISH_QUEST://S-a verificat mai devreme daca este "finished"

                bFinQuePanel = true;
                FQP_QuestNr = CurrentDialogTopic->val[ i ];

                QUEST.end( CurrentDialogTopic->val[ i ] );

                for( k = 0; k < 10; k ++ )
                FQP_ItmSelected[k] = false;
                qtemp        = DATABASE.GetQuest( CurrentDialogTopic->val[i] );
                FQP_ItmRewNr = qtemp.rew_itm_nrof;
                FQP_RewItmTaken = 0;

                FQP_RewItmLeft = qtemp.rew_itm_take;

                for( k = 0;k < FQP_ItmRewNr;k++ ){
                *FQP_rew_items[k] = *qtemp.RewItems[k];


                ssaux.str("");
                if( FQP_RewItmLeft > 0 )
                ssaux<<"You may take "<<FQP_RewItmLeft;
                if( FQP_RewItmLeft>1 ) ssaux<<" more";
                ssaux<<" item";
                if( FQP_RewItmLeft>1 )ssaux<<'s';

                if( FQP_Text != NULL ) SDL_DestroyTexture( FQP_Text ),FQP_Text = NULL;
                if( tempsurf != NULL ) SDL_FreeSurface(tempsurf),tempsurf = NULL;
                tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() ,WHITE );
                FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );

                FQP_ItmBox[k].x  = (SCREEN_WIDTH - (FQP_ItmRewNr)*75  )/2 +
                                    k*(FQP_ItmBox[0].w + 25);
                }
                break;
                case GIVE_ITEM:
                INVENTORY.additem_to_INV( CurrentDialogTopic->val [i],CurrentDialogTopic->val2[i],
                                          CurrentDialogTopic->val3[i],1 );
                break;
            }
                Dialog::open( CurrentDialogTopic->NextTopic[i] );

            }
        }
        else if( CurrentDialogTopic->instructions[i] == GIVE_ITEM && INVENTORY.GetFreeSpaces() == 0 ){
        RecItm_ShowTip = true;
        if( FQP_Text != NULL ) SDL_DestroyTexture(FQP_Text), FQP_Text = NULL;
        ssaux.str("");
        ssaux<<"Inventory Full.Press (I) to open.";
        if( tempsurf != NULL ) SDL_FreeSurface(tempsurf),tempsurf = NULL;
        tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() ,WHITE );
        FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );
        }
    }
    else{//FINISH QUEST PANEL BUTTONS
    bool found_box = false;
    x = event.button.x;
    y = event.button.y;


    //Push Finish Quest Button
    if( x > FQP_ButtonFinish.x && x < FQP_ButtonFinish.x + FQP_ButtonFinish.w &&
        y > FQP_ButtonFinish.y && y < FQP_ButtonFinish.y + FQP_ButtonFinish.h ){
    found_box = true;
    //Toate itemele luate
    if( FQP_FinishStatus == 0 ){
    FQP_FinishQuest = true;

    //Anime
    ApplyTex( FQP_ButtonFinish.x, FQP_ButtonFinish.y,txFQP_ButtonFQ_down );
    SDL_RenderPresent( RENDER_MAIN );
    SDL_Delay( 200 );
    }
    //Doar cateva iteme luate
    else if( FQP_FinishStatus == 2 ){
    FQP_ButtonTimer   = SDL_GetTicks();
    FQP_ButtonPressed = true;
    }
    }

    for( i = 0; i < FQP_ItmRewNr && found_box == false; i++ )
    if( x  > FQP_ItmBox[i].x && x < FQP_ItmBox[i].x + FQP_ItmBox[i].w &&
        y  > FQP_ItmBox[i].y && y < FQP_ItmBox[i].y + FQP_ItmBox[i].h )
    {
        bool update = false;
        found_box = true;
        if( FQP_ItmSelected[i] == true ){
        FQP_ItmSelected[i] = false;
        FQP_RewItmLeft ++;
        FQP_RewItmTaken--;
        update = true;
        }
        else if( FQP_RewItmLeft > 0 ){
        FQP_ItmSelected[i] = true;
        FQP_RewItmLeft --;
        FQP_RewItmTaken++;
        update = true;
        }

        if( update ){
        ssaux.str("");
        ssaux<<"You may take ";
        if( FQP_RewItmLeft > 0 )  ssaux<<FQP_RewItmLeft;
        else                      ssaux<<"no ";


        if( FQP_RewItmLeft != 1 ||
          ( FQP_RewItmLeft == 1 && FQP_ItmRewNr > 1 ) ) ssaux<<" more";
        ssaux<<" item";
        if( FQP_RewItmLeft != 1 )
        ssaux<<'s';
        if( FQP_Text != NULL ) SDL_DestroyTexture( FQP_Text ),FQP_Text = NULL;
        if( tempsurf != NULL ) SDL_FreeSurface( tempsurf )   ,tempsurf = NULL;
        tempsurf = TTF_RenderText_Blended( Font18, ssaux.str().c_str(),WHITE );
        FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );
        }

    }

    }
}
else if( event.type == SDL_MOUSEMOTION ){
bool found_box = false;
x = event.motion.x;
y = event.motion.y;

if( bFinQuePanel ){

if( x > FQP_ButtonFinish.x && x < FQP_ButtonFinish.x + FQP_ButtonFinish.w &&
    y > FQP_ButtonFinish.y && y < FQP_ButtonFinish.y + FQP_ButtonFinish.h ){
found_box = true;
int inv_space = INVENTORY.GetFreeSpaces();

//Destul spatiu, toate itemele luate
//<<!>> var 'FQP_RewItmLeft' nu este relevanta aici
if( inv_space >= FQP_RewItmTaken && ( FQP_RewItmLeft == 0 ) ){
if( FQP_Text != NULL ) SDL_DestroyTexture(FQP_Text), FQP_Text = NULL;
if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ) , tempsurf = NULL;
tempsurf = TTF_RenderText_Blended( Font18, "You may Finish the Quest",WHITE );
FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
FQP_FinishStatus = 0;}

else if( inv_space < FQP_RewItmTaken ){
if( FQP_Text != NULL ) SDL_DestroyTexture(FQP_Text), FQP_Text = NULL;
if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ) , tempsurf = NULL;
ssaux.str("");
ssaux<<"Inventory Full. "<<FQP_RewItmTaken - inv_space;
ssaux<<" spaces Needed";
tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() ,WHITE );
FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
if( Tip_tx != NULL ) SDL_DestroyTexture( Tip_tx ), Tip_tx = NULL;
if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ) , tempsurf = NULL;
tempsurf = TTF_RenderText_Blended( Font14, "I for Inventory",WHITE );
Tip_tx  = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
FQP_FinishStatus = 1;}
else
if( FQP_RewItmLeft != 0 ){
if( FQP_Text != NULL ) SDL_DestroyTexture(FQP_Text), FQP_Text = NULL;
if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ) , tempsurf = NULL;
ssaux.str("");
ssaux<<"You haven't picked ALL the Items! Hold the Button to finish";
if( FQP_Text != NULL ) SDL_DestroyTexture(FQP_Text), FQP_Text = NULL;
if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ) , tempsurf = NULL;
tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() ,WHITE );
FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
FQP_FinishStatus = 2;}
FQP_RENDER_ONCE = true;
}


for( i  = 0;i < FQP_ItmRewNr; i++ )
if( x > FQP_ItmBox[i].x && x < FQP_ItmBox[i].x + FQP_ItmBox[i].w &&
    y > FQP_ItmBox[i].y && y < FQP_ItmBox[i].y + FQP_ItmBox[i].h ){
    if( i == FQP_LastRewMotion ) break;
    else{
    FQP_RENDER_TOOLTIP = true;
    FQP_RENDER_ONCE    = true;
    FQP_CItm           = i;
    FQP_LastRewMotion  = i;

    RenderTooltip( FQP_rew_items[i],
              &tooltip_canvas ,
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
              /****/

    SDL_QueryTexture( tooltip_canvas,0,0,&tooltip_canvas_w,&tooltip_canvas_h );
    SDL_QueryTexture( tooltip_lines_name[0],0,0,&tooltip_lines_name_w[0],&tooltip_lines_name_h[0] );
    SDL_QueryTexture( tooltip_lines_name[1],0,0,&tooltip_lines_name_w[1],&tooltip_lines_name_h[1] );
    SDL_QueryTexture( tooltip_lines_name[2],0,0,&tooltip_lines_name_w[2],&tooltip_lines_name_h[2] );

    break;
    }
    found_box = true;
    }
    else if( i == FQP_ItmRewNr -1 ){//Niciun item atins
    FQP_LastRewMotion  = -1;
    FQP_RENDER_TOOLTIP = false;
    FQP_RENDER_ONCE    = true;
    }

    if( found_box == false ){
    ssaux.str("");
    ssaux<<"You may take ";
    if( FQP_RewItmLeft > 0 )  ssaux<<FQP_RewItmLeft;
    else                      ssaux<<"no ";


    if( FQP_RewItmLeft != 1 ||
      ( FQP_RewItmLeft == 1 && FQP_ItmRewNr > 1 ) ) ssaux<<" more";
    ssaux<<" item";
    if( FQP_RewItmLeft != 1 )
    ssaux<<'s';
    if( FQP_Text != NULL ) SDL_DestroyTexture(FQP_Text), FQP_Text = NULL;
    if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ) , tempsurf = NULL;
    tempsurf = TTF_RenderText_Blended( Font18, ssaux.str().c_str(),WHITE );
    FQP_Text = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
    FQP_RENDER_ONCE = true;
    }
}


}
else if( event.type == SDL_MOUSEBUTTONUP ){
if( FQP_ButtonPressed == true ){
FQP_ButtonPressed = false;
FQP_RENDER_ONCE = true;
}
}

}
void Dialog::handle_logics(){
int i;
if( FQP_ButtonPressed && FQP_ButtonTimer <= SDL_GetTicks() - 1200 ){

    FQP_ButtonPressed = false;
    FQP_FinishQuest   = true;
    }
    //Finish Quest
    if( FQP_FinishQuest && bFinQuePanel ){
    //Get all the Reward
    for( i = 0; i < FQP_ItmRewNr; i++ )
        if( FQP_ItmSelected[i] ){
        static int subtype;

        if( FQP_rew_items[i]->type == INV_TYPE_ARMOR ) subtype = FQP_rew_items[i]->armor->type;
        else
        subtype = 0;

        INVENTORY.additem_to_INV( FQP_rew_items[i]->type,subtype,FQP_rew_items[i]->nr,1 );
        }
    //Render Topics for current stage


        QUEST.bQUESTDONE[FQP_QuestNr] = true;
        bFinQuePanel            = false;
        FQP_FinishQuest         = false;
        FQP_RENDER_ONCE         = true ;
    }

if( UPDATE_DIALOG ){
int i,k;
bool HaveTopics = false;
SDL_Surface *tempsurf = NULL;

if( bFinQuePanel ){
for( i = 0; i < 10; i ++ ) FQP_ItmSelected[i] = false ;
SDL_QueryTexture( FQP_Text,0,0,&FQP_Text_w,0 );
SDL_QueryTexture( Tip_tx  ,0,0,&Tip_tx_w  ,0 );
}

for( i = 0; i < 3; i ++ )
switch( CurrentDialogTopic->action[i] ){
    case SPAWN:
    GAME_NPCS.manipulate( SPAWN  ,CurrentDialogTopic->act_info[i],CurrentDialogTopic->action_nr[i] ,CurrentDialogTopic->action_name[i]  );
    break;
    case UNSPAWN:
    GAME_NPCS.manipulate( UNSPAWN,CurrentDialogTopic->act_info[i],CurrentDialogTopic->action_nr[i] ,CurrentDialogTopic->action_name[i]  );
    break;
    case DIALACT_INTERMANIP:
    GAME_MAP.manipulate_inter( CurrentDialogTopic->act_id[i],CurrentDialogTopic->action_nr[i],CurrentDialogTopic->act_info[i] );
    break;
    case DIALACT_COLMANIP:
    GAME_MAP.manipulate_colider( CurrentDialogTopic->act_id[i],CurrentDialogTopic->act_info[i] );
    break;
}
for( i = 0; i < 4; i++ )
{
    if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
    tempsurf = TTF_RenderText_Blended( Font18,CurrentDialogTopic->npctext[i].c_str() ,WHITE );
    txDialogtxt[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
}



//Render Topics
k = 0;
HaveTopics = false;
for( i = 0; i < 4; i++ ){
//DEBUG
//DEBUG_outf<<'\n'<<CurrentDialogTopic->TopicName[i]<<' '<<CurrentDialogTopic->instructions[i];
if(  CurrentDialogTopic->instructions[i] == HIDE ||
    ( CurrentDialogTopic->instructions[i] == GIVE_ITEM && INVENTORY.HaveItem( CurrentDialogTopic->val[i],CurrentDialogTopic->val2[i],
                                                                      CurrentDialogTopic->val3[i] ) ) ||
    ( (CurrentDialogTopic->instructions[i] == ADD_QUEST && (QUEST.is_active(CurrentDialogTopic->val[i]) || QUEST.bQUESTDONE[CurrentDialogTopic->val[i]] == true ))) ||
    (CurrentDialogTopic->instructions[i] == FINISH_QUEST && !QUEST.is_finished(CurrentDialogTopic->val[i]))){

box_topic[i].h = box_topic[i].w =
box_topic[i].x = box_topic[i].y = 0;
CTopicInstr[i] = HIDE;
}
else {//if( CurrentDialogTopic->instructions[i] != GIVE_ITEM || ( CurrentDialogTopic->instructions[i] == GIVE_ITEM && INVENTORY.GetFreeSpaces() != 0 ) )

if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;

tempsurf  = TTF_RenderText_Blended( Font18 ,CurrentDialogTopic->TopicName[i].c_str() , WHITE );
txTopic[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );

CTopicInstr[i] = CurrentDialogTopic->instructions[i];
box_topic[i].h = 20;
box_topic[i].w = 500;
box_topic[i].x = 300;
box_topic[i].y = SCREEN_HEIGHT - BotBackslice_h + 20 + k*25;
HaveTopics = true;
k++;
}

}

if( HaveTopics == false ){//Add Close Topic
box_topic[0].h = 20;
box_topic[0].w = 500;
box_topic[0].x = 300;
box_topic[0].y = SCREEN_HEIGHT - BotBackslice_h + 20;
CTopicInstr[0] = CLOSE;

if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
tempsurf  = TTF_RenderText_Blended( Font18 ,"Farewell" , WHITE );
txTopic[0] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
}

UPDATE_DIALOG = false;
}
}
Dialog::~Dialog(){
SDL_FreeSurface( sBackground );
SDL_FreeSurface( sClose );
SDL_FreeSurface( sItmbg );
SDL_FreeSurface( sSlotback );
SDL_FreeSurface( sSlotlock );
SDL_FreeSurface( sFinish );
SDL_FreeSurface( sPgEffect );

}
