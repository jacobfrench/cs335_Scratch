#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <GL/glx.h>
#include <stdlib.h>

class Ball{

private:
    float xPos;
    float yPos;
	float yVel;
	float xVel;
	float radius;
	int player1Score;
	int player2Score;    

public:
    int xres;
    int yres;
    void setResolution(int in_xres,int in_yres);
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

    Ball(const int in_xres, const int yres);
    ~Ball();
};
#endif // BALL_H

