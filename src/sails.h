#include "main.h"

#ifndef SAILS_H
#define SAILS_H


class Sails {
public:
    Sails() {}
    Sails(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation, speedHor, speedVert, deltaRot;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void moveLeft();
    void moveRight();
    void moveAhead();
    void moveBehind();
    void bob(int sign);
    bounding_box_t bounding_box();
    double speed;
private:
    VAO *object;
};

#endif // SAILS_H
