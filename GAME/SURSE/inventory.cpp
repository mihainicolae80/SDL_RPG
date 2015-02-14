#include "inventory.h"


bool inventory::init(){
int i,j,k,l;

RENDER_ONCE = false;
RENDER_HERO_ONCE = false;
RENDER_Trans = false;///true
bTransfInvItem = false;
motion_CSlot = -1;

tooltipcanvas_h = 0;

background    = LoadTex( "textures//inventory//background.png"    );
slot_empty    = LoadTex( "textures//inventory//slot_empty.png"    );
slot_armor    = LoadTex( "textures//inventory//slot_armor.png"    );
slot_weapon   = LoadTex( "textures//inventory//slot_weapon.png"   );
slot_cloak    = LoadTex( "textures//inventory//slot_cloak.png"    );
slot_shield   = LoadTex( "textures//inventory//slot_shield.png"   );
surf_drop     = LoadTex( "textures//inventory//slot_drop.png"     );
surf_stats_bg = LoadTex( "textures//inventory//surf_stats_bg.png" );

player_bg = LoadTex( "textures//inventory//player_background.png" );

PLAYER.txEQUIPT_Chest = NULL;
PLAYER.txEQUIPT_Cloak = NULL;

sStats_Health  = NULL;
sStats_Defense = NULL;
sStats_Damage  = NULL;

txStats_Health  = NULL;
txStats_Defense = NULL;
txStats_Damage  = NULL;
txStats_Protect = NULL;

bStats_Update = false;///true
bTooltip_Update = false;
bToolTip_SearchChunkPos = false;

SDL_QueryTexture( background,0,0,&Background_w,&Background_h );
SDL_QueryTexture( surf_stats_bg,0,0,&SurfStats_w,&SurfStats_h );
SDL_QueryTexture( surf_drop,0,0,&surf_drop_w,&surf_drop_h );

rStats_MainRect.x = (SCREEN_WIDTH -Background_w)/2 + 50;
rStats_MainRect.y = (SCREEN_HEIGHT-Background_h)/2 + 130;
rStats_MainRect.w = SurfStats_w;
rStats_MainRect.h = SurfStats_h;

rect_drop.x = ( SCREEN_WIDTH -Background_w )/2 + 600;
rect_drop.y = ( SCREEN_HEIGHT-Background_h )/2 + 200;
rect_drop.w = surf_drop_w;
rect_drop.h = surf_drop_h;

player_rect.x = SCREEN_WIDTH/2 - Background_w/2 + 410;
player_rect.y = SCREEN_HEIGHT/2 + Background_h/2 - 600;
player_rect.w = 120;
player_rect.h = 120;

player_cframe_07 = 0;
player_cframe_04 = 0;
PlayerFrame_timer_04 = SDL_GetTicks();
PlayerFrame_timer_07 = SDL_GetTicks();

tooltip_highlight      = LoadTex( "textures//inventory//tooltip_highlight.PNG" );
tooltip_highlight_flip = LoadTex( "textures//inventory//tooltip_highlight_flip.PNG" );
tooltip_canvas     = NULL;
tooltip_lines_name[0] = tooltip_lines_name[1] = tooltip_lines_name[2] = NULL;
for( i = 0; i < 6; i++ )
tooltip_stats[i] = NULL;
for( i = 0; i < 6; i++ )
tooltip_lines_desc[i] = NULL;
motion_CBag = motion_CSlot_row = motion_CSlot_col = -1;

NULLii.armor = NULL;
NULLii.bag = NULL;
NULLii.weapon = NULL;
NULLii.next = NULL;
NULLii.nr = -1;
NULLii.tex = NULL;
NULLii.type = INV_TYPE_EMPTY;

for( i = 0; i < 3; i ++ ){
rStats_StatBox[i].x = 0;
rStats_StatBox[i].y = 0;
rStats_StatBox[i].w = SurfStats_w;
rStats_StatBox[i].h = SurfStats_h;
}
for( i = 0; i < 3; i ++ ){
    //Init Lists
    //FirstItemBag[i] = new Inventory_item;
    //LastItemBag[i]  = new Inventory_item;
    //FirstItemBag[i]->next = LastItemBag[i];
    //LastItemBag[i]->next = NULL;

    //Add rect on Bag slot
    k = l = 0;
    for( j = 0; j < 25; j ++ )
    {
        BagSlotRect[i][j].x = SCREEN_WIDTH/2  - Background_w/2 + 45  + 52*k
                              + i*((52*5) + (Background_w - 52*15)/4 );
        BagSlotRect[i][j].y = SCREEN_HEIGHT/2 + Background_h/2 - 300 + 52*l;
        BagSlotRect[i][j].w = 52;
        BagSlotRect[i][j].h = 52;
        k++;
        if( k > 4 )
        {
            k = 0;
            l++;
        }
    }
}
for( i = 0; i < 3; i ++ ){
    BagRect[i].h = 52;
    BagRect[i].w = 52;
    BagRect[i].x = SCREEN_WIDTH/2  - Background_w/2 + 45  + 52*2
                              + i*((52*5) + (Background_w - 52*15)/4 );  //SCREEN_WIDTH/2 - background->w/2 + background->w/3*(i+1) - background->w/( 3*2 ) - slot_empty->w/2;
    BagRect[i].y = SCREEN_HEIGHT/2 + Background_h/2 - 370;//400

    BagDropInSlots[i] = new Inventory_item;
    BagDropInSlots[i]->type = INV_TYPE_EMPTY;
    BagDropInSlots[i]->tex = NULL;

for( j = 0; j < 25; j ++ )
{
    BagSlot[i][j] = new Inventory_item;
    BagSlot[i][j]->tex = NULL;
    BagSlot[i][j]->type = INV_TYPE_EMPTY;
    BagSlot[i][j]->nr   = -1;
}
}

//weapon
PlayerSlotRect[0].w = 50;
PlayerSlotRect[0].h = 50;
PlayerSlotRect[0].x = SCREEN_WIDTH/2 - Background_w/2 + 400;
PlayerSlotRect[0].y = SCREEN_HEIGHT/2 + Background_h/2 - 450;
//chest armor
PlayerSlotRect[1].w = 50;
PlayerSlotRect[1].h = 50;
PlayerSlotRect[1].x = SCREEN_WIDTH/2 - Background_w/2 + 460;
PlayerSlotRect[1].y = SCREEN_HEIGHT/2 + Background_h/2 - 450;
//cloak armor
PlayerSlotRect[2].w = 50;
PlayerSlotRect[2].h = 50;
PlayerSlotRect[2].x = SCREEN_WIDTH/2 - Background_w/2 + 520;
PlayerSlotRect[2].y = SCREEN_HEIGHT/2 + Background_h/2 - 450;
//Shield
PlayerSlotRect[3].w = 50;
PlayerSlotRect[3].h = 50;
PlayerSlotRect[3].x = SCREEN_WIDTH/2 - Background_w/2 + 590;
PlayerSlotRect[3].y = SCREEN_HEIGHT/2 + Background_h/2 - 450;

PlayerChestSlot  = new Inventory_item;
PlayerWeaponSlot = new Inventory_item;
PlayerCloakSlot  = new Inventory_item;
PlayerShieldSlot = new Inventory_item;

PlayerChestSlot->type = INV_TYPE_EMPTY;
PlayerChestSlot->tex = NULL;
PlayerWeaponSlot->type = INV_TYPE_EMPTY;
PlayerWeaponSlot->tex = NULL;
PlayerCloakSlot->type = INV_TYPE_EMPTY;
PlayerCloakSlot->tex = NULL;
PlayerShieldSlot->type = INV_TYPE_EMPTY;
PlayerShieldSlot->tex = NULL;

return true;
}
void inventory::additem_to_INV(int type,int subtype ,int nr,int quant){
int i,j;
bool found_slot = false;

if( type == INV_TYPE_MISC )
for( i = 0; i < 3 && found_slot == false ; i++ )
    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
        for( j = 0; j < BagDropInSlots[i]->bag->slots && found_slot == false && quant > 0 ; j++ )
        if( BagSlot[i][j]->type == INV_TYPE_MISC && BagSlot[i][j]->nr == nr )
        {
            if( BagSlot[i][j]->misc->quant < BagSlot[i][j]->misc->maxstack )
            {
                if( BagSlot[i][j]->misc->maxstack - BagSlot[i][j]->misc->quant >= quant )
                {
                    BagSlot[i][j]->misc->quant += quant;
                    quant = 0;
                }
                else
                {
                    quant -= BagSlot[i][j]->misc->maxstack - BagSlot[i][j]->misc->quant;
                    BagSlot[i][j]->misc->quant = BagSlot[i][j]->misc->maxstack;
                }

                if( quant == 0 ) found_slot = true;
            }
        }


for( i = 0; i < 3 && found_slot == false ; i++ )
    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
        for( j = 0; j < BagDropInSlots[i]->bag->slots && found_slot == false ; j++ )
        if( BagSlot[i][j]->type == INV_TYPE_EMPTY )
        {
            switch( type )
            {
                case INV_TYPE_ARMOR:
                BagSlot[i][j]->armor = new Armor;
                *BagSlot[i][j]->armor  = DATABASE.GetArmorData  ( nr,subtype );
                BagSlot[i][j]->tex = BagSlot[i][j]->armor->tex;
                break;
                case INV_TYPE_WEAPON:
                BagSlot[i][j]->weapon = new Weapon;
                *BagSlot[i][j]->weapon = DATABASE.GetWeaponData ( nr );
                BagSlot[i][j]->tex = BagSlot[i][j]->weapon->tex;
                break;
                case INV_TYPE_BAG:
                BagSlot[i][j]->bag = new BAG;
                *BagSlot[i][j]->bag = DATABASE.GetBagData( nr );
                BagSlot[i][j]->tex = BagSlot[i][j]->bag->tex;
                break;
                case INV_TYPE_SHIELD:
                BagSlot[i][j] ->shield = new Shield;
                *BagSlot[i][j]->shield = DATABASE.GetShieldData( nr );
                BagSlot[i][j] ->tex   = BagSlot[i][j]->shield->tex;
                break;
                case INV_TYPE_MISC:
                BagSlot[i][j] ->misc = new Misc;
                *BagSlot[i][j]->misc = DATABASE.GetMiscData( nr );
                BagSlot[i][j] ->tex  = BagSlot[i][j]->misc->tex;
                BagSlot[i][j] ->misc->quant = quant;
                break;
            }
            BagSlot[i][j]->type = type;
            BagSlot[i][j]->nr = nr;
            found_slot = true;
        }

}
void inventory::init_bags(){
int i;

//BagDropIn Memory All
for( i = 0; i < 3; i ++ ){
    BagDropInSlots[i]->tex = NULL;
    BagDropInSlots[i]->bag = new BAG;
}

BagDropInSlots[0]->type = INV_TYPE_BAG;
BagDropInSlots[1]->type = INV_TYPE_EMPTY;
BagDropInSlots[2]->type = INV_TYPE_EMPTY;

*BagDropInSlots[0]->bag = DATABASE.GetBagData( 1 );
BagDropInSlots[1]->bag = NULL;
BagDropInSlots[2]->bag = NULL;

BagDropInSlots[0]->tex = BagDropInSlots[0]->bag->tex;
BagDropInSlots[1]->tex = NULL;
BagDropInSlots[2]->tex = NULL;

BagDropInSlots[0]->bag->slots = 25;

}
void inventory::addTestItems(){
additem_to_INV( INV_TYPE_ARMOR ,ARMOR_TYPE_CHEST ,2 ,0 );
additem_to_INV( INV_TYPE_ARMOR ,ARMOR_TYPE_CHEST ,2 ,0 );
additem_to_INV( INV_TYPE_ARMOR ,ARMOR_TYPE_CHEST ,1 ,0 );
additem_to_INV( INV_TYPE_ARMOR ,ARMOR_TYPE_CLOAK ,1 ,0 );
additem_to_INV( INV_TYPE_WEAPON,0,2 ,0 );
additem_to_INV( INV_TYPE_BAG   ,0,1 ,0 );
additem_to_INV( INV_TYPE_BAG   ,0,1 ,0 );
additem_to_INV( INV_TYPE_BAG   ,0,1 ,0 );
additem_to_INV( INV_TYPE_BAG   ,0,1 ,0 );
additem_to_INV( INV_TYPE_BAG   ,0,2 ,0 );
additem_to_INV( INV_TYPE_SHIELD,0,1 ,0 );
additem_to_INV( INV_TYPE_MISC  ,0,1 ,2 );
additem_to_INV( INV_TYPE_MISC  ,0,2 ,1 );
additem_to_INV( INV_TYPE_MISC  ,0,2 ,6 );
}
void inventory::open(){
bool run_inventory = true,found_box = false;
int i,j,k,mouse_x,mouse_y,
    row,col,CBag,CSlot;
SDL_Surface *tempsurf = NULL;

Inventory_item *CurrentInvItm;
stringstream ssaux;

RENDER_ONCE = true;
bStats_Update = true;

///Main LOOP
while( run_inventory ){

    ///EVENTS
    while( SDL_PollEvent( &event ) ){

        if( event.type == SDL_KEYDOWN && ( event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_i ) ){
            run_inventory = false;
        }
        else if( event.type == SDL_MOUSEBUTTONDOWN ){
            mouse_x = mousetovirtual(event.button.x);
            mouse_y = mousetovirtual(event.button.y);





            if( event.button.button == SDL_BUTTON_LEFT )
            {
            found_box = false;

            ///Character DropIn Slots
            //Weapon
            if( mouse_x > PlayerSlotRect[0].x && mouse_x < PlayerSlotRect[0].x + PlayerSlotRect[0].w &&
                mouse_y > PlayerSlotRect[0].y && mouse_y < PlayerSlotRect[0].y + PlayerSlotRect[0].h )
            {
                if( PlayerWeaponSlot->type != INV_TYPE_EMPTY )
                {
                    plus_w = PlayerSlotRect[0].x - mouse_x;
                    plus_h = PlayerSlotRect[0].y - mouse_y;
                    TransfInvItem = *PlayerWeaponSlot;
                    *PlayerWeaponSlot = NULLii;
                    TransfFirst = PlayerWeaponSlot;
                    bTransfInvItem = true;
                    bStats_Update = true;
                    RENDER_ONCE = true;
                    found_box = true;
                }
            }
            //Chest
            if( mouse_x > PlayerSlotRect[1].x && mouse_x < PlayerSlotRect[1].x + PlayerSlotRect[1].w &&
                mouse_y > PlayerSlotRect[1].y && mouse_y < PlayerSlotRect[1].y + PlayerSlotRect[1].h )
            {
                if( PlayerChestSlot->type != INV_TYPE_EMPTY )
                {
                    plus_w = PlayerSlotRect[1].x - mouse_x;
                    plus_h = PlayerSlotRect[1].y - mouse_y;
                    TransfInvItem = *PlayerChestSlot;
                    *PlayerChestSlot = NULLii;
                    TransfFirst = PlayerChestSlot;
                    bTransfInvItem = true;
                    bStats_Update = true;
                    RENDER_ONCE = true;
                    found_box = true;
                }
            }
            //Cloak
            else if( mouse_x > PlayerSlotRect[2].x && mouse_x < PlayerSlotRect[2].x + PlayerSlotRect[2].w &&
                     mouse_y > PlayerSlotRect[2].y && mouse_y < PlayerSlotRect[2].y + PlayerSlotRect[2].h )
            {
                if( PlayerCloakSlot->type != INV_TYPE_EMPTY )
                {
                    plus_w = PlayerSlotRect[2].x - mouse_x;
                    plus_h = PlayerSlotRect[2].y - mouse_y;
                    TransfInvItem = *PlayerCloakSlot;
                    *PlayerCloakSlot = NULLii;
                    TransfFirst = PlayerCloakSlot;
                    bTransfInvItem = true;
                    bStats_Update = true;
                    RENDER_ONCE = true;
                    found_box = true;
                }
            }

            //Check bags
            for( i = 0 ; i < 3 &&  found_box == false  ; i ++ )
            {
                if( BagDropInSlots[i]->type == INV_TYPE_BAG &&
                    mouse_x > BagRect[i].x && mouse_x < BagRect[i].x + BagRect[i].w &&
                    mouse_y > BagRect[i].y && mouse_y < BagRect[i].y + BagRect[i].h )
                    {
                        for( j = 0; j < BagDropInSlots[i]->bag->slots; j++ )
                        if( BagSlot[i][j]->type != INV_TYPE_EMPTY )
                        {
                            //Bag-ul are iteme in el
                            break;
                        }
                        else if( j == BagDropInSlots[i]->bag->slots -1 )
                        {
                            TransfInvItem = *BagDropInSlots[i];
                            BagDropInSlots[i]->type = INV_TYPE_EMPTY;
                            BagDropInSlots[i]->bag  = NULL;
                            BagDropInSlots[i]->tex = NULL;
                            TransfFirst = BagDropInSlots[i];
                            bTransfInvItem = true;
                            break;
                        }
                        found_box = true;
                    }

            }
            //Chech Shield
            if( mouse_x > PlayerSlotRect[3].x && mouse_x < PlayerSlotRect[3].x + PlayerSlotRect[3].w &&
                mouse_y > PlayerSlotRect[3].y && mouse_y < PlayerSlotRect[3].y + PlayerSlotRect[3].h )
            {
                if( PlayerShieldSlot->type != INV_TYPE_EMPTY )
                {
                    plus_w = PlayerSlotRect[3].x - mouse_x;
                    plus_h = PlayerSlotRect[3].y - mouse_y;
                    TransfInvItem = *PlayerShieldSlot;
                    *PlayerShieldSlot = NULLii;
                    TransfFirst = PlayerShieldSlot;
                    bTransfInvItem = true;
                    bStats_Update  = true;
                    RENDER_ONCE    = true;
                    found_box      = true;
                }
            }
            //Check slots
            for( i = 0 ; i < 3 &&  found_box == false  ; i ++ )
            {
                if( BagDropInSlots[i]->type == INV_TYPE_BAG )
                for( j = 0; j < BagDropInSlots[i]->bag->slots && found_box == false ; j++ )
                {
                    if( mouse_x > BagSlotRect[i][j].x && mouse_x < BagSlotRect[i][j].x + BagSlotRect[i][j].w &&
                        mouse_y > BagSlotRect[i][j].y && mouse_y < BagSlotRect[i][j].y + BagSlotRect[i][j].h &&
                        BagSlot[i][j]->type != INV_TYPE_EMPTY )
                    {
                        TransfInvItem = *BagSlot[i][j];
                        TransfFirst = BagSlot[i][j];

                        plus_w = BagSlotRect[i][j].x - mouse_x;
                        plus_h = BagSlotRect[i][j].y - mouse_y;

                        BagSlot[i][j]->armor = NULL;
                        BagSlot[i][j]->weapon = NULL;
                        BagSlot[i][j]->bag = NULL;
                        BagSlot[i][j]->tex = NULL;
                        BagSlot[i][j]->type = INV_TYPE_EMPTY;

                        RENDER_ONCE = true;
                        bTransfInvItem = true;
                        found_box = true;
                    }
                }
            }

            }
            else if( event.button.button == SDL_BUTTON_RIGHT )
            {

                found_box = false;

                for( i = 0; i < 3 && !found_box; i ++ )
                {
                    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
                    for( j = 0; j < BagDropInSlots[i]->bag->slots && found_box == false ; j++ )
                    {
                        if( mouse_x > BagSlotRect[i][j].x && mouse_x < BagSlotRect[i][j].x + BagSlotRect[i][j].w &&
                            mouse_y > BagSlotRect[i][j].y && mouse_y < BagSlotRect[i][j].y + BagSlotRect[i][j].h )
                        {
                            if( BagSlot[i][j]->type != INV_TYPE_MISC )
                            {
                                TransfInvItem = *BagSlot[i][j];

                                //ARMOR CHEST
                                if( TransfInvItem.type == INV_TYPE_ARMOR &&
                                    TransfInvItem.armor->type == ARMOR_TYPE_CHEST )
                                   {
                                       if( PlayerChestSlot->type != INV_TYPE_EMPTY )
                                       *BagSlot[i][j] = *PlayerChestSlot;
                                       else
                                       *BagSlot[i][j] = NULLii;

                                       *PlayerChestSlot = TransfInvItem;

                                       PlayerChestSlot->tex = PlayerChestSlot->armor->tex;

                                        bStats_Update = true;
                                   }
                                //ARMOR CLOAK
                                else if( TransfInvItem.type == INV_TYPE_ARMOR &&
                                    TransfInvItem.armor->type == ARMOR_TYPE_CLOAK )
                               {
                                   if( PlayerCloakSlot->type != INV_TYPE_EMPTY )
                                   *BagSlot[i][j] = *PlayerCloakSlot;
                                   else
                                   *BagSlot[i][j] = NULLii;

                                   *PlayerCloakSlot = TransfInvItem;
                                    PlayerCloakSlot->tex = PlayerCloakSlot->armor->tex;

                                    bStats_Update = true;
                               }
                               //WEAPON
                               else if( TransfInvItem.type == INV_TYPE_WEAPON )
                               {
                                   if( PlayerWeaponSlot->type != INV_TYPE_EMPTY )
                                   *BagSlot[i][j] = *PlayerWeaponSlot;
                                   else
                                   *BagSlot[i][j] = NULLii;

                                   *PlayerWeaponSlot = TransfInvItem;
                                   PLAYER.dmg_weapon = PlayerWeaponSlot->weapon->dmg;

                                   bStats_Update = true;
                               }
                               //BAG
                               else if( TransfInvItem.type == INV_TYPE_BAG )
                               {
                                   for( k = 0; k < 3;k ++ )
                                   if( BagDropInSlots[k]->type == INV_TYPE_EMPTY )
                                   {
                                       *BagDropInSlots[k] = TransfInvItem;
                                       TransfInvItem = NULLii;
                                       BagSlot[i][j]->type = INV_TYPE_EMPTY;
                                       BagSlot[i][j]->tex = NULL;
                                       break;
                                   }
                               }
                               //Shield
                               else if( TransfInvItem.type == INV_TYPE_SHIELD )
                               {
                                   if( PlayerWeaponSlot->type != INV_TYPE_EMPTY )
                                   *BagSlot[i][j] = *PlayerShieldSlot;
                                   else
                                   *BagSlot[i][j] = NULLii;

                                   *PlayerShieldSlot = TransfInvItem;

                                   PLAYER.def_itm_shield     = TransfInvItem.shield->def;
                                   PLAYER.protect_itm_shield = TransfInvItem.shield->protect;

                                   bStats_Update = true;
                               }

                            }
                            else ///Type == INV_TYPE_MISC
                            {
                                if( BagSlot[i][j]->misc->usable )
                                {
                                    if( BagSlot[i][j]->misc->effect_type == "heal_points" )
                                    {
                                        if( PLAYER._health_c < PLAYER._health_max )
                                        {
                                            PLAYER._health_c += BagSlot[i][j]->misc->effect_val;
                                            if( PLAYER._health_c > PLAYER._health_max )
                                            PLAYER._health_c = PLAYER._health_max;
                                        }
                                        else
                                        {
                                            BagSlot[i][j]->misc->quant++;
                                        }

                                    }
                                    else
                                    if( BagSlot[i][j]->misc->effect_type == "explode_door" )
                                    {
                                        if( PLAYER.GetZone() == 7 )
                                        GAME_EVENTS.add( Event_RequestTriggerScript,1 );
                                        else
                                        BagSlot[i][j]->misc->quant++;
                                    }
                                    BagSlot[i][j]->misc->quant--;
                                    if( BagSlot[i][j]->misc->quant <= 0 )
                                    *BagSlot[i][j] = NULLii;
                                }
                            }

                            RENDER_ONCE = true;
                            found_box = true;
                        }
                    }
                }
                if( found_box )
                motion_CBag = -1;

            }

            if( found_box )
            motion_CBag = -1;

        }
        else if( event.type == SDL_MOUSEBUTTONUP ){
            mouse_x = mousetovirtual(event.button.x);
            mouse_y = mousetovirtual(event.button.y);

            if( bTransfInvItem )
            {

            found_box = false;

            ///Character DropIn Slots
            //*Armor
            if( TransfInvItem.type == INV_TYPE_ARMOR )
            {
                //*Chest
                if( TransfInvItem.armor->type == ARMOR_TYPE_CHEST &&
                    mouse_x > PlayerSlotRect[1].x && mouse_x < PlayerSlotRect[1].x + PlayerSlotRect[1].w &&
                    mouse_y > PlayerSlotRect[1].y && mouse_y < PlayerSlotRect[1].y + PlayerSlotRect[1].h )
                {
                    if( PlayerChestSlot->type == INV_TYPE_EMPTY )
                    *TransfFirst = NULLii;
                    else
                    *TransfFirst = *PlayerChestSlot;

                    *PlayerChestSlot = TransfInvItem;
                    PlayerChestSlot->tex = PlayerChestSlot->armor->tex;

                    bStats_Update = true;
                    found_box = true;
                }
                //*Cloak
                else if( TransfInvItem.armor->type == ARMOR_TYPE_CLOAK &&
                    mouse_x > PlayerSlotRect[2].x && mouse_x < PlayerSlotRect[2].x + PlayerSlotRect[2].w &&
                    mouse_y > PlayerSlotRect[2].y && mouse_y < PlayerSlotRect[2].y + PlayerSlotRect[2].h )
                {
                    if( PlayerCloakSlot->type == INV_TYPE_EMPTY )
                    *TransfFirst = NULLii;
                    else
                    *TransfFirst = *PlayerCloakSlot;

                    *PlayerCloakSlot = TransfInvItem;
                    PlayerCloakSlot->tex = PlayerCloakSlot->armor->tex;

                    bStats_Update = true;
                    found_box = true;
                }
            }

            //*Weapon
            if( TransfInvItem.type == INV_TYPE_WEAPON )
            if( mouse_x > PlayerSlotRect[0].x && mouse_x < PlayerSlotRect[0].x + PlayerSlotRect[0].w &&
                mouse_y > PlayerSlotRect[0].y && mouse_y < PlayerSlotRect[0].y + PlayerSlotRect[0].h )
            {
                if( PlayerWeaponSlot->type == INV_TYPE_EMPTY )
                *TransfFirst = NULLii;
                else
                *TransfFirst = *PlayerWeaponSlot;

                *PlayerWeaponSlot = TransfInvItem;
                PlayerWeaponSlot->tex = PlayerWeaponSlot->weapon->tex;
                PLAYER.dmg_weapon = PlayerWeaponSlot->weapon->dmg;
                bStats_Update = true;
                found_box = true;
            }

            //IF release a bag
            if( TransfInvItem.type == INV_TYPE_BAG ){
                //Check Bag Drop-In slots
                for( i = 0; i < 3 && found_box == false ; i++ )
                if( mouse_x > BagRect[i].x && mouse_x < BagRect[i].x + BagRect[i].w &&
                    mouse_y > BagRect[i].y && mouse_y < BagRect[i].y + BagRect[i].h )
                {
                    if( BagDropInSlots[i]->type == INV_TYPE_EMPTY )
                    {
                        *BagDropInSlots[i] = TransfInvItem;
                        TransfInvItem = NULLii;

                        if( TransfFirst != BagDropInSlots[i] )
                        {
                            TransfFirst->type = INV_TYPE_EMPTY;
                            TransfFirst->tex = NULL;
                        }


                        //Add new spaces

                        int tempi;

                        for( tempi = 0; tempi < BagDropInSlots[i]->bag->slots; tempi++ )
                        {
                            BagSlot[i][tempi] = new Inventory_item;
                            BagSlot[i][tempi]->tex = NULL;
                            BagSlot[i][tempi]->type = INV_TYPE_EMPTY;
                        }

                        RENDER_ONCE = true;
                        bTransfInvItem = false;
                    }
                    else if( BagDropInSlots[i]->type == INV_TYPE_BAG )
                    {
                        int items_nr = 1;//1 pt ca 1 item( bag-ul mutat ) lipseste


                        for( j = 0; j < BagDropInSlots[i]->bag->slots; j++ )
                            if( BagSlot[i][j]->type != INV_TYPE_EMPTY ) items_nr ++;

                        if( items_nr > TransfInvItem.bag->slots )
                        {
                            //Nu se poate face schimbul
                        }
                        else
                        {
                            *TransfFirst = *BagDropInSlots[i];

                            //Move items

                            if( TransfInvItem.bag->slots < BagDropInSlots[i]->bag->slots )
                            {
                                for( j = TransfInvItem.bag->slots -1; j < BagDropInSlots[i]->bag->slots ; j ++ )
                                {
                                    if( BagSlot[i][j]->type != INV_TYPE_EMPTY )
                                    {
                                        found_box = false;
                                        for( k = 0; k < TransfInvItem.bag->slots -1 &&
                                                    found_box == false ;k ++ )
                                        if( BagSlot[i][k]->type == INV_TYPE_EMPTY )
                                        {
                                            *BagSlot[i][k] = *BagSlot[i][j];
                                            BagSlot[i][j]->type = INV_TYPE_EMPTY;
                                            BagSlot[i][j]->tex = NULL;

                                            found_box = true;
                                        }
                                    }
                                }
                            }

                            *BagDropInSlots[i] = TransfInvItem;

                            RENDER_ONCE = true;
                            bTransfInvItem = false;
                        }

                    }

                    found_box = true;
                }


            }
            //Shield
            if( TransfInvItem.type == INV_TYPE_SHIELD )
            if( mouse_x > PlayerSlotRect[3].x && mouse_x < PlayerSlotRect[3].x + PlayerSlotRect[3].w &&
                mouse_y > PlayerSlotRect[3].y && mouse_y < PlayerSlotRect[3].y + PlayerSlotRect[3].h )
            {
                if( PlayerShieldSlot->type == INV_TYPE_EMPTY )
                *TransfFirst = NULLii;
                else
                *TransfFirst = *PlayerShieldSlot;

                *PlayerShieldSlot = TransfInvItem;
                PlayerShieldSlot->tex = PlayerShieldSlot->shield->tex;
                bStats_Update = true;
                found_box = true;
            }
            for( i = 0 ; i < 3 &&  found_box == false  ; i ++ )
            {
                if( BagDropInSlots[i]->type == INV_TYPE_BAG )
                for( j = 0; j < BagDropInSlots[i]->bag->slots && found_box == false ; j++ )
                {
                    if( mouse_x > BagSlotRect[i][j].x && mouse_x < BagSlotRect[i][j].x + BagSlotRect[i][j].w &&
                        mouse_y > BagSlotRect[i][j].y && mouse_y < BagSlotRect[i][j].y + BagSlotRect[i][j].h )
                    {
                        if( BagSlot[i][j]->type == INV_TYPE_EMPTY )
                        {
                            *BagSlot[i][j] = TransfInvItem;

                            TransfInvItem = NULLii;

                            RENDER_ONCE = true;
                            bTransfInvItem = false;
                            found_box = true;
                        }
                        else
                        {
                            for( int itemp = 0; itemp < 3; itemp++ )
                            if( (TransfFirst == PlayerChestSlot || TransfFirst == PlayerCloakSlot ||
                                 TransfFirst == PlayerWeaponSlot)&&
                               TransfInvItem.type != BagSlot[i][j]->type )
                            {
                                break;
                            }
                            else if( itemp == 2 )
                            {
                                *TransfFirst = *BagSlot[i][j];
                                *BagSlot[i][j] = TransfInvItem;
                                TransfInvItem = NULLii;

                                RENDER_ONCE = true;
                                bTransfInvItem = false;
                                found_box = true;
                            }
                        }
                    }
                }
            }

            //Drop Itm
            if( found_box == false )
            {
                if( mouse_x > rect_drop.x && mouse_x < rect_drop.x + rect_drop.w &&
                    mouse_y > rect_drop.y && mouse_y < rect_drop.y + rect_drop.h )
                {
                    TransfInvItem = NULLii;
                    found_box = true;
                }
            }

            if( found_box == false )
            {
                *TransfFirst = TransfInvItem;
                TransfInvItem = NULLii;
                switch( TransfFirst->type )
                {
                    case INV_TYPE_ARMOR:
                    TransfFirst->tex = TransfFirst->armor->tex;
                    break;
                    case INV_TYPE_WEAPON:
                    TransfFirst->tex = TransfFirst->weapon->tex;
                    break;
                    case INV_TYPE_BAG:
                    TransfFirst->tex = TransfFirst->bag->tex;
                    break;
                    case INV_TYPE_SHIELD :
                    TransfFirst->tex = TransfFirst->shield->tex;
                    break;
                    ///default:
                    ///TransfFirst->tex = DEBUG_ExcMarkSurf;
                }
            }

            ///Search cChunk

                RENDER_ONCE    = true;
                bStats_Update  = true;
                bTransfInvItem = false;
                bTooltip_Update_special = true;
                //Hide tooltip untill mouse motion
                bTooltip_Update = true;
            }
        }
        else if( event.type == SDL_MOUSEMOTION ){
            mouse_x = mousetovirtual(event.motion.x);
            mouse_y = mousetovirtual(event.motion.y);


            bToolTip_SearchChunkPos = true;
        }
    }

    ///LOGICS

    if( bTransfInvItem ){
        if( mouse_x + plus_w < SCREEN_WIDTH/2 - Background_w/2 ||
            mouse_x + plus_w + 50 > SCREEN_WIDTH/2  + Background_w/2 ||
            mouse_y + plus_h < SCREEN_HEIGHT/2 - Background_h/2 ||
            mouse_y + plus_h + 50 > SCREEN_HEIGHT/2 + Background_h/2 )
            RENDER_Trans = false;
        else
        RENDER_Trans = true;
    }
    if( PlayerFrame_timer_04 + 150 < SDL_GetTicks() ){
        player_cframe_04++;
        if( player_cframe_04 > 4 ) player_cframe_04 = 0;

        RENDER_HERO_ONCE = true;
        PlayerFrame_timer_04 = SDL_GetTicks();
    }
    if( PlayerFrame_timer_07 + 150 < SDL_GetTicks() ){
        player_cframe_07++;
        if( player_cframe_07 > 7 ) player_cframe_07 = 0;

        RENDER_HERO_ONCE = true;
        PlayerFrame_timer_07 = SDL_GetTicks();
    }
    if( bStats_Update ){
    int iaux;

    //Reassign Dmg,DEF,ect. values

    //*Weapon
    if( PlayerWeaponSlot->type == INV_TYPE_EMPTY )
    {
        PLAYER.dmg_weapon = 0;
    }
    else
    {
        PLAYER.dmg_weapon = PlayerWeaponSlot->weapon->dmg;
    }
    //Chest
    if( PlayerChestSlot->type  == INV_TYPE_EMPTY ){
        PLAYER.def_itm_chest = 0;
        PLAYER.txEQUIPT_Chest = NULL;
    }
    else{
        PLAYER.def_itm_chest = PlayerChestSlot->armor->def;
        PLAYER.txEQUIPT_Chest = PlayerChestSlot->armor->skin_player;
    }
    //*Cloak
    if( PlayerCloakSlot->type  == INV_TYPE_EMPTY ){
        PLAYER.def_itm_cloak = 0;
        PLAYER.txEQUIPT_Cloak = NULL;
    }
    else{
        PLAYER.def_itm_cloak = PlayerCloakSlot->armor->def;
        PLAYER.txEQUIPT_Cloak = PlayerCloakSlot->armor->skin_player;
    }
    //Shield
    if( PlayerShieldSlot->type  == INV_TYPE_EMPTY ){
        PLAYER.def_itm_shield = 0;
        PLAYER.protect_itm_shield = 0;
        PLAYER.txEQUIPT_Shield = NULL;
    }
    else{
        PLAYER.def_itm_shield     = PlayerShieldSlot->shield->def;
        PLAYER.protect_itm_shield = PlayerShieldSlot->shield->protect;
        PLAYER.txEQUIPT_Shield = PlayerShieldSlot->shield->skin_player;
    }
//txStats_Protect
    //Cleanup
    //if( sStats_Title   != NULL ) SDL_FreeSurface( sStats_Title   ),sStats_Title   = NULL;
    if( sStats_Health  != NULL ) SDL_FreeSurface( sStats_Health  ),sStats_Health  = NULL;
    if( sStats_Defense != NULL ) SDL_FreeSurface( sStats_Defense ),sStats_Defense = NULL;
    if( sStats_Damage  != NULL ) SDL_FreeSurface( sStats_Damage  ),sStats_Damage  = NULL;
    //Render
    ssaux.str("");
    ssaux<<"Health: "<<PLAYER._health_c<<'/'<<PLAYER._health_max;// (+12) bonus health din items
    sStats_Health = TTF_RenderText_Blended( Font2_24,ssaux.str().c_str(),WHITE );
    txStats_Health = SDL_CreateTextureFromSurface( RENDER_MAIN,sStats_Health );
    ssaux.str("");
    ssaux<<"Defense: ";
    iaux = PLAYER.def_base;
    if( PlayerChestSlot ->type != INV_TYPE_EMPTY ) iaux += PlayerChestSlot ->armor->def;
    if( PlayerCloakSlot ->type != INV_TYPE_EMPTY ) iaux += PlayerCloakSlot ->armor->def;
    if( PlayerShieldSlot->type != INV_TYPE_EMPTY ) iaux += PlayerShieldSlot->shield->def;
    ssaux<<iaux;
    sStats_Defense = TTF_RenderText_Blended( Font2_24,ssaux.str().c_str(),WHITE );
    txStats_Defense = SDL_CreateTextureFromSurface( RENDER_MAIN,sStats_Defense );
    ssaux.str("");
    iaux = PLAYER.dmg_base+PLAYER.dmg_weapon;
    ssaux<<"Damage: "<<iaux<<" "<<iaux + PLAYER.dmg_var;
    sStats_Damage  = TTF_RenderText_Blended( Font2_24,ssaux.str().c_str(),WHITE );
    txStats_Damage = SDL_CreateTextureFromSurface( RENDER_MAIN,sStats_Damage );
    bStats_Update = false;

    if( tempsurf != NULL ) SDL_FreeSurface( tempsurf ),tempsurf = NULL;
    ssaux.str("");
    ssaux<<"Protection: "<<PLAYER.protect_itm_shield;
    tempsurf        = TTF_RenderText_Blended( Font2_24 ,ssaux.str().c_str(),WHITE );
    txStats_Protect = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
    }
    if( bTooltip_Update ){

        if( bTooltip_Update_special )
        bTooltip_Update_special = false;


    ///Render itminfo
    if( (found_box == true && CurrentInvItm != NULL) )
    {
    if( CBag != motion_CBag || col != motion_CSlot_col || row != motion_CSlot_row )
    {
        if( CurrentInvItm->type != INV_TYPE_EMPTY )
        {
            motion_CBag  = CBag;
            motion_CSlot_col = col;
            motion_CSlot_row = row;

            RENDER_ONCE = true;

            ///RENDER TOOLTIP

            RenderTooltip( CurrentInvItm, &tooltip_canvas ,
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

                          SDL_QueryTexture( tooltip_canvas,0,0,&TooltipCanvas_w,0 );
                          SDL_QueryTexture( tooltip_lines_name[0] ,0,0,&tooltip_ln_w[0] ,0 );
                          SDL_QueryTexture( tooltip_lines_name[1] ,0,0,&tooltip_ln_w[1] ,0 );
                          SDL_QueryTexture( tooltip_lines_name[2] ,0,0,&tooltip_ln_w[2] ,0 );

        }
                else
                {
                    motion_CBag = motion_CSlot_row = motion_CSlot_col = -1;
                    RENDER_ONCE = true;
                }
            }
    }
    else //found_box == false
    {
        if( motion_CBag != -1 ) RENDER_ONCE = true;
        motion_CBag = motion_CSlot_row = motion_CSlot_col = -1;
    }

    SDL_QueryTexture( tooltip_canvas,0,0,0,&tooltipcanvas_h );
    bTooltip_Update         = false;
    }
    if( bToolTip_SearchChunkPos ){

            found_box = false;


            CurrentInvItm = NULL;

            if( bTransfInvItem )
            RENDER_ONCE = true;
            else
            {
                //Check Char Inv Slots
                //Weapon
                if( mouse_x > PlayerSlotRect[0].x && mouse_x < PlayerSlotRect[0].x + PlayerSlotRect[0].w &&
                    mouse_y > PlayerSlotRect[0].y && mouse_y < PlayerSlotRect[0].y + PlayerSlotRect[0].h )
                {
                    if( PlayerWeaponSlot->type == INV_TYPE_EMPTY )
                    CurrentInvItm = NULL;
                    else
                    CurrentInvItm = PlayerWeaponSlot;

                    CBag = 21;

                    found_box = true;
                }
                //Chest Armor
                if( found_box == false )
                if( mouse_x > PlayerSlotRect[1].x && mouse_x < PlayerSlotRect[1].x + PlayerSlotRect[1].w &&
                    mouse_y > PlayerSlotRect[1].y && mouse_y < PlayerSlotRect[1].y + PlayerSlotRect[1].h )
                {
                    if( PlayerChestSlot->type == INV_TYPE_EMPTY )
                    CurrentInvItm = NULL;
                    else
                    CurrentInvItm = PlayerChestSlot;

                    CBag = 22;

                    found_box = true;
                }
                //Cloak Armor
                if( found_box == false )
                if( mouse_x > PlayerSlotRect[2].x && mouse_x < PlayerSlotRect[2].x + PlayerSlotRect[2].w &&
                    mouse_y > PlayerSlotRect[2].y && mouse_y < PlayerSlotRect[2].y + PlayerSlotRect[2].h )
                {
                    if( PlayerCloakSlot->type == INV_TYPE_EMPTY )
                    CurrentInvItm = NULL;
                    else
                    CurrentInvItm = PlayerCloakSlot;

                    CBag = 23;

                    found_box = true;
                }
                //Check bags
                if ( found_box == false )
                for( i = 0; i < 3 && found_box == false ; i ++ )
                if ( mouse_x > BagRect[i].x && mouse_x < BagRect[i].x + BagRect[i].w &&
                     mouse_y > BagRect[i].y && mouse_y < BagRect[i].y + BagRect[i].h )
                {
                    if( BagDropInSlots[i]->type == INV_TYPE_BAG  )
                    {
                        found_box = true;
                        CBag = 11 + i;
                        CurrentInvItm = BagDropInSlots[i];
                    }
                    else
                    {
                        found_box = true;
                        CBag = -1;
                        CurrentInvItm = NULL;
                    }

                }
                //Shield
                if( found_box == false )
                if( mouse_x > PlayerSlotRect[3].x && mouse_x < PlayerSlotRect[3].x + PlayerSlotRect[3].w &&
                    mouse_y > PlayerSlotRect[3].y && mouse_y < PlayerSlotRect[3].y + PlayerSlotRect[3].h )
                {
                    if( PlayerShieldSlot->type == INV_TYPE_EMPTY )
                    CurrentInvItm = NULL;
                    else
                    CurrentInvItm = PlayerShieldSlot;

                    CBag = 24;

                    found_box = true;
                }


                //Check Bags' Slots
                if( found_box == false )
                for( i = 0; i < 3 && !found_box; i ++ )
                {
                    row = 0;col = -1;
                    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
                    for( j = 0; j < BagDropInSlots[i]->bag->slots && found_box == false ; j++ )
                    {
                        col++;
                        if( col > 4 )
                        {
                            col = 0;
                            row++;
                        }
                            if( mouse_x > BagSlotRect[i][j].x && mouse_x < BagSlotRect[i][j].x + BagSlotRect[i][j].w &&
                                mouse_y > BagSlotRect[i][j].y && mouse_y < BagSlotRect[i][j].y + BagSlotRect[i][j].h )
                            {
                                CBag  = i;
                                CSlot = j;
                                CurrentInvItm = BagSlot[CBag][CSlot];
                                found_box = true;
                            }
                    }
                }
                bTooltip_Update = true;
            }

    }

    ///RENDER
    if( RENDER_ONCE ){
    static int dist = 14;
    static int tooltip_mod_h;
    tooltip_mod_h = 0;

    SDL_Surface *tempsurf = NULL;
    SDL_Texture *temptex  = NULL;

    //Check Canvas H
    if( SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*motion_CSlot_row + 50 + tooltipcanvas_h >
      ( SCREEN_HEIGHT + Background_h )/2 )
      tooltip_mod_h = -( 50 + tooltipcanvas_h );

ApplyTex( SCREEN_WIDTH/2 - Background_w/2, SCREEN_HEIGHT/2 - Background_h/2, background, NULL );

ApplyTex( rStats_MainRect.x,rStats_MainRect.y ,surf_stats_bg,NULL );
ApplyTex( rStats_MainRect.x + 3,rStats_MainRect.y + 50                          ,txStats_Health   );
ApplyTex( rStats_MainRect.x + 3,rStats_MainRect.y + 50 +  sStats_Health->h-6    ,txStats_Defense  );
ApplyTex( rStats_MainRect.x + 3,rStats_MainRect.y + 50 + (sStats_Health->h-6)*2 ,txStats_Damage   );
ApplyTex( rStats_MainRect.x + 3,rStats_MainRect.y + 50 + (sStats_Health->h-6)*3 ,txStats_Protect   );



for( i = 0; i < 3 ;i ++ )
{

    //Drop-Bag-Here Slots

    ApplyTex( BagRect[i].x,BagRect[i].y ,slot_empty ,NULL );
    ApplyTex( BagRect[i].x,BagRect[i].y ,BagDropInSlots[i]->tex ,NULL );

    //Bag SLots
    j = k = 0;
    //iii = FirstItemBag[i]->next;

    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
    while( j + k*5 < BagDropInSlots[i]->bag->slots )
    {
        //EMPTY CHUNK

        ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*j
                       + i*((50*5) + (Background_w - 50*15)/4 ) ,
                       SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*k ,
                       slot_empty, NULL  );

        if( BagSlot[i][ j + k*5 ]->type != INV_TYPE_EMPTY )
        {
            ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*j + 1
                       + i*((50*5) + (Background_w - 50*15)/4 ) ,
                       SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*k + 1 ,
                       BagSlot[i][ j + k*5 ]->tex , NULL  );

            if( BagSlot[i][ j + k*5 ]->type == INV_TYPE_MISC )
            {
                if( tempsurf != NULL ) SDL_FreeSurface( tempsurf )  ,tempsurf = NULL;
                if( temptex  != NULL ) SDL_DestroyTexture( temptex ),temptex  = NULL;

                ssaux.str("");
                ssaux<<BagSlot[i][ j + k*5 ]->misc->quant;

                tempsurf = TTF_RenderText_Blended( Font18,ssaux.str().c_str(),WHITE );
                temptex = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
                ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*j + 1
                          + i*((50*5) + (Background_w - 50*15)/4 ) + 5 ,
                          SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*k + 30 ,
                          temptex );
            }
        }

            j++;






        if( j > 4 )
        {
            j = 0;
            k++;
        }
    }

}

        ApplyTex( PlayerSlotRect[0].x, PlayerSlotRect[0].y,slot_weapon);
        ApplyTex( PlayerSlotRect[0].x, PlayerSlotRect[0].y,PlayerWeaponSlot->tex );
        ApplyTex( PlayerSlotRect[1].x, PlayerSlotRect[1].y,slot_armor);
        ApplyTex( PlayerSlotRect[1].x, PlayerSlotRect[1].y,PlayerChestSlot->tex);
        ApplyTex( PlayerSlotRect[2].x, PlayerSlotRect[2].y,slot_cloak);
        ApplyTex( PlayerSlotRect[2].x, PlayerSlotRect[2].y,PlayerCloakSlot->tex);
        ApplyTex( PlayerSlotRect[3].x, PlayerSlotRect[3].y,slot_shield);
        ApplyTex( PlayerSlotRect[3].x, PlayerSlotRect[3].y,PlayerShieldSlot->tex);

        ApplyTex( rect_drop.x, rect_drop.y,surf_drop ,NULL );


        if( bTransfInvItem && RENDER_Trans )
        ApplyTex( mouse_x + plus_w ,mouse_y + plus_h, TransfInvItem.tex , NULL );
        else if( motion_CBag != -1 && motion_CBag < 10 ){

            if( tooltip_mod_h == 0 )
            ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                      + motion_CBag*((50*5) + (Background_w - 50*15)/4 ) ,
                      SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*motion_CSlot_row ,
                      tooltip_highlight, NULL  );
            else
            ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                      + motion_CBag*((50*5) + (Background_w - 50*15)/4 ) ,
                      SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*motion_CSlot_row ,
                      tooltip_highlight_flip, NULL  );

            ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                      + motion_CBag*((50*5) + (Background_w - 50*15)/4 )
                      -( TooltipCanvas_w - 50 )/2,
                      SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*motion_CSlot_row + 50 + tooltip_mod_h  ,
                      tooltip_canvas, NULL  );
            for( i = 0; i < tooltip_name_nr; i++ )
            if( tooltip_lines_name[i] != NULL )
            ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                           + motion_CBag*((50*5) + (Background_w - 50*15)/4 )
                           -( TooltipCanvas_w - 50 )/2 + ( TooltipCanvas_w - tooltip_ln_w[i] )/2 ,
                           i*(20 - 6) +///Change val if needed
                           SCREEN_HEIGHT/2 + Background_h/2 - 300 + 50*motion_CSlot_row + 50 + tooltip_mod_h ,
                           tooltip_lines_name[ i ], NULL  );
            //STats I : Chest/cloak .. etc
            if( tooltip_stats[0] != NULL )
                ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                           + motion_CBag*((50*5) + (Background_w - 50*15)/4 )
                           -( TooltipCanvas_w - 50 )/2 + 2,
                           tooltip_name_nr*(dist -1) + //Ajustari
                           + SCREEN_HEIGHT/2 + Background_h/2 - 300//                              dist nume<->stats, stats<->stats
                           + 50*motion_CSlot_row + 50 + tooltip_mod_h ,
                           tooltip_stats[ 0 ], NULL  );
            //Stats II -> N
            for( i = 1; i < tooltip_statsnr; i++ )
            {
                if( tooltip_stats[i] != NULL )
                ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                           + motion_CBag*((50*5) + (Background_w - 50*15)/4 )
                           -( TooltipCanvas_w - 50 )/2 + 2,
                           tooltip_name_nr*(dist -2) + i*(dist -4 )///Ajustari
                           + SCREEN_HEIGHT/2 + Background_h/2 - 300 + 5 // dist nume<->stats, stats<->stats
                           + 50*motion_CSlot_row + 50 + tooltip_mod_h ,
                           tooltip_stats[ i ], NULL  );
            }

            for( i = 0; i < tooltip_desc_nr; i++ )
            {
                if( tooltip_lines_desc[i] != NULL )
                ApplyTex( SCREEN_WIDTH/2  - Background_w/2 + 45  + 50*motion_CSlot_col
                           + motion_CBag*((50*5) + (Background_w - 50*15)/4 )
                           -( TooltipCanvas_w - 50 )/2 + 2,
                           tooltip_name_nr*(dist -2) + tooltip_statsnr*(dist -4 ) + i*( dist -2)
                           + SCREEN_HEIGHT/2 + Background_h/2 - 300 + 5
                           + 50*motion_CSlot_row + 50 + tooltip_mod_h ,
                           tooltip_lines_desc[ i ], NULL  );
            }


        }
        else if( motion_CBag > 10  && motion_CBag < 20 ){
        SDL_Rect *crect;

        crect = &BagRect[motion_CBag - 11 ];

            ApplyTex( crect->x,crect->y ,tooltip_highlight, NULL  );

            ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2,
                           crect->y + 52  ,
                           tooltip_canvas, NULL  );
            for( i = 0; i < tooltip_name_nr; i++ )
            if( tooltip_lines_name[i] != NULL )
            ApplyTex( crect->x -( TooltipCanvas_w - 52 )/2 + ( TooltipCanvas_w - tooltip_ln_w[i] )/2 ,
                           i*(dist - 6) +
                           crect->y + 52  ,
                           tooltip_lines_name[ i ], NULL  );
            //STats I : Chest/cloak .. etc
            if( tooltip_stats[0] != NULL )
                ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2 + 2,
                           tooltip_name_nr*(dist -2) +
                           crect->y + 52  ,
                           tooltip_stats[ 0 ], NULL  );
            //Stats II -> N
            for( i = 1; i < tooltip_statsnr; i++ )
            {
                if( tooltip_stats[i] != NULL )
                ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2 + 2,
                           tooltip_name_nr*(dist -2) + i*(dist -4 ) +
                           crect->y + 52 + 5 ,
                           tooltip_stats[ i ], NULL );
            }

            for( i = 0; i < tooltip_desc_nr; i++ )
            {
                if( tooltip_lines_desc[i] != NULL )
                ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2 + 2,
                               tooltip_name_nr*(dist -2) + tooltip_statsnr*(dist -4 ) + i*( dist -2)+
                               crect->y + 52 + 5 ,
                               tooltip_lines_desc[ i ], NULL  );
            }

        }
        else if( motion_CBag > 20  && motion_CBag < 30 ){
            SDL_Rect *crect;
            crect = &PlayerSlotRect[motion_CBag - 21 ];

            ApplyTex( crect->x,crect->y ,tooltip_highlight, NULL  );
            ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2,
                           crect->y + 52  ,
                           tooltip_canvas, NULL  );
            for( i = 0; i < tooltip_name_nr; i++ )
            if( tooltip_lines_name[i] != NULL )
            ApplyTex( crect->x -( TooltipCanvas_w - 52 )/2 + ( TooltipCanvas_w - tooltip_ln_w[i] )/2 ,
                           i*(dist - 6) +
                           crect->y + 52  ,
                           tooltip_lines_name[ i ], NULL  );
            //STats I : Chest/cloak .. etc
            if( tooltip_stats[0] != NULL )
                ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2 + 2,
                           tooltip_name_nr*(dist -2) +
                           crect->y + 52  ,
                           tooltip_stats[ 0 ], NULL  );
            //Stats II -> N
            for( i = 1; i < tooltip_statsnr; i++ )
            {
                if( tooltip_stats[i] != NULL )
                ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2 + 2,
                           tooltip_name_nr*(dist -2) + i*(dist -4 ) +
                           crect->y + 52 + 5 ,
                           tooltip_stats[ i ], NULL );
            }

            for( i = 0; i < tooltip_desc_nr; i++ )
            {
                if( tooltip_lines_desc[i] != NULL )
                ApplyTex( crect->x - ( TooltipCanvas_w - 52 )/2 + 2,
                               tooltip_name_nr*(dist-2) + tooltip_statsnr*(dist -4 ) + i*(dist-2)+
                               crect->y + 52 + 5 ,
                               tooltip_lines_desc[ i ], NULL  );
            }
        }

    }
    if( RENDER_HERO_ONCE || RENDER_ONCE ){
        //Player Model

        //Background
        ApplyTex( player_rect.x, player_rect.y, player_bg,NULL );
        //Shield
        ///ApplyTex( player_rect.x + ( 120 - 100 )/2 ,player_rect.y + ( 120 - 100 )/2 + 6 ,sEQUIPT_Shield ,&rItm_Shield[2][0] );
        //Cloak
        ///ApplyTex( player_rect.x + ( player_bg->w - 100 )/2 ,player_rect.y + ( player_bg->h - 100 )/2 + 6 ,sEQUIPT_Cloak , &rItm_cloak[ 3 ][ player_cframe_04 ] );
        //Hero
        ///ApplyTex( player_rect.x + ( player_bg->w - 100 )/2 ,player_rect.y + ( player_bg->h - 100 )/2 ,HERO , &hero_anim[ 6 ][ player_cframe_07 ] );
        //Chest
        ///ApplyTex( player_rect.x + ( player_bg->w - 100 )/2 , player_rect.y + ( player_bg->h - 100 )/2 + 6 ,sEQUIPT_Chest , &rArmor[ 2 ] );
        //Arms
        ///ApplyTex( player_rect.x + ( player_bg->w - 100 )/2 ,player_rect.y + ( player_bg->h - 100 )/2 + 6 ,sHeroArms , &hero_arms[ 6 ][ 0 ] );
        RENDER_HERO_ONCE = false;

    }



    ENGINE_RenderPresent();

    RENDER_ONCE = false;
}

}
int  inventory::GetFreeSpaces(){
int i,j,nr = 0;
for( i = 0; i < 3; i ++ )
    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
    for( j = 0; j < BagDropInSlots[i]->bag->slots ;j++ )
    if( BagSlot[i][j]->type == INV_TYPE_EMPTY ) nr++;

    //DEBUG_outf<<"free spaces: "<<nr<<'\n';

return nr;
}
bool inventory::HaveItem(int type, int subtype, int nr,int *quant ){
int i,j;

if( quant == NULL )//Normal Search begining->end
{
    for( i = 0; i < 3; i ++ )
    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
    for( j = 0; j < BagDropInSlots[i]->bag->slots ;j++ )
    if( BagSlot[i][j]->type == type && BagSlot[i][j]->nr == nr )
    {
        if( (type == INV_TYPE_ARMOR && BagSlot[i][j]->armor->type == subtype) ||
             type != INV_TYPE_ARMOR )
         {
             if( type == INV_TYPE_MISC && quant != NULL )
             *quant = BagSlot[i][j]->misc->quant;
             return true;
         }

    }
}
else
{
    for( i = 2; i >= 0; i -- )
    if( BagDropInSlots[i]->type == INV_TYPE_BAG )
    for( j = BagDropInSlots[i]->bag->slots -1; j >= 0 ;j-- )
    if( BagSlot[i][j]->type == type && BagSlot[i][j]->nr == nr )
    {
        if( (type == INV_TYPE_ARMOR && BagSlot[i][j]->armor->type == subtype) ||
             type != INV_TYPE_ARMOR )
         {
             if( type == INV_TYPE_MISC && quant != NULL )
             *quant = BagSlot[i][j]->misc->quant;
             return true;
         }

    }
}


//armor weapon, cloak..etc
if( type == INV_TYPE_ARMOR )
{
    if( subtype == ARMOR_TYPE_CHEST && BagSlot[i][j]->armor->type == ARMOR_TYPE_CHEST &&
        PlayerChestSlot->nr == nr )return true;
    else if( subtype == ARMOR_TYPE_CLOAK && BagSlot[i][j]->armor->type == ARMOR_TYPE_CLOAK &&
             PlayerCloakSlot->nr == nr )return true;
}
else if( type == INV_TYPE_WEAPON && PlayerWeaponSlot->nr == nr )return true;
for( i = 0; i < 3; i ++ )
if( type == INV_TYPE_BAG && BagDropInSlots[i]->nr == nr )return true;

return false;
}
bool inventory::Is_ShieldEquip(void){
if( PlayerShieldSlot->type == INV_TYPE_SHIELD )
return true;
return false;
}
void inventory::cleanup(){
    /*
    SDL_FreeSurface( box_surface[0]      );
    SDL_FreeSurface( box_surface[1]      );
    SDL_FreeSurface( box_surface[2]      );
    SDL_FreeSurface( box_surface[3]      );
    SDL_FreeSurface( box_surface[4]      );
    SDL_FreeSurface( box_surface_hold[0] );
    SDL_FreeSurface( box_surface_hold[1] );
    SDL_FreeSurface( box_surface_hold[2] );
    SDL_FreeSurface( box_surface_hold[3] );
    SDL_FreeSurface( box_surface_hold[4] );
    SDL_FreeSurface( background );
    */
}
