/*********

----------
ADDITIONAL FEATURES: the treasure chests move with the movement of the wave (sin function)
                                    
*********/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON      1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define  DURATION 5



float A = 20, //amplitude
fq = 2,  //frequency
C = 0,   //horizontal phase shift
B = -1600;   //vertical phase shift
float angle[5] = { 0 };

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, F1key = true, space = false;
int  winWidth, winHeight; // current Window width and height

int x = 0, y = 0;
int xship = 0, yship = 0;
float wave_time = 0;

int s=0;
int move = 1;
int pos = -300;
int score=0;
int timeleft1=20;
int timeleft2=59;
typedef struct {
    float x, y;
} pos_t;

pos_t p[5];
pos_t init[5];

int check=0;
int active[300] = { 0 };
pos_t ball[300] = { {-1000. -1000} };
int b = 0;

// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);

    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void pie_filled(float x, float y, float r, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glVertex2f(r * cos(end * D2R) + x, r * sin(end * D2R) + y);
    glEnd();
}


void circle(float x, float y, float r) {
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 360; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glEnd();
}

float f(float x) {
    return A * sin((fq * x + C) * D2R) + B;
}

void drawGraph() {
    glBegin(GL_LINE_STRIP);
    for (float x = -250; x <= 1000; x += 2) {
        glVertex2f(x - 350, f(x) + 25);
    }
    glEnd();
}


void oval_pie(float x, float y, float r1, float r2, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r1 * cos(angle * D2R) + x, r2 * sin(angle * D2R) + y);
    }
    glVertex2f(r1 * cos(end * D2R) + x, r2 * sin(end * D2R) + y);
    glEnd();
}

void oval_wire(float x, float y, float r1, float r2, float start, float end) {
    glBegin(GL_LINE_STRIP);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r1 * cos(angle * D2R) + x, r2 * sin(angle * D2R) + y);
    }
    glVertex2f(r1 * cos(end * D2R) + x, r2 * sin(end * D2R) + y);
    glEnd();
}

void waves(int x, int y) {
    glLineWidth(3);
    glColor3ub(0, 76, 153);
    for (int i = 0; i < 10; i++) {
        B = i * 100 - 470;
        int num = 70 % 150;
        C = i * num + wave_time * (i % 3 + 1);
        drawGraph();
    }
}

void background() {
    glColor3ub(0, 128, 255);
    glRectf(-600, -400, 600, 400);

}


void flags(int x, int y) {
    //flAG 1
    glColor3ub(110, 110, 110);

    glBegin(GL_POLYGON);
    glVertex2f(-380 + x, 150 + y);
    glVertex2f(-380 + x, 220 + y);
    glVertex2f(-325 + x, 220 + y);
    glVertex2f(-325 + x, 150 + y);
    glEnd();

    oval_pie(-330 + x, 185 + y, 20, 35, 270, 360);
    oval_pie(-330 + x, 185 + y, 20, 35, 0, 90);

    glColor3ub(0, 128, 255);
    oval_pie(-380 + x, 185 + y, 15, 37, 270, 360);
    oval_pie(-380 + x, 185 + y, 15, 37, 0, 90);


    //flag 2
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-395 + x, 20 + y);
    glVertex2f(-395 + x, 140 + y);
    glVertex2f(-305 + x, 140 + y);
    glVertex2f(-305 + x, 20 + y);
    glEnd();

    oval_pie(-310 + x, 80 + y, 20, 60, 270, 360);
    oval_pie(-310 + x, 80 + y, 20, 60, 0, 90);

    glColor3ub(0, 128, 255);
    oval_pie(-400 + x, 80 + y, 20, 60, 270, 360);
    oval_pie(-400 + x, 80 + y, 20, 60, 0, 90);


    //another flag
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-350 + x, 250 + y);
    glVertex2f(-290 + x, 240 + y);
    glVertex2f(-350 + x, 225 + y);
    glEnd();
    //skull
    glColor3ub(255, 255, 255);
    circle(-338 + x, 100 + y, 15);
    glRectf(-345 + x, 78 + y, -332 + x, 95 + y);
    glColor3ub(0, 0, 0);
    circle(-344 + x, 100 + y, 5);
    circle(-332 + x, 100 + y, 5);
    glLineWidth(1.3);
    glBegin(GL_LINES);
    glVertex2f(-342 + x, 79 + y);
    glVertex2f(-342 + x, 84 + y);
    glVertex2f(-338.5 + x, 79 + y);
    glVertex2f(-338.5 + x, 84 + y);
    glVertex2f(-335 + x, 79 + y);
    glVertex2f(-335 + x, 84 + y);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(-341 + x, 88 + y);
    glVertex2f(-338.5 + x, 93 + y);
    glVertex2f(-335 + x, 88 + y);
    glEnd();

    //bones
    glLineWidth(5);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2f(-360 + x, 80 + y);
    glVertex2f(-315 + x, 50 + y);
    glVertex2f(-315 + x, 80 + y);
    glVertex2f(-360 + x, 50 + y);
    glEnd();
    circle(-355 + x, 80 + y, 4);
    circle(-359 + x, 75 + y, 4);
    circle(-314 + x, 53 + y, 4);
    circle(-317 + x, 48 + y, 4);

    circle(-320 + x, 80 + y, 4);
    circle(-316 + x, 75 + y, 4);
    circle(-360 + x, 53 + y, 4);
    circle(-357 + x, 48 + y, 4);
}

void ship(int x, int y) {

    glColor3ub(102, 51, 0);
    glBegin(GL_POLYGON);
    glVertex2f(-450 + x, -10 + y);
    glVertex2f(-450 + x, 55 + y);
    glVertex2f(-560 + x, 55 + y);
    glVertex2f(-545 + x, -20 + y);
    glEnd();



    glBegin(GL_POLYGON);
    glVertex2f(-175 + x, -20 + y);
    glVertex2f(-175 + x, 35 + y);
    glVertex2f(-525 + x, -20 + y);
    glVertex2f(-525 + x, 50 + y);
    glEnd();


    oval_pie(-350 + x, 0 + y, 180, 100, 180, 360);
    glBegin(GL_POLYGON);
    glVertex2f(-250 + x, -10 + y);
    glVertex2f(-250 + x, 50 + y);
    glVertex2f(-130 + x, 50 + y);
    glVertex2f(-175 + x, -20 + y);
    glEnd();

    glColor3ub(0, 128, 255);
    //glColor3ub(255, 0, 0);
    oval_pie(-300 + x, 70 + y, 175, 65, 192, 350);

    //oval_pie(-280+x, 55+y, 150, 50, 270, 360);

    glColor3ub(102, 51, 0);

    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-350 + x, -20 + y);
    glVertex2f(-350 + x, 250 + y);
    glEnd();

    glColor3ub(51, 25, 0);
    glLineWidth(15);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-560 + x, 55 + y);
    glVertex2f(-465 + x, 55 + y);
    for (float angle = 200; angle < 340; angle += 10) {
        glVertex2f(175 * cos(angle * D2R) + -300 + x, 65 * sin(angle * D2R) + 70 + y);
    }
    glVertex2f(175 * cos(340 * D2R) + -300 + x, 65 * sin(340 * D2R) + 70 + y);
    glVertex2f(-130 + x, 50 + y);
    glVertex2f(-80 + x, 90 + y);
    glEnd();
    glLineWidth(6);
    glBegin(GL_LINE_STRIP);
    for (float angle = 0; angle < 130; angle += 10) {
        glVertex2f(30 * cos(angle * D2R) + -65 + x, 30 * sin(angle * D2R) + 64 + y);
    }
    glEnd();



    glLineWidth(1);

    glBegin(GL_LINE_STRIP);
    for (float angle = 205; angle < 250; angle += 1) {
        glVertex2f(250 * cos(angle * D2R) + -320 + x, 80 * sin(angle * D2R) + 90 + y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (float angle = 200; angle < 312; angle += 1) {
        glVertex2f(250 * cos(angle * D2R) + -320 + x, 80 * sin(angle * D2R) + 70 + y);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (float angle = 201; angle < 307; angle += 1) {
        glVertex2f(250 * cos(angle * D2R) + -320 + x, 80 * sin(angle * D2R) + 50 + y);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (float angle = 205; angle < 304; angle += 1) {
        glVertex2f(250 * cos(angle * D2R) + -320 + x, 80 * sin(angle * D2R) + 30 + y);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (float angle = 220; angle < 297; angle += 1) {
        glVertex2f(250 * cos(angle * D2R) + -320 + x, 80 * sin(angle * D2R) + 10 + y);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (float angle = 233; angle < 283; angle += 1) {
        glVertex2f(250 * cos(angle * D2R) + -320 + x, 80 * sin(angle * D2R) + -10 + y);
    }
    glEnd();


    glColor3ub(204, 102, 0);
    circle(-420 + x, -35 + y, 20);
    circle(-340 + x, -45 + y, 20);
    circle(-260 + x, -35 + y, 20);

    glColor3ub(51, 25, 0);
    circle(-420 + x, -35 + y, 15);
    circle(-340 + x, -45 + y, 15);
    circle(-260 + x, -35 + y, 15);

    flags(x, y);

}
void cannon(int x, int y) {

    glColor3ub(70, 70, 70);
    circle(-245 + x, 45 + y, 25);
    glBegin(GL_POLYGON);
    glVertex2f(-245 + x, 20 + y);
    glVertex2f(-245 + x, 70 + y);
    glVertex2f(-175 + x, 60 + y);
    glVertex2f(-175 + x, 35 + y);
    glEnd();

    glLineWidth(2);
    oval_wire(-237 + x, 70 + y, 12, 15, 0, 90);
    glColor3ub(0, 0, 0);
    glRectf(-230 + x, 66 + y, -220 + x, 72 + y);

    //wheel
    glColor3ub(64, 35, 6);
    glLineWidth(5);
    circle_wire(-235 + x, 33 + y, 18);
    glLineWidth(2);
    circle_wire(-235 + x, 33 + y, 3);
    glBegin(GL_LINES);
    for (float angle = 0; angle < 360; angle += 45) {
        glVertex2f(3 * cos(angle * D2R) + -235 + x, 3 * sin(angle * D2R) + 34 + y);
        glVertex2f(18 * cos(angle * D2R) + -235 + x, 18 * sin(angle * D2R) + 34 + y);
    }
    glEnd();

}

void ballb(int x, int y) {
    glColor3ub(0, 0, 0);
    circle(x, y, 10);
}

void treasurechest(int x, int y) {


    glColor3ub(102, 51, 0);
    glBegin(GL_POLYGON);
    glVertex2f(210 + x, 95 + y);
    glVertex2f(190 + x, 60 + y);
    glVertex2f(300 + x, 60 + y);
    glVertex2f(280 + x, 95 + y);
    glEnd();


    oval_pie(275 + x, 55 + y, 25, 40, 0, 90);
    oval_pie(215 + x, 55 + y, 25, 40, 90, 180);
    //glColor3ub(153, 76, 0);
    glBegin(GL_POLYGON);
    glVertex2f(200 + x, 0 + y);
    glVertex2f(190 + x, 60 + y);
    glVertex2f(300 + x, 60 + y);
    glVertex2f(290 + x, 0 + y);
    glEnd();


    glColor3ub(51, 25, 0);
    glLineWidth(1);
    glBegin(GL_LINES);
    glVertex2f(200 + x, 15 + y);
    glVertex2f(290 + x, 15 + y);
    glVertex2f(190 + x, 30 + y);
    glVertex2f(300 + x, 30 + y);
    glVertex2f(190 + x, 45 + y);
    glVertex2f(300 + x, 45 + y);
    glVertex2f(200 + x, 75 + y);
    glVertex2f(290 + x, 75 + y);
    glVertex2f(200 + x, 90 + y);
    glVertex2f(290 + x, 90 + y);
    glEnd();

    glLineWidth(7);
    glColor3ub(204, 102, 0);
    glBegin(GL_LINE_STRIP);
    glVertex2f(200 + x, 0 + y);
    glVertex2f(190 + x, 60 + y);
    glVertex2f(300 + x, 60 + y);
    glVertex2f(290 + x, 0 + y);
    glVertex2f(200 + x, 0 + y);
    glEnd();

    glRectf(235 + x, 35 + y, 255 + x, 60 + y);
    glColor3ub(0, 0, 0);
    circle(245 + x, 52 + y, 3);
    glBegin(GL_TRIANGLES);
    glVertex2f(245 + x, 54 + y);
    glVertex2f(241 + x, 42 + y);
    glVertex2f(249 + x, 42 + y);
    glEnd();

    glColor3ub(204, 102, 0);
    oval_wire(275 + x, 55 + y, 25, 40, 0, 80);
    oval_wire(215 + x, 55 + y, 25, 40, 95, 180);
    glLineWidth(5);
    oval_wire(245 + x, 55 + y, 3, 40, 5, 80);
    oval_wire(245 + x, 55 + y, 3, 40, 95, 175);
    glColor3ub(204, 130, 0);
    oval_pie(303 + x, 64 + y, 20, 20, 180, 267);
    oval_pie(187 + x, 64 + y, 20, 20, -87, 0);
    oval_pie(197 + x, -3 + y, 20, 20, 0, 100);
    oval_pie(293 + x, -3 + y, 20, 20, 80, 180);

}
//
// To display onto window using OpenGL commands
//

double distance(double ax, double ay, double bx, double by) {
    return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}
void scorecount(){
    glColor3ub(0, 51, 102);
    glRectf(450, -400, 600, 400);
    glColor3ub(255, 255, 0);
    vprint2(470, 200, 0.2, "Score: %d",score);
    vprint2(510, 160, 0.1, "Press");
    vprint2(465, 130, 0.1, "F1 to Pause/Start");
    vprint2(465, 100, 0.1, "SACE to shoot");
    vprint2(465, 70, 0.1, "UP arrow to go up");
    vprint2(465, 40, 0.08, "DOWN arrow to go down");
    
    
    
     if (timeleft1<10){
        if (timeleft2<10)
            vprint2(465, -20, 0.15, "Time: 0%d:0%d",timeleft1,timeleft2);
        else
        vprint2(465, -20, 0.15, "Time: 0%d:%d",timeleft1,timeleft2);
        
    }
    else if(timeleft2<10)
        vprint2(465, -20, 0.15, "Time: %d:0%d",timeleft1,timeleft2);
    
    else
        vprint2(465, -20, 0.15, "Time: %d:%d",timeleft1,timeleft2);
}
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    background();
    waves(0, 0);
    ship(0, yship);
    //float c = 0.5 * sin(0.5 * angle * D2R) + 0.5; //between c=0 and c=1.0
    for (int c = 0; c < 5; c++) {
        treasurechest(p[c].x, p[c].y);
    }

    for (int i = 0; i < 300; i++) {
        if (active[i] == 1)
            ballb(ball[i].x, ball[i].y);
    }
    cannon(0, yship);
    scorecount();
    vprint2(465, -50, 0.1, "Shoot the treasure");
    vprint2(465, -70, 0.1, "chests from your");
    vprint2(465, -90, 0.1, "pirate ship to earn");
    vprint2(465, -110, 0.1, "a score");
   
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.

    if (key == ' ') {
        active[b] = 1;
        ball[b].x = -180;
        ball[b++].y = yship + 50;
        if (b == 300)b = 0;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;

    case GLUT_KEY_F1:
        F1key = !F1key;
        break;

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}
int k = 0;

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (F1key == true) {
        if(timeleft1==0 && timeleft2==0)
            F1key=false;
        
        
         check++;
         if(!(timeleft1==0 && timeleft2==0)){
             if(timeleft1>0 && check%60==0){
                 timeleft1--;
             }
             if(timeleft2>=0 && check%1==0)
                 timeleft2--;
             if(timeleft2<0)
                 timeleft2=59;
         }
        
        
        for (int i = 0; i < 300; i++) {
            if (active[i] == 1) {
                ball[i].x += 15;
                for (int k = 0; k < 5; k++) {
                    if (distance(ball[i].x, ball[i].y, p[k].x + 250, p[k].y + 50) < 50) {
                        int ry = rand() % 800 - 400;
                        p[k].y = ry;
                        init[k].x = 600;
                        angle[k] = 0;
                        ball[i].y+=2000;
                        if(p[k].x<450)
                            score++;
                    }
                }
            }
        }
        
        if (up == 1 && yship < 250) {
            yship += 10;
        }
        if (down == 1 && yship > -350) {
            yship -= 10;
        }
        wave_time += 1.5;

        for (int c = 0; c < 5; c++) {
            angle[c] += 0.7;
            if (angle[c] > 360)
                angle[c] = 0;
            p[c].y = init[c].y - 25 * sin(6 * angle[c] * D2R);
            p[c].x = init[c].x - 400 * sin(angle[c] * D2R )-150;
            for (int i = 0; i < 5; i++) {
                if (i != c) {
                    if (p[c].y<p[i].y && p[c].y + 100 > p[i].y)
                        p[c].y = p[i].y - 100;
                }
            }
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()
}
#endif
void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

int main(int argc, char* argv[]) {
    srand(time_t(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Saida Rustamova");
    for (int c = 0; c < 5; c++) {
        //int rx=rand()%500-250;
        int ry, rx;
        rx = rand() % 200 + 25;
        ry = rand() % 800 - 400;
        int flag = 0;
        while (flag == 0) {
            ry = rand() % 800 - 400;
            flag = 1;
            for (int i = 0; i < c; i++) {
                double q = ry - init[i].y;
                if (q < 0)q = -q;
                if (q < 100)
                    flag = 0;
            }

        }
        //p[c].x+=rx;
        init[c].x += rx + 300;
        init[c].y += ry;

    }

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);


    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    //glutMouseFunc(onClick);
    //glutMotionFunc(onMoveDown);
    //glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
// timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();

    return 0;
}
