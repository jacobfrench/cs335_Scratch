#include "Ball.h"

Ball::Ball(){

}

Ball::~Ball(){

}

void Ball::render(){
    int i;
    int triangleAmount = 20;

    float twicePi = 2.0f * 3.14159265358979f;

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.0f,1.0f,10.0f);
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
    float ballSpeed = 10.0f;

    //check collision with screen edges
    if(yPos >= yres && yVel > 0){
        yVel = -ballSpeed;
    }
    else if(yPos <= 0 && yVel < 0){
        yVel = ballSpeed;
    }
    else if(xPos >= xres && xVel > 0){
        xVel = -ballSpeed;
    }
    else if(xPos <= 0 && xVel < 0){
        xVel = ballSpeed;
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
