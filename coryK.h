#ifndef _coryK_H_
#define _coryK_H_

 

#include <iostream>
#include <stdlib.h>
#include "ppm.h"
#include <GL/glx.h>
using namespace std;


extern void submitScore();
extern Ppmimage *loadImage(const char* filename);
extern GLuint generateTexture(GLuint texture, Ppmimage * image);
extern void renderTexture(GLuint texture, int width, int height);

#endif
