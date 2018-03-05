#include "main.h"

#ifndef TARGET_H
#define TARGET_H


class Target {
public:
    Target() {}
    Target(float x, float y, float z, float radius, color_t color);
    glm::vec3 position;
    float rotation, radius;
    int currentSize;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
