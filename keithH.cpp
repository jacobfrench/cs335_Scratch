/*Author: Keith Harryman
 * Program Nam: hud.h
 * Purpose: To show start screen, and score screen:
 * */

#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstdio>
#include <cstring>
#include "fonts.h"
#include "keithH.h"
#include <string>


using namespace std;

Hud::Hud(const int in_xres, const int in_yres){
    xres = in_xres;
    yres = in_yres;
    is_show_welcome=true;
}

void Hud::setResolution(const int in_xres, const int in_yres){
    xres = in_xres;
    yres = in_yres;
}

void Hud::showCourtYard(){
      //DRAW OUTER RECTANGLE:
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      glOrtho( 0, xres, 0, yres, -1, 1);
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();
      glTranslatef( 0.5, 0.5, 0 );
      float offset = 40;
      glColor3ub( 0, 255, 0 );
      glBegin(GL_LINE_LOOP);
      glVertex2f( 0+offset, 0+offset );
      glVertex2f( 0+offset, yres-offset );
      glVertex2f( xres-offset, yres-offset );
      glVertex2f( xres-offset, 0+offset );
      glEnd();

      //DRAW LINE THROUGH MIDDLE:
      glColor3ub( 0, 255, 0 );
      glBegin(GL_LINES);
      glVertex2f(xres/2,0+offset);
      glVertex2f(xres/2,yres-offset);
      glEnd();
}


void Hud::showWelcome(int in_high_score){
    glColor3ub(0,255,255);
    unsigned int cref = 0x0000ffff;
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //PRINT WELCOME MESSAGE:    
    Rect r0;
    r0.bot = yres/2.0;
    r0.left = xres/2.0 - 50.0;
    r0.center = 0;
    ggprint16(&r0, 70, cref, "Welcome to Pong!");

    //PRINT HIGH SCORE:
    Rect r1;
    r1.bot = yres/2.0 - 30.0;
    r1.left = xres/2.0 - 50.0;
    r1.center = 0;
    char buf[50];
    sprintf(buf,"Current high score is:%d",in_high_score);
    ggprint12(&r1, 70, cref, buf);
    //--------------------------------------------------------
}

void Hud::showHealth(int player1_health, int player2_health){
    float width = 25.0;
    float height;
    //SHOW PLAYER1'S HEALTH:
    glColor3ub(0,255,0);
    glPushMatrix();
    glTranslatef(10, yres - 110.0, 0);
    height = player1_health;
    glRectf(0.0f, 0.0f, width, height);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    //----------------------------
    //SHOW PLAYER2'S HEALTH:
    glColor3ub(0,255,0);
    glPushMatrix();
    glTranslatef(xres - 35.0, yres - 110.0, 0);
    height = player2_health;
    glRectf(0.0f, 0.0f, width, height);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    //----------------------------
    //----------------------------
    //SHOW PLAYER1'S DEATH:
    glColor3ub(255,0,0);
    glPushMatrix();
    glTranslatef(10, player1_health + yres - 110.0, 0);
    height = 100.0 - player1_health;
    glRectf(0.0f, 0.0f, width, height);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    //----------------------------
    //SHOW PLAYER2'S DEATH:
    glColor3ub(255,0,0);
    glPushMatrix();
    glTranslatef(xres - 35.0, player2_health + yres - 110.0, 0);
    height = 100.0 - player2_health;
    glRectf(0.0f, 0.0f, width, height);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    //----------------------------

}


void Hud::showScore(int in_score1, int in_score2){    

    glColor3ub(255,255,255);
    char buf[50];
    unsigned int cref = 0x00ffffff;
    glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);

    //PRINT PLAYER 1'S SCORE:
    //PRINT HIGH SCORE:
    Rect r0;
    r0.bot = yres - 33.0;
    r0.left = 40.0,
    r0.center = 0;
    sprintf(buf,"Player 1 Score:%d",in_score1);
    ggprint16(&r0, 70, cref, buf);
    //--------------------------------------------------------

    //PRINT PLAYER 2'S SCORE:
    Rect r1;
    r1.bot = yres - 33.0;
    r1.left = xres/2,
    r1.center = 0;
    sprintf(buf,"Player 2 Score:%d",in_score2);
    ggprint16(&r1, 70, cref, buf);
    //--------------------------------------------------------

}
