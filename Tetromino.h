#pragma once
#include <SDL.h>
#include <array>

using namespace std;

typedef struct Vector2i
{
    int x;
    int y;
} Vector2i;

class Tetromino{
public:
    Tetromino();
    Tetromino(int type,int x,int y);

    int m_type;
    int m_x;
    int m_y;
    array<Vector2i,4> m_v;
    SDL_Color m_color;
    
    static SDL_Color tetrisColors[8];

    void RotateLeft();
    void RotateRight();
    void Draw(SDL_Renderer *renderer);
    int  MinX();
    int  MaxX();
    int  MaxY();
    int  Column();
    bool IsOutLeftLimit();
    bool IsOutRightLimit();
    bool IsOutLRLimit(int veloH);
    bool IsOutBottomLimit();
    bool HitGround(int *board);

};