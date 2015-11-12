/*Author: Keith Harryman
 * Program Nam: hud.h
 * Purpose: To show start screen, and score screen:
 * */

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cstdio>
#include <cstring>
#include "fonts.h"
#include "keithH.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include "brianC.h"

using namespace std;

Hud::Hud(const int in_xres, const int in_yres){
	xres = in_xres;
	yres = in_yres;
	is_show_welcome=true;
	player1_health = 100;
	player2_health = 100;
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


void Hud::showWelcome(int in_highscore){
	glColor3ub(0,255,255);    
	glEnable(GL_TEXTURE_2D);
	char buf[50];    
	//PRINT HIGH SCORE:
	Rect r1;
	r1.bot = yres - 100.0;
	r1.left = xres/2.0 - 100.0;
	r1.center = 0;    
	sprintf(buf,"Current high score is: %d",in_highscore);
	ggprint16(&r1, 16, 0x00ffffff, buf);
	//--------------------------------------------------------
}

void Hud::showHealth(int player1_health, int player2_health){    
	float width = 25.0;
	float height;

	glBegin(GL_QUADS);
	glColor3f(1.0f,0.0f,0.0f);
	height = 100.0;
	//SHOW PLAYER1'S DEATH:
	glVertex2f(10 + width, yres -110.0 + height);
	glVertex2f(10 + width, yres -110.0);
	glVertex2f(10, yres -110.0);
	glVertex2f(10, yres -110.0 +height);
	//----------------------------
	//SHOW PLAYER2'S DEATH:
	glVertex2f(xres - 35.0 + width, yres -110.0 + height);
	glVertex2f(xres - 35.0 + width, yres -110.0);
	glVertex2f(xres - 35.0, yres -110.0);
	glVertex2f(xres - 35.0, yres -110.0 +height);
	//----------------------------
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.0f,1.0f,0.0f);
	height = 9;
	//SHOW PLAYER1'S HEALTH:
	int num_rects = (int)(player1_health/10);
	for (int i=1;i<=num_rects;i++){
		glVertex2f(10 + width, yres -110.0 + (i* 10));
		glVertex2f(10 + width, yres -110.0 + (i* 10) - 9);
		glVertex2f(10, yres -110.0 + (i* 10) - 9);
		glVertex2f(10, yres -110.0 + (i* 10));
	}
	//----------------------------
	//SHOW PLAYER2'S HEALTH:
	num_rects = (int)(player2_health/10);
	for (int i=1;i<=num_rects;i++){
		glVertex2f(xres - 35.0 + width, yres -110.0 + (i* 10));
		glVertex2f(xres - 35.0 + width, yres -110.0 + (i* 10) - 9);
		glVertex2f(xres - 35.0, yres -110.0 + (i* 10) - 9);
		glVertex2f(xres - 35.0, yres -110.0 + (i* 10));
	}
	glEnd();
	//----------------------------
}


void Hud::showScore(int in_score1, int in_score2){    
	glEnable(GL_TEXTURE_2D);
	glColor3ub(255,255,255);
	char buf[50];
	unsigned int cref = 0x00ffffff;    
	//glBindTexture(GL_TEXTURE_2D, 0);

	//PRINT PLAYER 1'S SCORE:
	Rect r0;
	r0.bot = yres - 33.0;
	r0.left = 40.0,
	r0.center = 0;
	sprintf(buf,"Player 1 Score: %d",in_score1);
	ggprint16(&r0, 70, cref, buf);
	//--------------------------------------------------------

	//PRINT PLAYER 2'S SCORE:
	Rect r1;
	r1.bot = yres - 33.0;
	r1.left = xres/2,
	r1.center = 0;
	sprintf(buf,"Player 2 Score: %d",in_score2);
	ggprint16(&r1, 70, cref, buf);
	//--------------------------------------------------------
}

void Hud::showTimer(int timer){
	glEnable(GL_TEXTURE_2D);
	glColor3ub(255,255,255);
	char buf[50];
	unsigned int cref = 0x00ffffff;
    
	Rect r;
	r.bot = 10;
	r.left = xres/2 - 25;
	r.center = 0;
	sprintf(buf, "Time: %d", timer);
	ggprint16(&r, 70, cref, buf);
}

void Hud::showGameOver(int in_highscore,int p1_score,int p2_score){
	//PRINT HIGH SCORE:
	Rect r0;
	r0.bot = yres - 100.0;
	r0.left = xres/2.0 - 100.0;
	r0.center = 0;
	char buf1[100];
	char buf2[100];    
	sprintf(buf1,"Current high score is: %d",in_highscore);
	ggprint16(&r0, 16, 0x00ffffff, buf1);
	//--------------------------------------------------------

	glEnable(GL_TEXTURE_2D);
	glColor3ub(255,255,255);
	unsigned int cref = 0x00ffffff;

	//FOR TIME OUT:
	if (getPlayer1Health()>0 && getPlayer2Health()>0){
	  if (p1_score >= p2_score && getPlayer1Health()>0){        
		sprintf(buf1,"Player 1 wins! %d points ", p1_score);
		sprintf(buf2,"Player 2 loses! %d points ", p2_score);
	  }
	  else{        
		sprintf(buf1,"Player 2 wins! %d points ", p2_score);
		sprintf(buf2,"Player 1 loses! %d points ", p1_score);
	  }
	}
	else{
	  if (getPlayer1Health()>0){        
		sprintf(buf1,"Player 1 wins! %d points ", p1_score);
		sprintf(buf2,"Player 2 loses! %d points ->DEAD", p2_score);
	  }
	  else{        
		sprintf(buf1,"Player 2 wins! %d points ", p2_score);
		sprintf(buf2,"Player 1 loses! %d points ->DEAD", p1_score);
	  }
	  
	}

	//PRINT FIRST SCORE:
	Rect r1;
	r1.bot = yres/2;
	r1.left = xres/2.0 - 100.0,
	r1.center = 0;
	ggprint16(&r1, 70, cref, buf1);
	//--------------------------------------------------------

	//PRINT SECOND SCORE:
	Rect r2;
	r2.bot = yres/2 - 80.0;
	r2.left = xres/2.0 - 100.0,
	r2.center = 0;
	ggprint16(&r2, 70, cref, buf2);
	//--------------------------------------------------------

	//PRINT RETURN TO MENU:
	Rect r3;
	r3.bot = yres/2 - 160.0;
	r3.left = xres/2.0 - 100.0,
	r3.center = 0;
	ggprint16(&r3, 70, 0x00ff9999, "PRESS 'B' TO GO BACK TO MAIN MENU");
	//--------------------------------------------------------
}

void Hud::selectLeftScreen(){
	glColor3ub( 0, 255, 0);
	glBegin(GL_QUADS);
	glLineWidth(10);
	glVertex2f( xres/2 - 360, yres/2 - 150 + 10 );
	glVertex2f( xres/2 -360, yres/2 - 350 - 10 );
	glVertex2f( xres/2 - 90, yres/2 - 350 - 10);
	glVertex2f( xres/2 - 90, yres/2 - 150 + 10);
	glEnd();
}

void Hud::selectRightScreen(){
	glColor3ub( 0, 255, 0 );
	glBegin(GL_QUADS);
	glLineWidth(10);
	glVertex2f( xres/2 + 360, yres/2 -150 + 10 );
	glVertex2f( xres/2 + 360, yres/2 - 350 - 10 );
	glVertex2f( xres/2 + 90, yres/2 - 350 - 10);
	glVertex2f( xres/2 + 90, yres/2 -150 + 10);
	glEnd();
}

void Hud::setPlayer1Health(int in_player1_health){
	player1_health = in_player1_health;
}

void Hud::setPlayer2Health(int in_player2_health){
	player2_health = in_player2_health;
}

int Hud::getPlayer1Health(){
	return player1_health;
}

int Hud::getPlayer2Health(){
	return player2_health;
}
