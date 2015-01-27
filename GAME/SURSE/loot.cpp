#include "loot.h"


void Loot::init(){
int i;
txBackground = LoadTex( "textures//interface//loot//background.png" );
txButtonTake_up = LoadTex( "textures//interface//loot//ButtonTakeAll_up.png" );
txButtonClose_up = LoadTex( "textures//interface//loot//ButtonClose_up.png" );
for( i = 0; i < 10; i++ )
{
    LootItems[i].type = INV_TYPE_EMPTY;
    LootItems[i].tex  = NULL;
}
for( i = 0; i < 5; i ++ )
{
    rItemBox[i].x = SCREEN_WIDTH/2-145;
    rItemBox[i].y = SCREEN_HEIGHT/2-145 + i*52;
    rItemBox[i].w = rItemBox[i].h = 50;
    txMiscQuant[5] = NULL;
}

rButtonBox_take.x = SCREEN_WIDTH/2-140;
rButtonBox_take.y = SCREEN_HEIGHT/2+112;
rButtonBox_take.w = 97;
rButtonBox_take.h = 30;

rButtonBox_close.x = SCREEN_WIDTH/2+10;
rButtonBox_close.y = SCREEN_HEIGHT/2+112;
rButtonBox_close.w = 97;
rButtonBox_close.h = 30;

tooltip_canvas = NULL;

NULLii.type = INV_TYPE_EMPTY;
NULLii.nr   = -1;
NULLii.tex  = NULL;
NULLii.armor  = NULL;
NULLii.weapon = NULL;
NULLii.bag    = NULL;
NULLii.misc   = NULL;
NULLii.shield = NULL;
}
void Loot::loadtable(int x){
int val[4],i;
string reads;
ifstream file;
stringstream ssaux;
SDL_Surface *tempsurf = NULL;

///Init
NrItems = 0;
srand( SDL_GetTicks() );
for( i = 0; i < 10 ;i++ )
{
    LootItems[i].tex  = NULL;
    LootItems[i].type = INV_TYPE_EMPTY;
}
for( i = 0; i < 5 ;i++ )
if( txMiscQuant[i] != NULL ) SDL_DestroyTexture( txMiscQuant[i] ),txMiscQuant[i] = NULL;

ssaux<<"Data//loottables//"<<x<<".lt";
file.open( ssaux.str().c_str() );


///FAIL Check
if( file.is_open() == false )
{
    ssaux.str("");
    ssaux<<"Can't open loottab"<<x;
    CONSOLE.out(ssaux.str().c_str() );
}
else
///READ ITEMS
while( file.peek() != EOF && !file.fail() && NrItems < 10 )
{

    file>>reads>>val[0];
    file>>reads>>val[1];
    if( val[1] == INV_TYPE_ARMOR || val[1] == INV_TYPE_MISC )
        file>>reads>>val[2];
    else
        val[2] = 0;
    file>>reads>>val[3];

    if( rand()%100 + 1  <= val[3] )
    {
        LootItems[NrItems].nr = val[0];
        LootItems[NrItems].type = val[1];
        //Load Item
        switch( val[1] )
        {
            case INV_TYPE_ARMOR:
            LootItems[NrItems].armor = new Armor;
            *LootItems[NrItems].armor = DATABASE.GetArmorData( val[0],val[2] );
            LootItems[NrItems].tex   = LootItems[NrItems].armor->tex;
            break;
            case INV_TYPE_WEAPON:
            LootItems[NrItems].weapon = new Weapon;
            *LootItems[NrItems].weapon = DATABASE.GetWeaponData( val[0] );
            LootItems[NrItems].tex   = LootItems[NrItems].weapon->tex;
            break;
            case INV_TYPE_SHIELD:
            LootItems[NrItems].shield = new Shield;
            *LootItems[NrItems].shield = DATABASE.GetShieldData( val[0] );
            LootItems[NrItems].tex   = LootItems[NrItems].shield->tex;
            break;
            case INV_TYPE_BAG:
            LootItems[NrItems].bag = new BAG;
            *LootItems[NrItems].bag = DATABASE.GetBagData( val[0] );
            LootItems[NrItems].tex   = LootItems[NrItems].bag->tex;
            break;
            case INV_TYPE_MISC:
            LootItems[NrItems].misc = new Misc;
            *LootItems[NrItems].misc = DATABASE.GetMiscData( val[0] );
            LootItems[NrItems].misc->quant = val[2];
            LootItems[NrItems].tex   = LootItems[NrItems].misc->tex;

            ssaux.str("");
            ssaux<<LootItems[NrItems].misc->quant;

            if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ), tempsurf = NULL;
            tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() ,WHITE );
            txMiscQuant[NrItems] = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );
            break;
        }
         NrItems++;
    }


}

}
void Loot::handle_events(SDL_Event event){
int x,y,i;

if( event.type == SDL_MOUSEBUTTONDOWN )
{
    x = event.button.x;
    y = event.button.y;

    if( x > rButtonBox_close.x && x < rButtonBox_close.x + rButtonBox_close.w &&
        y > rButtonBox_close.y && y < rButtonBox_close.y + rButtonBox_close.h )
    {
        LOOT.SHOW_LOOT = false;
        PLAYER.AvailableInterSpot->type = -1;
    }
    else
    if( x > rButtonBox_take.x && x < rButtonBox_take.x + rButtonBox_take.w &&
        y > rButtonBox_take.y && y < rButtonBox_take.y + rButtonBox_take.h )
    {
        int quant = 0,subtype = 0;
        bool TakeAllItems = true;
        for( i = 0; i < 5 ; i++ )
            if( LootItems[i].type != INV_TYPE_EMPTY )
        {
            quant = 0;
            subtype = 0;
            if( LootItems[i].type == INV_TYPE_MISC  )quant = LootItems[i].misc->quant;
            else
            if( LootItems[i].type == INV_TYPE_ARMOR )subtype = LootItems[i].armor->type;


            if( INVENTORY.GetFreeSpaces() > 0 )
            {
                INVENTORY.additem_to_INV(LootItems[i].type,subtype,LootItems[i].nr,quant );
                LootItems[i] = NULLii;
            }
            else
            {
                if( LootItems[i].type == INV_TYPE_MISC )
                {
                    int quant;
                    if( INVENTORY.HaveItem( INV_TYPE_MISC,0,LootItems[i].nr,&quant ) )
                    {
                        if( LootItems[i].misc->quant + quant <= LootItems[i].misc->maxstack )
                        {
                            INVENTORY.additem_to_INV(INV_TYPE_MISC,0,LootItems[i].nr,quant );
                        }
                        else
                        {
                            stringstream ssaux;
                            static SDL_Surface *tempsurf = NULL;
                            INVENTORY.additem_to_INV(INV_TYPE_MISC,0,LootItems[i].nr,
                                                     LootItems[i].misc->maxstack - quant );

                            LootItems[i].misc->quant -= LootItems[i].misc->maxstack - quant;

                            //Rerender nr
                            ssaux.str("");
                            ssaux<<LootItems[i].misc->quant;

                            if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
                            SDL_DestroyTexture( txMiscQuant[i] ),txMiscQuant[i] = NULL;
                            tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() , WHITE );
                            txMiscQuant[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );


                            TakeAllItems = false;
                        }
                    }
                }
                else
                TakeAllItems = false;
            }
        }

        if( TakeAllItems )
        {
            LOOT.SHOW_LOOT = false;
            PLAYER.AvailableInterSpot->type = -1;
        }


    }
    else
    if( event.button.button == SDL_BUTTON_RIGHT )
    {
        for( i = 0; i < 5 ; i++ )
        if( x > rItemBox[i].x && x < rItemBox[i].x + rItemBox[i].w &&
            y > rItemBox[i].y && y < rItemBox[i].y + rItemBox[i].h &&
            LootItems[i].type != INV_TYPE_EMPTY )
        {
            int quant = 0,subtype = 0;
            if( LootItems[i].type == INV_TYPE_MISC  )quant = LootItems[i].misc->quant;
            else
            if( LootItems[i].type == INV_TYPE_ARMOR )subtype = LootItems[i].armor->type;

            if( INVENTORY.GetFreeSpaces() > 0 )
            {
                INVENTORY.additem_to_INV(LootItems[i].type,subtype,LootItems[i].nr,quant );
                LootItems[i] = NULLii;
            }
            else
            {
                if( LootItems[i].type == INV_TYPE_MISC )
                {
                    int quant;
                    if( INVENTORY.HaveItem( INV_TYPE_MISC,0,LootItems[i].nr,&quant ) )
                    {
                        if( LootItems[i].misc->quant + quant <= LootItems[i].misc->maxstack )
                        {
                            INVENTORY.additem_to_INV(INV_TYPE_MISC,0,LootItems[i].nr,quant );
                        }
                        else
                        {
                            stringstream ssaux;
                            static SDL_Surface *tempsurf = NULL;
                            INVENTORY.additem_to_INV(INV_TYPE_MISC,0,LootItems[i].nr,
                                                     LootItems[i].misc->maxstack - quant );

                            LootItems[i].misc->quant -= LootItems[i].misc->maxstack - quant;

                            //Rerender nr
                            ssaux.str("");
                            ssaux<<LootItems[i].misc->quant;

                            if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
                            SDL_DestroyTexture( txMiscQuant[i] ),txMiscQuant[i] = NULL;
                            tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str() , WHITE );
                            txMiscQuant[i] = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );

                        }
                    }
                }
            }


            break;
        }
    }
}
else
if( event.type == SDL_MOUSEMOTION )
{
    x = event.motion.x;
    y = event.motion.y;

    for( i = 0; i < 5; i ++ )
    if( x > rItemBox[i].x && x < rItemBox[i].x + rItemBox[i].w &&
        y > rItemBox[i].y && y < rItemBox[i].y + rItemBox[i].h && LootItems[i].type != INV_TYPE_EMPTY )
    {
        cItem = i;

        RenderTooltip( &LootItems[i],
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

              for( i = 0; i < 3; i ++ )
              SDL_QueryTexture( tooltip_lines_name[i],0,0,
                                &tooltip_lines_name_w[i],
                                &tooltip_lines_name_h[i] );

        break;
    }
    else if( i == 4 ) cItem = -1;
}

}
void Loot::show(){
int i;

ApplyTex( SCREEN_WIDTH/2-150,SCREEN_HEIGHT/2-150,txBackground );
for( i = 0; i < 5 ; i++ )
{
    ApplyTex( rItemBox[i].x ,rItemBox[i].y ,LootItems[i].tex );
    if( LootItems[i].type == INV_TYPE_MISC )
    {
        ApplyTex( rItemBox[i].x + 5 ,rItemBox[i].y + 30 ,txMiscQuant[i] );
    }
}

ApplyTex( rButtonBox_take.x ,rButtonBox_take.y ,txButtonTake_up );
ApplyTex( rButtonBox_close.x ,rButtonBox_close.y ,txButtonClose_up );


///TOOLTIP
if( cItem != -1 )
{
    ApplyTex( SCREEN_WIDTH/2-90 ,SCREEN_HEIGHT/2-150 + cItem*52 ,tooltip_canvas, NULL );

    for( i = 0; i < tooltip_name_nr; i++ )
    if( tooltip_lines_name[i] != NULL )
    ApplyTex( SCREEN_WIDTH/2-90 + 50 +(tooltip_canvas_w-tooltip_lines_name_w[i])/2 ,
              i*(tooltip_lines_name_h[i] - 6) + SCREEN_HEIGHT/2-150 + cItem*52 ,
              tooltip_lines_name[ i ] );

    //STats I : Chest/cloak .. etc
    if( tooltip_stats[0] != NULL )
        ApplyTex( SCREEN_WIDTH/2-85,
                  (tooltip_name_nr + 1)*(tooltip_lines_name_h[0] - 6) + SCREEN_HEIGHT/2-150 + cItem*52,
                  tooltip_stats[ 0 ] );

    //Stats II -> N
    for( i = 1; i < tooltip_statsnr; i++ )
    {
        if( tooltip_stats[i] != NULL )
        ApplyTex( SCREEN_WIDTH/2-85,
                  (tooltip_name_nr + 1)*(tooltip_lines_name_h[0] - 6) + SCREEN_HEIGHT/2-150 +
                  (tooltip_lines_name_h[0] - 5) *i + cItem*52 ,
                  tooltip_stats[ i ]  );
    }


    for( i = 0; i < tooltip_desc_nr; i++ )
    {
        if( tooltip_lines_desc[i] != NULL )
        ApplyTex( SCREEN_WIDTH/2-85,
                  (tooltip_name_nr + 1)*(tooltip_lines_name_h[0] - 6) + SCREEN_HEIGHT/2-150 +
                  (tooltip_lines_name_h[0] - 5) *tooltip_statsnr + i*(tooltip_lines_name_h[0] - 5)+ cItem*52 ,
                  tooltip_lines_desc[ i ], NULL  );
    }
}

}
