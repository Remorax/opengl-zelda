#include "main.h"

#ifndef CYLINDER_H
#define CYLINDER_H


class Cylinder {
public:
    Cylinder() {}
    Cylinder(float x, float y, float z, float radius, color_t color);
    glm::vec3 position;
    float rotation, radius;
    int currentSize;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
