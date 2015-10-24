#include "paddle.h"
Paddle::Paddle(){

}

Paddle::~Paddle(){

}

void Paddle::render(float xPos, float yPos, float height, float width){

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
