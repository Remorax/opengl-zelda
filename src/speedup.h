#include "main.h"

#ifndef SPEEDUP_H
#define SPEEDUP_H


class Speedup {
public:
    Speedup() {}
    Speedup(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    int chances, deltaRot;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
