#include "main.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation, speedHor, speedVert, deltaRot, speedy;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void moveLeft();
    void windMoveLeft(int sign);
    void moveRight();
    void windMoveRight(int sign);
    void moveAhead();
    void moveBehind();
    void speedMoveAhead();
    void speedMoveBehind();
    void bob(int sign);
    int  jump();
    int  fall();
    bounding_box_t bounding_box();
    double speed;
private:
    VAO *object;
};

#endif // BALL_H
