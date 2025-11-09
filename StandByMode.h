#pragma once
//#include "Game.h"
#include "IGameMode.h"

class Game;
class StandByMode : public IGameMode
{
public:
    StandByMode(Game *g);
    bool ProcessEvent(SDL_Event &e);
    void Draw();

};