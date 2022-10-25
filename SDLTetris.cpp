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

typedef struct HighScore{
    std::string name;
    int         score;
}HighScore;

typedef enum {
    STAND_BY,
    PLAY,
    GAME_OVER,
    HIGH_SCORES
}GameMode;

Tetromino *curTetromino=NULL;
Tetromino *nextTetromino=NULL;
int         board[NB_COLUMNS*NB_ROWS];
GameMode    curMode = GameMode::STAND_BY;
bool        fFastDown = false;
bool        fQuitGame = false;
bool        fDrop = false;
int         velocityX = 0;
bool        fPause = false;
int         nbCompletedLine = 0;
int         horizontalMove = 0;
int         horizontalStartColumn = 0;
int         curScore = 0;
int         idHighScore = -1;
int         iHighScoreColor = 0;

std::string playerName("");
HighScore   highScores[] = {
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

int         tetrisBag[] = {1,2,3,4,5,6,7,1,2,3,4,5,6,7};
int         idTetrisBag = 14;
std::map<int,char>  keyMaps = {
                    {SDLK_a,'A'},
                    {SDLK_b,'B'},
                    {SDLK_c,'C'},
                    {SDLK_d,'D'},
                    {SDLK_e,'E'},
                    {SDLK_f,'F'},
                    {SDLK_i,'I'},
                    {SDLK_j,'J'},
                    {SDLK_k,'K'},
                    {SDLK_l,'L'},
                    {SDLK_m,'M'},
                    {SDLK_n,'N'},
                    {SDLK_o,'O'},
                    {SDLK_p,'P'},
                    {SDLK_q,'Q'},
                    {SDLK_r,'R'},
                    {SDLK_s,'S'},
                    {SDLK_t,'T'},
                    {SDLK_u,'U'},
                    {SDLK_v,'V'},
                    {SDLK_w,'W'},
                    {SDLK_x,'X'},
                    {SDLK_y,'Y'},
                    {SDLK_z,'Z'},
                    {SDLK_0,'0'},
                    {SDLK_1,'1'},
                    {SDLK_2,'2'},
                    {SDLK_3,'3'},
                    {SDLK_4,'4'},
                    {SDLK_5,'5'},
                    {SDLK_6,'6'},
                    {SDLK_7,'7'},
                    {SDLK_8,'8'},
                    {SDLK_9,'9'},
                    {SDLK_KP_0,'0'},
                    {SDLK_KP_1,'1'},
                    {SDLK_KP_2,'2'},
                    {SDLK_KP_3,'3'},
                    {SDLK_KP_4,'4'},
                    {SDLK_KP_5,'5'},
                    {SDLK_KP_6,'6'},
                    {SDLK_KP_7,'7'},
                    {SDLK_KP_8,'8'},
                    {SDLK_KP_9,'9'}
                    };

bool        (*ProcessEvent) (SDL_Event &e);

//Globally used font
TTF_Font* gFont = NULL;

int RandomInt(int a, int b) 
{
	int diff = b - a + 1;
	int r = rand() % diff;
	return a + r;
}

int TetrisRandomizer(){
    int iSrc;
    int ityp;
    //-----------------------------------------------
    if (idTetrisBag<14){
        ityp = tetrisBag[idTetrisBag];
        idTetrisBag++;
    }else{
        //-- Shuttle
        for (int i=0;i<14;i++){
            iSrc = RandomInt(0, 13);
            ityp = tetrisBag[iSrc];
            tetrisBag[iSrc] = tetrisBag[0];
            tetrisBag[0] = ityp;
        }
        ityp = tetrisBag[0];
        idTetrisBag = 1;
    }
    return ityp;
}

void InitGame(){
    curScore = 0;
    for (int i=0;i<NB_COLUMNS*NB_ROWS;i++){
        board[i] = 0;
    }
}

void NewTetromino(){
    if (curTetromino!=NULL){
        delete curTetromino;
    }
    curTetromino = nextTetromino;
    curTetromino->m_x = 6*CELL_SIZE;
    curTetromino->m_y = 0;
    curTetromino->m_y = -curTetromino->MaxY()*CELL_SIZE;
    nextTetromino = new Tetromino(TetrisRandomizer() ,(NB_COLUMNS+3)*CELL_SIZE, 10*CELL_SIZE);

}

int ComputeCompletedLines(){
    int nbLines = 0;
    bool fCompled = false;
    //----------------------------------------------
    for (int r=0;r<NB_ROWS;r++){
        fCompled = true;
        for (int c=0;c<NB_COLUMNS;c++){
            if (board[r*NB_COLUMNS+c]==0){
                fCompled = false;
                break;
            }
        }
        if (fCompled){
            nbLines++;
        }
    }
    return nbLines;
}

int ComputeScore(int nbLines){
    int score = 0;
    switch (nbLines)
    {
    case 0:
        score = 0;
        break;
    case 1:
        score = 40;
        break;
    case 2:
        score = 100;
        break;
    case 3:
        score = 300;
        break;
    case 4:
        score = 1200;
        break;
    default:
        score = 2000;
        break;
    }
    return score;

}

int IsHighScore(int score){
    //-----------------------------------------------
    for (int i=0;i<10;i++){
        if (score>=highScores[i].score){
            return i;
        }
    }
    return -1;
}

void InsertHightScore(int id,std::string playerName,int playerScore){
    //-----------------------------------------------
    if ((id>=0)&&(id<10)){
        if (id==9){
            highScores[9].name = playerName;
            highScores[9].score = playerScore;
        }else{
            //-- Shift Down Array
            for (int i=9;i>id;i--){
                highScores[i] = highScores[i-1];
            }
            highScores[id] = {playerName,playerScore};
        }
    }
}

void SaveHighScores(){

    std::filebuf fb;
    if (fb.open ("../highscores.txt",std::ios::out)){
        std::ostream os(&fb);
        for (int i=0;i<10;i++){
            os << highScores[i].name << "," << highScores[i].score << std::endl;
        }
        fb.close();
    }

}

void LoadHighScores(){
    int         i = 0;
    std::string strWord;
    std::filebuf fb;
    std::string pathName("../highscores.txt");
    if (std::filesystem::exists(pathName)) {
        if (fb.open (pathName,std::ios::in)){
            std::istream fs(&fb);
            for (std::string line;std::getline(fs,line,'\n');){
                std::stringstream ss(line);
                if (i<10){
                    if (std::getline(ss,strWord,',')){
                        highScores[i].name = strWord;
                    }
                    if (std::getline(ss,strWord,',')){
                        highScores[i].score = std::atoi(strWord.c_str());
                    }
                    i++;
                }else{
                    break;
                }
            }
            fb.close();
        }
    }
}

void FreezeCurTetromino(){
    if (curTetromino!=NULL){
        int x,y;
        int ix = (int) ((curTetromino->m_x + 1)/CELL_SIZE);
        int iy = (int) ((curTetromino->m_y + 1)/CELL_SIZE);
        for (auto p : curTetromino->m_v){
            x = p.x + ix;
            y = p.y + iy;
            if ((x>=0)&&(x<NB_COLUMNS)&&(y>=0)&&(y<NB_ROWS)){
                board[y*NB_COLUMNS+x] = curTetromino->m_type;
            }
        }
        nbCompletedLine = ComputeCompletedLines();
        if (nbCompletedLine>0){
            curScore += ComputeScore(nbCompletedLine);
        }

    }

}

void EraseFirstCompledLine(){
    bool fCompled = false;
    //----------------------------------------------
    for (int r=0;r<NB_ROWS;r++){
        fCompled = true;
        for (int c=0;c<NB_COLUMNS;c++){
            if (board[r*NB_COLUMNS+c]==0){
                fCompled = false;
                break;
            }
        }
        if (fCompled){
            //-- Shift Down
            for (int r1=r;r1>0;r1--){
                int rSrc = (r1-1)*NB_COLUMNS;
                int rDes = r1*NB_COLUMNS;
                for (int c1=0;c1<NB_COLUMNS;c1++){
                    board[rDes+c1] = board[rSrc+c1];
                }
            }
            return;
        }
    }

}

bool IsGameOver(){
    //----------------------------------------------
    for (int c=0;c<NB_COLUMNS;c++){
        if (board[c]!=0){
            return true;
        }
    }
    return false;
}

void DrawBoard(SDL_Renderer *renderer){

    SDL_Color   col;
    SDL_Rect    rect;
    int         typ;
    int         a;
    int         x,y;
    //----------------------------------------------
    a = CELL_SIZE - 2;
    for (int r=0;r<NB_ROWS;r++){
        for (int c=0;c<NB_COLUMNS;c++){
            typ = board[r*NB_COLUMNS+c];
            if (typ!=0){
                x = c*CELL_SIZE + LEFT + 1;
                y = r*CELL_SIZE + TOP + 1;
                rect = {x,y,a,a};
                col = Tetromino::tetrisColors[typ];
                SDL_SetRenderDrawColor(renderer,col.r,col.g,col.b,col.a);
                SDL_RenderFillRect(renderer,&rect);
            }
        }

    }

}

bool ProcessPlayEvent(SDL_Event &e){
    
    if( e.type == SDL_QUIT )
    {
        fQuitGame = true;
        return true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            fQuitGame = false;
            return true;
            break;
        case SDLK_SPACE:
            fDrop = true;
            break;
        case SDLK_UP:
            //-- Rotate Tetromino
            if (curTetromino!=NULL){
                curTetromino->RotateLeft();
                if (curTetromino->HitGround( board)){
                    //-- Undo Rotate
                    curTetromino->RotateRight();
                }else if (curTetromino->IsOutRightLimit()){
                    int backupX = curTetromino->m_x;
                    //-- Move Tetromino inside board
                    do{
                        curTetromino->m_x--;
                    }while(curTetromino->IsOutRightLimit());
                    if (curTetromino->HitGround( board)){
                        //-- Undo Move
                        curTetromino->m_x = backupX;
                        //-- Undo Rotate
                        curTetromino->RotateRight();
                    }
                }else if (curTetromino->IsOutLeftLimit()){
                    int backupX = curTetromino->m_x;
                    //-- Move Tetromino inside board
                    do{
                        curTetromino->m_x++;
                    }while(curTetromino->IsOutLeftLimit());
                    if (curTetromino->HitGround(board)){
                        //-- Undo Move
                        curTetromino->m_x = backupX;
                        //-- Undo Rotate
                        curTetromino->RotateRight();
                    }
                }
            }
            break;
        case SDLK_DOWN:
            fFastDown = true;
            break;
        case SDLK_LEFT:
            velocityX = -1;
            break;
        case SDLK_RIGHT:
            velocityX = 1;
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

bool ProcessStandByEvent(SDL_Event &e){
    

    if( e.type == SDL_QUIT )
    {
        fQuitGame = true;
        return true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            fQuitGame = true;
            return true;
            break;
        case SDLK_SPACE:
            //-- Start Game
            curMode = GameMode::PLAY;
            ProcessEvent = &ProcessPlayEvent;
            InitGame();
            NewTetromino();
            break;
        }
    }


    return false;
}

bool ProcessHightScoresEvent(SDL_Event &e){
    
    if( e.type == SDL_QUIT )
    {
        fQuitGame = true;
        return true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            ProcessEvent = &ProcessStandByEvent;
            curMode = GameMode::STAND_BY;
            return false;
            break;
        case SDLK_BACKSPACE:
            {
                int l = playerName.length();
                if (l>0){
                    playerName = playerName.substr(0,l-1);
                    highScores[idHighScore].name = playerName;
                }
                break;
            }
        case SDLK_KP_ENTER:
        case SDLK_RETURN:
            if (playerName.length()==0){
                highScores[idHighScore].name = "XXXXXX";
            }else{
                highScores[idHighScore].name = playerName;
            }
            SaveHighScores();
            ProcessEvent = &ProcessStandByEvent;
            curMode = GameMode::STAND_BY;
            return false;
            break;
        default:
            if (keyMaps.find((int)(e.key.keysym.sym)) != keyMaps.end()){
                char c = keyMaps[(int)(e.key.keysym.sym)];
                playerName += c;
                highScores[idHighScore].name = playerName;
            }
            break;
        }
    }

    return false;
}

bool ProcessGameOverEvent(SDL_Event &e){
    
    if( e.type == SDL_QUIT )
    {
        fQuitGame = true;
        return true;
    }
    else if( e.type == SDL_KEYDOWN )
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_ESCAPE:
        case SDLK_SPACE:
            fQuitGame = false;
            curMode = GameMode::STAND_BY;
            ProcessEvent = &ProcessStandByEvent;
            break;
        }
    }

    return false;
}

void DrawScore(SDL_Renderer *renderer){

    SDL_Color col = {255,255,0,255};

    std::ostringstream  ss;
    ss << "SCORE : " << std::setfill('0') << std::setw(6) << curScore;
    std::string strSCore = ss.str();

    auto surfScore = TTF_RenderUTF8_Blended(gFont,strSCore.c_str(), col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

        if (texScore!=NULL){
            int w,h;
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                SDL_Rect rectDest={LEFT,(NB_ROWS+1)*CELL_SIZE,w,h};
                SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }

}

void DrawHighScores(SDL_Renderer *renderer){
    int         x,y;
    int         w=0,h=0;
    SDL_Color   col= {255,255,0,255};
    //---------------------------------------------
    y = TOP + CELL_SIZE;
    //-- Draw Title
    SDL_Surface *surfScore = TTF_RenderUTF8_Blended(gFont,"HIGH SCORES", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
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

        if (i==idHighScore){
            if (iHighScoreColor % 2){
                col = {100,100,0,255};
            }else{
                col = {255,255,0,255};
            }
        }else{
            col = {255,255,0,255};
        }

        SDL_Surface *surfScore = TTF_RenderUTF8_Blended(gFont,highScores[i].name.c_str(), col);
        if (surfScore!=NULL){

            SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

            if (texScore!=NULL){
                int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
                if (err==0){
                    SDL_Rect rectDest={xCol0,y,w,h};
                    SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
                }
                //--
                SDL_DestroyTexture(texScore);
            }
            //--
            SDL_free(surfScore);
        }

        std::ostringstream  ss;
        ss << std::setfill('0') << std::setw(6) << highScores[i].score;
        std::string strSCore = ss.str();

        surfScore = TTF_RenderUTF8_Blended(gFont,strSCore.c_str(), col);
        if (surfScore!=NULL){

            SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

            if (texScore!=NULL){
                int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
                if (err==0){
                    SDL_Rect rectDest={xCol1,y,w,h};
                    SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
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

void DrawStandBy(SDL_Renderer *renderer){
    int         x,y;
    int         w=0,h=0;
    SDL_Color   col= {255,255,0,255};
    //---------------------------------------------
    y = TOP + (NB_ROWS/4)*CELL_SIZE;
    //-- Draw Title
    SDL_Surface *surfScore = TTF_RenderUTF8_Blended(gFont,"C++ Tetris using SDL2", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
                y += 2*h + 8;

            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }

    surfScore = TTF_RenderUTF8_Blended(gFont,"Press SPACE to Play", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
                //y += 2*h + 8;

            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }
    

}

void DrawGameOver(SDL_Renderer *renderer){

    int         x,y;
    int         w=0,h=0;
    SDL_Color   col= {255,255,0,255};
    //---------------------------------------------
    y = TOP + (NB_ROWS/4)*CELL_SIZE;
    //-- Draw Title
    SDL_Surface *surfScore = TTF_RenderUTF8_Blended(gFont,"GAME OVER", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
                y += 2*h + 8;

            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }

    surfScore = TTF_RenderUTF8_Blended(gFont,"Press SPACE to Continue", col);
    if (surfScore!=NULL){

        SDL_Texture *texScore = SDL_CreateTextureFromSurface( renderer, surfScore);

        if (texScore!=NULL){
            int err = SDL_QueryTexture(texScore,NULL,NULL,&w,&h);
            if (err==0){
                x = LEFT + (NB_COLUMNS/2)*CELL_SIZE - (w/2); 
                SDL_Rect rectDest={x,y,w,h};
                SDL_RenderCopy(renderer,texScore,NULL,&rectDest);
                //y += 2*h + 8;

            }
            //--
            SDL_DestroyTexture(texScore);
        }
        //--
        SDL_free(surfScore);
    }
    

}

int main(int argc, char *argv[])
{
    //std::cout << "Hello World !!" << SDL_GetError() << std::endl;

    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window

    SDL_Renderer *renderer=NULL;

    //std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    TTF_Init();
    //Open the font
    gFont = TTF_OpenFont( "../resources/sansation.ttf", 18 );
    if ( gFont == NULL ) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        TTF_Quit();
        return 0;
    }
    TTF_SetFontStyle(gFont,TTF_STYLE_ITALIC|TTF_STYLE_BOLD);

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {

        //--
            
        //Create window
        window = SDL_CreateWindow( "C++ SDL Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {

            LoadHighScores();

            Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
            auto tetrisMusic = Mix_LoadMUS("../resources/Tetris.wav");

            auto succesSound = Mix_LoadWAV("../resources/109662__grunz__success.wav");
            if (succesSound!=NULL){
                Mix_VolumeChunk(succesSound,15);
            }

            //Get window surface
            //screenSurface = SDL_GetWindowSurface( window );

            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

            InitGame();
            ProcessEvent = &ProcessStandByEvent;

            curMode = GameMode::STAND_BY;
            nextTetromino = new Tetromino(TetrisRandomizer(),(NB_COLUMNS+3)*CELL_SIZE, 10*CELL_SIZE);

            if (tetrisMusic!=NULL){
                Mix_PlayMusic(tetrisMusic,-1);
                Mix_VolumeMusic(20);
            }

            Uint32 startTimeV = SDL_GetTicks();
            Uint32 startTimeH = startTimeV;
            Uint32 startTimeR = startTimeV;

            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while( !quit )
            {

                //--
                SDL_SetRenderDrawColor(renderer, 48, 48, 255, 255);
                SDL_RenderClear(renderer);

                //--
                SDL_Rect rect = {LEFT,TOP,CELL_SIZE*NB_COLUMNS,CELL_SIZE*NB_ROWS};
                SDL_SetRenderDrawColor(renderer, 10, 10, 100, 255);
                SDL_RenderFillRect(renderer,&rect);

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    quit = ProcessEvent(e);

                    if (fQuitGame){
                        break;
                    }

                    if (quit){
                        quit = false;

                        //-- Check Hight Scores
                        curTetromino->m_type = 0;
                        if ((curScore)&&((idHighScore = IsHighScore(curScore))>=0)){
                            InsertHightScore(idHighScore,playerName,curScore);
                            InitGame();
                            ProcessEvent = &ProcessHightScoresEvent;
                            curMode = GameMode::HIGH_SCORES;
                       }else{
                            InitGame();
                            ProcessEvent = &ProcessStandByEvent;
                            curMode = GameMode::STAND_BY;
                        }

                    }

                }

                if (curMode==GameMode::PLAY){

                    if ((curTetromino!=NULL)&&(!fPause)){

                        int curTime = SDL_GetTicks();

                        if (nbCompletedLine>0){
                            if ((curTime-startTimeV)>250){
                                startTimeV = curTime;
                                nbCompletedLine--;
                                EraseFirstCompledLine();
                                //-- Play sound
                                if (succesSound!=NULL){
                                    Mix_PlayChannel(-1, succesSound, 0);
                                }
                            }

                        }else if (horizontalMove!=0){

                            if ((curTime-startTimeH)>20){
                                startTimeH = curTime;

                                for(int i=0;i<4;i++){
                                    int backupX = curTetromino->m_x;
                                    curTetromino->m_x += horizontalMove;

                                    if (curTetromino->IsOutLRLimit(horizontalMove)){
                                        curTetromino->m_x = backupX;
                                        horizontalMove = 0;
                                        break;
                                    }else if (curTetromino->HitGround(board)){
                                        curTetromino->m_x = backupX;
                                        horizontalMove = 0;
                                        break;
                                    }

                                    if (horizontalMove!=0){
                                        if (horizontalStartColumn!=curTetromino->Column()){
                                            curTetromino->m_x = backupX;
                                            horizontalMove = 0;
                                            startTimeH = SDL_GetTicks();
                                            break;
                                        }
                                    }

                                }

                            }

                        }else if (fDrop){
                            int curTime = SDL_GetTicks();
                            if ((curTime-startTimeV)>10){
                                startTimeV = curTime;
                                for (int i=0;i<6;i++){
                                    //-- Move down an check
                                    curTetromino->m_y++;
                                    if (curTetromino->HitGround( board)){
                                        curTetromino->m_y--;
                                        FreezeCurTetromino();
                                        NewTetromino();
                                        fDrop = false;
                                    }else if (curTetromino->IsOutBottomLimit()){
                                        curTetromino->m_y--;
                                        FreezeCurTetromino();
                                        NewTetromino();
                                        fDrop = false;
                                    }
                                    if (fDrop){
                                        if (velocityX!=0){

                                            if ((curTime-startTimeH)>20){
                                                int backupX = curTetromino->m_x;
                                                curTetromino->m_x += velocityX;

                                                if (curTetromino->IsOutLRLimit(velocityX)){
                                                    curTetromino->m_x = backupX;
                                                }else if (curTetromino->HitGround(board)){
                                                    curTetromino->m_x = backupX;
                                                }else{
                                                    startTimeH = curTime;
                                                    horizontalMove = velocityX;
                                                    horizontalStartColumn = curTetromino->Column();
                                                    break;
                                                }
                                            }

                                        }
                                    }
                                }
                            }

                        }else{

                            int curTime = SDL_GetTicks();
                            int limitElapse = (fFastDown?10:25);
                            if ((curTime-startTimeV)>limitElapse){
                                startTimeV = curTime;

                                for (int i=0;i<3;i++){
                                    curTetromino->m_y += 1;
                                    bool fMove =true;
                                    if (curTetromino->HitGround( board)){
                                        curTetromino->m_y -= 1;
                                        FreezeCurTetromino();
                                        NewTetromino();
                                        fMove = false;
                                    }else if (curTetromino->IsOutBottomLimit()){
                                        curTetromino->m_y -= 1;
                                        FreezeCurTetromino();
                                        NewTetromino();
                                        fMove = false;                    
                                    }

                                    if (fMove){
                                        if (velocityX!=0){

                                            curTime = SDL_GetTicks();

                                            if ((curTime-startTimeH)>15){

                                                int backupX = curTetromino->m_x;
                                                curTetromino->m_x += velocityX;

                                                if (curTetromino->IsOutLRLimit(velocityX)){
                                                    curTetromino->m_x = backupX;
                                                }else if (curTetromino->HitGround( board)){
                                                    curTetromino->m_x = backupX;
                                                }else{
                                                    horizontalMove = velocityX;
                                                    horizontalStartColumn = curTetromino->Column();
                                                    startTimeH = curTime;
                                                    break;
                                                }

                                            }
                                        }
                                    }
                                }

                                //-- Check Game Over
                                if (IsGameOver()){

                                    curTetromino->m_type = 0;
                                    if ((curScore)&&((idHighScore = IsHighScore(curScore))>=0)){
                                        InsertHightScore(idHighScore,playerName,curScore);
                                        InitGame();
                                        ProcessEvent = &ProcessHightScoresEvent;
                                        curMode = GameMode::HIGH_SCORES;
                                    }else{
                                        InitGame();
                                        ProcessEvent = &ProcessGameOverEvent;
                                        curMode = GameMode::GAME_OVER;
                                    }

                                }

                            }


                        }

                        //-- Rotate NextTetromino
                        if ((curTime-startTimeR)>500){
                            startTimeR = curTime;
                            nextTetromino->RotateRight();
                        }

                    }

                }else if (curMode==GameMode::HIGH_SCORES){
                    int curTime = SDL_GetTicks();
                    if ((curTime-startTimeV)>500){
                        startTimeV = curTime;
                        iHighScoreColor++;
                    }

                }

                DrawBoard(renderer);

                if (curTetromino){
                    curTetromino->Draw(renderer);
                }

                if (nextTetromino){
                    nextTetromino->Draw(renderer);
                }

                if (curMode==GameMode::STAND_BY){
                    DrawStandBy(renderer);
                }else if (curMode==GameMode::HIGH_SCORES){
                    DrawHighScores(renderer);
                }else if (curMode==GameMode::GAME_OVER){
                    DrawGameOver(renderer);
                }


                //--
                DrawScore(renderer);

                //--
                SDL_RenderPresent(renderer);

            }

            //Fill the surface white
            //SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
            //Update the surface
            //SDL_UpdateWindowSurface( window );

            if (tetrisMusic!=NULL){
                Mix_FreeMusic(tetrisMusic);
            }

            
            if (succesSound){
                Mix_FreeChunk(succesSound);
            }

            Mix_CloseAudio();
        }

        //-- Renderer
        if (renderer){
        SDL_DestroyRenderer(renderer);
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