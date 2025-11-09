#pragma once
#include "IGameMode.h"
#include <map>

class Game;
class HighScoresMode : public IGameMode
{
public:

    int startTimeV;
    int iHighScoreColor;

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

    HighScoresMode(Game *g);
    
    bool ProcessEvent(SDL_Event &e);
    void Draw();
    void Update();

};