#include "Game.h"
#include "HighScoresMode.h"


HighScoresMode::HighScoresMode(Game *g):
    IGameMode(g),
    iHighScoreColor(0)
{
    game = g;
}

bool HighScoresMode::ProcessEvent(SDL_Event &e)
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
        case SDLK_BACKSPACE:
            {
                int l = game->playerName.length();
                if (l>0){
                    game->playerName = game->playerName.substr(0,l-1);
                    game->highScores[game->idHighScore].name = game->playerName;
                }
                break;
            }
        case SDLK_ESCAPE:
        case SDLK_KP_ENTER:
        case SDLK_RETURN:
            if (game->playerName.length()==0){
                game->highScores[game->idHighScore].name = "XXXXXX";
            }else{
                game->highScores[game->idHighScore].name = game->playerName;
            }
            game->SaveHighScores();
            game->SetStandByMode();
            return false;
            break;
        default:
            if (keyMaps.find((int)(e.key.keysym.sym)) != keyMaps.end()){
                char c = keyMaps[(int)(e.key.keysym.sym)];
                game->playerName += c;
                game->highScores[game->idHighScore].name = game->playerName;
            }
            break;
        }
    }

    return false;

}

void HighScoresMode::Draw()
{
    int         x,y;
    int         w=0,h=0;
    SDL_Color   col= {255,255,0,255};
    //---------------------------------------------
    y = TOP + CELL_SIZE;
    //-- Draw Title
    SDL_Surface *surfScore = TTF_RenderUTF8_Blended(game->gFont,"HIGH SCORES", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( game->renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(game->renderer,texScore,NULL,&rectDest);
            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }

    y += 3*h; 
    int xCol0 = LEFT + CELL_SIZE;
    int xCol1 = LEFT + (NB_COLUMNS/2+2)*CELL_SIZE;
    for (int i=0;i<9;i++){

        if (i==game->idHighScore){
            if (iHighScoreColor % 2){
                col = {100,100,0,255};
            }else{
                col = {255,255,0,255};
            }
        }else{
            col = {255,255,0,255};
        }

        SDL_Surface *surfScore = TTF_RenderUTF8_Blended(game->gFont,game->highScores[i].name.c_str(), col);
        if (surfScore!=NULL){

            SDL_Texture *texScore = SDL_CreateTextureFromSurface( game->renderer, surfScore);

            if (texScore!=NULL){
                int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
                if (err==0){
                    SDL_Rect rectDest={xCol0,y,w,h};
                    SDL_RenderCopy(game->renderer,texScore,NULL,&rectDest);
                }
                //--
                SDL_DestroyTexture(texScore);
            }
            //--
            SDL_free(surfScore);
        }

        std::ostringstream  ss;
        ss << std::setfill('0') << std::setw(6) << game->highScores[i].score;
        std::string strSCore = ss.str();

        surfScore = TTF_RenderUTF8_Blended(game->gFont,strSCore.c_str(), col);
        if (surfScore!=NULL){

            SDL_Texture *texScore = SDL_CreateTextureFromSurface( game->renderer, surfScore);

            if (texScore!=NULL){
                int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
                if (err==0){
                    SDL_Rect rectDest={xCol1,y,w,h};
                    SDL_RenderCopy(game->renderer,texScore,NULL,&rectDest);
                }
                y += h +8;
                //--
                SDL_DestroyTexture(texScore);
            }
            //--
            SDL_free(surfScore);
        }

    }

}

void HighScoresMode::Update()
{

    int curTime = SDL_GetTicks();
    if ((curTime-startTimeV)>500){
        startTimeV = curTime;
        iHighScoreColor++;
    }
}