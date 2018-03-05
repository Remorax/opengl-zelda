#include "main.h"

#ifndef ENEMY3_H
#define ENEMY3_H


class Enemy3 {
public:
    Enemy3() {}
    Enemy3(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    int lives;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
