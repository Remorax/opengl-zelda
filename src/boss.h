#include "main.h"

#ifndef BOSS_H
#define BOSS_H


class Boss {
public:
    Boss() {}
    Boss(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    int lives, flag, deltaRot;
    void draw(glm::mat4 VP);
    void move();
    void moveAhead();
    void moveBehind();
    void moveLeft();
    void moveRight();
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
