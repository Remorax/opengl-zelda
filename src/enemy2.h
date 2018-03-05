#include "main.h"

#ifndef ENEMY2_H
#define ENEMY2_H


class Enemy2 {
public:
    Enemy2() {}
    Enemy2(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    int lives, deltaRot, flag;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void move();
    void moveAhead();
    void moveBehind();
    void moveLeft();
    void moveRight();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
