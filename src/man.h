#include "main.h"

#ifndef MAN_H
#define MAN_H


class Man {
public:
    Man() {}
    Man(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    int deltaRot, lives;
    void draw(glm::mat4 VP);
    void moveAhead();
    void moveBehind();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
