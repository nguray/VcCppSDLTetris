#pragma once
#include <SDL.h>

class Game;
class IGameMode 
{
public:
    Game *game;
    IGameMode(Game *g):game(g){};
    virtual bool ProcessEvent(SDL_Event &e)=0;
    virtual void Update(){};
    virtual void Draw(){};
    virtual void Init(){};
};