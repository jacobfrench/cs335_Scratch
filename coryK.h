#ifndef _coryK_H_
#define _coryK_H_

 

#include <iostream>
#include <stdlib.h>
#include "ppm.h"
#include <GL/glx.h>
#include "Ball.h"
#include "player.h"

using namespace std;


extern void submitScore();
extern Ppmimage *loadImage(const char* filename);
extern GLuint generateTexture(GLuint texture, Ppmimage * image);
extern GLuint generateTransparentTexture(GLuint texture, Ppmimage * image);
extern unsigned char *buildAlphaData(Ppmimage *img);
extern void renderTexture(GLuint texture, int width, int height);
extern void convertToRGBA(Ppmimage *picture);
extern int setHighScore(int p1Score, int p2Score);

class GameObject {
	private:
		float xPos;
		float yPos;
		float yVel;
		float width;
		float height;
	public:
		GameObject(float xPos, float yPos, float width, float height);
		GameObject();
		void render();
		void setXPos(float xPos);
		void setYPos(float yPos);
		void setWidth(float width);
		void setHeight(float height);
		float getXPos();
		float getYPos();
		float getWidth();
		float getHeight();
		void setYVel(float yVel);
		float getYVel();
		~GameObject();

};

class Obstacle: public GameObject 
{
	private:
	public:
		Obstacle();
		void checkCollision(int xres, int yres, Ball &ball, Player &player);
		void render();
		~Obstacle();
};


class Portal:Obstacle
{
	private:
		int portalType;
	public:
		Portal();
		void setPortalType(int type);
		int getPortalType();
		void render(GLuint portalTexture);
		void checkCollision(Ball &ball, Portal &portal);
		void transportBall(Ball &ball, Portal &portal);
		~Portal();
};

extern int beginTesting();

#endif
