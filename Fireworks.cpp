/*****

 PROBLEMS: this code was written using Xcode, the GLUT library name has to be changed
 
 Additional functions: once the firework reaches the star, it blows up and a firework animation appears
*****/
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 800

#define  TIMER_PERIOD  16 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, space = false;
int  winWidth, winHeight; // current Window width and height


/* User defined Types and variables for the application */
#define FOV  30    // Field of View Angle
#define ENEMY_SPEED 2
#define PLAYER_SPEED 5

int a = 1;


typedef struct{
    vec_t pos;
    vec_t vel;
    float angle;
    bool active;
} fire_t;

typedef struct{
    vec_t pos;
    vec_t vel;
    float angle;
    bool active;
} star_t;

fire_t F = { { 0 , 0 }, 0 , 0};
star_t S = {{0,300},0};

fire_t oldPos = {{0,0}, 0, 0};
int p = 0;
//
// to draw circle, center at (x,y)
//  radius r
//
void circle( int x, int y, int r )
{
#define PI 3.1415
    float angle ;
    glBegin( GL_POLYGON ) ;
    for ( int i = 0 ; i < 100 ; i++ )
    {
        angle = 2*PI*i/100;
        glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
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
        angle = 2 * PI*i / 100;
        glVertex2f(x + r*cos(angle), y + r*sin(angle));
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

void print(int x, int y, const char *string, void *font )
{
    int len, i ;

    glRasterPos2f( x, y );
    len = (int) strlen( string );
    for ( i =0; i<len; i++ )
    {
        glutBitmapCharacter( font, string[i]);
    }
}


// display text with variables.
void vprint(int x, int y, void *font, const char *string , ...)
{
    va_list ap;
    va_start ( ap, string );
    char str[1024] ;
    vsprintf( str, string, ap ) ;
    va_end(ap) ;
    
    int len, i ;
    glRasterPos2f( x, y );
    len = (int) strlen( str );
    for ( i =0; i<len; i++ )
    {
        glutBitmapCharacter( font, str[i]);
    }
}

void vprint2(int x, int y, float size, const char *string, ...) {
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
        for (i = 0; i<len; i++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
        }
    glPopMatrix();
}


void drawFire(fire_t f){
    glColor3ub(250, 200, 0);
    vec_t f1 = addV(f.pos, pol2rec({30,f.angle+260}));
    vec_t f2 = addV(f.pos, pol2rec({30,f.angle+290}));
    vec_t f3 = addV(f.pos, pol2rec({40,f.angle+65}));
    vec_t f4 = addV(f.pos, pol2rec({40,f.angle+115}));
    
    vec_t f5 = addV(f.pos, pol2rec({70,f.angle+90}));
    
    vec_t f6 = addV(f.pos, pol2rec({29,f.angle+273}));
    vec_t f7 = addV(f6, pol2rec({40,f.angle+270}));
    
    vec_t f8 = addV(f.pos, pol2rec({55,f.angle+90}));
    vec_t f9 = addV(f.pos, pol2rec({43,f.angle+90}));
    
    vec_t f10 = addV(f.pos, pol2rec({35,f.angle+90}));
    vec_t f11 = addV(f.pos, pol2rec({23,f.angle+90}));
    
    vec_t f12 = addV(f.pos, pol2rec({15,f.angle+90}));
    vec_t f13 = addV(f.pos, pol2rec({3,f.angle+90}));
    
    
    glBegin(GL_POLYGON);
    glVertex2f(f1.x, f1.y);
    glVertex2f(f2.x, f2.y);
    glVertex2f(f3.x, f3.y);
    glVertex2f(f4.x, f4.y);
    glEnd();
    
    glColor3ub(200, 0, 0);
    oval_pie(f5.x, f5.y, 40, 40, 240+f.angle, 300+f.angle);
    
    
    glBegin(GL_POLYGON);
    
    glVertex2f(30 * cos((f.angle + 240) * D2R) + f9.x, 30 * sin((f.angle + 240) * D2R) + f9.y);
    
    for (float angle = 240; angle < 304; angle += 1) {
        glVertex2f(30 * cos((f.angle + angle) * D2R) + f8.x, 30 * sin((f.angle + angle) * D2R) + f8.y);
    }
    
    glVertex2f(30 * cos((f.angle + 300) * D2R) + f9.x, 30 * sin((f.angle + 300) * D2R) + f9.y);
    
    for (float angle = 300; angle > 240; angle -= 1) {
        glVertex2f(30 * cos((f.angle + angle) * D2R) + f9.x, 30 * sin((f.angle + angle) * D2R) + f9.y);
    }
    
    
    glEnd();

    glBegin(GL_POLYGON);
    
    glVertex2f(30 * cos((f.angle + 250) * D2R) + f11.x, 30 * sin((f.angle + 250) * D2R) + f11.y);
    
    for (float angle = 245; angle < 300; angle += 1) {
        glVertex2f(30 * cos((f.angle + angle) * D2R) + f10.x, 30 * sin((f.angle+angle) * D2R) + f10.y);
    }
    
    glVertex2f(30 * cos((f.angle + 297) * D2R) + f11.x, 30 * sin((f.angle + 305) * D2R) + f11.y);
    
    for (float angle = 295; angle > 235; angle -= 1) {
        glVertex2f(30 * cos((f.angle + angle )* D2R) + f11.x, 30 * sin((f.angle + angle) * D2R) + f11.y);
    }
    
    
    glEnd();

    
    glBegin(GL_POLYGON);
    
    glVertex2f(30 * cos((f.angle+257) * D2R) + f13.x, 30 * sin((f.angle + 250) * D2R) + f13.y);
    
    for (float angle = 252; angle < 295; angle += 1) {
        glVertex2f(30 * cos((f.angle + angle) * D2R) + f12.x, 30 * sin((f.angle + angle) * D2R) + f12.y);
    }
    
    glVertex2f(30 * cos((f.angle + 292) * D2R) + f13.x, 30 * sin((f.angle + 305) * D2R) + f13.y);
    
    for (float angle = 292; angle > 257; angle -= 1) {
        glVertex2f(30 * cos((f.angle + angle) * D2R) + f13.x, 30 * sin((f.angle + angle) * D2R) + f13.y);
    }
    
    
    glEnd();


    glLineWidth(5);
    glColor3ub(51, 25, 0);
    
    glBegin(GL_LINES);
    glVertex2f(f6.x, f6.y);
    glVertex2f(f7.x, f7.y);
    glEnd();
    
    glLineWidth(2);
    glColor3ub(255, 255, 0);
    glBegin(GL_LINES);
    glVertex2f(f6.x-1, f6.y);
    glVertex2f(f7.x-1, f7.y+3);
    glEnd();
    
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = 0; i < 100; i+=10)
    {
        //glVertex2f(f7.x, f7.y);
        float angle = 2 * PI*i / 100;
        glVertex2f(f7.x + 2*cos(angle), f7.y + 2*sin(angle));
        glVertex2f(f7.x + 12*cos(angle), f7.y + 12*sin(angle));
    }
    glEnd();
}

void drawStar(star_t s){
    glColor3ub(255, 255, 0);
    //star_t s1 = addV(s.pos, pol2rec({30,s.angle+260}));
    vec_t s1 = addV(s.pos, pol2rec({0,s.angle}));
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    float angle = 2 * PI*0 / 100;
    glVertex2f(s1.x + 10*cos(angle), s1.y + 10*sin(angle));
     angle = 2 * PI*40 / 100;
    glVertex2f(s1.x + 10*cos(angle), s1.y + 10*sin(angle));
     angle = 2 * PI*80 / 100;
    glVertex2f(s1.x + 10*cos(angle), s1.y + 10*sin(angle));
     angle = 2 * PI*20 / 100;
    glVertex2f(s1.x + 10*cos(angle), s1.y + 10*sin(angle));
     angle = 2 * PI*60/ 100;
    glVertex2f(s1.x + 10*cos(angle), s1.y + 10*sin(angle));
     angle = 2 * PI*0 / 100;
    glVertex2f(s1.x + 10*cos(angle), s1.y + 10*sin(angle));
    glEnd();
    
    
}

int k = -1;
int oldx = 0, oldy = 0;
void firework(){
    if(k != -1){
        glColor3ub(255, 255, 255);
        //circle(old.pos.x, old.pos.y, 10);
        float angle;
        
        glBegin(GL_LINES);
        for (int i = 0; i < 100; i+=10)
        {
            angle = 2 * PI*i / 100;
            glVertex2f(oldx + k * cos(angle), oldy + k * sin(angle));
            glVertex2f(oldx + (20+k)*cos(angle), oldy + (20+k)*sin(angle));
        }
        glEnd();
    }
}
void background(){
    
    glBegin(GL_POLYGON);
    glColor3ub(0, 0, 30);
    glVertex2f(-500,400);
    glVertex2f(500, 400);
    glColor3b(20, 0, 30);
    glVertex2f(500, -400);
    glVertex2f(-500, -400);
    glEnd();
    
    glColor3ub(25, 25, 25);
    glBegin(GL_POLYGON);
    glVertex2f(500, -400);
    glVertex2f(500, -50);
    glVertex2f(350, -110);
    glVertex2f(350, -400);
    glEnd();
    
    glColor3ub(25, 25, 25);
    glRectf(350, -400, 200, -200);
    glRectf(200, -400, 100, 0);
    //windows
    glColor3ub(0, 20, 50);
    
    for(int a=0; a<35*10;a+=35){
        glRectf(185, -45-a, 165, -20-a);
        glRectf(160, -45-a, 140, -20-a);
        glRectf(135, -45-a, 115, -20-a);
    }
    
    
    glColor3ub(0, 20, 50);
    glRectf(480, -180,450, -130);
    glRectf(440, -180,410, -130);
    glRectf(400, -180,370, -130);
    
    glRectf(480, -190,450, -240);
    glRectf(440, -190,410, -240);
    glRectf(400, -190,370, -240);
    
    glRectf(480, -250,450, -300);
    glRectf(440, -250,410, -300);
    glRectf(400, -250,370, -300);
    
    glRectf(480, -310,450, -360);
    glRectf(440, -310,410, -360);
    glRectf(400, -310,370, -360);
    
    
    
    
    
   
    glColor3ub(10, 10, 32);
    glBegin(GL_POLYGON);
    glVertex2f(-500, -400);
    glVertex2f(-500, -50);
    glVertex2f(-350, -110);
    glVertex2f(-350, -400);
    glEnd();
    //building
    glColor3ub(0, 20, 50);
    glRectf(-480, -180,-450, -130);
    glRectf(-440, -180,-410, -130);
    glRectf(-400, -180,-370, -130);
    
    glRectf(-480, -190,-450, -240);
    glRectf(-440, -190,-410, -240);
    glRectf(-400, -190,-370, -240);
    
    glRectf(-480, -250,-450, -300);
    glRectf(-440, -250,-410, -300);
    glRectf(-400, -250,-370, -300);
    
    glRectf(-480, -310,-450, -360);
    glRectf(-440, -310,-410, -360);
    glRectf(-400, -310,-370, -360);
    
    glColor3ub(7, 7, 30);
    glLineWidth(3);
    circle_wire(250, -180, 140);
    circle_wire(250, -180, 100);
    glBegin(GL_LINES);
    float angle;
    for (int i = 0; i < 100; i+=10)
    {
        glVertex2f(250, -180);
        angle = 2 * PI*i / 100;
        glVertex2f(250 + 150*cos(angle), -180 + 150*sin(angle));
    }
    
    glEnd();
    
    float angle1;
    for (int i = 0; i < 100; i+=10)
    {
        angle1 = 2 * PI*i / 100;
        circle(250 + 160*cos(angle1), -180 + 160*sin(angle1),10);
    }
    
    glLineWidth(8);
    glBegin(GL_LINES);
    glVertex2f(350, -400);
    glVertex2f(250, -180);
    glVertex2f(150, -400);
    glVertex2f(250, -180);
    glEnd();
    
    glColor3ub(10, 10, 32);
    glRectf(-350, -400, -200, -200);
    glRectf(-200, -400, -100, 0);
    //windows
    glColor3ub(0, 20, 50);
    
    for(int a=0; a<35*10;a+=35){
        glRectf(-185, -45-a, -165, -20-a);
        glRectf(-160, -45-a, -140, -20-a);
        glRectf(-135, -45-a, -115, -20-a);
    }
    
    
    
    glColor3ub(10, 10, 32);
    glRectf(-100, -400, 0, -250);
    glRectf(20, 100, 60, 160);
    glRectf(0, -400, 80, 100);
    glColor3ub(0, 20, 50);
    for(int i=0;i<20*17;i+=20){
        glRectf(10, 80-i, 70, 90-i);
    }
   

    
}

void display()
{
    
    //
    // clear window to black
    //
    glClearColor( 0, 0 , 0 , 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    background();
    if(F.active)
        drawFire(F);
    drawStar(S);
    firework();
    vprint2(-250, -350, 0.2, "Click your mouse for the firework to appear");
    glutSwapBuffers();


}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
   //if ( key == ' ' )
     //  F.active  =  !F.active;
    
    
    // to refresh the window it calls display() function
   glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y )
{
   // exit when ESC is pressed.
  // if ( key == ' ' )
    //   F.active = false;
    
    // to refresh the window it calls display() function
   glutPostRedisplay() ;
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown( int key, int x, int y )
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;

    }
     // to refresh the window it calls display() function
    glutPostRedisplay() ;
}


//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp( int key, int x, int y )
{
      // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;

    }
     // to refresh the window it calls display() function
    glutPostRedisplay() ;
}


void onClick( int button, int stat, int x, int y )
{
    // Write your codes here.
    

    
     // to refresh the window it calls display() function
    glutPostRedisplay() ;
}


//
void onResize( int w, int h )
{
    winWidth = w;
    winHeight = h;
    glViewport( 0,0,w,h) ;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown( int x, int y ) {
    // Write your codes here.
    
    F.active=1;
    F.pos.x= x-500;
    F.pos.y=-y+400;
    
     // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

void onMove( int x, int y ) {
    // Write your codes here.


    
     // to refresh the window it calls display() function
    glutPostRedisplay() ;
}

#if TIMER_ON == 1
void onTimer( int v ) {
     
    glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
    // Write your codes here.

    // Change orientation and/or position of the Player
    if (left) P.angle += 3;
    if (right) P.angle -= 3;
    if (up) P.pos = addV(P.pos, pol2rec({ PLAYER_SPEED, P.angle }));
    if (down) P.pos = subV(P.pos, pol2rec({ PLAYER_SPEED, P.angle }));
    
    S.pos.x=S.pos.x+a*3;
    if(S.pos.x>=400)
        a *=-1;
    if (S.pos.x<=-400)
        a *=-1;
    
    
    if(k != -1)k++;
    if(k >= 200)k = -1;
    

    F.angle=angleV(subV({S.pos.x,S.pos.y}, F.pos))+270;

    // flee or chase
    // velocity vector from Enemy to Player
    E.vel = mulV( ENEMY_SPEED, unitV(subV(P.pos, E.pos)));
    F.vel = mulV(5,unitV(subV(S.pos, F.pos)));


    // if the Player sees the Enemy, it flees (gets away)
    // otherwise, it chases Player.
    
   

    F.pos = addV(F.pos, F.vel);// chase
    //}

    

    
    if(sqrtf((F.pos.x-S.pos.x)*(F.pos.x-S.pos.x)+(F.pos.y-S.pos.y)*(F.pos.y-S.pos.y))<30){
        if(F.active){
            oldx = F.pos.x;
            oldy = F.pos.y;
            k = 0;
        }
        F.active=false;
    }
    else {
        p+=2;
    }
    
       
    
     // to refresh the window it calls display() function
    glutPostRedisplay() ; // display()

}
#endif

void Init() {
    
    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

}


int main( int argc, char *argv[] )
{

    glutInit(&argc, argv );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow( "Saida Rustamova" ) ;

    glutDisplayFunc( display ) ;
    glutReshapeFunc( onResize );
    //
    // keyboard registration
    //
    glutKeyboardFunc( onKeyDown ) ;
    glutSpecialFunc( onSpecialKeyDown ) ;

    glutKeyboardUpFunc( onKeyUp ) ;
    glutSpecialUpFunc( onSpecialKeyUp ) ;

    //
    // mouse registration
    //
    glutMouseFunc( onClick) ;
    glutMotionFunc( onMoveDown ) ;
    glutPassiveMotionFunc( onMove ) ;
    
    #if  TIMER_ON == 1
    // timer event
    glutTimerFunc( TIMER_PERIOD, onTimer, 0 ) ;
    #endif

    Init();
    
    glutMainLoop();
    return 0;
}

