#include "main.h"

#ifndef SEA_H
#define SEA_H


class Sea {
public:
    Sea() {}
    Sea(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    double speed;
private:
    VAO *object;
};

#endif // SEA_H
