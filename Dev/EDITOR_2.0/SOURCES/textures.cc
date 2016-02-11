#include "textures.h"


Textures TEXTURES;



Textures::Textures(){

    int i,j;

    for(i = 0; i < MAX_ITEMS; i++)
        for(j = 0; j < MAX_STATES; j++){

            surf[i][j] = NULL;
        }
}

SDL_Surface* Textures::getSurf(int nr,PixelManipulationRotation state){

    stringstream ssaux;

    //If the image index is valid
    if(nr < MAX_ITEMS && nr >= 0 && state < 8 && state >= 0){

        //If the image wasn't loaded yet
        if(surf[nr][state] == NULL){

            //The image must not be rotated
            if(state == ROTATE_NONE){

                if(nr < 200){

                    ssaux<<"textures//BUILDING//build//";
                    ssaux<<nr;
                    ssaux<<".png";
                    surf[nr][state] = loadSurf(ssaux.str().c_str());
                }
                else if(nr < MAX_ITEMS){

                    ssaux<<"textures//BUILDING//decoration//";
                    ssaux<<nr;
                    ssaux<<".png";
                    surf[nr][state] = loadSurf(ssaux.str().c_str());
                }
            }
        }

        //Return the loaded Image
        return surf[nr][state];
    }
    //If the image request is invalide
    else{

        return NULL;
    }
}
