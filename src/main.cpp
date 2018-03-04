#include "main.h"
#include "timer.h"
#include "boat.h"
#include "sea.h"
#include "rock.h"
#include "prism.h"
#include "waves.h"
#include "sails.h"
#include "healthBars.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Boat boat;
Sea sea;
Rock rocks[25];
// Prism front, rear;
Waves f,m,r;
Waves wind1, wind2, wind3, wind4, wind5, wind6;
Sails sails;
vector <HealthBars> healthBars(15);

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float eyex, eyey, eyez, targetx, targety, targetz, normalx, normaly, normalz;
char viewMode = 'f';
bool changed = false, gameover=false;
bool still = true, deleteEnemy=false;
int stillMode = 20;
int frame=0, duration=-1, angle=0;
char prevState = 'f';
float tempx=0,tempy=0,tempz=0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
    // use the loaded shader 
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!

    if(!changed)
        eyey = 0;
    if(!still || viewMode=='b'){
        eyex = boat.position.x + 5*sin(camera_rotation_angle*M_PI/180.0f);
        eyez = boat.position.z + 5*cos(camera_rotation_angle*M_PI/180.0f);
        targetx = boat.position.x;
        targetz = boat.position.z;
        targety = boat.position.y;
    }
    else {
        eyex = tempx + 5*sin(camera_rotation_angle*M_PI/180.0f);
        eyez = tempz + 5*cos(camera_rotation_angle*M_PI/180.0f);
        targetx = tempx;
        targetz = tempz;
        targety = tempy;
    }
    normalx = 0, normaly = 1, normalz = 0;

    if (viewMode=='t'){
        eyex = -5.0f;
        eyey = -10.0f;
        eyez = 5.0f;
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
    if(!still){
        f.draw(VP);
        m.draw(VP);
        r.draw(VP);
        showWaves();
        tempx = boat.position.x;
        tempy = boat.position.y;
        tempz = boat.position.z;   
    }
    else {
        boat.bob(stillMode);
        sails.bob(stillMode);
        if(stillMode>-20)
            stillMode--;
        else
            stillMode = 20;
    }
    // printf("boat:%f \n", boat.position.z);
    for (int i = 0; i < healthBars.size(); ++i){
        healthBars[i].position.z = boat.position.z - 3;
        healthBars[i].position.x = boat.position.x - 4 - 0.3*i;
    }
    // Scene render
    boat.draw(VP);
    sea.draw(VP);
    // front.draw(VP);
    // rear.draw(VP);
    sails.draw(VP);
    if(duration!=-1){
        wind1.draw(VP);
        wind2.draw(VP);
        wind3.draw(VP);
        wind4.draw(VP);
        wind5.draw(VP);
        wind6.draw(VP);
    }
    for (int i = 0; i < 25; ++i) {
        if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())){
            healthBars.pop_back();
            boat.moveBehind();
            if(healthBars.size()==0)
                gameover = true;
        }
    }
    for (int i = 0; i < 25; ++i)
        rocks[i].draw(VP);
    for (int i=0; i<healthBars.size(); ++i)
        healthBars[i].draw(VP);
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

void showWaves () {
    f.position.x = boat.position.x + 2;
    f.position.z = boat.position.z - 5;
    f.rotation = m.rotation = r.rotation = 90 + boat.rotation;
    m.position.x = boat.position.x ;
    m.position.z = boat.position.z ;
    r.position.x = boat.position.x - 2;
    r.position.z = boat.position.z - 5;
    still = false;
    return;
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int skyView = glfwGetKey(window, GLFW_KEY_S);
    int towerView = glfwGetKey(window, GLFW_KEY_T);
    int boatView = glfwGetKey(window, GLFW_KEY_B);
    int followView = glfwGetKey(window, GLFW_KEY_N);
    int lmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
    int rmb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
    // int zin
    still = true; 
    if (left){
        if(duration==-1)
            boat.moveLeft();
        else {
            boat.windMoveLeft((angle>180)?1:-1);
        }
        still = false;
    }
    else if (right){
        still = false;
        if(duration==-1)
            boat.moveRight();
        else {
            boat.windMoveRight((angle<180)?1:-1);
        }
    }
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
    else if (up) {
        still = false;
        boat.moveAhead();
    }
    else if (down) {
        still = false;
        boat.moveBehind();
    }
    return;
}

void showWind(){
    wind1.rotation = wind2.rotation = wind3.rotation = wind4.rotation = wind5.rotation = wind6.rotation = angle;
    wind1.position.x = boat.position.x - 2, wind1.position.z = boat.position.z - 2;
    wind2.position.x = boat.position.x - 3, wind2.position.z = boat.position.z - 3;
    wind3.position.x = boat.position.x - 1, wind3.position.z = boat.position.z - 1;
    wind4.position.x = boat.position.x + 3, wind4.position.z = boat.position.z + 3;
    wind5.position.x = boat.position.x + 2, wind5.position.z = boat.position.z + 2;
    wind6.position.x = boat.position.x + 1, wind6.position.z = boat.position.z + 1;
    return;
}

void tick_elements() {
    // printf("%f %f %f\n", boat.position.x, boat.position.y, boat.position.z);
    reset_screen();
    sails.position.x = boat.position.x;
    sails.position.y = boat.position.y-0.81;
    sails.position.z = boat.position.z;
    sails.rotation = boat.rotation;
    if(duration!=-1){
        showWind();
    }
    // front.position.x = boat.position.x;
    // front.position.z = boat.position.z + 2.0f;
    // if(viewMode!='s'){
    //     rear.position.z = boat.position.z - 2.0f;
    //     rear.position.x = boat.position.x + 0.2f;
    // }
    // else{
    //     rear.position.z = boat.position.z - 2.6f;
    //     rear.position.x = boat.position.x;
    // }
    
}


void createRocks()
{
    for (int i=0; i<25; ++i)
    {
        float x = -79.5 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(159)));
        float z = -79.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/159));
        rocks[i] = Rock(x,0,z,COLOR_GRAY);
    }
    return;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat       = Boat(0, 0, 0, COLOR_RED);
    // front      = Prism(-1.0, 1.0f, -2.0, COLOR_BROWN, 0);
    // rear      = Prism(1.0, 1.0f, 1.0, COLOR_GREEN, 180.0f);
    sea        = Sea(0, 2, 0, COLOR_LIGHTBLUE);
    f = Waves(-1, 1, 0, COLOR_WHITE);
    r = Waves(1, 1, 0, COLOR_WHITE);
    m = Waves(0, 1, 1, COLOR_WHITE);
    sails = Sails(0, -0.81, 0, COLOR_WHITE);
    wind1 = Waves(-2,-5,-2, COLOR_WHITE);
    wind2 = Waves(-3,-5,-3, COLOR_WHITE);
    wind3 = Waves(-1,-5,-1, COLOR_WHITE);
    wind4 = Waves(3,-5,3, COLOR_WHITE);
    wind5 = Waves(2,-5,2, COLOR_WHITE);
    wind6 = Waves(1,-5,1, COLOR_WHITE);
    float x=-8;
    for (int i = 0; i < 15; ++i){
        healthBars[i] = HealthBars(x,-8,-4, COLOR_WHITE);
        x+=0.3;
    }
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
            if(duration==-1){
                frame++;   
            }
            else {
                duration--;
            }
            if(frame==480){
                frame = 0;
                duration = (5+rand()%5) * 60;
                angle = (float)(rand()%360);
                boat.rotation = angle;
            }
            tick_elements();
            if(gameover)
                break;
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.y - b.y) * 2 < (a.width + b.width)) &&
            ((abs(a.x - b.x) * 2 < (a.length + b.length))) &&
           (abs(a.z - b.z) * 2 < (a.height + b.height));
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
