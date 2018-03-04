#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation, speedHor, speedVert, deltaRot;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void moveLeft();
    void moveRight();
    void moveAhead();
    void moveBehind();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
