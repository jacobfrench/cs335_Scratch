#include "Ball.h"
#include "paddle.h"
#include "timer.h"
#include "brianC.h"

#define PI 3.141592653589

Ball::Ball(const int in_xres, const int in_yres)
{
	this->player1Score = 0;
	this->player2Score = 0;
	xres = in_xres;
	yres = in_yres;
}

void Ball::setResolution(int in_xres, int in_yres)
{
	this->xres = in_xres;
	this->yres = in_yres;
}

void Ball::resetScore()
{
	this->player1Score = 0;
	this->player2Score = 0;
}

Ball::~Ball(){

}

void Ball::render()
{
	int i;
	int triangleAmount = 20;
	float twicePi = 2.0f * 3.14159265358979f;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex2f(xPos, yPos);
		for(i = 0; i <= triangleAmount;i++) {
			glVertex2f(xPos + (radius * cos(i *  twicePi / triangleAmount)), 
				yPos + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}



bool Ball::checkCollision(float xres, float yres)
{    
	float ballspeed = 10.0f;
	float ballXVel = ballspeed * cos(0) + 10;
	float ballYVel = ballspeed * -sin(35);
    
	bool hitTopOfScreen = yPos >= yres && yVel > 0;
	bool hitBottomOfScreen = yPos <= 0 && yVel < 0;
	bool hitRightSide = xPos >= xres && xVel > 0;
	bool hitLeftSide = xPos <= 0 && xVel < 0;    
   
	//check collision with screen edges
	if(hitTopOfScreen){
		createSound(2);
		yVel = -ballYVel;
	}
	else if(hitBottomOfScreen){
		createSound(2);
		yVel = ballYVel;
	}
	else if(hitRightSide){
		this->xVel = -ballXVel;
		player1Score++;
		createSound(3);
		return true;
	}
	else if(hitLeftSide){
		xVel = ballXVel;
		player2Score++;
		createSound(4);
		return true;
	}
	return false;
}

void Ball::setXVel(float xVel)
{
	this->xVel = xVel;
	this->xPos += xVel;
}

float Ball::getXVel()
{
	return xVel;
}

void Ball::setYVel(float yVel)
{
	this->yVel = yVel;
 	this->yPos += yVel;
}

float Ball::getYVel()
{
	return yVel;
}

void Ball::setXPos(float xPos)
{
	this->xPos += xPos;
}

float Ball::getXPos()
{
	return xPos;
}

void Ball::setYPos(float yPos)
{
	this->yPos += yPos;
}

float Ball::getYPos(){
	return yPos;
}

void Ball::setRadius(float radius)
{
	this->radius = radius;
}

float Ball::getRadius()
{
	return radius;
}

int Ball::getPlayer1Score()
{
	return player1Score;
}

int Ball::getPlayer2Score()
{
	return player2Score;
}


Paddle::Paddle(const int in_yres)
{
	this->yres = in_yres;
}

void Paddle::setWindowHeight(int in_yres)
{
	this->yres = in_yres;
}

Paddle::~Paddle()
{
	isCpu = false;
}

void Paddle::render()
{
	glPushMatrix();
	glTranslatef(xPos, yPos, 0);
	glRectf(0.0f, 0.0f, width, height);
	glEnd();
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void Paddle::setXPos(float xPos)
{
	this->xPos = xPos;
}

float Paddle::getXPos()
{
	return xPos;
}

void Paddle::setYPos(float yPos)
{
	this->yPos = yPos;
}

float Paddle::getYPos()
{
	return yPos;
}

void Paddle::setYVel(float yVel)
{
	this->yVel = yVel;
	this->yPos += yVel;
}

float Paddle::getYVel()
{
	return yVel;
}

void Paddle::setHeight(float height)
{
	this->height = height;
}

float Paddle::getHeight()
{
	return height;
}

void Paddle::setWidth(float width)
{
	this->width = width;
}

float Paddle::getWidth()
{
	return width;
}

bool Paddle::checkCollision(int yres, Ball &ball)
{
	float ballspeed = 15.0f;
	float ballXVel = ballspeed * cos(0)+10;
	float ballYVel = ballspeed * -sin(35);
    
	//ball collision with paddles
	bool onLeftSide = ball.getXPos() < 150 && xPos < 150;
	bool onRightSide = ball.getXPos() > 150 && xPos > 150;
	bool hitLeftPaddle = (ball.getXPos()-ball.getRadius() <= xPos) &&
		ball.getYPos() >= yPos && ball.getYPos() <= yPos + height;
	bool hitRightPaddle  = (ball.getXPos() >= xPos) &&
		ball.getYPos() >= yPos && ball.getYPos() <= yPos + height;


	//check if paddle is moving up or down
	bool paddleMovingUp = yVel > 0;
	bool paddleMovingDown = yVel < 0;


	checkAI(yres, ball);
	checkScreenCollision(yres);
	float angle = PI/2;


	//collision with ball
	//left paddle
	if(onLeftSide && hitLeftPaddle){
		ball.setXVel(ballXVel);
		createSound(1);
        
		if(paddleMovingUp){
			ballYVel = ballspeed * -sin(-angle);
			ball.setYVel(ballYVel);
			ball.setXVel(ballXVel);
		}
		else if(paddleMovingDown){
			ballYVel = ballspeed * -sin(-angle);
			ball.setYVel(-ballYVel);
			ball.setXVel(ballXVel);
		}
		return true;
	}
	//right paddle
	else if(onRightSide && hitRightPaddle){
		ball.setXVel(-ballXVel);
		createSound(1);
		if(paddleMovingUp){
			ballYVel = ballspeed * -sin(angle);
			ball.setYVel(ballYVel);
			ball.setXVel(-ballXVel);
		}
		else if(paddleMovingDown){
			ballYVel = ballspeed * -sin(-angle);
			ball.setYVel(-ballYVel);
			ball.setXVel(-ballXVel);
		}
		return true;
	}
	

	return false;
}

void Paddle::checkAI(int yres, Ball &ball)
{
	//If paddle is CPU
	int center = yPos + (height / 2);
	int top = yPos+height;
	int bottom = yPos;
	float absoluteBallYVel = abs(ball.getYVel());
	if(isCpu){
		if(ball.getYPos() == center){
			this->setYVel(0.0f);
		}
		else{
			if(ball.getYPos() > center){
				this->setYVel(absoluteBallYVel - 0.5f);
			}
			else if(ball.getYPos() < center){
				this->setYVel(-absoluteBallYVel + 0.5f);
			}

		}
		checkScreenCollision(yres);
		
	}


}

void Paddle::checkScreenCollision(int yres)
{
	bool hitTop = yPos + height >= yres && yVel > 0;
	bool hitBottom = yPos <= 0 && yVel < 0;
	if(hitTop){
		yPos = yres - height;
	}
	else if(hitBottom){
		yPos = 0;
	}

}

void Paddle::setCpuPlayer(bool isCpu)
{
	this->isCpu = isCpu;
}

Timer::Timer()
{
	startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}

bool Timer::isStarted()
{
	return started;
}

bool Timer::isStopped()
{
	return !started;
}

bool Timer::isPaused()
{
	return paused;
}

bool Timer::isActive()
{
	return !paused && started;
}

void Timer::pause()
{
	if( paused || !started )
		return;      
	paused = true;
	pausedAt = clock();
}

void Timer::resume()
{
	if( !paused )
		return;
        
	paused = false;
	startedAt += clock() - pausedAt;
}

void Timer::stop()
{
	started = false;
}

void Timer::start()
{
	if( started )
		return;
        
	started = true;
	paused = false;
	startedAt = clock();
}

void Timer::reset()
{
	paused = false;
	startedAt = clock();
}

clock_t Timer::getTicks()
{
	if( !started )
		return 0;
	if( paused )
		return pausedAt - startedAt;        
	return clock() - startedAt;
}
