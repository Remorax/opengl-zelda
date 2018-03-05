#include "enemy1.h"
#include "main.h"
// #include "cannon.h"

Enemy1::Enemy1(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->lives = 2;
    // Cannon cannon       = Cannon(-1, -1, -1, COLOR_GOLD);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.8f,-3.0f,-0.5f, //triangle 1 : begin
        -0.8f,-3.0f, 0.5f,
        -0.8f, 3.0f, 0.5f,//triangle 1 : end
        0.8f, 3.0f,-0.5f, //triangle 2 : begin
        -0.8f,-3.0f,-0.5f,
        -0.8f, 3.0f,-0.5f, //triangle 2 : end
        0.8f,-3.0f, 0.5f,
        -0.8f,-3.0f,-0.5f,
        0.8f,-3.0f,-0.5f,
        0.8f, 3.0f,-0.5f,
        0.8f,-3.0f,-0.5f,
        -0.8f,-3.0f,-0.5f,
        -0.8f,-3.0f,-0.5f,
        -0.8f, 3.0f, 0.5f,
        -0.8f, 3.0f,-0.5f,
        0.8f,-3.0f, 0.5f,
        -0.8f,-3.0f, 0.5f,
        -0.8f,-3.0f,-0.5f,
        -0.8f, 3.0f, 0.5f,
        -0.8f,-3.0f, 0.5f,
        0.8f,-3.0f, 0.5f,
        0.8f, 3.0f, 0.5f,
        0.8f,-3.0f,-0.5f,
        0.8f, 3.0f,-0.5f,
        0.8f,-3.0f,-0.5f,
        0.8f, 3.0f, 0.5f,
        0.8f,-3.0f, 0.5f,
        0.8f, 3.0f, 0.5f,
        0.8f, 3.0f,-0.5f,
        -0.8f, 3.0f,-0.5f,
        0.8f, 3.0f, 0.5f,
        -0.8f, 3.0f,-0.5f,
        -0.8f, 3.0f, 0.5f,
        0.8f, 3.0f, 0.5f,
        -0.8f, 3.0f, 0.5f,
        0.8f,-3.0f, 0.5f,

        -0.3f,-3.3f,-0.1f, // triangle 1 : begin
        -0.3f,-3.3f, 1.1f,
        -0.3f,-2.7f, 1.1f, // triangle 1 : end
        0.3f,-2.7f,-0.1f, // triangle 2 : begin
        -0.3f,-3.3f,-0.1f,
        -0.3f,-2.7f,-0.1f, // triangle 2 : end
        0.3f,-3.3f, 1.1f,
        -0.3f,-3.3f,-0.1f,
        0.3f,-3.3f,-0.1f,
        0.3f,-2.7f,-0.1f,
        0.3f,-3.3f,-0.1f,
        -0.3f,-3.3f,-0.1f,
        -0.3f,-3.3f,-0.1f,
        -0.3f,-2.7f, 1.1f,
        -0.3f,-2.7f,-0.1f,
        0.3f,-3.3f, 1.1f,
        -0.3f,-3.3f, 1.1f,
        -0.3f,-3.3f,-0.1f,
        -0.3f,-2.7f, 1.1f,
        -0.3f,-3.3f, 1.1f,
        0.3f,-3.3f, 1.1f,
        0.3f,-2.7f, 1.1f,
        0.3f,-3.3f,-0.1f,
        0.3f,-2.7f,-0.1f,
        0.3f,-3.3f,-0.1f,
        0.3f,-2.7f, 1.1f,
        0.3f,-3.3f, 1.1f,
        0.3f,-2.7f, 1.1f,
        0.3f,-2.7f,-0.1f,
        -0.3f,-2.7f,-0.1f,
        0.3f,-2.7f, 1.1f,
        -0.3f,-2.7f,-0.1f,
        -0.3f,-2.7f, 1.1f,
        0.3f,-2.7f, 1.1f,
        -0.3f,-2.7f, 1.1f,
        0.3f,-3.3f, 1.1f        
    };

    this->object = create3DObject(GL_TRIANGLES, 24*3, vertex_buffer_data, color, GL_FILL);
}

void Enemy1::draw(glm::mat4 VP) {
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

void Enemy1::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Enemy1::bounding_box() {
    float x = this->position.x, y = this->position.y, z =this->position.z;
    bounding_box_t bbox = { x, y, z, 0.8, 3.0, 0.5 };
    return bbox;
}