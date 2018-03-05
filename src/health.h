#include "main.h"

#ifndef HEALTH_H
#define HEALTH_H


class Health {
public:
    Health() {}
    Health(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    VAO *object;
};

#endif // WAVES_H
