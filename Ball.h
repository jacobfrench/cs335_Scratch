#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <GL/glx.h>
#include <stdlib.h>
#include "ppm.h"

class Ball{

private:
  float xPos;
  float yPos;
	float yVel;
	float xVel;
	float radius;
	int player1Score;
	int player2Score;
  Ppmimage *ballImage;

public:
    void render();
	void setXVel(float xVel);
	float getXVel();

	void setYVel(float yVel);
	float getYVel();

	void setXPos(float xPos);
	float getXPos();

	void setYPos(float yPos);
	float getYPos();

	void setRadius(float radius);
	float getRadius();

	void checkCollision(float xres, float yres);

	int getPlayer1Score();
	int getPlayer2Score();

    Ball();
    ~Ball();
};
#endif // BALL_H

