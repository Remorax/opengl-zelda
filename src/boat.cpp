#include "boat.h"
#include "main.h"

Boat::Boat(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    deltaRot = 1;
    speedVert = 0.3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.5f,-0.4f,-1.0f, //triangle 1 : begin
        -0.5f,-0.4f, 1.0f,
        -0.5f, 0.4f, 1.0f,//triangle 1 : end
        0.5f, 0.4f,-1.0f, //triangle 2 : begin
        -0.5f,-0.4f,-1.0f,
        -0.5f, 0.4f,-1.0f, //triangle 2 : end
        0.5f,-0.4f, 1.0f,
        -0.5f,-0.4f,-1.0f,
        0.5f,-0.4f,-1.0f,
        0.5f, 0.4f,-1.0f,
        0.5f,-0.4f,-1.0f,
        -0.5f,-0.4f,-1.0f,
        -0.5f,-0.4f,-1.0f,
        -0.5f, 0.4f, 1.0f,
        -0.5f, 0.4f,-1.0f,
        0.5f,-0.4f, 1.0f,
        -0.5f,-0.4f, 1.0f,
        -0.5f,-0.4f,-1.0f,
        -0.5f, 0.4f, 1.0f,
        -0.5f,-0.4f, 1.0f,
        0.5f,-0.4f, 1.0f,
        0.5f, 0.4f, 1.0f,
        0.5f,-0.4f,-1.0f,
        0.5f, 0.4f,-1.0f,
        0.5f,-0.4f,-1.0f,
        0.5f, 0.4f, 1.0f,
        0.5f,-0.4f, 1.0f,
        0.5f, 0.4f, 1.0f,
        0.5f, 0.4f,-1.0f,
        -0.5f, 0.4f,-1.0f,
        0.5f, 0.4f, 1.0f,
        -0.5f, 0.4f,-1.0f,
        -0.5f, 0.4f, 1.0f,
        0.5f, 0.4f, 1.0f,
        -0.5f, 0.4f, 1.0f,
        0.5f,-0.4f, 1.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Boat::draw(glm::mat4 VP) {
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

void Boat::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boat::moveLeft() {
    this->rotation -= deltaRot;
}

void Boat::moveRight() {
    this->rotation += deltaRot;
}

void Boat::windMoveLeft(int sign) {
    this->rotation += (sign<0)?(deltaRot/10):(deltaRot*10);
}

void Boat::windMoveRight(int sign) {
    this->rotation += (sign<0)?(deltaRot/10):(deltaRot*10);
}

void Boat::moveAhead() {
    this->position.z -= 0.1 * cosf(this->rotation * M_PI / 180.0f);
    this->position.x -= 0.1 * sinf(this->rotation * M_PI / 180.0f);
}

void Boat::moveBehind() {
    this->position.z += 0.1 * cosf(this->rotation * M_PI / 180.0f);
    this->position.x += 0.1 * sinf(this->rotation * M_PI / 180.0f);
}

void Boat::bob(int sign) {
    // printf("sign: %d\n", sign);
    this->position.y += sign * 0.0003;
}

bounding_box_t Boat::bounding_box() {
    float x = this->position.x, y = this->position.y, z =this->position.z;
    bounding_box_t bbox = { x, y, z, 0.5, 0.4, 1.0 };
    return bbox;
}