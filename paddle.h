#ifndef PADDLE_H
#define PADDLE_H

#include <cmath>
#include <GL/glx.h>
#include "Ball.h"
#include <stdlib.h>


class Paddle{
	private:
		float xPos;
		float yPos;
		float width;
		float height;
		float yVel;    
		int yres;
	public:
		void render();
		void setWindowHeight(int in_yres);
		void setXPos(float xPos);
		float getXPos();
		void setYPos(float yPos);
		float getYPos();
		void setWidth(float width);
		float getWidth();
		void setHeight(float height);
		float getHeight();
		void setYVel(float yVel);
		float getYVel();
		bool checkCollision(int yres, Ball &ball);
		Paddle(const int yres);
		~Paddle();
};
#endif // PADDLE_H

