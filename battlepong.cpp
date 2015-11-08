//cs335 Spring 2015
//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014
//mod spring 2015: added constructors

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
#include "Ball.h"
#include "coryK.h"
#include "player.h"
#include "keithH.h"
#include "timer.h"

#include "paddle.h"
extern "C" {
#include "fonts.h"
}

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];




//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;


//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

//screen width and height
int xres=1250, yres=900;
int intro = 0;
//instance variables
Ball ball(xres,yres);
Paddle paddle1(yres);
Paddle paddle2(yres);

float ballXPos;
float ballYPos;
float ballXVel;
float ballYVel;
bool gameStarted;

float paddle1YVel;
float paddle2YVel;

int startTime = 3000000; //3 minutes
string timeStr;

struct Game {
	bool mouseThrustOn;
	Game() {
		mouseThrustOn = false;
	}
};

string BG_IMAGE_PATH = "./images/titlescreen.ppm";
string MAINBG_IMAGE_PATH = "./images/mainBG.ppm";
string ATOM_IMAGE_PATH = "./images/atom.ppm";

Ppmimage *introBG = NULL;
GLuint introTexture;

Ppmimage *mainBG = NULL;
GLuint mainTexture;
string BG_IMAGE_PATH1 = "./images/ninja_robot.ppm";
string BG_IMAGE_PATH2 = "./images/ninja_robot2.ppm";

Ppmimage *bgImage1 = NULL;
Ppmimage *bgImage2 = NULL;

Ppmimage *atomImage = NULL;

Ppmimage *gameOverImage = NULL;
string GAMEOVER_IMAGE_PATH = "./images/game_over.ppm";

GLuint bgStartTexture, bgTexture, bgTexture1, bgTexture2, atomTexture, gameOverTexture;

int keys[65536];

Game g;
Timer timer;



//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
int check_keys(XEvent *e, Game *g);
void init(Game *g);
void init_sounds(void);
void init_ball_paddles(void);
void physics(Game *game);
void render(Game *game);
void init_powerup_x_y(void);
int getTimer();
void stopGame();
int powerup_posx, powerup_posy, powerup_width = 50, powerup_height = 50;
//offset is margin around retro background(courtyard):
int offset = 40;


Hud *hud;
Player p1;
Player p2;
int high_score;


//Variable for declaring which level
//is selected
int level;

/* Test - Create Base Class - Game Object */
GameObject* obj = new GameObject(xres / 2.0, yres / 2.0, 50.0f, 50.0f);
/* Test - Create Derivd Class - Obstacle */
Obstacle *obstacle = new Obstacle(3);



time_t timeBegin, timeSpawn, timeRandom;
enum BG_Screen {LEFT,RIGHT};
BG_Screen selected_screen;

int is_render_powerup;
bool is_gameover;
//-----------------

int main(void)
{

	logOpen();
	initXWindows();
	init_opengl();
	Game game;
	init(&game);
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);	

	hud = new Hud(xres ,yres);	
	selected_screen = LEFT;    
    is_gameover = false;
    high_score = 0;
    is_render_powerup = 0;
    gameStarted = false;

	//MAIN MENU LOOP 
	while(intro != 0) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			intro = check_keys(&e, &game);
		}
		render(&game);
		glXSwapBuffers(dpy, win);
    }

    timeBegin = time(NULL);
    timeRandom = random(7);    

	//BEGIN MAIN GAME LOOP
	int done=0;
	while (!done) {        
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			done = check_keys(&e, &game);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics(&game);
			physicsCountdown -= physicsRate;
		}
        if (timeBegin + timeRandom < time(NULL)){
            init_powerup_x_y();
            timeBegin = time(NULL);
            is_render_powerup = is_render_powerup^1;
            if (is_render_powerup){
            timeRandom = random(10);
            }
            else{
            timeRandom = random(4);
            }
        }
		render(&game);
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	logClose();

	return 0;
}

void init_ball_paddles(){
//init ball variables
ball.resetScore();
ball.setXPos(xres/2);
ball.setYPos(yres/2);
ball.setRadius(15.0f);

//ball velocity
ballXVel = 8.0f * cos(30);
ballYVel = 8.0f * sin(90);

ball.setYVel(ballXVel);
ball.setXVel(ballYVel);

//init paddle1
paddle1.setXPos(50.0f);
paddle1.setYPos((float)yres/2);
paddle1.setHeight(120.0f);
paddle1.setWidth(15.0f);

//init paddle2
paddle2.setXPos((float)xres - 65.0f);
paddle2.setYPos((float)yres/2);
paddle2.setHeight(120.0f);
paddle2.setWidth(15.0f);
}

void stopGame(){
ball.setXPos(xres/2);
ball.setYPos(yres/2);
ball.setXVel(0);
ball.setYVel(0);;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Battle Pong!");
}

void setup_screen_res(const int w, const int h)
{
	xres = w;
	yres = h;
}

void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XSetWindowAttributes swa;
	setup_screen_res(xres, yres);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
		PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
	xres = width;
	yres = height;

	//window has been resized.
	setup_screen_res(width, height);
	//


	hud->setResolution(xres,yres);

    //RE-INITIATE POWERUP POSITIONS:
    init_powerup_x_y();

	//RESET THE TWO PADDLES POSITION AND BALL RESOLUTION:
	paddle1.setXPos(50.0f);
	paddle1.setYPos((float)yres/2);
	paddle2.setXPos((float)xres - 65.0f);
	paddle2.setYPos((float)yres/2);
	paddle1.setWindowHeight(yres);
	paddle2.setWindowHeight(yres);
	ball.setResolution(xres,yres);
	//-------------------------

	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	set_title();
}

void init_opengl(void)
{


	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

    //Load powerup image(s):
    atomImage = loadImage(ATOM_IMAGE_PATH.c_str());
    atomTexture = generateTransparentTexture(atomTexture, atomImage);

    //Create background texture elements
    introBG = loadImage(BG_IMAGE_PATH.c_str());
    introTexture = generateTexture(introTexture, introBG);
    mainBG = loadImage(MAINBG_IMAGE_PATH.c_str());
    mainTexture = generateTexture(mainTexture, mainBG);
    bgImage1 = loadImage(BG_IMAGE_PATH1.c_str());
    bgTexture = generateTexture(bgTexture, bgImage1);
    bgTexture1 = generateTexture(bgTexture1, bgImage1);
    bgImage2 = loadImage(BG_IMAGE_PATH2.c_str());
	bgTexture2 = generateTexture(bgTexture2, bgImage2);

    //Create gameover texture:
    gameOverImage = loadImage(GAMEOVER_IMAGE_PATH.c_str());
    gameOverTexture = generateTexture(gameOverTexture, gameOverImage);
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		//Window size did change.        
		reshape_window(xce.width, xce.height);
	}
}

void init(Game *g) {
	g->mouseThrustOn=false;
}

void normalize(Vec v) {
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}



int check_keys(XEvent *e, Game *g){
	g->mouseThrustOn=false;
	//keyboard input?
	static int shift=0;
	int key = XLookupKeysym(&e->xkey, 0);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		if(key == XK_w){
			paddle1YVel = 0;
			paddle1.setYPos(paddle1.getYPos());

		}
		if(key == XK_s){
			paddle1YVel = 0;
			paddle1.setYPos(paddle1.getYPos());
		}
		if(key == XK_o){
			paddle2YVel = 0;
			paddle2.setYPos(paddle2.getYPos());

		}
		if(key == XK_l){
			paddle2YVel = 0;
			paddle2.setYPos(paddle2.getYPos());
		}


		return 0;
	}

	if (e->type == KeyPress) {
		keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return 0;
		}
		if(key == XK_b) {
			printf("Enter pressed\n");
            gameStarted = true;
            if (is_gameover == true){
            hud->is_show_welcome = true;
            gameStarted = false;
            intro = 0;
            }
            else{
            hud->is_show_welcome = false;
            init_ball_paddles();
            intro = 1;
            }
            is_gameover = false;
            timer.reset();
            timer.start();            
		}

		if (hud->is_show_welcome == true){
			if (key == XK_Left) {
				bgTexture = generateTexture(bgTexture, bgImage1);
				selected_screen = LEFT;
				level = 1;
			}
			else if (key == XK_Right) {
				bgTexture = generateTexture(bgTexture, bgImage2);            
				selected_screen = RIGHT;
				level = 2;

			}			
		}
	}
	else {
		return 0;
	}

	float paddleSpeed = 20.0f;
	if (shift){}
	switch(key) {
		case XK_Escape:
			return 1;
		case XK_w:
			paddle1YVel = paddleSpeed;
			break;
		case XK_s:
			paddle1YVel = -paddleSpeed;
			break;
		case XK_a:
			break;
		case XK_d:
			break;
		case XK_o:
			paddle2YVel = paddleSpeed;
			break;
		case XK_l:
			paddle2YVel = -paddleSpeed;
			break;

	}

	return 0;

}

void physics(Game *g)
{
	g->mouseThrustOn=false;

	//ball collision
    if(gameStarted){
        ball.setYVel(ball.getYVel());
        ball.setXVel(ball.getXVel());
        ball.checkCollision(xres, yres);
    }
	

	//paddle collision
	paddle1.checkCollision(yres, ball);
	paddle2.checkCollision(yres, ball);
	obstacle->checkCollision(xres, yres, ball, p1);

	//paddle1 movement
	paddle1.setYVel(paddle1YVel);

	//paddle2 movement
	paddle2.setYVel(paddle2YVel);        

}

void init_powerup_x_y(){
    int y_range = yres - (2*offset) - (powerup_height);
    powerup_posx = (int)(random(xres/2) + (xres/4) - (powerup_width/2));
    powerup_posy = (int)(random(y_range) + (offset));
}

int getTimer(){    
    int time = startTime - timer.getTicks();
    string str;
    stringstream ss;
    stringstream ss2;
    ss << time;
    timeStr = ss.str();
    int ret = atoi(timeStr.c_str()) / 10000;
    if (ret < 0){
        ret = 0;
        is_gameover = true;
        stopGame();
    }
    return ret;
}

void render(Game *g)
{    
	g->mouseThrustOn=false;    
	glClear(GL_COLOR_BUFFER_BIT);
	if(intro < 1) {
		//DRAW titlescreen.ppm:
		renderTexture(introTexture, xres, yres);

		//PRINT CHOOSE BACKGROUND SCREEN:
		Rect r1;
		r1.bot = yres/2.0 - 110.0;
		r1.left = xres/2.0 - 100.0;
		r1.center = 0;
        ggprint16(&r1, 16, 0xffffff, "Press 'LEFT/RIGHT'' for background");

		Rect r2;
		r2.bot = (yres / 2.0) - 150;
		r2.left = xres / 2.0 - 70.0;
		r2.center = 0;
		ggprint16(&r2, 16, 0xffffff, "Press 'B' to start");

        //PASS showWelcome the high score:
        hud->showWelcome(high_score);
        switch(selected_screen){
			case LEFT:
				hud->selectLeftScreen();
				break;
			case RIGHT:
				hud->selectRightScreen();
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
		return;
	}
	else{
        renderTexture(bgTexture, xres, yres);
	}

    if (is_gameover){
        renderTexture(gameOverTexture, xres, yres);
        hud->showGameOver(high_score,ball.getPlayer1Score(), ball.getPlayer2Score());
        //NEED TO SAVE HIGH SCORE HERE:

        return;
    }


	hud->showScore(ball.getPlayer1Score(), ball.getPlayer2Score());
	hud->showHealth(100, 70);
    hud->showCourtYard();

    if (is_render_powerup){
        //DRAW ATOM(RANDOMLY):        
        //init_powerup_x_y();                
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, atomTexture);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(powerup_posx + powerup_width, powerup_posy + powerup_height);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(powerup_posx + powerup_width, powerup_posy);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(powerup_posx, powerup_posy);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(powerup_posx, powerup_posy + powerup_height);
        glEnd();                
        glDisable(GL_BLEND);
        glPopMatrix();
    }
    glBindTexture(GL_TEXTURE_2D, 0);

	//Draw the paddle    
	glColor3f(0.0, 0.5, 0.5);
	paddle1.render();
	glColor3f(0.7, 0.5, 0.0);
	paddle2.render();
	glEnd();
	//Draw the ball
	ball.render();

	//TODO
	//If option 2 is selected i.e. ninja_robot.ppm is the background
	//Level 2 selected
	//Draw some obstacles to showcase a difference between level 1 and level2
	if(level == 2) {
		//obstacle->render();

	}
    obstacle->render();
    
    
    hud->showTimer(getTimer());

}

