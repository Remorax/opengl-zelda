#include "main.h"
#include "timer.h"
#include "boat.h"
#include "sea.h"
#include "rock.h"
#include "prism.h"
#include "waves.h"
#include "sails.h"
#include "healthBars.h"
#include "target.h"
#include "cannon.h"
#include "bullet.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "boss.h"
#include "cylinder.h"
#include "health.h"
#include "speedup.h"
#include "coin.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Boat boat;
Sea sea;
Rock rocks[50];
Waves f,m,r;
Waves wind1, wind2, wind3, wind4, wind5, wind6;
Sails sails;
vector <HealthBars> healthBars(20);
vector <HealthBars> coinBars(10);
Target target1;
Bullet bullet, bossBullet;
Cannon cannon;
Boss boss;
Speedup speedup[4], bossSpeedup;
Coin coins[4];
Health health[4];
int healthSecs[4], speedupSecs[4], coinSecs[4];
Cylinder cylinder[4], cylinders[4], cylinderc[4];
vector <Enemy1> enemy1s(10);
vector <Enemy2> enemy2s(30);
vector <Enemy3> enemy3s(4);
vector <Bullet> enemy1Bullets(10);
vector <Bullet> enemy3Bullets(8);
vector <Coin> enemy1coins(10);
vector <Health> enemy3health(4);

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float eyex, eyey, eyez, targetx, targety, targetz, normalx, normaly, normalz;
char viewMode = 'f';
bool changed = false, gameover=false;
bool still = true, deleteEnemy=false, fired=false, firedEnemy1 = false, firedEnemy3 = false;
int stillMode = 20, createdBoss = false, enemy1coinsSize = 0, enemy3healthSize=0;
int frame=0, duration=-1, angle=0, count=0, count2=0, count3=0, isJump=0, isFall=0, speedupCount = -1, coinBarsSize=0;
char prevState = 'f';
float tempx=0,tempy=0,tempz=0;
bool isBoss = false, firedBoss=false, bossDestroyed=false;

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
    for (int i = 0; i < healthBars.size(); ++i){
        healthBars[i].position.z = boat.position.z - 4;
        healthBars[i].position.x = boat.position.x - 0.7*i;
    }
    for (int i = 0; i < coinBarsSize; ++i)
    {
        coinBars[i].position.z = boat.position.z - 4;
        coinBars[i].position.x = boat.position.x + 10 - 0.7*i;
    }
    if(frame%60==0 && !firedEnemy3){
        createEnemy3Bullet();
        glm::vec3 v = glm::vec3(0.0f,1.0f,20.0f);
        for (int i = 0; i < enemy3Bullets.size(); ++i)
        {
            enemy3Bullets[i].shoot(v/length(v));
            enemy3Bullets[i].draw(VP);
        }
        firedEnemy3 = true;
        count2 = 0;
     }
    else {
        if(firedEnemy3){
            for (int i = 0; i < enemy3Bullets.size(); ++i)
            {
                glm::vec3 v;
                if(enemy3s[i/2].position.z<0)
                    v = glm::vec3(0.0f,1.0f,20.0f);
                else
                    v = glm::vec3(0.0f,1.0f,-20.0f);
                enemy3Bullets[i].shoot(v/length(v));
                enemy3Bullets[i].draw(VP);
            }
            count2++;
            if(count2==120)
                firedEnemy3 = false;
        }
     }

    if(frame%120==0 && !firedEnemy1){
        createEnemy1Bullet();
        // for (int i = 0; i < enemy1Bullets.size(); ++i){
        glm::vec3 v = glm::vec3(0.0f,1.0f,10.0f);
        for (int i = 0; i < enemy1Bullets.size(); ++i)
        {
            enemy1Bullets[i].shoot(v/length(v));
            enemy1Bullets[i].draw(VP);
        }
        firedEnemy1 = true;
        count = 0;
     }
     else {
        if(firedEnemy1){
            for (int i = 0; i < enemy1Bullets.size(); ++i)
            {
                glm::vec3 v;
                if(enemy1s[i].position.z<0)
                    v = glm::vec3(0.0f,1.0f,10.0f);
                else
                    v = glm::vec3(0.0f,1.0f,-10.0f);
                enemy1Bullets[i].shoot(v/length(v));
                enemy1Bullets[i].draw(VP);
            }
            count++;
            if(count==180)
                firedEnemy1 = false;
        }
     }
    
    // Scene render
    boat.draw(VP);
    for (int i=0; i<4; ++i){
        if (healthSecs[i]==-1){
            health[i].draw(VP);
            health[i].rotation++;
        }
        else
            healthSecs[i]--;
        cylinder[i].draw(VP);
    }

    for (int i=0; i<4; ++i){
        if (speedupSecs[i]==-1){
            speedup[i].draw(VP);
            speedup[i].rotation++;
        }
        else
            speedupSecs[i]--;
        cylinders[i].draw(VP);
    }

    for (int i=0; i<4; ++i){
        if (coinSecs[i]==-1){
            coins[i].draw(VP);
            coins[i].rotation++;
        }
        else
            coinSecs[i]--;
        cylinderc[i].draw(VP);
    }

    sea.draw(VP);
    if(enemy3s.size()<2 && enemy1s.size()<5 && !bossDestroyed){
        isBoss = true;
        // printf("yes\n");
        if(!createdBoss){
            boss       = Boss(-20.0f, 0.0f, -20.0f, COLOR_PURPLE);
            createdBoss = true;            
        }
        // printf("no\n");
        boss.draw(VP);
        if(frame%120==0)
            boss.move();
        if (frame%60==0 && !firedBoss)
        {
            createBossBullet();
            // printf("but\n");
            glm::vec3 v = glm::vec3(0.0f,1.0f,30.0f);
            bossBullet.shoot(v/length(v));
            // printf("ggg\n");
            bossBullet.draw(VP);
            // printf("wot\n");
            firedBoss = true;
            count3 = 0;
            // printf("nice\n");
         }
         else {
            if(firedBoss){
                glm::vec3 v;
                if(boss.position.z<0)
                    v = glm::vec3(0.0f,1.0f,30.0f);
                else
                    v = glm::vec3(0.0f,1.0f,-30.0f);
                bossBullet.shoot(v/length(v));
                bossBullet.draw(VP);
                // printf("boo\n");
            }
            count3++;
            if(count3==120)
                firedBoss = false;
        }
        // printf("okay\n");
        if (detect_collision(boat.bounding_box(), boss.bounding_box())){
            healthBars.pop_back();
            if(healthBars.size()==0){
                gameover = true;
                return;
            }
        }
        if (detect_collision(boat.bounding_box(), bossBullet.bounding_box())){
            for (int i = 0; i < 5; ++i){
                // printf("healthBars.size: %d\n", healthBars.size());
                healthBars.pop_back();   
                if(healthBars.size()==0){
                    gameover = true;
                    return;
                }
            }
        }
        if (detect_collision(boss.bounding_box(), bullet.bounding_box())){
            printf("Boss lost one life\n");
            boss.lives--;
            if(boss.lives==0){
                // gameover = true;
                bossSpeedup = Speedup(boss.position.x, -1, boss.position.z, COLOR_YELLOW);
                bossDestroyed = true;
                // return;
            }
        }
    }
    // cannon.draw(VP);
    for (int i=0; i<enemy1s.size(); ++i)
        enemy1s[i].draw(VP);
    for (int i=0; i<enemy2s.size(); ++i){
        enemy2s[i].move();
        enemy2s[i].draw(VP);
    }
    for (int i=0; i<enemy3s.size(); ++i)
        enemy3s[i].draw(VP);
    // enemy1.draw(VP);
    sails.draw(VP);
    if(fired){
        glm::vec3 v = target1.position-boat.position;
        bullet.draw(VP); 
        bullet.shoot(v/length(v));
    }
    target1.draw(VP);
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
            boat.moveBehind();
            healthBars.pop_back();
            if(healthBars.size()==0){
                gameover = true;
                return;
            }
        }
    }
    for (int i = 0; i < enemy1Bullets.size(); ++i) {
        if (detect_collision(boat.bounding_box(), enemy1Bullets[i].bounding_box())){
            for (int i=0; i<2; ++i) {
                healthBars.pop_back();
                if(healthBars.size()==0){
                    gameover = true;
                    return;
                }
            }
        }
    }

    for (int i = 0; i < enemy3Bullets.size(); ++i) {
        if (detect_collision(boat.bounding_box(), enemy3Bullets[i].bounding_box())){
            for (int i=0; i<3; ++i) {
                healthBars.pop_back();
                if(healthBars.size()==0){
                    gameover = true;
                    return;
                }
            }
        }
    }

    for (int i = 0; i < enemy1s.size(); ++i) {
        if (detect_collision(boat.bounding_box(), enemy1s[i].bounding_box())){
            healthBars.pop_back();
            if(healthBars.size()==0){
                gameover = true;
                return;
            }
        }
        if (detect_collision(enemy1s[i].bounding_box(), bullet.bounding_box())){
            enemy1s[i].lives--;
            printf("Enemy 1 lost one life\n");
            if(enemy1s[i].lives==0){
                enemy1coins[enemy1coinsSize++] = Coin(enemy1s[i].position.x, -1.0, enemy1s[i].position.z, 1.0f, COLOR_BRONZE);
                enemy1s.erase(enemy1s.begin()+i);
            }
        }
    }

    for (int i = 0; i < enemy2s.size(); ++i) {
        if (detect_collision(boat.bounding_box(), enemy2s[i].bounding_box())){
            healthBars.pop_back();
            if(healthBars.size()==0){
                gameover = true;
                return;
            }
        }
        if (detect_collision(enemy2s[i].bounding_box(), bullet.bounding_box())){
            printf("Enemy 2 died\n");
            enemy2s[i].lives--;
            if(enemy2s[i].lives==0)
                enemy2s.erase(enemy2s.begin()+i);
        }
    }

    for (int i = 0; i < enemy3s.size(); ++i) {
        if (detect_collision(boat.bounding_box(), enemy3s[i].bounding_box())){
            healthBars.pop_back();
            if(healthBars.size()==0){
                gameover = true;
                return;
            }
        }
        if (detect_collision(enemy3s[i].bounding_box(), bullet.bounding_box())){
            printf("Enemy 3 lost one life\n");
            enemy3s[i].lives--;
            if(enemy3s[i].lives==0){
                enemy3health[enemy3healthSize++] = Health(enemy3s[i].position.x, -1.0, enemy3s[i].position.z, COLOR_RED);
                enemy3s.erase(enemy3s.begin()+i);
            }
        }
    }
    for (int i=0; i<4; ++i)
    {
        if (detect_collision(boat.bounding_box(), health[i].bounding_box()) && healthSecs[i]==-1){
            for (int j = 0; j < 2; ++j)
            {
                HealthBars h = HealthBars(-4+(healthBars.size()-1)*0.2,-13,-4,COLOR_WHITE);
                healthBars.push_back(h);
            }
            if (health[i].chances<=0){
                healthSecs[i] = 480;
            }
            health[i].chances--;
        }
    }
    
    for (int i=0; i<4; ++i)
    {
        if (detect_collision(boat.bounding_box(), speedup[i].bounding_box()) && speedupSecs[i]==-1){
            
            // printf("%d\n", health[i].chances);
            speedupCount = 600;
            if (speedup[i].chances<=0)
                speedupSecs[i] = 900;
            speedup[i].chances--;
        }
    }

    for (int i=0; i<4; ++i)
    {
        if (detect_collision(boat.bounding_box(), coins[i].bounding_box()) && coinSecs[i]==-1){
            addCoinsToBar();
            if (coins[i].chances<=0)
                coinSecs[i] = 480;
            coins[i].chances--;
        }
    }
    
    for (int i = 0; i < enemy1coinsSize; ++i)
        enemy1coins[i].draw(VP);
    for (int i = 0; i < enemy1coinsSize; ++i)
    {
        if (detect_collision(boat.bounding_box(), enemy1coins[i].bounding_box())){
            addCoinsToBar();
            enemy1coins.erase(enemy1coins.begin()+i);
            enemy1coinsSize--;
        }
    }
    
    for (int i = 0; i < enemy3healthSize; ++i)
        enemy3health[i].draw(VP);
    for (int i = 0; i < enemy3healthSize; ++i)
    {
        if (detect_collision(boat.bounding_box(), enemy3health[i].bounding_box())){
            HealthBars h = HealthBars(-4+(healthBars.size()-1)*0.2,-13,-4,COLOR_WHITE);
            healthBars.push_back(h);
            enemy3health.erase(enemy3health.begin()+i);
            enemy3healthSize--;
        }
    }
    if(bossDestroyed){
        bossSpeedup.draw(VP);
        if (detect_collision(boat.bounding_box(), bossSpeedup.bounding_box())){
            speedupCount= 1800;
        }
    }
    for (int i = 0; i < 25; ++i)
        rocks[i].draw(VP);
    for (int i=0; i<healthBars.size(); ++i)
        healthBars[i].draw(VP);
    // printf("%d\n", coinBarsSize);
    for (int i=0; i<coinBarsSize; ++i)
        coinBars[i].draw(VP);
}

void addCoinsToBar(){
    HealthBars c = HealthBars(boat.position.x+10-(coinBarsSize-1)*0.7,-13,boat.position.z+4,COLOR_YELLOW);
    coinBars[coinBarsSize] = c;
    coinBarsSize++;
    if (coinBarsSize>=10){
        coinBars.clear();
        coinBarsSize = 0;
        HealthBars h = HealthBars(-4+(healthBars.size()-1)*0.2,-13,-4,COLOR_WHITE);
        healthBars.push_back(h);
        // coinBars[coinBarsSize] = h;
    }
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
    m.position.x = boat.position.x;
    m.position.z = boat.position.z;
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
    int fire = glfwGetKey(window, GLFW_KEY_F);
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
    else if (fire) {
        createBullet();
        fired = true;
    }
    else if(space) {
        isJump = 1;
        isFall = 0;
        boat.jump();
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
        viewMode = prevState;
    }
    else if (up) {
        still = false;
        if(speedupCount==-1)
            boat.moveAhead();
        else{
            speedupCount--;
            boat.speedMoveAhead();
        }
    }
    else if (down) {
        still = false;
        if(speedupCount==-1)
            boat.moveBehind();
        else{
            speedupCount--;
            boat.speedMoveBehind();
        }
    }
    return;
}

void showWind(){
    wind1.rotation = wind2.rotation = wind3.rotation = wind4.rotation = wind5.rotation = wind6.rotation = angle;
    wind1.position.x = boat.position.x - 0.5, wind1.position.z = boat.position.z - 3;
    wind2.position.x = boat.position.x - 2, wind2.position.z = boat.position.z - 3;
    wind3.position.x = boat.position.x - 3.5, wind3.position.z = boat.position.z - 3;
    wind4.position.x = boat.position.x + 0.5, wind4.position.z = boat.position.z + 3;
    wind5.position.x = boat.position.x + 2, wind5.position.z = boat.position.z + 3;
    wind6.position.x = boat.position.x + 3.5, wind6.position.z = boat.position.z + 3;
    return;
}

void tick_elements() {
    reset_screen();
    sails.position.x = boat.position.x;
    sails.position.y = boat.position.y-0.81;
    sails.position.z = boat.position.z;
    sails.rotation = boat.rotation;
    if(duration!=-1){
        showWind();
    }

    double x,y;
    glfwGetCursorPos(window, &x, &y);
    target1.position.x = boat.position.x + 13 - (float)((26*x)/600.0f);
    target1.position.y= boat.position.y -13+(float)((26*y)/600.0f), target1.position.z = boat.position.z-3;
    // cannon.position.x = boat.position.x;
    // cannon.position.y = boat.position.y-0.7;
    // cannon.position.z = boat.position.z+1   ;
    // cannon.rotation = boat.rotation;
    if(isJump==1){
        isJump = boat.jump();
        if (isJump==0)
            isFall = 1;
    }
    else if (isFall==1){
        isFall = boat.fall();
        if (isFall==0){
            boat.position.y=0;
            isJump=0;
            boat.speedy = 0.1;
        }
    }
}


void createRocks()
{
    for (int i=0; i<50; ++i)
    {
        float x = -79.5 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(159)));
        float z = -79.5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/159));
        rocks[i] = Rock(x,0,z,COLOR_GRAY);
    }
    return;
}

void createEnemy1s() {
    for (int i = 0; i<enemy1s.size(); ++i)
    {
        float x = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        float y = 0;
        float z = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        enemy1s[i]     = Enemy1(x, y, z, COLOR_VIOLET);
    }
}

void createEnemy2s () {
    for (int i = 0; i < enemy2s.size(); ++i)
    {
        float x = (rand()%2==0)?(10 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70)))):(-10 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70))));
        float y = 0;
        float z = (rand()%2==0)?(10 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70)))):(-10 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70))));
        enemy2s[i]     = Enemy2(x, y, z, COLOR_BROWN);
    }
}

void createEnemy3s () {
    for (int i = 0; i < enemy3s.size(); ++i)
    {
        float x = (rand()%2==0)?(10 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70)))):(-10 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70))));
        float y = 0;
        float z = (rand()%2==0)?(10 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70)))):(-10 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(70))));
        enemy3s[i]     = Enemy3(x, y, z, COLOR_OLIVE);
    }
}

void createBullet() {
    bullet     = Bullet(boat.position.x, boat.position.y, boat.position.z,1, COLOR_GREEN);
}

void createBossBullet() {
    bossBullet     = Bullet(boss.position.x, boss.position.y-3.0f, boss.position.z,20, COLOR_BLACK);
}

void createEnemy1Bullet() {
    for (int i = 0; i < enemy1s.size(); ++i)
        enemy1Bullets[i] = Bullet(enemy1s[i].position.x, enemy1s[i].position.y-3.0f, enemy1s[i].position.z,10, COLOR_RED);
}

void createEnemy3Bullet() {
    for (int i = 0; i < (enemy3s.size()*2); i+=2){
        enemy3Bullets[i] = Bullet(enemy3s[i/2].position.x-2.0f, enemy3s[i/2].position.y-3.0f, enemy3Bullets[i/2].position.z,15, COLOR_DARKVIOLET);
        enemy3Bullets[i+1] = Bullet(enemy3s[i/2].position.x+2.0f, enemy3s[i/2].position.y-3.0f, enemy3s[i/2].position.z,15, COLOR_DARKVIOLET);
    }
}

void createHealth() {
    for (int i=0; i<4; ++i)
        healthSecs[i] = -1;
    for (int i=0; i<4; ++i) {
        float x = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        float y = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        health[i]     = Health(x, -1, y, COLOR_RED);
        cylinder[i]   = Cylinder(x, 1.5, y, 1, COLOR_BLACK);
    }
    return;
}

void createSpeedup() {
    for (int i=0; i<4; ++i)
        speedupSecs[i] = -1;
    for (int i=0; i<4; ++i) {
        float x = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        float y = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        speedup[i]     = Speedup(x, -1, y, COLOR_YELLOW);
        cylinders[i]   = Cylinder(x, 1.5, y, 1, COLOR_BLACK);
    }
    return;
}

void createCoins(){
    for (int i=0; i<4; ++i)
        coinSecs[i] = -1;
    for (int i=0; i<4; ++i) {
        float x = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        float y = (rand()%2==0)?(30 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50)))):(-30 - static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(50))));
        coins[i]     = Coin(x, -1.0f, y, 1.0f, COLOR_BRONZE);
        cylinderc[i]   = Cylinder(x, 1.5, y, 1, COLOR_BLACK);
    }
    return;    
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat       = Boat(0, 0, 0, COLOR_DARKBROWN);
    // cannon       = Cannon(-1, -1, -1, COLOR_GOLD);
    sea        = Sea(0, 2, 0, COLOR_LIGHTBLUE);
    createHealth();
    createSpeedup();
    createCoins();
    // enemy1     = Enemy1(-10, 0, -10, COLOR_VIOLET);
    createBullet();
    createEnemy1s();
    createEnemy2s();
    createEnemy3s();
    // createEnemy1Bullet();
    target1     = Target(0, -3, 0, 0.5f, COLOR_RED);
    f = Waves(-1, 1, 0, COLOR_DARKBLUE);
    r = Waves(1, 1, 0, COLOR_DARKBLUE);
    m = Waves(0, 1, 1, COLOR_DARKBLUE);
    sails = Sails(0, -0.81, 0, COLOR_GOLD);
    wind1 = Waves(-2,-5,-2, COLOR_WHITE);
    wind2 = Waves(-3,-5,-3, COLOR_WHITE);
    wind3 = Waves(-1,-5,-1, COLOR_WHITE);
    wind4 = Waves(3,-5,3, COLOR_WHITE);
    wind5 = Waves(2,-5,2, COLOR_WHITE);
    wind6 = Waves(1,-5,1, COLOR_WHITE);
    float x=-4;
    for (int i = 0; i < 20; ++i){
        healthBars[i] = HealthBars(x,-13,-4, COLOR_WHITE);
        x+=0.2;
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
            if(gameover){
                printf("You lost! Game is over\n");
                break;
            }
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
        Matrices.projection = glm::perspective(-90.0f,top/right, 0.1f, 500.0f);
}
