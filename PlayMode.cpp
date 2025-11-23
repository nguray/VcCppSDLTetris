#include "Game.h"
#include "PlayMode.h"
#include "Tetromino.h"


PlayMode::PlayMode(Game *g):IGameMode(g)
{
}

bool PlayMode::ProcessEvent(SDL_Event &e)
{
 
    Tetromino *curTetro;
    //------------------------------------

    if ((curTetro=game->curTetromino)==nullptr) return false;
    
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
            game->CheckHighScore();
            return false;
        case SDLK_SPACE:
            fDrop = true;
            break;
        case SDLK_UP:
            //-- Request current tetromino rotation
            fRotateTetromino = true;
            break;
        case SDLK_DOWN:
            fFastDown = true;
            break;
        case SDLK_LEFT:
            velocityX = -1;
            IsOutLimit = &Tetromino::IsOutLeftLimit;
            break;
        case SDLK_RIGHT:
            velocityX = 1;
            IsOutLimit = &Tetromino::IsOutRightLimit;
            break;
        }

    }
    else if( e.type == SDL_KEYUP )
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            velocityX = 0;
            break;
        case SDLK_RIGHT:
            velocityX = 0;
            break;
        case SDLK_DOWN:
            fFastDown = false;
            break;
        }
    }

    return false;

}

void PlayMode::Update()
{
    Tetromino *curTetro;
    //------------------------------------

    if ((curTetro=game->curTetromino)==nullptr) return;

    if (!fPause){

        Uint32 curTime = SDL_GetTicks();

        if (nbCompletedLine>0){
            if ((curTime-startTimeE)>150){
                startTimeE = curTime;
                nbCompletedLine--;
                game->EraseFirstCompledLine();
                //-- Play sound
                if (game->succesSound!=NULL){
                    Mix_PlayChannel(-1, game->succesSound, 0);
                }
            }

        }
        
        if (horizontalMove!=0){

            if ((curTime-startTimeH)>20){
                startTimeH = curTime;

                for(int i=0;i<5;i++){
                    int backupX = curTetro->m_x;
                    curTetro->m_x += horizontalMove;

                    //if (curTetro->IsOutLRLimit(horizontalMove)){
                    if ((*curTetro.*IsOutLimit)()){
                        curTetro->m_x = backupX;
                        horizontalMove = 0;
                        break;
                    }else if (curTetro->HitGround(game->board)){
                        curTetro->m_x = backupX;
                        horizontalMove = 0;
                        break;
                    }

                    if (horizontalMove!=0){
                        if (horizontalStartColumn!=curTetro->Column()){
                            curTetro->m_x = backupX;
                            horizontalMove = 0;
                            startTimeH = SDL_GetTicks();
                            break;
                        }
                    }

                }

            }
        }else if (fRotateTetromino){
            //-- Do current tetromino rotation when there no more horizontal move
            curTetro->RotateLeft();
            if (curTetro->HitGround( game->board)){
                //-- Undo Rotate
                curTetro->RotateRight();
            }else if (curTetro->IsOutRightLimit()){
                int backupX = curTetro->m_x;
                //-- Move Tetromino inside board
                do{
                    curTetro->m_x--;
                }while(curTetro->IsOutRightLimit());
                if (curTetro->HitGround( game->board)){
                    //-- Undo Move
                    curTetro->m_x = backupX;
                    //-- Undo Rotate
                    curTetro->RotateRight();
                }
            }else if (curTetro->IsOutLeftLimit()){
                int backupX = curTetro->m_x;
                //-- Move Tetromino inside board
                do{
                    curTetro->m_x++;
                }while(curTetro->IsOutLeftLimit());
                if (curTetro->HitGround(game->board)){
                    //-- Undo Move
                    curTetro->m_x = backupX;
                    //-- Undo Rotate
                    curTetro->RotateRight();
                }
            }
            fRotateTetromino = false;
        }else if (fDrop){
            int curTime = SDL_GetTicks();
            if ((curTime-startTimeV)>10){
                startTimeV = curTime;
                for (int i=0;i<6;i++){
                    //-- Move down an check
                    curTetro->m_y++;
                    if (curTetro->HitGround( game->board)){
                        curTetro->m_y--;
                        nbCompletedLine = game->FreezeCurTetromino();
                        game->NewTetromino();
                        fDrop = false;
                        break;
                    }else if (curTetro->IsOutBottomLimit()){
                        curTetro->m_y--;
                        nbCompletedLine = game->FreezeCurTetromino();
                        game->NewTetromino();
                        fDrop = false;
                        break;
                    }
                    if (velocityX!=0){

                        if ((curTime-startTimeH)>20){
                            int backupX = curTetro->m_x;
                            curTetro->m_x += velocityX;

                            if ((*curTetro.*IsOutLimit)()){
                                curTetro->m_x = backupX;
                            }else if (curTetro->HitGround(game->board)){
                                curTetro->m_x = backupX;
                            }else{
                                startTimeH = curTime;
                                horizontalMove = velocityX;
                                horizontalStartColumn = curTetro->Column();
                                break;
                            }
                        }

                    }
                }
            }

        }else{

            int curTime = SDL_GetTicks();
            int limitElapse = (fFastDown?10:30);
            if ((curTime-startTimeV)>limitElapse){
                startTimeV = curTime;

                for (int i=0;i<4;i++){
                    curTetro->m_y += 1;
                    if (curTetro->HitGround( game->board)){
                        curTetro->m_y -= 1;
                        nbCompletedLine = game->FreezeCurTetromino();
                        game->NewTetromino();
                        break;
                    }else if (curTetro->IsOutBottomLimit()){
                        curTetro->m_y -= 1;
                        nbCompletedLine = game->FreezeCurTetromino();
                        game->NewTetromino();
                        break;                    
                    }

                    if (velocityX!=0){

                        curTime = SDL_GetTicks();

                        if ((curTime-startTimeH)>15){

                            int backupX = curTetro->m_x;
                            curTetro->m_x += velocityX;

                            if ((*curTetro.*IsOutLimit)()){
                                curTetro->m_x = backupX;
                            }else if (curTetro->HitGround( game->board)){
                                curTetro->m_x = backupX;
                            }else{
                                horizontalMove = velocityX;
                                horizontalStartColumn = curTetro->Column();
                                startTimeH = curTime;
                                break;
                            }

                        }
                    }
                }


            }


        }


    }


}

void PlayMode::Init()
{
    startTimeV = 0;
    startTimeH = 0;
    startTimeE = 0;
    nbCompletedLine = 0;
    horizontalMove = 0;
    fFastDown = false;
    fDrop = false;
    velocityX = 0;
    fRotateTetromino = false;

}

void PlayMode::Draw()
{
    Tetromino *curTetro;
    //------------------------------------
    if ((curTetro=game->curTetromino)==nullptr) return;
    curTetro->Draw(game->renderer);

}

