#include "main.h"

#ifndef ISLAND_H
#define ISLAND_H


class Island {
public:
    Island() {}
    Island(float x, float y, float z, float radius, color_t color);
    glm::vec3 position;
    float rotation, radius;
    int currentSize;
    void draw(glm::mat4 VP);
private:
    VAO *object;
};

#endif // BALL_H
