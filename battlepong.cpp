//program: battlepong.cpp
//authors: Jacob F., Cory K., Keith H., Brian C.
//date:    2015

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
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
#include "Ball.h"
#include "coryK.h"
#include "player.h"
#include "keithH.h"
#include "timer.h"
#include "brianC.h"
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
//offset is margin around retro background(courtyard):
int offset = 40;

struct Game {
	bool mouseThrustOn;
	Game() {
		mouseThrustOn = false;
	}
};

//SET IMAGES
string BG_IMAGE_PATH = "./images/titlescreen.ppm";
string MAINBG_IMAGE_PATH = "./images/mainBG.ppm";
string BOMB_IMAGE_PATH = "./images/bomb.ppm";
string GAMEOVER_IMAGE_PATH = "./images/game_over.ppm";
string BG_IMAGE_PATH1 = "./images/ninja_robot.ppm";
string BG_IMAGE_PATH2 = "./images/ninja_robot2.ppm";
string HELP_MENU_IMAGE_PATH = "./images/help_menu.ppm";
string EXPLODE_IMAGE_PATH = "./images/explode.ppm";
Ppmimage *introBG = NULL;
Ppmimage *mainBG = NULL;
Ppmimage *bgImage1 = NULL;
Ppmimage *bgImage2 = NULL;
Ppmimage *bombImage = NULL;
Ppmimage *gameOverImage = NULL;
Ppmimage *explodeImage = NULL;
Ppmimage *helpMenuImage = NULL;
GLuint introTexture,mainTexture,bgStartTexture, bgTexture, bgTexture1, bgTexture2, bombTexture, gameOverTexture, explodeTexture, helpMenuTexture;
//-------------

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
int getTimer();
void stopGame();
void pauseGame();

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

int startTime = 5000000; //5 minutes
string timeStr;

Hud *hud;
Player p1;
Player p2;
int high_score;

int bomb_posx, bomb_posy, bomb_width = 150, bomb_height = 150;

//Variable for declaring which level
//is selected
int level;

/* Test - Create Base Class - Game Object */
GameObject* obj = new GameObject(xres / 2.0, yres / 2.0, 50.0f, 50.0f);
/* Test - Create Derivd Class - Obstacle */
Obstacle *obstacle = new Obstacle(3);

time_t bombBegin, bombRandom, beginSmallLeftPaddle, smallLeftPaddleTime, beginSmallRightPaddle, smallRightPaddleTime;
time_t beginExplode;
enum BG_Screen {LEFT,RIGHT};
BG_Screen selected_screen;

char lastPaddleHit;

int is_render_powerup;
bool is_gameover;
float ball_saved_Y_velocity;
float ball_saved_X_velocity;
bool is_paused;

//BOMB variables:
float bomb_theta= 0;
int bomb_radius;
float speed_theta=1/(10*PI);
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
	gameStarted = false;
	lastPaddleHit = 'N';//'N' means no paddle hit

	bombBegin = time(NULL);
	bombRandom = random(7);

	beginSmallLeftPaddle = time(NULL);
	smallLeftPaddleTime = 7;
	beginSmallRightPaddle = time(NULL);
	smallRightPaddleTime = 7;
	hud->setAI(false);//DEFAULT: player2 is human
	is_paused = false;
	ball_saved_X_velocity = 8.0f * cos(30);
	ball_saved_Y_velocity = 8.0f * sin(90);

	int min;
	if (xres<yres){
		min=xres;
  }
	else{
		min=yres;
	}
	bomb_radius = ((int)(3*min)/10);

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
		while (physicsCountdown >= physicsRate && !is_paused) {
			physics(&game);
			physicsCountdown -= physicsRate;
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
void pauseGame(){
	ball_saved_Y_velocity = ball.getYVel();
	ball_saved_X_velocity = ball.getXVel();
	ball.setXVel(0);
	ball.setYVel(0);
	obstacle->setYVel(0);
	timer.pause();
}

void resumeGame(){
	ball.setXVel(ball_saved_X_velocity);
	ball.setYVel(ball_saved_Y_velocity);
	obstacle->setYVel(-5);
	timer.resume();
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

	//SET HUD RESOLUTION:
	hud->setResolution(xres,yres);

	//RESET BOMB RADIUS:
	int min;
	if (xres<yres){
		min=xres;
	}
	else{
		min=yres;
	}
	bomb_radius = ((int)(3*min)/10);

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

	//Load bomb image(s):
	bombImage = loadImage(BOMB_IMAGE_PATH.c_str());
	bombTexture = generateTransparentTexture(bombTexture, bombImage);
	explodeImage = loadImage(EXPLODE_IMAGE_PATH.c_str());
	explodeTexture = generateTransparentTexture(explodeTexture, explodeImage);

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
	//Create help menu texture:
	helpMenuImage = loadImage(HELP_MENU_IMAGE_PATH.c_str());
	helpMenuTexture = generateTexture(helpMenuTexture, helpMenuImage);
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
		if(key == XK_Return) {
			//printf("Enter pressed\n");
			createSound2();
			gameStarted = true;
			hud->setPlayer1Health(100);
			hud->setPlayer2Health(100);
			if (is_gameover == true){
                hud->setIsShowWelcome(true);
				gameStarted = false;
				intro = 0;
			}
			else{
				hud->setIsShowWelcome(false);
				init_ball_paddles();
				//REINITIALIZE OBSTACLE POSITION AND VELIOCITY:
			        obstacle->setXPos((1250 / 2.0) - 25);
				obstacle->setYPos(900 / 2.0);	
				obstacle->setYVel(-5.0f);	
				intro = 1;
			}
			is_gameover = false;
			timer.reset();
			timer.start();            
		}
if (hud->isShowHelpMenu()==false && hud->isShowWelcome()==false){
			if (key == XK_p && is_paused){
				is_paused = false;
				resumeGame();
			}
			else if (key == XK_p && !is_paused){
				is_paused = true;
				pauseGame();
			}
            else if (key == XK_q){//to quit out of game
                hud->setIsShowWelcome(true);
                gameStarted = false;
                intro = 0;
            }
		}
		if (hud->isShowWelcome() == true){
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
else if (key == XK_Up) {
				hud->setAI(false);//player 2 is human
                paddle2.setCpuPlayer(false);
			}
			else if (key == XK_Down){
				hud->setAI(true);//player 2 is computer
                paddle2.setCpuPlayer(true);
			}
			else if (key == XK_h) {
				hud->setIsShowWelcome(false);
				hud->setIsShowHelpMenu(true);
			}
			return 0;
		}
		if (hud->isShowHelpMenu() == true){
			if (key == XK_b){
				//SHOW HELP MENU:
				hud->setIsShowHelpMenu(false);
				hud->setIsShowWelcome(true);
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
		obstacle->setYVel(obstacle->getYVel());
		bool is_ball_hit_edge = ball.checkCollision(xres, yres);
		if (is_ball_hit_edge){
			lastPaddleHit = 'N';
		}
	}
	

	//paddle collision
	bool isLeftHit = paddle1.checkCollision(yres, ball);
	if (isLeftHit){
		lastPaddleHit = 'L';        
	}
	bool isRightHit = paddle2.checkCollision(yres, ball);
	if (isRightHit){
		lastPaddleHit = 'R';        
	}

	if(level == 2){
		obstacle->checkCollision(xres, yres, ball, p1);
	}
	

	//paddle1 movement
	paddle1.setYVel(paddle1YVel);

	//paddle2 movement
	paddle2.setYVel(paddle2YVel);

    if (level == 1){
	//SET BOMBS POSITION:
	bomb_theta = bomb_theta + speed_theta;
	if (fabs(bomb_theta) >= 2*PI){
		bomb_theta=0;
		speed_theta *= -1;
	}
	bomb_posx=(int)(xres/2 + bomb_radius*cos(bomb_theta - PI/2) - bomb_width/2);
	bomb_posy=(int)(yres/2 + bomb_radius*sin(bomb_theta - PI/2) - bomb_height/2);

	//CHECK LEFT COLLISION WITH BOMB:
	if ((beginSmallLeftPaddle + smallLeftPaddleTime) < time(NULL)){
		paddle1.setHeight(120.0f);
    bool isBallBetweenX = (ball.getXPos() > bomb_posx) && (ball.getXPos() < (bomb_posx + bomb_width));
    bool isBallBetweenY = (ball.getYPos() > bomb_posy) && (ball.getYPos() < (bomb_posy + bomb_height));
    if (lastPaddleHit == 'L' && (isBallBetweenX && isBallBetweenY)){
			bombBegin = time(NULL);
			createSound(8);
			createSound(9);
			//set to half normal height:            
			paddle1.setHeight(60.0f);
			if (hud->getPlayer1Health()>0){
				hud->setPlayer1Health(hud->getPlayer1Health()-10*(1+random(8)));
				//GAMEOVER:
				if (hud->getPlayer1Health() <= 0){
					createSound(5);
					is_gameover = true;
					ball.setXVel(0.0f);
					ball.setYVel(0.0f);
					stopGame();
				}
			}
			beginSmallLeftPaddle = time(NULL);
    }
	}

	//CHECK RIGHT COLLISION WITH BOMB:
	if ((beginSmallRightPaddle + smallRightPaddleTime) < time(NULL)){
		paddle2.setHeight(120.0f);
		//is_bomb_visible = true;
		bool isBallBetweenX = (ball.getXPos() > bomb_posx) && (ball.getXPos() < (bomb_posx + bomb_width));
		bool isBallBetweenY = (ball.getYPos() > bomb_posy) && (ball.getYPos() < (bomb_posy + bomb_height));
		if (lastPaddleHit == 'R' && (isBallBetweenX && isBallBetweenY)){
			bombBegin = time(NULL);
			createSound(8);
			createSound(9);
			//is_bomb_visible = false;
			//set to half normal height:
			paddle2.setHeight(60.0f);
			if (hud->getPlayer2Health()>0){
				hud->setPlayer2Health(hud->getPlayer2Health()- 10*(1+random(8)));
				//GAMEOVER:
				if (hud->getPlayer2Health() <= 0){
					createSound(5);
					is_gameover = true;
					ball.setXVel(0.0f);
					ball.setYVel(0.0f);
					stopGame();
				}
			}
      beginSmallRightPaddle = time(NULL);
		}
	}
    }
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
		createSound(5);
		ret = 0;
		is_gameover = true;
		ball.setXVel(0.0f);
		ball.setYVel(0.0f);
		stopGame();
		//createSound(5);
	}
	return ret;
}

void render(Game *g)
{        
	g->mouseThrustOn=false;    
	glClear(GL_COLOR_BUFFER_BIT);
	if(intro < 1) {
char screen;
		if (selected_screen == LEFT){
			screen = 'L';
		}
		else{
			screen = 'R';
		}
		if (hud->isShowWelcome() == true){
			hud->showIntro(screen,introTexture, bgTexture1, bgTexture1);
			if (hud->getAI() == true){
				hud->selectAI();
			}
			else{
				hud->selectHuman();
			}
			return;
		}
		else if (hud->isShowHelpMenu() == true){
			hud->showHelpMenu(helpMenuTexture);
			return;
		}
return;
	}
	else{
		renderTexture(bgTexture, xres, yres);
	}

	if (is_gameover){
		renderTexture(gameOverTexture, xres, yres);
		high_score = setHighScore(ball.getPlayer1Score(), ball.getPlayer2Score());
		//cout << "Final Score : " << high_score << "\n";
		hud->showGameOver(high_score,ball.getPlayer1Score(), ball.getPlayer2Score());
		return;
	}

	hud->showScore(ball.getPlayer1Score(), ball.getPlayer2Score());
	hud->showHealth(hud->getPlayer1Health(), hud->getPlayer2Health());
	hud->showCourtYard();

	//DRAW BOMB:
	GLuint which_bomb_texture;
	if ((bombBegin + 2) > time(NULL)){
		which_bomb_texture = explodeTexture;
	}
	else{
		which_bomb_texture = bombTexture;
	}
    if (level == 1){
	hud->renderBomb(which_bomb_texture,bomb_posx,bomb_posy,bomb_width,bomb_height);
    }

	//Draw the paddle    
	glColor3f(0.0, 0.5, 0.5);
	paddle1.render();
	glColor3f(0.7, 0.5, 0.0);
	paddle2.render();
	glEnd();
	//Draw the ball
	ball.render();

	//If option 2 is selected i.e. ninja_robot.ppm is the background
	//Level 2 selected
	//Draw some obstacles to showcase a difference between level 1 and level2
	if(level == 2) {
		obstacle->render();
	}
	hud->showTimer(getTimer());
}
