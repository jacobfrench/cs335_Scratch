//cs335 Spring 2015
//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014
//mod spring 2015: added constructors
//
//This program is a game starting point for 335
//
// Possible requirements:
// ----------------------
// welcome screen
// menu
// multiple simultaneous key-press
// show exhaust for thrusting
// move the asteroids
// collision detection for bullet on asteroid
// collision detection for asteroid on ship
// control of bullet launch point
// life span for each bullet
// cleanup the bullets that miss a target
// split asteroids into pieces when blasted
// random generation of new asteroids
// score keeping
// levels of difficulty
// sound
// use of textures
//

#include <iostream>
#include <cstdlib>
#include <cstring>
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

int xres=1250, yres=900;



//temporary instance variables
Ball ball;
float ballXPos;
float ballYPos;
float ballXVel;
float ballYVel;

Paddle paddle1;
float paddle1YVel;


Paddle paddle2;
float paddle2YVel;


struct Game {
    bool mouseThrustOn;
    Game() {
        mouseThrustOn = false;
    }
};


int keys[65536];

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
int check_keys(XEvent *e, Game *g);
void init(Game *g);
void init_sounds(void);
void physics(Game *game);
void render(Game *game);


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


    //init ball variables
    ballXPos = xres/2;
    ballYPos = yres/2;
    ballYVel = 8.0f;
    ballXVel = 8.0f;

    //init paddle1
    paddle1.setXPos(50.0f);
    paddle1.setYPos((float)yres/2);
    paddle1.setHeight(120.0f);
    paddle1.setWidth(15.0f);

    //init paddle2
    paddle2.setXPos((float)xres - 50.0f);
    paddle2.setYPos((float)yres/2);
    paddle2.setHeight(120.0f);
    paddle2.setWidth(15.0f);

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
        render(&game);
        glXSwapBuffers(dpy, win);
    }
    cleanupXWindows();
    cleanup_fonts();
    logClose();

    return 0;
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
    //window has been resized.
    setup_screen_res(width, height);
    //
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
    //keyboard input?
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    //Log("key: %i\n", key);
    if (e->type == KeyRelease) {
        keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        if(key == XK_W)
            paddle1.setYVel(0.0f);
        return 0;
    }

    if (e->type == KeyPress) {
        keys[key]=1;
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift=1;
            return 0;
        }
    }
    else {
        return 0;
    }



    paddle1.setYPos(paddle1.getYPos());
    float paddleSpeed = 10.0f;
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




    //ball physics & collision
    float ballSpeed = 10.0f;
    if(ballYPos >= yres && ballYVel > 0){
        ballYVel = -ballSpeed;
    }
    else if(ballYPos <= 0 && ballYVel < 0){
        ballYVel = ballSpeed;
    }
    else if(ballXPos >= xres && ballXVel > 0){
        ballXVel = -ballSpeed;
    }
    else if(ballXPos <= 0 && ballXVel < 0){
        ballXVel = ballSpeed;
    }


    //paddle collision
    paddle1.checkCollision(xres, yres);
    paddle2.checkCollision(xres, yres);



    //ball movement
    ballYPos += ballYVel;
    ballXPos += ballXVel;

    //paddle1 movement
    paddle1.setYVel(paddle1YVel);

    //paddle2 movement
    paddle2.setYVel(paddle2YVel);


}

void render(Game *g)
{
    //draw text
    Rect r;
    glClear(GL_COLOR_BUFFER_BIT);
    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "cs335 - Pong!");

    //Draw the paddle
    paddle1.render();
    paddle2.render();

    //Draw the ball
    ball.render(ballXPos, ballYPos, 5.0f);

    //---------------------------------------------------------------------

}
