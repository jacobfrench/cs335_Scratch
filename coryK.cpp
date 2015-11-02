/*Author: Cory Kitchens
 * Programe Name: coryK.cpp
 * Purpose: Submits score to a php file
 * that reads through submitted scores and displays high scores
 *
 * */

#include <iostream>
#include <stdlib.h>
#include "coryK.h"
#include "ppm.h"
#include <GL/glx.h>

using namespace std;



//TODO
//Create class that holds constant strings
//to paths to images
/*class ImageContainer {
public:
	static string BG_IMAGE_PATH = "./images/pipboy.ppm";
	static string ATOM_IMAGE_PATH = "./images/atom.ppm";

	ImageContainer();
	~ImageContainer();
};
*/

void submitScore() 
{
  char postToWeb[] = "curl --data param1=5 http://cs.csubak.edu/~ckitchens/cs335/finalproject/index.php";
  system(postToWeb);
}


Ppmimage *loadImage(const char* filename)
{
	return ppm6GetImage(filename);
}

GLuint generateTexture(GLuint texture, Ppmimage * image)
{

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
							image->width, image->height,
							0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
	return texture;
}

void renderTexture(GLuint texture, int width, int height) 
{
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, height);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(width, height);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(width, 0);
	glEnd();
}

/*======
Class: Obstacle
Purpose: This class is a blueprint for
creating various obstacles in the game world

Examples :
	Moving Platforms


=======*/
class Obstacle {
 private:
 	float xPos;
 	float yPos;
 	float width;
 	float height;
 	int yres;
 public:
 	Obstacle();
 	~Obstacle();
 	void render();
};

Obstacle::Obstacle() {
	//TODO
}

void Obstacle::render() {
	glPushMatrix();
	glTranslatef(xPos, yPos, 0);
	glRectf(0.0f, 0.0f, width, height);
	glEnd();
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

/*======
PowerUps
=======*/
struct test {
 int a;
 int b;
};



/*===================
Sockets/Multiplayer?
===================*/