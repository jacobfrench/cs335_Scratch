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
extern void renderTexture(GLuint texture, int width, int height);

//Base Game Object class
//This class can be used to derive from
//i.e. x and y coordinates, and size
class GameObject {
	private:
		float xPos;
		float yPos;
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
		~GameObject();

};

class Obstacle: public GameObject {
	private:
		int numOfPoints;
	public:
		Obstacle(int numOfPoints);
		void checkCollision(int xres, int yres, Ball &ball, Player &player);
		void render();

};

class PowerUp: GameObject {
	private:
		Ppmimage* image;
		GLuint textureId;
	public:
		PowerUp(Ppmimage *image, GLuint textureId);

};



#endif
