#include "boss.h"
#include "main.h"

Boss::Boss(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->lives = 7;
    deltaRot = 1;
    this->flag = (z<0)?-1:1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -3.0f,-8.0f,-0.5f, //triangle 1 : begin
        -3.0f,-8.0f, 0.5f,
        -3.0f, 8.0f, 0.5f,//triangle 1 : end
        3.0f, 8.0f,-0.5f, //triangle 2 : begin
        -3.0f,-8.0f,-0.5f,
        -3.0f, 8.0f,-0.5f, //triangle 2 : end
        3.0f,-8.0f, 0.5f,
        -3.0f,-8.0f,-0.5f,
        3.0f,-8.0f,-0.5f,
        3.0f, 8.0f,-0.5f,
        3.0f,-8.0f,-0.5f,
        -3.0f,-8.0f,-0.5f,
        -3.0f,-8.0f,-0.5f,
        -3.0f, 8.0f, 0.5f,
        -3.0f, 8.0f,-0.5f,
        3.0f,-8.0f, 0.5f,
        -3.0f,-8.0f, 0.5f,
        -3.0f,-8.0f,-0.5f,
        -3.0f, 8.0f, 0.5f,
        -3.0f,-8.0f, 0.5f,
        3.0f,-8.0f, 0.5f,
        3.0f, 8.0f, 0.5f,
        3.0f,-8.0f,-0.5f,
        3.0f, 8.0f,-0.5f,
        3.0f,-8.0f,-0.5f,
        3.0f, 8.0f, 0.5f,
        3.0f,-8.0f, 0.5f,
        3.0f, 8.0f, 0.5f,
        3.0f, 8.0f,-0.5f,
        -3.0f, 8.0f,-0.5f,
        3.0f, 8.0f, 0.5f,
        -3.0f, 8.0f,-0.5f,
        -3.0f, 8.0f, 0.5f,
        3.0f, 8.0f, 0.5f,
        -3.0f, 8.0f, 0.5f,
        3.0f,-8.0f, 0.5f,

        1.7f,-3.3f,-0.1f, // triangle 1 : begin
        1.7f,-3.3f, 1.1f,
        1.7f,-2.7f, 1.1f, // triangle 1 : end
        2.3f,-2.7f,-0.1f, // triangle 2 : begin
        1.7f,-3.3f,-0.1f,
        1.7f,-2.7f,-0.1f, // triangle 2 : end
        2.3f,-3.3f, 1.1f,
        1.7f,-3.3f,-0.1f,
        2.3f,-3.3f,-0.1f,
        2.3f,-2.7f,-0.1f,
        2.3f,-3.3f,-0.1f,
        1.7f,-3.3f,-0.1f,
        1.7f,-3.3f,-0.1f,
        1.7f,-2.7f, 1.1f,
        1.7f,-2.7f,-0.1f,
        2.3f,-3.3f, 1.1f,
        1.7f,-3.3f, 1.1f,
        1.7f,-3.3f,-0.1f,
        1.7f,-2.7f, 1.1f,
        1.7f,-3.3f, 1.1f,
        2.3f,-3.3f, 1.1f,
        2.3f,-2.7f, 1.1f,
        2.3f,-3.3f,-0.1f,
        2.3f,-2.7f,-0.1f,
        2.3f,-3.3f,-0.1f,
        2.3f,-2.7f, 1.1f,
        2.3f,-3.3f, 1.1f,
        2.3f,-2.7f, 1.1f,
        2.3f,-2.7f,-0.1f,
        1.7f,-2.7f,-0.1f,
        2.3f,-2.7f, 1.1f,
        1.7f,-2.7f,-0.1f,
        1.7f,-2.7f, 1.1f,
        2.3f,-2.7f, 1.1f,
        1.7f,-2.7f, 1.1f,
        2.3f,-3.3f, 1.1f,

        -2.3f,-3.3f,-0.1f, // triangle 1 : begin
        -2.3f,-3.3f, 1.1f,
        -2.3f,-2.7f, 1.1f, // triangle 1 : end
        -1.7f,-2.7f,-0.1f, // triangle 2 : begin
        -2.3f,-3.3f,-0.1f,
        -2.3f,-2.7f,-0.1f, // triangle 2 : end
        -1.7f,-3.3f, 1.1f,
        -2.3f,-3.3f,-0.1f,
        -1.7f,-3.3f,-0.1f,
        -1.7f,-2.7f,-0.1f,
        -1.7f,-3.3f,-0.1f,
        -2.3f,-3.3f,-0.1f,
        -2.3f,-3.3f,-0.1f,
        -2.3f,-2.7f, 1.1f,
        -2.3f,-2.7f,-0.1f,
        -1.7f,-3.3f, 1.1f,
        -2.3f,-3.3f, 1.1f,
        -2.3f,-3.3f,-0.1f,
        -2.3f,-2.7f, 1.1f,
        -2.3f,-3.3f, 1.1f,
        -1.7f,-3.3f, 1.1f,
        -1.7f,-2.7f, 1.1f,
        -1.7f,-3.3f,-0.1f,
        -1.7f,-2.7f,-0.1f,
        -1.7f,-3.3f,-0.1f,
        -1.7f,-2.7f, 1.1f,
        -1.7f,-3.3f, 1.1f,
        -1.7f,-2.7f, 1.1f,
        -1.7f,-2.7f,-0.1f,
        -2.3f,-2.7f,-0.1f,
        -1.7f,-2.7f, 1.1f,
        -2.3f,-2.7f,-0.1f,
        -2.3f,-2.7f, 1.1f,
        -1.7f,-2.7f, 1.1f,
        -2.3f,-2.7f, 1.1f,
        -1.7f,-3.3f, 1.1f       
    };

    this->object = create3DObject(GL_TRIANGLES, 36*3, vertex_buffer_data, color, GL_FILL);
}

void Boss::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boss::moveLeft() {
    this->rotation -= deltaRot;
}

void Boss::moveRight() {
    this->rotation += deltaRot;
}

void Boss::moveAhead() {
    this->position.z -= 0.1 * cosf(this->rotation * M_PI / 180.0f);
    this->position.x -= 0.1 * sinf(this->rotation * M_PI / 180.0f);
}

void Boss::moveBehind() {
    this->position.z += 0.1 * cosf(this->rotation * M_PI / 180.0f);
    this->position.x += 0.1 * sinf(this->rotation * M_PI / 180.0f);
}

void Boss::move() {
    int x = rand()%10;
    if (x<6){
        if(((this->flag==-1)&&(this->position.z<=0))||((this->flag==1)&&(this->position.z<=0)))
                this->moveBehind();
            else
                this->moveAhead();
    }
    else if (x<8)
        this->moveLeft();
    else
        this->moveRight();
}

void Boss::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Boss::bounding_box() {
    float x = this->position.x, y = this->position.y, z =this->position.z;
    bounding_box_t bbox = { x, y, z, 3.0, 8.0, 0.5 };
    return bbox;
}