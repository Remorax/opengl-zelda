#include "main.h"

#ifndef HEALTHBARS_H
#define HEALTHBARS_H


class HealthBars {
public:
    HealthBars() {}
    HealthBars(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *object;
};

#endif // HEALTHBARS_H
