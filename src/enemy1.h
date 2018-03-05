#include "main.h"

#ifndef ENEMY1_H
#define ENEMY1_H


class Enemy1 {
public:
    Enemy1() {}
    Enemy1(float x, float y, float z, color_t color);
    glm::vec3 position;
    int lives;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
