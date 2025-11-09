#include "Game.h"
#include "StandByMode.h"


StandByMode::StandByMode(Game *g)
{
    game = g;
}

bool StandByMode::ProcessEvent(SDL_Event &e)
{
     if( e.type == SDL_QUIT )
    {
        //fQuitGame = true;
        return true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            //fQuitGame = true;
            return true;
            break;
        case SDLK_SPACE:
            //-- Start Game
            game->SetPlayMode();
            game->InitGame();
            game->NewTetromino();
            break;
        }
    }


    return false;

}


void StandByMode::Draw()
{
    int         x,y;
    int         w=0,h=0;
    SDL_Color   col= {255,255,0,255};
    //---------------------------------------------
    y = TOP + (NB_ROWS/4)*CELL_SIZE;
    //-- Draw Title
    SDL_Surface *surfScore = TTF_RenderUTF8_Blended(game->gFont,"C++ Tetris using SDL2", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( game->renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(game->renderer,texScore,NULL,&rectDest);
                y += 2*h + 8;

            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }

    surfScore = TTF_RenderUTF8_Blended(game->gFont,"Press SPACE to Play", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( game->renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(game->renderer,texScore,NULL,&rectDest);
                //y += 2*h + 8;

            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }
    

}
