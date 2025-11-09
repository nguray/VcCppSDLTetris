#pragma once
#include "IGameMode.h"
//#include "Game.h"

class Game;
class PlayMode : public IGameMode
{
public:
    PlayMode(Game *g);
    bool ProcessEvent(SDL_Event &e);
    void Update();
    void Init();
    void Draw();

    bool fDrop;
    bool fFastDown;
    bool fPause;
    int  velocityX;
    int nbCompletedLine;
    int horizontalMove;
    int horizontalStartColumn;
    int startTimeV;
    int startTimeH;
    int startTimeR;


};