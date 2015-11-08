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
#include "Ball.h"
#include "player.h"
#include <GL/glx.h>

using namespace std;
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

GLuint generateTransparentTexture(GLuint texture, Ppmimage * image)
{

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                            image->width, image->height,
                            0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    GLuint silhouetteTexture;
    glGenTextures(1, &silhouetteTexture);

    glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData = buildAlphaData(image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    delete [] silhouetteData;
    return silhouetteTexture;
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

void convertToRGBA(Ppmimage *picture)
{
    int w = picture->width;
    int y = picture->height;

    unsigned char *silhouetteData = buildAlphaData(picture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, y, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    delete [] silhouetteData;
}

unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    //newdata = (unsigned char *)malloc(img->width * img->height * 4);
    newdata = new unsigned char[img->width * img->height * 4];
    ptr = newdata;
    for (int i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //
        //get the alpha value
        //

        *(ptr+3) = (a|b|c);
        //
        ptr += 4;
        data += 3;
    }
    return newdata;
}


/*======
GameObject
========*/
GameObject::GameObject(float xPos, float yPos, float width, float height) {
	this->xPos = xPos;
	this->yPos = yPos;
	this->width = width;
	this->height = height;
}

//TEMP, will remove
GameObject::GameObject() {
	this->xPos = 1250 / 2.0;
	this->yPos = 900 / 2.0;
	this->width = 50.f;
	this->height = 250.f;
}

void GameObject::setXPos(float xPos) {
	this->xPos = xPos;
}

void GameObject::setYPos(float yPos) {
	this->yPos = yPos;
}

void GameObject::setWidth(float width) {
	this->width = width;
}

void GameObject::setHeight(float height) {
	this->height = height;
}

float GameObject::getXPos() {
	return this->xPos; 
}

float GameObject::getYPos() {
	return this->yPos;
}

float GameObject::getWidth() {
	return this->width;
}

float GameObject::getHeight() {
	return this->height;
}

void GameObject::render() {
	glPushMatrix();
	glTranslatef(this->xPos, this->yPos, 0);
	glRectf(0.0f, 0.0f, width, height);
	glEnd();
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}



/*======
PowerUps

=======*/



/*======
Obstacles
Obstacles can be 2d polygons that
have collision detection

Examples 
 Moving platforms

=======*/
Obstacle::Obstacle(int numOfPoints):GameObject() {
	this->numOfPoints = numOfPoints;
}

void Obstacle::render() {
    glColor3ub(50,50,50);
    glPushMatrix();
    glTranslatef(this->getXPos(), this->getYPos(), 0);
    glRectf(0.0f, 0.0f, this->getWidth(), this->getHeight());
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
}

void Obstacle::checkCollision(int xres, int yres, Ball &ball, Player &player) {
    //This function will check if the ball collides with
    //the obstacle
    float ballspeed = 15.0f;
    float ballXVel = ballspeed * cos(0)+10;
    float ballYVel = ballspeed * -sin(35);

    int xPos = this->getXPos();
    int yPos = this->getYPos();
    int width = this->getWidth();
    int height = this->getHeight();

    //Boundries
    int leftWall = xPos - (width / 2);
    int rightWall = xPos + (width / 2);
    int topWall = yPos - (width / 2);
    int bottomWall = yPos + (width / 2);

    //Moving to the right
    if((ball.getXPos()-ball.getRadius() <= xPos) &&
            ball.getYPos() >= yPos && ball.getYPos() <= yPos + height) {
        ball.setXVel(ballXVel);
    }


}

/*===================
Sockets/Multiplayer?
===================*/
