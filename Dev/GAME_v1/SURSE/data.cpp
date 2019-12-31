#include "data.h"


void DataBase::init(){
int i,j;
for( i = 0; i < 100;i++ )
{
    for( j = 0; j < 8; j ++ )
    ChunkSkins[i][j] = NULL;
}

for( i = 0; i < 10; i ++ )
{
    NPCSkin[i] = NULL;
    BagTex[ i ] = NULL;
    MiscTex[ i ] = NULL;
    LoadedArmor_chest[i] = new Armor;
    LoadedArmor_chest[i]->nr = NONE;
    LoadedArmor_cloak[i] = new Armor;
    LoadedArmor_cloak[i]->nr = NONE;
    LoadedShield[i] = new Shield;
    LoadedShield[i]->nr = NONE;
    LoadedWeapon[i] = new Weapon;
    LoadedWeapon[i]->nr = NONE;
    LoadedBag[i] = new BAG;
    LoadedBag[i]->nr = NONE;
    LoadedMisc[i] = new Misc;
    LoadedMisc[i]->nr = NONE;
}

CloakModel = LoadSurf( "textures//items//cloak//equipt//model.png" );
}
Armor  DataBase::GetArmorData(int nr,int type ){
Armor temp;
int nr_bonus,i ;
char auxchar[100];
string reads;
stringstream itm_src;
switch( type )
{
    case ARMOR_TYPE_CHEST:
    if( LoadedArmor_chest[nr]->nr != NONE )
    {
        CONS.out( "Already Loaded chest!" );
        return *LoadedArmor_chest[nr];
    }

    else
    {
        itm_src <<"items//armors//chest//" << nr << ".itm";
    }
    break;
    case ARMOR_TYPE_CLOAK:
    if( LoadedArmor_cloak[nr]->nr != NONE )
    {
        CONS.out( "Already Loaded chest!" );
        return *LoadedArmor_cloak[nr];
    }

    else
    {
        itm_src <<"items//armors//cloak//" << nr << ".itm";
    }
    break;
}



stringstream ssaux;
ssaux<<"Loading ";
if( type == ARMOR_TYPE_CHEST ) ssaux<<"chest";
if( type == ARMOR_TYPE_CLOAK ) ssaux<<"cloak";
ssaux<<" nr "<<nr;
CONS.out( ssaux.str().c_str() );




ifstream file( itm_src.str().c_str() );
temp.nr = nr;
temp.type = type;
    file >> reads;
    file.get(temp.name,'~' );

    //Elimina spatiile de la inceputul numelui
    i = 0;
    while( i < 5 && ( temp.name[i] == ' ' || temp.name[i] == '\0' )  )
    i++;
    for( int j = i; (unsigned)j < SDL_strlen( temp.name ) ;j++ )
    temp.name[ j - i ] = temp.name[ j ];
    temp.name[ SDL_strlen( temp.name ) -i ] = '\0';

    file >> reads >> temp.def;

    //Skin_inventory
    file >> reads >> nr_bonus;
    if( ArmorTex[type - 1][nr_bonus] == NULL)
    {
        itm_src.str("");
        switch( type )
        {
            case ARMOR_TYPE_CHEST:
            itm_src <<"textures//items//chest//inventory//" << nr_bonus << ".png";
            break;
            case ARMOR_TYPE_CLOAK:
            itm_src <<"textures//items//cloak//inventory//" << nr_bonus << ".png";
            break;
        }

        ArmorTex[type - 1][nr_bonus] = LoadTex( itm_src.str().c_str() );

        //If FAIL
        if( ArmorTex[type - 1][nr_bonus] == NULL )
        {
            ArmorTex[type - 1][nr_bonus] = DEBUG.txExMark;
        }

    }
    temp.tex = ArmorTex[type - 1][nr_bonus];

    //Skin_equipt
    file >> reads >> nr_bonus;
    if( ArmorSkin_equipt[ type -1 ][nr_bonus] == NULL)
    {
        itm_src.str("");
        switch( type )
        {
            case ARMOR_TYPE_CHEST:
            itm_src <<"textures//items//chest//equipt//" << nr_bonus << ".png";
            ArmorSkin_equipt[ type -1 ][nr_bonus] = LoadTex( itm_src.str().c_str() );
            break;
            case ARMOR_TYPE_CLOAK:
            itm_src <<"textures//items//cloak//equipt//" << nr_bonus << ".png";
            //Load temp img
            ArmorSkin_equipt[ type -1 ][nr_bonus] = LoadTex( itm_src.str().c_str() );
            //Render new one
            ///ArmorSkin_equipt[ type -1 ][nr_bonus]= Pixels.CreatePixAnime( ArmorSkin_equipt[ type -1 ][nr_bonus]
            ///                                       ,CloakModel );
            break;
        }


    }
    temp.skin_player = ArmorSkin_equipt[ type -1 ][nr_bonus];

    //Description
    file>>reads;
    if( reads != "no_desc" )
    {
        file.get( auxchar ,100 );
        BAS_ctos( auxchar,temp.description );
    }

    itm_src.str("");

    file >> reads >> nr_bonus;
    for( int i = 0; i < nr_bonus; i++ )
    {
        file >> reads >> temp.stats[i].type >> reads >> temp.stats[i].value;
    }
    temp.statsnr = nr_bonus;

file.close();

if( type == ARMOR_TYPE_CHEST ) *LoadedArmor_chest[nr] = temp;
else
if( type == ARMOR_TYPE_CLOAK ) *LoadedArmor_cloak[nr] = temp;
return temp;
}
Shield DataBase::GetShieldData(int nr){
Shield temp;
int nr_bonus,i ;
char auxchar[100];
string reads;
stringstream itm_src;



if( LoadedShield[nr]->nr != NONE )
{
    CONS.out( "Already loaded shield!" );
    return *LoadedShield[nr];
}
CONS.out( "Loading Shield!" );


itm_src <<"items//shields//" << nr << ".itm";

ifstream file( itm_src.str().c_str() );
temp.nr = nr;
    file >> reads;
    file.get(temp.name,'~' );

    //Elimina spatiile de la inceputul numelui
    i = 0;
    while( i < 5 && ( temp.name[i] == ' ' || temp.name[i] == '\0' )  )
    i++;
    for( int j = i; (unsigned)j < SDL_strlen( temp.name ) ;j++ )
    temp.name[ j - i ] = temp.name[ j ];
    temp.name[ SDL_strlen( temp.name ) -i ] = '\0';

    file >> reads >> temp.def;
    file >> reads >> temp.protect;

    //Skin_inventory
    file >> reads >> nr_bonus;
    if( ShieldTex[nr_bonus] == NULL )
    {
        itm_src.str("");

        itm_src <<"textures//items//shield//inventory//" << nr_bonus << ".png";

        ShieldTex[nr_bonus] = LoadTex( itm_src.str().c_str() );

        //If FAIL
        if( ShieldTex[nr_bonus] == NULL )
        {
            ShieldTex[nr_bonus] = DEBUG.txExMark;
        }
    }
    temp.tex = ShieldTex[nr_bonus];

    //Skin_equipt
    file >> reads >> nr_bonus;
    if( ShieldSkin_equipt[nr_bonus] == NULL)
    {
        itm_src.str("");

        itm_src <<"textures//items//shield//equipt//" << nr_bonus << ".png";

        ShieldSkin_equipt[nr_bonus] = LoadTex( itm_src.str().c_str() );
    }
    temp.skin_player = ShieldSkin_equipt[nr_bonus];

    //Description
    file>>reads;
    if( reads != "no_desc" )
    {
        file.get( auxchar ,100 );
        BAS_ctos( auxchar,temp.description );
    }

    itm_src.str("");

    file >> reads >> nr_bonus;
    for( int i = 0; i < nr_bonus; i++ )
    {
        file >> reads >> temp.stats[i].type >> reads >> temp.stats[i].value;
    }
    temp.statsnr = nr_bonus;

file.close();

*LoadedShield[nr] = temp;

return temp;
}
Weapon DataBase::GetWeaponData(int nr){
ifstream file;
string sread;
int iread,i;
char auxchar[100];
Weapon temp;
stringstream itm_src;

temp.nr = nr;

if( LoadedWeapon[nr]->nr != NONE )
{
    CONS.out( "Already loaded weapon!" );
    return *LoadedWeapon[nr];
}
    CONS.out( "Loading Weapon" );

itm_src <<"items//weapons//"<<nr<<".itm";


file.open( itm_src.str().c_str());
file >> sread;
file.get(temp.name,'~');

//Elimina spatiile de la inceputul numelui
    i = 0;
    while( i < 5 && ( temp.name[i] == ' ' || temp.name[i] == '\0' )  )
    i++;
    for( int j = i; (unsigned)j < SDL_strlen( temp.name );j++ )
    temp.name[ j - i ] = temp.name[ j ];
    temp.name[ SDL_strlen( temp.name ) -i ] = '\0';

file >> sread >> temp.dmg;
file >> sread >> temp.sharp;
file >> sread >> iread;

//Inventory skin
if( WeaponTex[iread] == NULL )
{
    itm_src.str("");
    itm_src << "textures//items//weapon//inventory//" << iread << ".PNG";
    WeaponTex[iread] = LoadTex( itm_src.str().c_str() );

    //If FAIL
    if( WeaponTex[iread] == NULL )
    {
        WeaponTex[iread] = DEBUG.txExMark;
    }
}
temp.tex = WeaponTex[iread];

//Description
file>>sread;
if( sread != "no_desc" )
{
    file.get( auxchar ,100 );
    BAS_ctos( auxchar,temp.description );
}

file >> sread >> iread;

file.close();

*LoadedWeapon[nr] = temp;

return temp;
}
BAG    DataBase::GetBagData( int nr ){
ifstream file;
string sread;
int iread,i;
char auxchar[100];
BAG temp;
stringstream itm_src;

temp.nr = nr;

if( LoadedBag[nr]->nr != NONE )
{
    CONS.out( "Already loaded bag!" );
    return *LoadedBag[nr];
}


CONS.out( "Loading new bag" );
itm_src <<"items//bags//"<<nr<<".itm";


file.open( itm_src.str().c_str());
file >> sread;
file.get(temp.name,'~');

//Elimina spatiile de la inceputul numelui
    i = 0;
    while( i < 5 && ( temp.name[i] == ' ' || temp.name[i] == '\0' )  )
    i++;
    for( int j = i; (unsigned)j < SDL_strlen( temp.name );j++ )
    temp.name[ j - i ] = temp.name[ j ];
    temp.name[ SDL_strlen( temp.name ) -i ] = '\0';

file >> sread >> temp.slots;
file >> sread >> iread;

//Inventory skin
if( BagTex[iread] == NULL )
{
    itm_src.str("");
    itm_src << "textures//items//bags//" << iread << ".PNG";
    BagTex[iread] = LoadTex( itm_src.str().c_str() );

    //If FAIL
    if( BagTex[iread] == NULL )
    {
        BagTex[iread] = DEBUG.txExMark;
    }
}
temp.tex = BagTex[iread];

//Description
file>>sread;
if( sread != "no_desc" )
{
    file.get( auxchar ,'~' );
    BAS_ctos( auxchar,temp.description );
}

file.close();

*LoadedBag[nr] = temp;

return temp;
}
Misc   DataBase::GetMiscData( int nr ){
ifstream file;
string sread;
int iread,i;
char auxchar[100];
Misc temp;
stringstream itm_src;

temp.nr = nr;

if( LoadedMisc[nr]->nr != NONE )
{
    CONS.out( "Already loaded Misc!" );
    return *LoadedMisc[nr];
}


CONS.out( "Loading new misc" );
itm_src <<"items//misc//"<<nr<<".itm";


file.open( itm_src.str().c_str());
file >> sread;
file.get(temp.name,'~');

//Elimina spatiile de la inceputul numelui
    i = 0;
    while( i < 5 && ( temp.name[i] == ' ' || temp.name[i] == '\0' )  )
    i++;
    for( int j = i; (unsigned)j < SDL_strlen( temp.name );j++ )
    temp.name[ j - i ] = temp.name[ j ];
    temp.name[ SDL_strlen( temp.name ) -i ] = '\0';

file >> sread >> iread;
//Inventory skin
if( MiscTex[iread] == NULL )
{
    itm_src.str("");
    itm_src << "textures//items//misc//" << iread << ".png";
    MiscTex[iread] = LoadTex( itm_src.str().c_str() );

    //If FAIL
    if( MiscTex[iread] == NULL )
    {
        MiscTex[iread] = DEBUG.txExMark;
    }
}
temp.tex = MiscTex[iread];

file >> sread >> temp.maxstack;
file >> sread >> temp.price;

//Description
file>>sread;
if( sread != "no_desc" )
{
    file.get( auxchar ,'~' );
    BAS_ctos( auxchar,temp.description );
}

file >> sread >> temp.usable;

if( temp.usable )
{
    file>>sread>>temp.effect_type;
    file>>sread>>temp.effect_val;
}

file.close();

temp.quant = 1;

*LoadedMisc[nr] = temp;

return temp;
}
SDL_Surface *DataBase::GetNPCSkin(int nr){
if(NPCSkin[nr] == NULL)
{
    stringstream filename;
    filename << "textures//NPC//"<<nr<<".png";
    NPCSkin[nr] = LoadSurf( filename.str().c_str() );
}
return NPCSkin[nr];
}
newQuest     DataBase::GetQuest(int nr){
newQuest temp;
ifstream file;
int i;
stringstream filename;
string sread;
char caux[100];
filename<<"Data//quests//"<<nr<<".que";
file.open( filename.str().c_str() );
temp.nr = 0;//Initial 0

temp.desc_lines = 0;
temp.rew_itm_nrof = 0;

for( i = 0; i < 10; i ++ )
temp.RewItems[ i ] = NULL;

//IF File OK
if( file.is_open() ){
while( file.peek() != EOF ){
file >> sread;

if( sread == "Name:"){
    file.getline( caux,90 );
    BAS_ctos( caux,temp.name );
}
else if( sread == "descline:" ){
file.getline(caux,60);
BAS_ctos( caux,temp.desc[temp.desc_lines++] );
}
else if( sread == "rewarditmnr:" )
file >> temp.rew_itm_nrof;
else if( sread == "rewarditmtake:" )
file >> temp.rew_itm_take;
else if( sread == "goalprogress:")
file >> temp.goalprog;
else if( sread == "rewardgold:")
file >> temp.rew_gold;
else if( sread == "rewardexp:")
file >> temp.rew_exp;
else if( sread == "rewarditm:" )
{
int itm_nr,quant,type,subtype;;
type = subtype = itm_nr = quant = -1;


temp.RewItems[temp.rew_itm_nrof] = new Inventory_item;

file>>sread>>type;

if( type == INV_TYPE_ARMOR )
file>>sread>>subtype;
else if( type == INV_TYPE_MISC )
file>>sread>>quant;

file>>sread>>itm_nr;

switch( type )
{
    case INV_TYPE_ARMOR:
    temp.RewItems[temp.rew_itm_nrof]->armor = new Armor;
    *temp.RewItems[temp.rew_itm_nrof]->armor = DATABASE.GetArmorData( itm_nr,subtype );
    temp.RewItems[temp.rew_itm_nrof]->tex = temp.RewItems[temp.rew_itm_nrof]->armor->tex;
    break;
    case INV_TYPE_WEAPON:
    temp.RewItems[temp.rew_itm_nrof]->weapon = new Weapon;
    *temp.RewItems[temp.rew_itm_nrof]->weapon = DATABASE.GetWeaponData( itm_nr );
    temp.RewItems[temp.rew_itm_nrof]->tex = temp.RewItems[temp.rew_itm_nrof]->weapon->tex;
    break;
    case INV_TYPE_SHIELD:
    temp.RewItems[temp.rew_itm_nrof]->shield = new Shield;
    *temp.RewItems[temp.rew_itm_nrof]->shield = DATABASE.GetShieldData( itm_nr );
    temp.RewItems[temp.rew_itm_nrof]->tex = temp.RewItems[temp.rew_itm_nrof]->shield->tex;
    break;
    case INV_TYPE_BAG:
    temp.RewItems[temp.rew_itm_nrof]->bag = new BAG;
    *temp.RewItems[temp.rew_itm_nrof]->bag = DATABASE.GetBagData( itm_nr );
    temp.RewItems[temp.rew_itm_nrof]->tex = temp.RewItems[temp.rew_itm_nrof]->bag->tex;
    break;
    case INV_TYPE_MISC:
    temp.RewItems[temp.rew_itm_nrof]->misc = new Misc;
    *temp.RewItems[temp.rew_itm_nrof]->misc = DATABASE.GetMiscData( itm_nr );
    temp.RewItems[temp.rew_itm_nrof]->misc->quant = quant;
    temp.RewItems[temp.rew_itm_nrof]->tex = temp.RewItems[temp.rew_itm_nrof]->misc->tex;
    break;
}

temp.RewItems[temp.rew_itm_nrof]->type = type;
temp.RewItems[temp.rew_itm_nrof]->nr   = itm_nr;
temp.rew_itm_nrof++;

}
}
temp.nr = nr;
}

file.close();
temp.cprog = 0;
return temp;
}
SDL_Surface *DataBase::GetChunkSkin(int nr,int orient,bool animated,int frames ){
static stringstream sstr;
sstr.str( "" );

if( ChunkSkins[nr][0] == NULL )
{
    if( nr < 200 )
    {
        sstr<<"textures//BUILDING//build//"<<nr<<".png";
        ChunkSkins[nr][0] = LoadSurf( sstr.str().c_str() );

        if( ChunkSkins[nr][0] != NULL )
        {
            ChunkSkins[nr][4] = Pixels.RenderCorner( ChunkSkins[nr][0] );
        }
        else
            ChunkSkins[nr][4] = Pixels.RenderCorner( DEBUG.sfExMark );
    }
    else
    {
        sstr<<"textures//BUILDING//decoration//"<<nr<<".png";
        ChunkSkins[nr][0] = LoadSurf( sstr.str().c_str() );
    }

}
//If The Loading Failed (BUGG)
if( ChunkSkins[nr][0] == NULL )
{
    //Imagine sfExcMark nu arata cum trebuie!
    //ChunkSkins[nr][0] = DEBUG.getNewsfExc();
}
else
//ROTATE or FLIP
switch( orient )
{
    case 1:
    if( ChunkSkins[nr][1] == NULL )
    ChunkSkins[nr][1] = Pixels.rotate_img( ChunkSkins[nr][0],RIGHT,animated,frames );
    break;
    case 2:
    if( ChunkSkins[nr][2] == NULL )
    ChunkSkins[nr][2] = Pixels.flip_img( ChunkSkins[nr][0],HOR );
    break;
    case 3:
    if( ChunkSkins[nr][3] == NULL )
    ChunkSkins[nr][3] = Pixels.rotate_img( ChunkSkins[nr][0],LEFT,animated,frames );
    break;
    case 5:
    if( ChunkSkins[nr][5] == NULL )
    ChunkSkins[nr][5] = Pixels.rotate_img( ChunkSkins[nr][4],RIGHT,animated,frames );
    break;
    case 6:
    if( ChunkSkins[nr][6] == NULL )
    {
        if( ChunkSkins[nr][5] == NULL )//Necesita imaginea anterioara
        ChunkSkins[nr][5] = Pixels.rotate_img( ChunkSkins[nr][4],RIGHT,animated,frames );
        ChunkSkins[nr][6] = Pixels.rotate_img( ChunkSkins[nr][5],RIGHT,animated,frames );
    }
    break;
    case 7:
    if( ChunkSkins[nr][7] == NULL )
    ChunkSkins[nr][7] = Pixels.rotate_img( ChunkSkins[nr][4],LEFT,animated,frames );
    break;
}

if( orient >= 0 && orient <= 7 )
return ChunkSkins[nr][orient];


//Daca nu exisat un object cu acest nr
return NULL;
}
DataBase::~DataBase(){
int i;

for( i = 0 ; i < 1 ; i++ ){
    SDL_DestroyTexture( ArmorTex[0][i] );
    SDL_DestroyTexture( ArmorTex[1][i] );
}
for( i = 0 ; i < 1 ; i++ )
SDL_DestroyTexture( WeaponTex[i] );
for( i = 0 ; i < 1 ; i++ )
SDL_FreeSurface( NPCSkin[i] );

for( i = 0; i < 10; i ++ )
{
    delete LoadedArmor_chest[i];
    delete LoadedArmor_cloak[i];
}

}
