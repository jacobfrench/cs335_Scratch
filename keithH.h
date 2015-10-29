#ifndef _keithH_H_
#define _keithH_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstdio>
#include <cstring>
#include "fonts.h"
#include <string>

class Hud{
  private:
  int xres;
  int yres;  
  public:
   bool is_show_welcome;
   Hud(const int in_xres, const int in_yres);
   void showWelcome(int in_high_score);
   void showScore(int in_score1, int in_score2);
   void showCourtYard();
   void setResolution(int in_xres, int in_yres);
   void showHealth(int player1_health, int player2_health);
};





#endif
