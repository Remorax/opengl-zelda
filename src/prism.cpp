#include "prism.h"
#include "main.h"

Prism::Prism(float x, float y, float z, color_t color, float rot) {
    this->position = glm::vec3(x, y, z);
    this->rotation = rot;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        0.0f,0.0f,0.0f, //triangle 1 : begin
        0.5f,0.0f,-1.0f,
        -0.5f, 0.0f,-1.0f,//triangle 1 : end

        -0.5f, 0.0f,-1.0f, //triangle 2 : begin
        -0.5f, 1.0f,-1.0f,
        0.0f,0.0f,0.0f,

        0.0f,0.0f,0.0f,
        -0.5f, 1.0f,-1.0f,
        0.0f,1.0f,0.0f,

        0.0f,0.0f,0.0f,
        0.5f, 0.0f,-1.0f,
        0.0f,1.0f,0.0f,

        0.5f, 0.0f,-1.0f,
        0.5f, 1.0f,-1.0f,
        0.0f,0.0f,0.0f,

        0.5f, 1.0f,-1.0f,
        -0.5f, 0.0f,-1.0f,
        0.5f, 0.0f,-1.0f,

        -0.5f, 0.0f,-1.0f,
        -0.5f, 1.0f,-1.0f,
        0.5f, 1.0f,-1.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 7*3, vertex_buffer_data, color, GL_FILL);
}

void Prism::draw(glm::mat4 VP, glm::vec3 v) {
    Matrices.model = glm::mat4(1.0f);
    v = this->position - v;
    glm::mat4 translate1 = glm::translate (v);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * translate1 * rotate * inverse(translate1));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Prism::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Prism::bob(int sign) {
    // printf("sign: %d\n", sign);
    this->position.y += sign * 0.0003;
}