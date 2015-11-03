#include "Ball.h"
#include "paddle.h"

Ball::Ball(const int in_xres, const int in_yres){
    this->player1Score = 0;
    this->player2Score = 0;
    xres = in_xres;
    yres = in_yres;
}

void Ball::setResolution(int in_xres, int in_yres){
    xres = in_xres;
    yres = in_yres;
}

Ball::~Ball(){

}

void Ball::render(){
    int i;
    int triangleAmount = 20;

    float twicePi = 2.0f * 3.14159265358979f;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(1.0f,1.0f,10.0f);
        glVertex2f(xPos, yPos); // center of circle
        for(i = 0; i <= triangleAmount;i++) {
            glVertex2f(
                    xPos + (radius * cos(i *  twicePi / triangleAmount)),
                yPos + (radius * sin(i * twicePi / triangleAmount))
            );
        }
    //glTranslatef(xPos, yPos, 0);
    glEnd();
}

void Ball::checkCollision(float xres, float yres){
    
    float ballspeed = 15.0f;
    float ballXVel = ballspeed * cos(0) + 10;
    float ballYVel = ballspeed * -sin(35);
    
    

    //check collision with screen edges
    //y-axis
    if(yPos >= yres && yVel > 0){
        yVel = -ballYVel;
       
    }
    else if(yPos <= 0 && yVel < 0){
        yVel = ballYVel;
    }
    //x-axis
    else if(xPos >= xres && xVel > 0){
        xVel = -ballXVel;
        player1Score++;
    }
    else if(xPos <= 0 && xVel < 0){
        xVel = ballXVel;
        player2Score++;
    }

}

void Ball::setXVel(float xVel){
    this->xVel = xVel;
    this->xPos += xVel;
}

float Ball::getXVel(){
    return xVel;
}

void Ball::setYVel(float yVel){
    this->yVel = yVel;
    this->yPos += yVel;
}

float Ball::getYVel(){
    return yVel;
}

void Ball::setXPos(float xPos){
    this->xPos += xPos;
}

float Ball::getXPos(){
    return xPos;
}

void Ball::setYPos(float yPos){
    this->yPos += yPos;
}

float Ball::getYPos(){
    return yPos;
}

void Ball::setRadius(float radius){
    this->radius = radius;
}

float Ball::getRadius(){
    return radius;
}

int Ball::getPlayer1Score(){
    return player1Score;
}

int Ball::getPlayer2Score(){
    return player2Score;
}


Paddle::Paddle(const int in_yres){
yres = in_yres;
}

void Paddle::setWindowHeight(int in_yres){
    yres = in_yres;
}

Paddle::~Paddle(){

}

void Paddle::render(){

    glPushMatrix();
    glTranslatef(xPos, yPos, 0);
    glRectf(0.0f, 0.0f, width, height);
    glEnd();
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();

}

void Paddle::setXPos(float xPos){
    this->xPos = xPos;
}

float Paddle::getXPos(){
    return xPos;
}

void Paddle::setYPos(float yPos){
    this->yPos = yPos;
}

float Paddle::getYPos(){
    return yPos;
}

void Paddle::setYVel(float yVel){
    this->yVel = yVel;
    this->yPos += yVel;
}

float Paddle::getYVel(){
    return yVel;
}

void Paddle::setHeight(float height){
    this->height = height;
}

float Paddle::getHeight(){
    return height;
}

void Paddle::setWidth(float width){
    this->width = width;
}

float Paddle::getWidth(){
    return width;
}

void Paddle::checkCollision(int yres, Ball &ball){
    float ballSpeed = 15.0f;
    float maxBounceAngle = 75;
    float relativeIntersectY = (this->getYPos()+(this->getHeight()/2));
    float normalizedTelativeIntersectionY = (relativeIntersectY/(this->getHeight()/2));
    float bounceAngle = normalizedTelativeIntersectionY + maxBounceAngle;
    
    
    
    float ballXVel = cos(bounceAngle) * ballSpeed;
    float ballYVel = -sin(bounceAngle) * ballSpeed;

    
    
    bool onLeftSide = ball.getXPos() < 150 && xPos < 150;
    bool onRightSide = ball.getXPos() > 150 && xPos > 150;
    bool hitLeftPaddle = (ball.getXPos()-ball.getRadius() <= xPos) &&
            ball.getYPos() >= yPos && ball.getYPos() <= yPos + height;
    bool hitRightPaddle  = (ball.getXPos() >= xPos)
            && ball.getYPos() >= yPos && ball.getYPos() <= yPos + height;

    //collision with edges of screen
    if(yPos + height >= yres && yVel > 0){
        yPos = yres - height;
    }
    else if(yPos <= 0 && yVel < 0){
        yPos = 0;
    }

    //collision with ball
    if(onLeftSide && hitLeftPaddle){
        ball.setXVel(ballXVel);
        //moving up
        if(yVel > 0){
            ball.setYVel(ballYVel);
            ball.setXVel(-ballXVel);
        }
        //moving down
        else if(yVel < 0)
            ball.setYVel(ballYVel);
            ball.setXVel(ballXVel);
    }
    else if(onRightSide && hitRightPaddle){
        ball.setXVel(-ballXVel);
        if(yVel > 0){
            ball.setYVel(ballYVel);
            ball.setXVel(-ballXVel);
        }
        else if(yVel < 0)
            ball.setYVel(-ballYVel);
    }

}
