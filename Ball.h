#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <GL/glx.h>

class Ball{

private:
    float xPos;
    float yPos;
	float yVel;
	float xVel;
	float radius;

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

    Ball();
    ~Ball();
};
#endif // BALL_H

