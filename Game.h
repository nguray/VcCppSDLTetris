#pragma once
#include "IGameMode.h"
#include "StandByMode.h"
#include "PlayMode.h"
#include "HighScoresMode.h"
#include "GameOverMode.h"
#include "Tetromino.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include "TetrisConst.h"

typedef struct HighScore{

    std::string name;
    int         score;
}HighScore;


class Game
{
public:

    StandByMode     *standByMode;
    PlayMode        *playMode;
    HighScoresMode  *highScoresMode;
    GameOverMode    *gameOverMode;

    IGameMode   *curGameMode;
    Tetromino   *curTetromino = NULL;
    Tetromino   *nextTetromino = NULL;

    TTF_Font    *gFont = NULL;
    Mix_Chunk   *succesSound = NULL;
    Mix_Music   *tetrisMusic = NULL;

    SDL_Renderer    *renderer=NULL;

    int         curScore = 0;
    int         idHighScore = -1;
    int         iHighScoreColor = 0;
    int         startTimeR = 0;

    std::string playerName = "";

    HighScore   highScores[10] = {
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0},
                    {"XXXXXX",0}
                };

    int         tetrisBag[14] = {1,2,3,4,5,6,7,1,2,3,4,5,6,7};
    int         idTetrisBag = 14;

    int         board[NB_COLUMNS*NB_ROWS];

    Game(SDL_Window *window);
    ~Game();

    int TetrisRandomizer();
    void InitGame();
    void NewTetromino();
    int ComputeCompletedLines();
    int IsHighScore(int score);
    void InsertHightScore(int id,std::string playerName,int playerScore);
    void SaveHighScores();
    void LoadHighScores();
    int FreezeCurTetromino();
    void EraseFirstCompledLine();
    bool IsGameOver();
    void DrawBoard(SDL_Renderer *renderer);

    void SetStandByMode();
    void SetPlayMode();
    void SetHighScoresMode();
    void SetGameOverMode();
    void DrawScore();
    void Draw();
    void Update();
    void CheckHighScore();

};