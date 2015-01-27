#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include "structs.h"
#include "basics.h"
#include "console_extern.h"
#include "pixel_manip.h"

extern class DataBase{
private:
Armor ARMOR;
Weapon WEAPON;
SDL_Texture *ArmorSkin_equipt[2][20],*ShieldSkin_equipt[20];
SDL_Surface *ChunkSkins[300][8];
SDL_Surface *NPCSkin[10];
SDL_Surface *CloakModel;

SDL_Texture *ArmorTex[2][20],*WeaponTex[20],*ShieldTex[20],*BagTex[20],*MiscTex[20];

Armor *LoadedArmor_chest[10],*LoadedArmor_cloak[10];
Shield *LoadedShield[10];
Weapon *LoadedWeapon[10];
BAG *LoadedBag[10];
Misc *LoadedMisc[10];

public:
void init();
~DataBase();
Armor GetArmorData(int nr,int type );
Shield GetShieldData(int nr);
Weapon GetWeaponData(int nr);
BAG GetBagData(int nr);
Misc GetMiscData( int nr );
SDL_Surface *GetNPCSkin(int nr);
newQuest GetQuest(int nr);
SDL_Surface *GetChunkSkin(int nr,int orient, bool animated,int frames );

}DATABASE;

#endif // DATA_H_INCLUDED
