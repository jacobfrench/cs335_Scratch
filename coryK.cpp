/*Author: Cory Kitchens
* Program Name: coryK.cpp
*
* This file defines classes
* Helper functions for graphics/textures
* Class methods for base GameObject, PowerUps, and Obstacles
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "coryK.h"
#include "ppm.h"
#include "Ball.h"
#include "player.h"
#include <GL/glx.h>
#include "brianC.h"

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
GameObject::GameObject(float xPos, float yPos, float width, float height)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->width = width;
	this->height = height;
}

GameObject::GameObject() 
{
	this->xPos = (1250 / 2.0) - 25;
	this->yPos = 900 / 2.0;
	this->width = 50.f;
	this->height = 250.f;
	setYVel(-5.0f);
}

void GameObject::setXPos(float xPos) 
{
	this->xPos = xPos;
}

void GameObject::setYPos(float yPos) 
{
	this->yPos = yPos;
}

void GameObject::setWidth(float width) 
{
	this->width = width;
}

void GameObject::setHeight(float height) 
{
	this->height = height;
}

float GameObject::getXPos() 
{
	return this->xPos; 
}

float GameObject::getYPos() 
{
	return this->yPos;
}

float GameObject::getWidth() 
{
	return this->width;
}

float GameObject::getHeight()
{
	return this->height;
}

void GameObject::setYVel(float yVel)
{
	this->yVel = yVel;
	this->yPos += yVel;
}

float GameObject::getYVel()
{
	return yVel;
}

void GameObject::render() 
{
	glPushMatrix();
	glTranslatef(this->xPos, this->yPos, 0);
	glRectf(0.0f, 0.0f, width, height);
	glEnd();
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

Obstacle::Obstacle():GameObject()
{

}

void Obstacle::render() 
{
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


void Obstacle::checkCollision(int xres, int yres, Ball &ball, Player &player) 
{
	int player_score = player.getScore();
	player_score--;
	float ballspeed = 15.0f;
	float ballXVel = ballspeed * cos(0)+10;

	int xPos = this->getXPos();
	int yPos = this->getYPos();
	int width = this->getWidth();
	int height = this->getHeight();

	bool onLeftSide = (ball.getXPos() < xres/2);
	bool onRightSide = (ball.getXPos() > xres/2);
    
	//Ball moving to the right
	if(onLeftSide && ball.getXVel() > 0 && ball.getXPos() >= xPos && ball.getYPos() >= yPos && ball.getYPos() <= yPos + height){
		createSound(6); 
    ball.setXVel(-ballXVel);
	}
	//Ball moving to the left
	else if(onRightSide && ball.getXVel() < 0 && ball.getXPos() <= xPos+width && ball.getYPos() >= yPos && ball.getYPos() <= yPos+height){
		createSound(6); 
		ball.setXVel(ballXVel);
	}

	//Obstacle movment
	float obstacleSpeed = 5.0f;
	//if object hits bottom of screen
	if(yPos <= 0.0f){
		setYVel(obstacleSpeed);
	}
	//if object hits top of screen
	else if(yPos + height >= yres){
		setYVel(-obstacleSpeed);
	}
}

Portal::Portal()
{

}

void Portal::setPortalType(int type)
{
	if(type == 0) {
		this->setXPos(250);
		this->setYPos(100);
	}

	if(type == 1) {
		this->setXPos(900);
		this->setYPos(400);
	}
	portalType = type;
}

void Portal::render(GLuint portalTexture)
{
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, portalTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(this->getXPos() + this->getWidth(), this->getYPos() + this->getHeight());
	glTexCoord2f(0.0f, 0.0f); glVertex2i(this->getXPos() + this->getWidth(), this->getYPos());
	glTexCoord2f(1.0f, 0.0f); glVertex2i(this->getXPos(), this->getYPos());
	glTexCoord2f(1.0f, 1.0f); glVertex2i(this->getXPos(), this->getYPos() + this->getHeight());
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}



/*======
High score
=======*/
int setHighScore(int p1Score, int p2Score)
{
	int finalHighScore = 0;
	//test if scores are actually there
	if(p1Score >= 0 || p2Score >= 0){

		ifstream highScoreFile;
		ofstream finalScoreFile;
		string line;
		highScoreFile.open("highscores.txt");

		if(highScoreFile.is_open()) {
			int currentLocalScore = (p1Score > p2Score) ? p1Score : p2Score;
			while(!highScoreFile.eof()) {
				while(getline(highScoreFile, line)) {
					int scoreFromFile = atoi(line.c_str());
					finalHighScore = (scoreFromFile > currentLocalScore) ? scoreFromFile : currentLocalScore;
				}
			}
			highScoreFile.close();
			finalScoreFile.open("highscores.txt");
			finalScoreFile << finalHighScore;
			finalScoreFile.close();
		} else {
			cout << "Error";
		}
	}
	return finalHighScore;
}
