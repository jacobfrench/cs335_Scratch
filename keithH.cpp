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
#include "coryK.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include "brianC.h"
#include "ppm.h"


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
	r1.bot = yres - 200.0;
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
	r2.left = xres/2.0 - 100.0;
	r2.center = 0;
	ggprint16(&r2, 70, cref, buf2);
	//--------------------------------------------------------

	//PRINT RETURN TO MENU:
	Rect r3;
	r3.bot = yres/2 - 160.0;
	r3.left = xres/2.0 - 100.0,
		r3.center = 0;
	ggprint16(&r3, 70, 0x00ff9999, "PRESS 'ENTER' TO GO BACK TO MAIN MENU");
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

void Hud::showIntro(char which_screen,GLuint introTexture, GLuint bgTexture1, GLuint bgTexture2){
	//DRAW titlescreen.ppm:
	renderTexture(introTexture, xres, yres);

	//PRINT PROMPT HELP MENU:
	Rect r01;
	r01.bot = yres - 40.0;
	r01.left = xres/2.0 - 150.0;
	r01.center = 0;
	ggprint16(&r01, 80, 0xffffff, "Press 'H' for help menu:");

	//PRINT CHOOSE PLAYER 2:
	Rect r0;
	r0.bot = yres - 100.0;
	r0.left = xres/2.0 - 150.0;
	r0.center = 0;
	ggprint16(&r0, 80, 0xffffff, "Press 'UP/DOWN' to choose human or computer:");

	//PRINT SELECT HUMAN:
	Rect r0A;
	r0A.bot = yres - 130.0;
	r0A.left = xres/2.0 - 70.0;
	r0A.center = 0;
	ggprint16(&r0A, 16, 0xffffff, "HUMAN");

	//PRINT SELECT COMPUTER:
	Rect r0B;
	r0B.bot = yres - 160.0;
	r0B.left = xres/2.0 - 70.0;
	r0B.center = 0;
	ggprint16(&r0B, 16, 0xffffff, "COMPUTER");

	//PRINT CHOOSE BACKGROUND SCREEN:
	Rect r1;
	r1.bot = yres/2.0 - 110.0;
	r1.left = xres/2.0 - 100.0;
	r1.center = 0;
	ggprint16(&r1, 16, 0xffffff, "Press 'LEFT/RIGHT' for background");

	Rect r2;
	r2.bot = (yres / 2.0) - 150;
	r2.left = xres / 2.0 - 70.0;
	r2.center = 0;
	ggprint16(&r2, 16, 0xffffff, "Press 'Enter' to start");

	//PASS showWelcome the high score:
	int high_score = setHighScore(0, 0);
	showWelcome(high_score);
	switch(which_screen){
		case 'L':
			selectLeftScreen();
			break;
		case 'R':
			selectRightScreen();
			break;
		default:
			break;
	}
	glColor3f(1.0, 1.0, 1.0);
	//RENDER OPTION BG1:
	glBindTexture(GL_TEXTURE_2D, bgTexture1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(xres/2 - 350, yres/2 - 350);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(xres/2 - 350, yres/2-150);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres/2 -100 , yres/2-150);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres/2 - 100, yres/2 - 350);
	glEnd();
	//RENDER OPTION BG2:
	glBindTexture(GL_TEXTURE_2D, bgTexture2);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(xres/2 + 350, yres/2 - 350);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(xres/2 + 350, yres/2 - 150);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres/2 + 100 , yres/2 - 150);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres/2 + 100, yres/2 - 350);
	glEnd();   
}

void Hud::setAI(bool in_AI){
	isAI = in_AI;
}

bool Hud::getAI(){
	return isAI;
}

void Hud::selectAI(){
	glColor3ub( 0, 255, 0);
	glLineWidth(10.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f( xres/2 +100, yres - 170 );
	glVertex2f(xres/2 + 100, yres - 130 );
	glVertex2f( xres/2 - 120, yres - 130 );
	glVertex2f( xres/2 - 120, yres - 170 );
	glEnd();
}

void Hud::selectHuman(){
	glColor3ub( 0, 255, 0);
	glLineWidth(10.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f( xres/2 +100, yres - 130 );
	glVertex2f(xres/2 + 100, yres - 100 );
	glVertex2f( xres/2 - 120, yres - 100 );
	glVertex2f( xres/2 - 120, yres - 130 );
	glEnd();
}

void Hud::renderBomb(GLuint which_bomb_texture, float bomb_posx, float bomb_posy, float bomb_width, float bomb_height){
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, which_bomb_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(bomb_posx + bomb_width, bomb_posy + bomb_height);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(bomb_posx + bomb_width, bomb_posy);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(bomb_posx, bomb_posy);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(bomb_posx, bomb_posy + bomb_height);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Hud::showHelpMenu(GLuint help_menu_texture){
	renderTexture(help_menu_texture, xres, yres);
	int cref = 0x00ffffff;
	int push_down_offset = 200;
	//PRINT PROMPT AI:
	Rect r1;
	r1.bot = yres - 50 - push_down_offset;
	r1.left = xres/2 - 150.0,
		r1.center = 0;
	ggprint16(&r1, 70, cref, "TO CHOOSE AI:");
	Rect r2;
	r2.bot = yres - 70 - push_down_offset;
	r2.left = xres/2 - 150.0,
		r2.center = 0;
	ggprint16(&r2, 100, cref, "--------------------------");
	//--------------------------------------------------------

	//PRINT SELECT COMPUTER:
	Rect r3;
	r3.bot = yres - 90.0 - push_down_offset;
	r3.left = xres/2 - 150.0,
		r3.center = 0;
	ggprint16(&r3, 70, cref, "PRESS 'UP' FOR HUMAN");
	//--------------------------------------------------------

	//PRINT SELECT HUMAN:
	Rect r4;
	r4.bot = yres - 110.0 - push_down_offset;
	r4.left = xres/2 - 150.0,
		r4.center = 0;
	ggprint16(&r4, 70, cref, "PRESS 'DOWN' FOR COMPUTER");
	//--------------------------------------------------------

	//PRINT PROMPT LEFT PADDLE CONTROLS:
	Rect r5;
	r5.bot = yres - 140 - push_down_offset;
	r5.left = xres/2 - 350.0,
		r5.center = 0;
	ggprint16(&r5, 70, cref, "FOR LEFT PADDLE:");
	Rect r6;
	r6.bot = yres - 160 - push_down_offset;
	r6.left = xres/2 - 350.0,
		r6.center = 0;
	ggprint16(&r6, 70, cref, "----------------");
	//--------------------------------------------------------

	//PRINT LEFT PADDLE CONTROLS GO UP:
	Rect r7;
	r7.bot = yres - 180.0 - push_down_offset;
	r7.left = xres/2 - 350.0,
		r7.center = 0;
	ggprint16(&r7, 70, cref, "PRESS 'W' TO MOVE UP");
	//--------------------------------------------------------

	//PRINT LEFT PADDLE CONTROLS GO DOWN:
	Rect r8;
	r8.bot = yres - 200.0 - push_down_offset;
	r8.left = xres/2 - 350.0,
		r8.center = 0;
	ggprint16(&r8, 70, cref, "PRESS 'S' TO MOVE DOWN");
	//--------------------------------------------------------

	//PRINT PROMPT RIGHT PADDLE CONTROLS:
	Rect r9;
	r9.bot = yres - 140 - push_down_offset;
	r9.left = xres/2 - 0;
	r9.center = 0;
	ggprint16(&r9, 70, cref, "FOR RIGHT PADDLE:");
	Rect r10;
	r10.bot = yres - 160  - push_down_offset;
	r10.left = xres/2 - 0;
	r10.center = 0;
	ggprint16(&r10, 70, cref, "-----------------");
	//--------------------------------------------------------

	//PRINT RIGHT PADDLE CONTROLS GO UP:
	Rect r11;
	r11.bot = yres - 180.0 - push_down_offset;
	r11.left = xres/2 - 0;
	r11.center = 0;
	ggprint16(&r11, 70, cref, "PRESS 'O' TO MOVE UP");
	//--------------------------------------------------------

	//PRINT RIGHT PADDLE CONTROLS GO DOWN:
	Rect r12;
	r12.bot = yres - 200.0 - push_down_offset;
	r12.left = xres/2 - 0;
	r12.center = 0;
	ggprint16(&r12, 70, cref, "PRESS 'L' TO MOVE DOWN");
	//--------------------------------------------------------

	//PRINT PROMPT FOR PAUSE:
	Rect r13;
	r13.bot = yres - 250.0 - push_down_offset;
	r13.left = xres/2 - 150;
	r13.center = 0;
	ggprint16(&r13, 70, cref, "PRESS 'P' TO PAUSE");
	//--------------------------------------------------------

	//PRINT PROMPT TO GO BACK:
	Rect r14;
	r14.bot = yres - 280.0 - push_down_offset;
	r14.left = xres/2 - 150;
	r14.center = 0;
	ggprint16(&r14, 70, cref, "PRESS 'B' TO GO BACK TO MAIN MENU");
	//--------------------------------------------------------
}

bool Hud::isShowHelpMenu(){
	return is_show_help_menu;
}

void Hud::setIsShowHelpMenu(bool in_is_show_help_menu){
	is_show_help_menu = in_is_show_help_menu;
}

bool Hud::isShowWelcome(){
	return is_show_welcome;
}

void Hud::setIsShowWelcome(bool in_is_show_welcome){
	is_show_welcome = in_is_show_welcome;
}

