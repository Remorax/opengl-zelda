#include "main.h"
#include "target.h"

Target::Target(float x, float y, float z, float radius, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->radius = radius;
    float theta = 0;
    currentSize = 0;
    const int numPoints=120;
    GLfloat g_vertex_buffer_data[numPoints*3] = {};

    while (theta < 360) {

        GLfloat x = (GLfloat) radius * cosf(theta * M_PI / 180.0f);
        GLfloat y = (GLfloat) radius * sinf(theta * M_PI / 180.0f);
        GLfloat z1 = z;

        g_vertex_buffer_data[currentSize++] = x;
        g_vertex_buffer_data[currentSize++] = y;
        g_vertex_buffer_data[currentSize++] = z;
        theta = theta + (360/numPoints);
    }
    this->object = create3DObject(GL_TRIANGLE_FAN, 120, g_vertex_buffer_data, color, GL_POINT);
}

void Target::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

