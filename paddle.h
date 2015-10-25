#ifndef PADDLE_H
#define PADDLE_H

#include <cmath>
#include <GL/glx.h>
#include "Ball.h"


class Paddle{

private:
    float xPos;
    float yPos;
	float width;
	float height;
    float yVel;

public:
    void render();
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
    void checkCollision(int yres, Ball &ball);
    Paddle();
    ~Paddle();
};

#endif // PADDLE_H

