//#include "structures.h"
#include <cstdio>
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include "fmod.h"
#include "ppm.h"
#include "brianC.h"

extern Ppmimage *bgImg;
extern GLuint bgTxtr;
void init_sounds(void)
{
    if(fmod_init()) {
    printf("ERROR");
    return;
    }
    if(fmod_createsound((char *)"./sounds/pong.wav", 0)) {
    printf("ERROR");
    return;
    }
    if(fmod_createsound((char *)"./sounds/gamemusic.wav", 1)) {
    printf("ERROR");
    return;
    }
    fmod_setmode(0,FMOD_LOOP_NORMAL);
    fmod_setmode(1,FMOD_LOOP_NORMAL);
}
  
void play_music(int a) {
    fmod_playsound(a);
}
void init(&game)(int b){
    fmod_playmusic(b);
}   
       
int done()
{
       if(done == 3){
        play_music(0);
        init(&game);
        break;
    }
    glXSwapBuffers(dpy, win);
}



return 0;
}
         
