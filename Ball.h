#include <cmath>
#include <GL/glx.h>

class Ball{

private:
    float xPos;
    float yPos;

public:
    void render(float xPos, float yPos, float radius);
    Ball();
    ~Ball();
};


