
#pragma once

#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#elif __has_include(<SDL.h>)
#include <SDL.h>
#else
#error "SDL headers not found. Ensure the SDL include path is configured."
#endif

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