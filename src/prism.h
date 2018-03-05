#include "main.h"

#ifndef PRISM_H
#define PRISM_H


class Prism {
public:
    Prism() {}
    Prism(float x, float y, float z, color_t color, float rot);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, glm::vec3 v);
    void bob(int sign);
    void set_position(float x, float y);
private:
    VAO *object;
};

#endif // PRISM_H
