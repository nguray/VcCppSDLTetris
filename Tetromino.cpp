
#include "Tetromino.h"
#include "TetrisConst.h"

SDL_Color Tetromino::tetrisColors[8] = {
    {0x00,0x00,0x00,0xFF},
    {0xFF,0x60,0x60,0xFF},
    {0x60,0xFF,0x60,0xFF},
    {0x60,0x60,0xFF,0xFF},
    {0xCC,0xCC,0x60,0xFF},
    {0xCC,0x60,0xCC,0xFF},
    {0x60,0xCC,0xCC,0xFF},
    {0xDA,0xAA,0x00,0xFF}
};

Tetromino::Tetromino():
    m_type(0),
    m_x(0),
    m_y(0)
{

}

Tetromino::Tetromino(int typ,int x,int y)
{
    m_type = typ;
    m_x = x;
    m_y = y;
    switch (m_type)
    {
    case 0:
        m_v[0] = {0,0};
        m_v[1] = {0,0};
        m_v[2] = {0,0};
        m_v[3] = {0,0};
        break;
    case 1:
        m_v[0]= {0,-1};
        m_v[1]= {0,0};
        m_v[2]= {-1,0};
        m_v[3]= {-1,1};
        break;
    case 2:
        m_v[0] = {0,-1};
        m_v[1] = {0,0};
        m_v[2] = {1,0};
        m_v[3] = {1,1};
        break;
    case 3:
        m_v[0] = {0,-1};
        m_v[1] = {0,0};
        m_v[2] = {0,1};
        m_v[3] = {0,2};
        break;
    case 4:
        m_v[0] = {-1,0};
        m_v[1] = {0,0};
        m_v[2] = {1,0};
        m_v[3] = {0,1};
        break;
    case 5:
        m_v[0] = {0,0};
        m_v[1] = {1,0};
        m_v[2] = {0,1};
        m_v[3] = {1,1};
        break;
    case 6:
        m_v[0] = {-1,-1};
        m_v[1] = {0,-1};
        m_v[2] = {0,0};
        m_v[3] = {0,1};
        break;
    case 7:
        m_v[0] = {1,-1};
        m_v[1] = {0,-1};
        m_v[2] = {0,0};
        m_v[3] = {0,1};
        break;
    default:
        m_v[0]= {0,0};
        m_v[1]= {0,0};
        m_v[2]= {0,0};
        m_v[3]= {0,0};
        break;
    }
    m_color = tetrisColors[m_type];
}

void Tetromino::RotateLeft()
{
    int x,y;
    //-----------------------------------------
    if (m_type!=5){
        for (auto i=0UL;i<m_v.size();i++){
            x = m_v[i].y;
            y = -m_v[i].x;
            m_v[i].x = x;
            m_v[i].y = y;
        }
    }

}

void Tetromino::RotateRight()
{
    int x,y;
    //-----------------------------------------
    if (m_type!=5){
        for (auto i=0UL;i<m_v.size();i++){
            x = -m_v[i].y;
            y = m_v[i].x;
            m_v[i].x = x;
            m_v[i].y = y;
        }
    }

}

void Tetromino::Draw(SDL_Renderer *renderer)
{
    int         cx,cy,a;
    SDL_Rect    rect;
    //-----------------------------------------
    if (m_type){
        
        SDL_SetRenderDrawColor(renderer,m_color.r,m_color.g,m_color.b,m_color.a);
        a = CELL_SIZE - 2;

        for (auto p : m_v){
            cx = p.x*CELL_SIZE + m_x + LEFT + 1;
            cy = p.y*CELL_SIZE + m_y + TOP + 1;
            if (cy>=TOP){
                rect = {cx,cy,a,a};
                SDL_RenderFillRect(renderer,&rect);
            }
        }
    }

}

int Tetromino::MinX()
{
    int x,minX;
    //-----------------------------------------
    minX = m_v[0].x;
    for (auto i=1UL;i<m_v.size();i++){
        x = m_v[i].x;
        if (x<minX){
            minX = x;
        }
    }
    return minX;
}

int Tetromino::MaxX()
{
    int x,maxX;
    //-----------------------------------------
    maxX = m_v[0].x;
    for (auto i=1UL;i<m_v.size();i++){
        x = m_v[i].x;
        if (x>maxX){
            maxX = x;
        }
    }
    return maxX;
}

int Tetromino::MaxY()
{
    int y,maxY;
    //-----------------------------------------
    maxY = m_v[0].y;
    for (auto i=1UL;i<m_v.size();i++){
        y = m_v[i].y;
        if (y>maxY){
            maxY = y;
        }
    }
    return maxY;
}

int Tetromino::Column()
{
    //-----------------------------------------
    return (m_x/CELL_SIZE);
}

bool Tetromino::IsOutLeftLimit(){
    //-----------------------------------------
    return (MinX()*CELL_SIZE+m_x)<0;

}

bool Tetromino::IsOutRightLimit(){
    //-----------------------------------------
    return ((MaxX() + 1)*CELL_SIZE + m_x)>(CELL_SIZE*NB_COLUMNS);

}

bool Tetromino::IsOutBottomLimit(){
    //-----------------------------------------
    return ((MaxY()+1)*CELL_SIZE + m_y)>(CELL_SIZE*NB_ROWS);
}

bool Tetromino::HitGround(int *board){
    int x,y;
    int ix,iy;
    //-----------------------------------------
    for (auto p : m_v){

        //-- Top Left Corner
        x = p.x*CELL_SIZE + m_x + 1;
        y = p.y*CELL_SIZE + m_y + 1;
        ix = (int)(x/CELL_SIZE);
        iy = (int)(y/CELL_SIZE);
        if ((ix>=0)&&(ix<NB_COLUMNS)&&(iy>=0)&&(iy<NB_ROWS)){
            if (board[iy*NB_COLUMNS+ix]!=0){
                return true;
            }
        }

        //-- Top Right Corner
        x = p.x*CELL_SIZE + CELL_SIZE - 1 + m_x;
        y = p.y*CELL_SIZE + m_y + 1;
        ix = (int)(x/CELL_SIZE);
        iy = (int)(y/CELL_SIZE);
        if ((ix>=0)&&(ix<NB_COLUMNS)&&(iy>=0)&&(iy<NB_ROWS)){
            if (board[iy*NB_COLUMNS+ix]!=0){
                return true;
            }
        }

        //-- Bottom Right Corner
        x = p.x*CELL_SIZE + CELL_SIZE - 1 + m_x;
        y = p.y*CELL_SIZE + CELL_SIZE - 1 + m_y;
        ix = (int)(x/CELL_SIZE);
        iy = (int)(y/CELL_SIZE);
        if ((ix>=0)&&(ix<NB_COLUMNS)&&(iy>=0)&&(iy<NB_ROWS)){
            if (board[iy*NB_COLUMNS+ix]!=0){
                return true;
            }
        }

        //-- Bottom Left Corner
        x = p.x*CELL_SIZE + m_x + 1;
        y = p.y*CELL_SIZE + CELL_SIZE - 1 + m_y;
        ix = (int)(x/CELL_SIZE);
        iy = (int)(y/CELL_SIZE);
        if ((ix>=0)&&(ix<NB_COLUMNS)&&(iy>=0)&&(iy<NB_ROWS)){
            if (board[iy*NB_COLUMNS+ix]!=0){
                return true;
            }
        }

    }

    return false;
}