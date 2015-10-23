#include <cmath>
#include <GL/glx.h>

class Ball{

private:
    float xPos;
    float yPos;

public:
    void render(float k, float r, float h);
    Ball();
    ~Ball();
};


