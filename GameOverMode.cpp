#include "Game.h"
#include "GameOverMode.h"


GameOverMode::GameOverMode(Game *g):IGameMode(g)
{
}


bool GameOverMode::ProcessEvent(SDL_Event &e)
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
        case SDLK_SPACE:
            //fQuitGame = false;
            game->InitGame();
            game->SetStandByMode();
            break;
        } 
    }

    return false;

}

void GameOverMode::Draw()
{

    int         x,y;
    int         w=0,h=0;
    SDL_Color   col= {255,255,0,255};
    //---------------------------------------------
    y = TOP + (NB_ROWS/4)*CELL_SIZE;
    //-- Draw Title
    SDL_Surface *surfScore = TTF_RenderUTF8_Blended(game->gFont,"GAME OVER", col);
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

    surfScore = TTF_RenderUTF8_Blended(game->gFont,"Press SPACE to Continue", col);
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
