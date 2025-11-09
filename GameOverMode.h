#pragma once
#include "IGameMode.h"

class Game;
class GameOverMode : public IGameMode
{
public:

    GameOverMode(Game *g);
    bool ProcessEvent(SDL_Event &e);
    void Draw();

};
