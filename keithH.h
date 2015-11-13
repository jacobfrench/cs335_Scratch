#ifndef _keithH_H_
#define _keithH_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cstdio>
#include <cstring>
#include "fonts.h"
#include <string>
using namespace std;

class Hud{
  private:
    int xres;
    int yres;
    int player1_health;
    int player2_health;
    bool isAI;
    bool is_show_welcome;
    bool is_show_help_menu;
  public:    
    Hud(const int in_xres, const int in_yres);
    bool isShowHelpMenu();
    void setIsShowHelpMenu(bool in_is_show_help_menu);
    bool isShowWelcome();
    void setIsShowWelcome(bool in_is_show_welcome);
    void showWelcome(int in_highscore);
    void setPlayer1Health(int in_player1_health);
    void setPlayer2Health(int in_player2_health);
    int getPlayer1Health();
    int getPlayer2Health();
    void showScore(int in_score1, int in_score2);
    void showCourtYard();
    void setResolution(int in_xres, int in_yres);
    void showHealth(int player1_health, int player2_health);
    void selectLeftScreen();
    void selectRightScreen();
    void showTimer(int timer);
    void showGameOver(int in_highscore,int p1_score,int p2_score);
    void showIntro(char which_screen, GLuint introTexture, GLuint bgTexture1, GLuint bgTexture2);
    void setAI(bool in_AI);
    bool getAI();
    void showHelpMenu(GLuint help_menu_texture);
    void selectAI();
    void selectHuman();
    void renderBomb(GLuint which_bomb_texture, float bomb_posx, float bomb_posy, float bomb_width, float bomb_height);
};
#endif
