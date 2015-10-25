#include "paddle.h"
Paddle::Paddle(){

}

Paddle::~Paddle(){

}

void Paddle::render(){

    glPushMatrix();
    glTranslatef(xPos, yPos, 0);
    glRectf(0.0f, 0.0f, width, height);
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);
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

void Paddle::checkCollision(int yres){
    if(yPos + height >= yres && yVel > 0){
        yPos = yres - height;
    }
    else if(yPos <= 0 && yVel < 0){
        yPos = 0;
    }

}
