#include "Ball.h"
Ball::Ball(){

}

Ball::~Ball(){

}
void Ball::render(float k, float r, float h){
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < 180; i++){
        this->xPos = r * cos(i) - h;
        this->  yPos = r * sin(i) + k;
        glVertex3f(xPos + k, yPos - h, 0);

        this->xPos = r * cos(i + 0.1) - h;
        this->yPos = r * sin(i + 0.1) + k;
        glVertex3f(xPos + k, yPos - h, 0);
    }
    glEnd();
}
