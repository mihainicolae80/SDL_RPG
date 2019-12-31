#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

#include "main_headers.h"
#include "basics.h"
#include "data.h"
#include "gevents.h"

extern class inventory{
    private:
    ///New Inventory

    SDL_Surface *sStats_Health,*sStats_Defense,*sStats_Damage;

    SDL_Texture *background,*slot_empty,*slot_armor,*slot_weapon,*slot_cloak,*slot_shield,
                *surf_drop ,*surf_stats_bg,*player_bg,*tooltip_tex,*tooltip_stats[6],
                *tooltip_lines_desc[6],*tooltip_highlight,*tooltip_highlight_flip,*tooltip_canvas,
                *tooltip_lines_name[3],*txStats_Health,*txStats_Defense,*txStats_Damage,*txStats_Protect;

    SDL_Rect BagSlotRect[3][25],BagRect[3],PlayerSlotRect[4],player_rect,rect_drop,rStats_MainRect,
             rStats_StatBox[3] ;
    Inventory_item *BagSlotLink[3][25],TransfInvItem,*TransfFirst,*BagDropInSlots[3],NULLii,*BagSlot[3][25],
                   *PlayerChestSlot,*PlayerWeaponSlot,*PlayerCloakSlot,*PlayerShieldSlot;
    bool RENDER_ONCE,RENDER_HERO_ONCE,bTransfInvItem,RENDER_Trans,bStats_Update,
         bTooltip_Update,bTooltip_Update_special,bToolTip_SearchChunkPos;
    int plus_w,plus_h,player_cframe_07,player_cframe_04,motion_CBag,motion_CSlot_row,
        motion_CSlot_col,motion_CSlot,tooltip_statsnr,tooltip_name_nr,tooltip_desc_nr,
        Background_w,Background_h,SurfStats_w,SurfStats_h,surf_drop_w,surf_drop_h,
        TooltipCanvas_w,tooltip_ln_w[3],tooltipcanvas_h ;
    Uint32 PlayerFrame_timer_07,PlayerFrame_timer_04 ;

    void add_slot( int bag );

    public:
    bool init          (void);
    void additem_to_INV(int type,int subtype ,int nr,int quant);
    void init_bags     (void);
    void open          (void);
    int  GetFreeSpaces (void);
    bool HaveItem      (int type, int subtype, int nr,int *quant = NULL );
    bool Is_ShieldEquip(void);
    void cleanup       (void);
    void addTestItems  (void);

}INVENTORY;

#endif // INVENTORY_H_INCLUDED
