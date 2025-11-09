#include "Game.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

Game::Game(SDL_Window *window)
{
    //Load font
    gFont = TTF_OpenFont( "../resources/sansation.ttf", 18 );
    if ( gFont == NULL ) {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        TTF_Quit();
    }
    TTF_SetFontStyle(gFont,TTF_STYLE_ITALIC|TTF_STYLE_BOLD);

    LoadHighScores();

    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,1024);
    tetrisMusic = Mix_LoadMUS("../resources/Tetris.wav");
    if (tetrisMusic!=NULL){
        Mix_PlayMusic(tetrisMusic,-1);
        Mix_VolumeMusic(20);
    }

    succesSound = Mix_LoadWAV("../resources/109662__grunz__success.wav");
    if (succesSound!=NULL){
        Mix_VolumeChunk(succesSound,15);
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    standByMode = new StandByMode(this);
    playMode = new PlayMode(this);
    highScoresMode = new HighScoresMode(this);
    gameOverMode = new GameOverMode(this);
    curGameMode = standByMode;

    InitGame();
}

Game::~Game()
{
    if (tetrisMusic!=NULL){
        Mix_FreeMusic(tetrisMusic);
    }

    
    if (succesSound){
        Mix_FreeChunk(succesSound);
    }

    Mix_CloseAudio();

    //-- Renderer
    if (renderer){
        SDL_DestroyRenderer(renderer);
    }


}


int RandomInt(int a, int b) 
{
	int diff = b - a + 1;
	int r = rand() % diff;
	return a + r;
}

int Game::TetrisRandomizer(){
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

void Game::InitGame(){
    curScore = 0;
    for (int i=0;i<NB_COLUMNS*NB_ROWS;i++){
        board[i] = 0;
    }
}

void Game::NewTetromino(){

    if (nextTetromino==NULL){
        nextTetromino = new Tetromino(TetrisRandomizer() ,(NB_COLUMNS+3)*CELL_SIZE, 10*CELL_SIZE);
    }

    if (curTetromino!=NULL){
        delete curTetromino;
    }
    curTetromino = nextTetromino;
    curTetromino->m_x = 6*CELL_SIZE;
    curTetromino->m_y = 0;
    curTetromino->m_y = -curTetromino->MaxY()*CELL_SIZE;

    nextTetromino = new Tetromino(TetrisRandomizer() ,(NB_COLUMNS+3)*CELL_SIZE, 10*CELL_SIZE);

}

int Game::ComputeCompletedLines(){
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

int Game::IsHighScore(int score){
    //-----------------------------------------------
    for (int i=0;i<10;i++){
        if (score>=highScores[i].score){
            return i;
        }
    }
    return -1;
}

void Game::InsertHightScore(int id,std::string playerName,int playerScore){
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

void Game::SaveHighScores(){

    std::filebuf fb;
    if (fb.open ("../highscores.txt",std::ios::out)){
        std::ostream os(&fb);
        for (int i=0;i<10;i++){
            os << highScores[i].name << "," << highScores[i].score << std::endl;
        }
        fb.close();
    }

}

void Game::LoadHighScores(){
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

int Game::FreezeCurTetromino(){
    int nbCompletedLine = 0;
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
    return nbCompletedLine;

}

void Game::EraseFirstCompledLine(){
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

bool Game::IsGameOver(){
    //----------------------------------------------
    for (int c=0;c<NB_COLUMNS;c++){
        if (board[c]!=0){
            return true;
        }
    }
    return false;
}

void Game::DrawBoard(SDL_Renderer *renderer){

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

void Game::DrawScore()
{

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



void Game::SetStandByMode()
{
    curGameMode = standByMode;
    curGameMode->Init();
}

void Game::SetPlayMode()
{
    curGameMode = playMode;
    curGameMode->Init();
}

void Game::SetHighScoresMode()
{
    curGameMode = highScoresMode;
    curGameMode->Init();
}

void Game::SetGameOverMode()
{
    curGameMode = gameOverMode;
    curGameMode->Init();
}

void Game::Draw()
{

    //--
    SDL_SetRenderDrawColor(renderer, 48, 48, 255, 255);
    SDL_RenderClear(renderer);

    //--
    SDL_Rect rect = {LEFT,TOP,CELL_SIZE*NB_COLUMNS,CELL_SIZE*NB_ROWS};
    SDL_SetRenderDrawColor(renderer, 10, 10, 100, 255);
    SDL_RenderFillRect(renderer,&rect);
    
    DrawBoard(renderer);

    curGameMode->Draw();

    if (nextTetromino){
        nextTetromino->Draw(renderer);
    }

    //--
    DrawScore();

    //--
    SDL_RenderPresent(renderer);

}

void Game::Update()
{
    if (nextTetromino){
        int curTime = SDL_GetTicks();
        //-- Rotate NextTetromino
        if ((curTime-startTimeR)>500){
            startTimeR = curTime;
            nextTetromino->RotateRight();
        }
    }

    curGameMode->Update();

    //-- Check Game Over
    if (IsGameOver()){
        if ((curScore)&&((idHighScore = IsHighScore(curScore))>=0)){
            InsertHightScore(idHighScore,playerName,curScore);
            InitGame();
            SetHighScoresMode();
        }else{
            SetGameOverMode();
        }

    }

}

void Game::CheckHighScore()
{
    //-- Check Hight Scores
    if ((curScore)&&((idHighScore = IsHighScore(curScore))>=0)){
        InsertHightScore(idHighScore,playerName,curScore);
        InitGame();
        SetHighScoresMode();
    }else{
        InitGame();
        SetStandByMode();
    }

}