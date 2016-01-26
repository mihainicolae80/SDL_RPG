#include "map.h"


void Map::init(){

LastNpc = 0;

RendNormalMap = NULL;

FirstLight = new LIGHT;
LastLight  = new LIGHT;
FirstLight ->next = LastLight;
LastLight ->next = NULL;

tstart = SDL_GetTicks();
tpause = SDL_GetTicks() - 1000;
tAnime = SDL_GetTicks();
tSuccSpeed = 3000;
DaySuccession = true;

LastChunk_back  = FirstFreeChunk_back;
LastChunk_dynam = FirstFreeChunk_dynam;
LastChunk_fore  = FirstFreeChunk_fore;

DisplayLocation_x = DisplayLocation_y = 0;

cDayTime = 6;
for( i = 0 ; i < 4;i ++ )
{
    box_ROTATE[i].w = box_ROTATE[i].h = 50;
}
for( i = 0 ; i < 6;i ++ )
{
    DayNightPhase[i] = NULL;
}
box_ROTATE[0].x = box_ROTATE[2].x = 0;
box_ROTATE[1].x = box_ROTATE[3].x = 50;
box_ROTATE[0].y = box_ROTATE[1].y = 0;
box_ROTATE[2].y = box_ROTATE[3].y = 50;

DayNightPhase[3] = NULL;
DayNightPalette = LoadSurf( "textures//environmental//DayNight_palette.png" );

}
void Map::PreLoadChanges(){
/*
if( PLAYER.czone == 1 )
{
    cEnemy = 1;
    Enemy[0].maxhealth = 300;
    Enemy[0].dmg = 20;
    Enemy[0].skin = LoadSurf( "textures//mobs//1.png" );
    Enemy[0].x = 200;
    Enemy[0].y = 200;
    Enemy[0].behavior[0] = "test_mob_behavior";
    Enemy[0].behavior[1] = "aggresive";

    Enemy[1].maxhealth = 300;
    Enemy[1].dmg = 20;
    Enemy[1].skin = LoadSurf( "textures//mobs//1.png" );
    Enemy[1].x = 100;
    Enemy[1].y = 300;
    Enemy[1].behavior[0] = "test_mob_behavior";
    Enemy[1].behavior[1] = "aggresive";
}
*/
}
void Map::PostLoadChanges(){
switch( PLAYER.GetZone() )
{
    //Map 0: Old House
    case 0:
    GAME_NPCS.manipulate( UNSPAWN,FIRST,-1,"Enemy NPC" );
    break;
    //Sewers
    case 2:
    if( QUEST.is_active( 2 )  )
    {
        manipulate_colider( 1,0 );
        manipulate_inter( 1,0,-1,-1 );
    }
    break;
    case 9:
    GAME_NPCS.manipulate( UNSPAWN,FIRST,-1,"Big Boss" );
    FirstEnemy->next->npc->boss = true;
    break;

}
}
void Map::load(int MapNumber){
SDL_Surface *tempsurf = NULL;
stringstream filename;

filename<<"maps//map"<<MapNumber<<".mp";
int i,j,k,info[9],iaux,cinstruct,ctopic,cactions;
char caux[90];
string saux;
stringstream ssaux;
DIALOG_TOPIC *dtaux;
FreeChunk *TempFreeChunk = new FreeChunk;
LIGHT *TempLight = new LIGHT;
COLLIDER *TempCol = new COLLIDER;
NotPLayerCreature *modelchunk = new NotPLayerCreature;
string read_str;
ifstream file(filename.str().c_str()),in_aux;

if( file.is_open() == false )
CONS.out( "Failed to open map" );

//Test only
//ofstream out_temp("LoadMap_NPCFILE.txt");


GAME_NPCS.cleanup_allnpc();
Map::CleanCMap();
Map::PreLoadChanges();


file >> read_str;
name = read_str;
file >> NrBlockX >> NrBlockY;

//Harta statica
for(i = 0; i < NrBlockY; i++)
    for(j = 0; j < NrBlockX; j++)
    {
        file >> cTerrain[i][j];
    }

for(i = 0; i <NrBlockY; i++)
    for(j = 0; j < NrBlockX; j++)
    {
        file >> k;
        Normal_Map[i][j]  = DATABASE.GetChunkSkin( cTerrain[i][j],k,0,0 );
    }

MapWidth  = NrBlockX*BLOCK_WIDTH;
MapHeight = NrBlockY*BLOCK_HEIGHT;


file >> saux;
file >> saux;

while( saux != "CLOSE_BG" && !file.fail() )
{
    file >> saux;
    file >> TempFreeChunk ->nr;


        file >> saux >> TempFreeChunk ->x;
        file >> saux >> TempFreeChunk ->y;
        file >> saux >> TempFreeChunk ->orient;
        file >> saux >> TempFreeChunk ->animated;
        file >> saux >> TempFreeChunk ->frames;


    TempFreeChunk->surf = NULL;
    //Sursa presup De Eroare
    TempFreeChunk->surf = DATABASE.GetChunkSkin( TempFreeChunk ->nr,TempFreeChunk ->orient,
                                      TempFreeChunk ->animated,TempFreeChunk ->frames );

/// POSIBIL LOC DE EROARE
    if( TempFreeChunk->surf != NULL )
    {
        TempFreeChunk->tex = SDL_CreateTextureFromSurface( RENDER_MAIN,TempFreeChunk->surf );
        AddFreeChunk( TempFreeChunk,BACK );
    }
    else
    {
        TempFreeChunk->tex = SDL_CreateTextureFromSurface( RENDER_MAIN,DEBUG.sfExMark );
    }



    file >> saux;
}

file >> saux;
file >> saux;

while( saux != "CLOSE_FG" && !file.fail() )
{
    file >> saux;
    file >> TempFreeChunk ->nr;

        file >> saux >> TempFreeChunk ->x;
        file >> saux >> TempFreeChunk ->y;
        file >> saux >> TempFreeChunk ->orient;
        file >> saux >> TempFreeChunk ->animated;
        file >> saux >> TempFreeChunk ->frames;


    TempFreeChunk->surf = DATABASE.GetChunkSkin( TempFreeChunk ->nr,TempFreeChunk ->orient,
                                      TempFreeChunk ->animated,TempFreeChunk ->frames );
    TempFreeChunk->tex = SDL_CreateTextureFromSurface( RENDER_MAIN,TempFreeChunk->surf );

    AddFreeChunk( TempFreeChunk,FORE );

    file >> saux;
}


file >> saux;
file >> saux;


while( saux != "CLOSE_DYNAMIC" && !file.fail() )
{
    file >> saux;
    file >> TempFreeChunk ->nr;

        file >> saux >> TempFreeChunk ->x;
        file >> saux >> TempFreeChunk ->y;
        file >> saux >> TempFreeChunk ->orient;
        file >> saux >> TempFreeChunk ->animated;
        file >> saux >> TempFreeChunk ->frames;


    tempsurf = DATABASE.GetChunkSkin( TempFreeChunk ->nr,TempFreeChunk ->orient,
                                      TempFreeChunk ->animated,TempFreeChunk ->frames );
    TempFreeChunk->tex = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );

    AddFreeChunk( TempFreeChunk,DYNAMIC );

    file >> saux;
}



file >> saux;
file >> saux;

while( saux != "CLOSE_LIGHT" && !file.fail() )
{
    file >> saux >> TempLight ->nr;
    file >> saux >> TempLight ->x;
    file >> saux >> TempLight ->y;
    file >> saux >> TempLight ->range;
    file >> saux;
    addlight(TempLight ->nr,TempLight ->range,TempLight ->x,TempLight ->y);
}

file >> saux;
file >> saux;

while( saux != "CLOSE_COLIDER" && !file.fail() )
{
    file >> saux >> TempCol ->id;
    file >> saux >> TempCol ->x;
    file >> saux >> TempCol ->y;
    file >> saux >> TempCol ->w;
    file >> saux >> TempCol ->h;
    file >> saux;
    AddCollider(TempCol ->x - TempCol ->w/2,TempCol ->y - TempCol ->h/2,TempCol ->w,TempCol ->h,TempCol->id);
}

file >> saux;
file >> saux;


while( saux != "CLOSE_INTERACTION" && !file.fail() )
{
    int idaux;
    file >> saux >> info[0];
    file >> saux >> info[1];
    file >> saux >> info[2];
    file >> saux >> info[3];
    file >> saux >> info[4];//type
    file >> saux >> idaux;
    file >> saux >> info[5];
    file >> saux >> info[6];
    file >> saux >> info[7];
    file >> saux >> info[8];
    AddInterSpot( info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],idaux );
    file >> saux;
}

file >> saux;
file >> saux;


while( saux != "CLOSE_NOTPLAYERCREATURE" && !file.fail() )
{
    file >> saux;
    file.get( caux,90,'\n' );
    BAS_ctos( caux,modelchunk->Name );
    file >> saux >> modelchunk->rFrame.x;
    file >> saux >> modelchunk->rFrame.y;
    file >> saux >> modelchunk->rFrame.w;
    file >> saux >> modelchunk->rFrame.h;
    file >> saux >> iaux;
    file >> saux >> modelchunk->filenr;

    modelchunk->behaviors = ~( FLAG_TALK_TO_PLAYER | FLAG_COMBAT_ABLE ) ;
    //NPC FILE BEGINS


    ssaux.str("");
    ssaux << "Data//NPCs//" << modelchunk->filenr << ".npc";
    in_aux.open( ssaux.str().c_str() );

    if( in_aux.is_open() )
    {
        in_aux >> saux >> saux;

    while( saux != "<END_BEHAVIORS>" )
    {
        if( saux == "TALK_TO_PLAYER" )
        modelchunk->behaviors |= FLAG_TALK_TO_PLAYER;
        else if( saux == "COMBAT_ABLE" )
        modelchunk->behaviors |= FLAG_COMBAT_ABLE;

        in_aux >> saux;
    }

    if( modelchunk->behaviors & FLAG_COMBAT_ABLE )
    {
        in_aux >> saux;
        in_aux >> saux >> modelchunk->maxhealth;
        in_aux >> saux >> modelchunk->health;
        in_aux >> saux >> modelchunk->tMoveSpeed;
        in_aux >> saux >> modelchunk->attackspeed;
        in_aux >> saux >> modelchunk->dmg;
        in_aux >> saux >> modelchunk->Body.x;
        in_aux >> saux >> modelchunk->Body.y;
        in_aux >> saux >> modelchunk->Body.w;
        in_aux >> saux >> modelchunk->Body.h;
    }

    if( modelchunk->behaviors & FLAG_TALK_TO_PLAYER )
    {
        in_aux >> saux;

        modelchunk->Dialog_maintopic = AddDialogTopic();

        dtaux = modelchunk->Dialog_maintopic;
        cinstruct = ctopic = cactions = 0;
        dtaux->lines_nr = 0;
        in_aux >> saux;

        for( i = 0; i < 4; i++ )
        dtaux->instructions[ i ] = HIDE;

        while( saux != "<ENDDIALOG>" )
        {
            if( saux == "txtline:" )
            {
                in_aux.get( caux,90 );
                BAS_ctos( caux,dtaux->npctext[dtaux->lines_nr++] );
            }
            else if( saux == "NEWTOPIC:" )
            {
                dtaux->instructions[ ctopic ] = NONE;
                dtaux->NextTopic[ ctopic ] = Map::AddDialogTopic(  );
                in_aux.get( caux,90 );
                BAS_ctos( caux,dtaux->TopicName[ctopic++] );
            }
            else if( saux == "NEWTOPIC(SQ):" )
            {
                dtaux->instructions[ ctopic ] = ADD_QUEST;
                in_aux >> dtaux->val[ctopic];
                dtaux->NextTopic[ ctopic ] = Map::AddDialogTopic(  );
                in_aux.get( caux,90 );
                BAS_ctos( caux,dtaux->TopicName[ctopic++] );
            }
            else if( saux == "NEWTOPIC(FQ):" )
            {
                dtaux->instructions[ ctopic ] = FINISH_QUEST;
                in_aux >> dtaux->val[ctopic];
                dtaux->NextTopic[ ctopic ] = Map::AddDialogTopic(  );
                in_aux.get( caux,90 );
                BAS_ctos( caux,dtaux->TopicName[ctopic++] );
            }
            else if( saux == "NEWTOPIC(GIHO):" )
            {
                dtaux->instructions[ ctopic ] = GIVE_ITEM;
                in_aux >> dtaux->val[ctopic];
                in_aux >> dtaux->val2[ctopic];
                in_aux >> dtaux->val3[ctopic];
                dtaux->NextTopic[ ctopic ] = Map::AddDialogTopic(  );
                in_aux.get( caux,90 );
                BAS_ctos( caux,dtaux->TopicName[ctopic++] );
            }
            else if( saux == "NEWTOPIC(CS):" )
            {
                dtaux->instructions[ ctopic ] = CLOSE;
                //in_aux >> dtaux->val[ctopic];
                dtaux->NextTopic[ ctopic ] = NULL;//Map::AddDialogTopic(  );
                in_aux.get( caux,90 );
                BAS_ctos( caux,dtaux->TopicName[ctopic++] );
            }
            else if( saux == "OPENTOPIC:" )
            {
                ctopic = cinstruct = cactions = 0;

                in_aux.get( caux,90 );
                dtaux = modelchunk->Dialog_maintopic;

                for( i = 1; (unsigned)i < SDL_strlen( caux ); i++ )
                {
                    caux[i-1] = caux[i];
                }
                caux[i-1] = '\0';

                while( SDL_strlen( caux ) > 0 )
                {
                    dtaux = dtaux->NextTopic[ (int)caux[0] - (int)'0' -1 ];

                    for( i = 1; (unsigned)i < SDL_strlen( caux ); i++ )
                    caux[i-1] = caux[i];
                    caux[i-1] = '\0';
                }
                dtaux->lines_nr = 0;

                for( i = 0; i < 4; i++ )
                dtaux->instructions[ i ] = HIDE;
                for( i = 0; i < 3; i++ )
                dtaux->action[ i ] = NONE;
            }
            else if( saux == "npcmanip:" )
            {
                in_aux >> saux;

                if( saux == "SPAWN" )
                dtaux->action[ cactions ] = SPAWN ;
                else if( saux == "UNSPAWN" )
                dtaux->action[ cactions ] = UNSPAWN ;

                in_aux >> saux;

                if( saux == "FIRST" ) dtaux->act_info[ cactions ]= FIRST ;
                else if( saux == "LAST" ) dtaux->act_info[ cactions ] = LAST ;
                else if( saux == "ALL" ) dtaux->act_info[ cactions ] = ALL ;

                in_aux >> dtaux->action_nr[ cactions ];
                in_aux.get( caux,30 );
                BAS_ctos( caux,dtaux->action_name[ cactions++ ] );
            }
            else if( saux == "intermanip:" )
            {
                dtaux->action[ cactions ] = DIALACT_INTERMANIP;
                in_aux>>dtaux->act_id[cactions]>>dtaux->action_nr[cactions]>>dtaux->act_info[cactions];
                cactions++;
                CONS.out( "Red Intermanip Command" );
            }
            else if( saux == "colidemanip:" )
            {
                dtaux->action[ cactions ] = DIALACT_COLMANIP;
                in_aux>>dtaux->act_id[cactions]>>dtaux->act_info[cactions];
                cactions++;
                CONS.out( "Red Colmanip Command" );
            }

            in_aux >> saux;
        }



    }

    in_aux.close();
    }
    else //If the file doesn't exist
    {

    }


    file >> saux;
    modelchunk->anime_categ = modelchunk->anime_frame = 0;
    modelchunk->skin = DATABASE.GetNPCSkin( iaux );
    modelchunk->tex  = SDL_CreateTextureFromSurface( RENDER_MAIN,modelchunk->skin );

    modelchunk->rBody.x = 20;
    modelchunk->rBody.y = 65;
    modelchunk->rBody.w = 55;
    modelchunk->rBody.h = 34;
    modelchunk->tLastMove = SDL_GetTicks();
    modelchunk->attackstyle = 0;
    modelchunk->tAttack = SDL_GetTicks();
    modelchunk->Dead = false;
    modelchunk->Spawned = true;
    Map::AddNPC( modelchunk );


}



tempsurf = Pixels.RenderNormalMap();
RendNormalMap = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
SDL_FreeSurface( tempsurf );

tempsurf = Pixels.RenderForeLayerMap();
RendForeLayer = SDL_CreateTextureFromSurface( RENDER_MAIN, tempsurf );
SDL_FreeSurface( tempsurf );

//Light map
for( i = 0;i < 4; i++ )
{
    tempsurf = Pixels.RenderLightMap( i );
    DayNightPhase[i] = SDL_CreateTextureFromSurface( RENDER_MAIN,tempsurf );
    SDL_FreeSurface( tempsurf );
}


file.close();

PostLoadChanges();
delete modelchunk;
}

void Map::AddFreeChunk( FreeChunk *model,int ground ){
FreeChunk *newchunk = new FreeChunk;
*newchunk = *model;
newchunk->cframe = 0;

SDL_QueryTexture( newchunk->tex,0,0,&newchunk->w,&newchunk->h );

if( ground == BACK )
{
    newchunk->next = LastChunk_back->next;
    LastChunk_back->next = newchunk;
    LastChunk_back = newchunk;
}
else if( ground == FORE )
{
    newchunk->next = LastChunk_fore->next;
    LastChunk_fore->next = newchunk;
    LastChunk_fore = newchunk;
}
else if( ground == DYNAMIC )
{
    newchunk->next = LastChunk_dynam->next;
    LastChunk_dynam->next = newchunk;
    LastChunk_dynam = newchunk;
}

///Add Alpha settings
if( newchunk->nr == 209 || newchunk->nr == 210 )
{
    SDL_SetTextureAlphaMod( newchunk->tex,120 );
    SDL_SetTextureBlendMode( newchunk->tex,SDL_BLENDMODE_BLEND );
}


}
void Map::UpdateLastChunk(){
FreeChunk *i;
for( i = FirstFreeChunk_back; i != LastFreeChunk_back; i = i->next )
if( i->next == LastFreeChunk_back ) LastChunk_back = i;

for( i = FirstFreeChunk_fore; i != LastFreeChunk_fore; i = i->next )
if( i->next == LastFreeChunk_fore ) LastChunk_fore = i;

for( i = FirstFreeChunk_dynam; i != LastFreeChunk_dynam; i = i->next )
if( i->next == LastFreeChunk_dynam ) LastChunk_dynam = i;

}
void Map::addlight(int nr,int range,int x,int y){

LIGHT* newlight = new LIGHT;
newlight ->next = FirstLight->next;
FirstLight->next = newlight;
newlight->nr = nr;
newlight->range = range;
newlight->x = x;
newlight->y = y;
}
void Map::AddCollider(int x,int y,int w,int h,int id){
COLLIDER *newcol = new COLLIDER;
newcol -> ON = true;
newcol -> id = id;
newcol -> x = x;
newcol -> y = y;
newcol -> w = w;
newcol -> h = h;
newcol -> next = FirstCol -> next;
FirstCol -> next = newcol;
}
DIALOG_TOPIC * Map::AddDialogTopic(){
DIALOG_TOPIC *newd = new DIALOG_TOPIC;
newd->NextTopic[0] = newd->NextTopic[1] = newd->NextTopic[2] = newd->NextTopic[3] = NULL;
for( int i = 0; i < 3; i++ )
newd->action[i] = newd->act_info[i] = -1;
return newd;
}
void Map::AddNPC(NotPLayerCreature* model){
NotPLayerCreature *newnpc = new NotPLayerCreature;
*newnpc = *model;
newnpc->tLastMove = SDL_GetTicks();
newnpc->tMove     = SDL_GetTicks();
newnpc->anime_frame = 0;
newnpc->boss = 0;


if( newnpc->behaviors & FLAG_COMBAT_ABLE )
AddToEnemyList( newnpc );

newnpc->next = FirstNPC->next;
FirstNPC->next = newnpc;
}
void Map::AddToEnemyList( NotPLayerCreature *target ){
NPCList *newtarget = new NPCList;

newtarget->next = FirstEnemy->next;
FirstEnemy->next = newtarget;
newtarget->npc = target;
newtarget->npc->comb = false;
}
void Map::display_back(){
FreeChunk *fcf;

SDL_SetRenderDrawBlendMode( WIN_MAIN.render, SDL_BLENDMODE_BLEND );

//Normal Map + Back Layer
ApplyTex( DisplayLocation_x  , DisplayLocation_y , RendNormalMap );


//Anime Handle
if( SDL_GetTicks() - tAnime > 250 )
{
    change_frame = true;
    tAnime = SDL_GetTicks();
}
else
change_frame = false;

for( fcf = FirstFreeChunk_back->next;fcf != LastFreeChunk_back; fcf = fcf->next )
{
    if( change_frame )
    {
        fcf->cframe++;
        if( fcf->cframe >= fcf->frames )
        fcf->cframe = 0;
    }


    if( fcf->animated )
    {
        AnimeRect.y = 0;
        AnimeRect.w = fcf->w/fcf->frames;
        AnimeRect.h = fcf->h;
        AnimeRect.x = AnimeRect.w*fcf->cframe;
        ApplyTex( DisplayLocation_x + fcf->x,DisplayLocation_y + fcf->y,fcf->tex ,&AnimeRect );
    }

}


}
void Map::display_fore(){

FreeChunk *fcf;

SDL_SetRenderDrawBlendMode( WIN_MAIN.render, SDL_BLENDMODE_MOD );

ApplyTex( DisplayLocation_x  , DisplayLocation_y , RendForeLayer );

for( fcf = FirstFreeChunk_fore->next;fcf != LastFreeChunk_fore; fcf = fcf->next )
{
    if( change_frame )
    {
        fcf->cframe++;
        if( fcf->cframe >= fcf->frames )
        fcf->cframe = 0;
    }

    if( fcf->animated )
    {
        AnimeRect.y = 0;
        AnimeRect.w = fcf->w/fcf->frames;
        AnimeRect.h = fcf->h;
        AnimeRect.x = AnimeRect.w*fcf->cframe;
        ApplyTex( DisplayLocation_x + fcf->x,DisplayLocation_y + fcf->y,fcf->tex ,&AnimeRect );
    }
}
}
void Map::display_dynamic( int layer ){
/** layer 0 BG ; layer 1 FG **/
/**
FreeChunk *ifcd;
for( ifcd = FirstFreeChunk_dynam->next; ifcd != LastFreeChunk_dynam; ifcd = ifcd->next )
if( ( layer == 0 && ifcd->y + ifcd->skin->h <  PLAYER.rBody.y + PLAYER.rBody.h ) ||
    ( layer == 1 && ifcd->y + ifcd->skin->h >= PLAYER.rBody.y + PLAYER.rBody.h )  )//Above hero condition
{
    ///ApplySurf(DisplayLocation_x + ifcd->x,
    ///              DisplayLocation_y + ifcd->y,
    ///              ifcd->skin,NULL ) ;
}
**/
}
void Map::display_lights(){

switch( cDayTime )
{

    case 3://Seara-Portocaliu
    ApplyTex(DisplayLocation_x,DisplayLocation_y ,DayNightPhase[1],NULL);
    break;
    case 4://Seara-deschis
    ApplyTex(DisplayLocation_x,DisplayLocation_y ,DayNightPhase[2],NULL);
    break;
    case 5://Seara-inchis
    ApplyTex(DisplayLocation_x,DisplayLocation_y ,DayNightPhase[3],NULL);
    break;
    case 6:
    ApplyTex(DisplayLocation_x,DisplayLocation_y ,DayNightPhase[2],NULL);
    break;
    case 1:
    ApplyTex(DisplayLocation_x,DisplayLocation_y ,DayNightPhase[0],NULL);
    break;

}
}
void Map::handle_logics(){

if( MapWidth < SCREEN_WIDTH )
    DisplayLocation_x = ( SCREEN_WIDTH - MapWidth )/2;
if( MapHeight < SCREEN_HEIGHT )
    DisplayLocation_y = ( SCREEN_HEIGHT - MapHeight )/2;

}
void Map::manipulate_inter( int id,int val,int info,int type ){
InterSpot *isaux;

for( isaux = FirstItr->next ; isaux != LastItr;isaux = isaux->next )
{
    if( isaux->id == id )
    {
        if( val >= 0 )
        isaux->info[val] = info;//!val si info sunt puse bine, chear daca nu pare!

        if( type != -999 )
        isaux->type = type;

        stringstream ssaux;
        ssaux<<"id: "<<isaux->id<<" val: "<<isaux->info[0];
        CONS.out( ssaux.str().c_str() );


    }

}

}
void Map::manipulate_colider( int id,int val ){
COLLIDER *isaux;

for( isaux = FirstCol->next ; isaux != LastCol;isaux = isaux->next )
{
    if( isaux->id == id )
    {
        isaux->ON = val;
        stringstream ssaux;
        ssaux<<"Colider id: "<<isaux->id<<" ON: "<<isaux->ON;
        CONS.out( ssaux.str().c_str() );
    }

}

}

//Day - Night cicle
void Map::handle_time(){
if( SDL_GetTicks() > tstart + tSuccSpeed && DaySuccession == true)
{
    cDayTime++;
    if( cDayTime > 6 )
    cDayTime = 1;
    tstart = SDL_GetTicks();
}
}
void Map::SetcDayTime(int nr){
cDayTime = nr;
}
void Map::SetDaySuccesion(bool succ){
if( succ == 1 )
{
    DaySuccession = true;
    if( tpause > tstart )
    tstart = SDL_GetTicks() - ( tpause-tstart );
}
else if( succ == 0 )
{
    DaySuccession = false;
    tpause = SDL_GetTicks();
}

}
void Map::SetDaySuccSpeed(int nr){
tSuccSpeed = nr;
}

//Map cleanup
void Map::CleanCMap(){

    FreeChunk *FreeChunkAux;
    COLLIDER *ColliderDelAux;
    InterSpot *ISAux;

    SDL_DestroyTexture( RendNormalMap );

    //Clean BG
    while( FirstFreeChunk_back -> next != LastFreeChunk_back )
    {
        FreeChunkAux = FirstFreeChunk_back -> next;
        FirstFreeChunk_back -> next = FirstFreeChunk_back -> next -> next;
        delete FreeChunkAux;
    }
    LastChunk_back = FirstFreeChunk_back;
    //Clean BG
    while( FirstFreeChunk_fore -> next != LastFreeChunk_fore )
    {
        FreeChunkAux = FirstFreeChunk_fore -> next;
        FirstFreeChunk_fore -> next = FirstFreeChunk_fore -> next -> next;
        delete FreeChunkAux;
    }
    LastChunk_fore = FirstFreeChunk_fore;

    while( FirstFreeChunk_dynam -> next != LastFreeChunk_dynam )
    {
        FreeChunkAux = FirstFreeChunk_dynam -> next;
        FirstFreeChunk_fore -> next = FirstFreeChunk_dynam -> next -> next;
        delete FreeChunkAux;
    }
    LastChunk_dynam = FirstFreeChunk_dynam;

    //Clean Lights
    while( FirstLight ->next != LastLight )
    {
        LIGHT *p = FirstLight,*delaux;
        for(;p ->next != LastLight && p ->next != NULL ;p = p->next)
        {
            delaux = p->next;
            p->next = p->next ->next;
            delete delaux;
        }
    }

    while( FirstCol -> next != LastCol )
    {
        ColliderDelAux = FirstCol ->next;
        FirstCol -> next = FirstCol -> next -> next;
        delete ColliderDelAux;
    }

    while( FirstItr->next != LastItr )
    {
        ISAux = FirstItr->next;
        FirstItr->next = FirstItr->next->next;
        delete ISAux;
    }


}
Map::~Map(){

}
