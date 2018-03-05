#include "enemy2.h"
#include "main.h"

Enemy2::Enemy2(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->lives = 1;
    this->flag = (z<0)?-1:1;
    deltaRot = 1;
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
        0.5f, 1.0f,-1.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 7*3, vertex_buffer_data, color, GL_FILL);
}

void Enemy2::draw(glm::mat4 VP) {
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

void Enemy2::moveLeft() {
    this->rotation -= deltaRot;
}

void Enemy2::moveRight() {
    this->rotation += deltaRot;
}

void Enemy2::moveAhead() {
    this->position.z -= 0.1 * cosf(this->rotation * M_PI / 180.0f);
    this->position.x -= 0.1 * sinf(this->rotation * M_PI / 180.0f);
}

void Enemy2::moveBehind() {
    this->position.z += 0.1 * cosf(this->rotation * M_PI / 180.0f);
    this->position.x += 0.1 * sinf(this->rotation * M_PI / 180.0f);
}

void Enemy2::move() {
    int x = rand()%10;
    if (x<6){
        if(((this->flag==-1)&&(this->position.z<=0))||((this->flag==1)&&(this->position.z<=0)))
                this->moveBehind();
            else
                this->moveAhead();
    }
    // else if (x==1)
    //     this->moveBehind();
    else if (x<8)
        this->moveLeft();
    else
        this->moveRight();
}

void Enemy2::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Enemy2::bounding_box() {
    float x = this->position.x, y = this->position.y, z =this->position.z;
    bounding_box_t bbox = { x, y, z, 1.5, 1.5, 1.5 };
    return bbox;
}