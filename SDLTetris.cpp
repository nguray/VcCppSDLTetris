/*-------------------------------------------------------------------------*\
                        Simple Tetris using SDL2

    Raymond NGUYEN THANH                                    2022-10-10
\*-------------------------------------------------------------------------*/

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <random>
#include "TetrisConst.h"
#include "Tetromino.h"
#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include "StandByMode.h"
#include "PlayMode.h"
#include "HighScoresMode.h"
#include "GameOverMode.h"
#include "Game.h"


int main(int argc, char *argv[])
{
    //std::cout << "Hello World !!" << SDL_GetError() << std::endl;

    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    TTF_Init();
 
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {

        //Create window
        window = SDL_CreateWindow( "C++ SDL Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {

            Game *myGame = new Game(window);

            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while( !quit )
            {

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    quit = myGame->curGameMode->ProcessEvent(e);

                }

                myGame->Update();

                myGame->Draw();


            }

            //Fill the surface white
            //SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
            //Update the surface
            //SDL_UpdateWindowSurface( window );

        }

        //-- Destroy window
        SDL_DestroyWindow( window );

        //Quit SDL subsystems
        SDL_Quit();
    }

    //--
    TTF_Quit();

    return 0;
}