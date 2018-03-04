#include "main.h"
#include "timer.h"
#include "boat.h"
#include "sea.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Boat boat;
Sea sea;
Boat rocks[25];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float eyex, eyey, eyez, targetx, targety, targetz, normalx, normaly, normalz;
char viewMode = 'f';
bool changed = false;
char prevState = 'f';

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    eyex = boat.position.x + 5*sin(camera_rotation_angle*M_PI/180.0f);
    // if (!zoomin &&)
    // {
    //     /* code */
    // }
    if(!changed)
        eyey = 0;
    eyez = boat.position.z + 5*cos(camera_rotation_angle*M_PI/180.0f);
    targetx = boat.position.x;
    targetz = boat.position.z;
    targety = boat.position.y;
    normalx = 0, normaly = 1, normalz = 0;

    if (viewMode=='t'){
        eyex = 5.0f;
        eyey = -10.0f;
        eyez = -5.0f;
    }
    else if (viewMode=='s'){
        eyex = boat.position.x + 5*sin(camera_rotation_angle*M_PI/180.0f);
        eyey = -10.0f;
        eyez = boat.position.z + 5*cos(camera_rotation_angle*M_PI/180.0f);
        normaly = -1;
    }
    else if (viewMode=='b'){
        eyex = boat.position.x - 5*cos(camera_rotation_angle*M_PI/180.0f);
        eyey = boat.position.y;
        eyez = boat.position.z - 5*sin(camera_rotation_angle*M_PI/180.0f);
        targetx = boat.position.x - 10;
        targetz = boat.position.z - 10;
    }
    else if (viewMode=='l'){
        double x,y;
        // printf("%d\n", eyex);
        glfwGetCursorPos(window, &x, &y);
        targetx = (float)x;
        targetz = (float)y;
        changed = true;
    }
    // printf("2.%f %f %c\n", eyex,eyez, viewMode);
    glm::vec3 eye (eyex,eyey,eyez);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (targetx,targety,targetz);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (normalx,normaly,normalz);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    boat.draw(VP);
    sea.draw(VP);
    for (int i = 0; i < 25; ++i)
        rocks[i].draw(VP);
}

void changeHeight (int sign) {
    if(sign<0){
        if(eyey<-0.5){
            eyey += 0.5;
            changed = true;
            return;
        }
    }
    else {
        eyey -= 0.5;
        changed = true;
        return;
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    // int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int skyView = glfwGetKey(window, GLFW_KEY_S);
    int towerView = glfwGetKey(window, GLFW_KEY_T);
    int boatView = glfwGetKey(window, GLFW_KEY_B);
    int followView = glfwGetKey(window, GLFW_KEY_F);
    int lmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
    int rmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
    // int zin 
    if (left)
        boat.moveLeft();
    else if (right)
        boat.moveRight();
    else if(skyView)
        viewMode = 's';
    else if (boatView)
        viewMode = 'b';
    else if (followView)
        viewMode = 'f';
    else if (towerView)
        viewMode = 't';
    else if (lmb){
        // printf("%c\n", prevState);
        if(viewMode!='l')
            prevState = viewMode;
        viewMode = 'l';
    }
    else if (rmb){
        printf("%c\n", rmb);
        viewMode = prevState;
    }
    // else if (space) {
    //     isJump = 1;
    //     isFall = 0;
    //     boat.speedy = 0.1;
    //     boat.jump();
    // }
    else if (up) {
        boat.moveAhead();
    }
    else if (down) {
        boat.moveBehind();
    }
    return;
}

void tick_elements() {
    // printf("%f %f %f\n", boat.position.x, boat.position.y, boat.position.z);
    reset_screen();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat       = Boat(0, 0, 0, COLOR_RED);
    sea        = Sea(0, 2, 0, COLOR_LIGHTBLUE);
    createRocks();

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
}

void createRocks()
{
    for (int i=0; i<25; ++i)
    {
        float x = -79.5 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(159)));
        float z = -79.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/159));
        rocks[i] = Boat(x,0,z,COLOR_GRAY);
    }
    return;
}

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    if (viewMode=='s')
        Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    else
        Matrices.projection = glm::perspective(80.0f,top/right, 0.1f, 500.0f);
}
