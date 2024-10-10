
/*

 Additional functions: press s to drop a gift
                        moving snowflakes
 Possibble problems: this code was wrtitten using Xcode compiler therefore the librery name is different. Vsprintf is used instead of vsprintf_s
 */
 

#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  15 // Period for the timer.
#define TIMER_ON      1   // 0:disable timer, 1:enable timer

#define D2R 0.0174532

bool activeTimer = true;

//draw a circle
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

//draw the moon
void moon( int x, int y, int r )
{
#define PI 3.1415
   float angle ;
   glBegin( GL_POLYGON ) ;
    glColor3ub(255, 255, 192);
    //set the central colour so there is a gradient
    glVertex2f(x, y);
    glColor3ub(255, 255, 110);
    
   for ( int i = 0 ; i < 120 ; i++ )
   {
      angle = 2*PI*i/100;
      glVertex2f( x+r*cos(angle), y+r*sin(angle)) ;
   }
   glEnd();
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


int x=0;
int giftx[1000]={0}, gifty[1000]={0}, isgift[1000]={0}, giftnum=0;
void onTimer(int v) {

    glutTimerFunc(16, onTimer, 0);

    if (activeTimer) {
        x++;
        
    }
    for(int f=0; f<giftnum; f++){
        //to drop the gift
        gifty[f]-=3;
    }
    //sleigh re-appears
    if(x == 500){
        x=-200;
    }
    glutPostRedisplay();
}

   
void gift(int x, int y){
    glColor3ub(255, 0, 0);
    glRectf(x-10,y-10,x+10,y+10) ;
    glBegin(GL_LINES);
    glLineWidth(2.5f);
    glColor3ub(255, 255, 0);
    glVertex2f(x, y-10);
    glVertex2f(x, y+10);
    glVertex2f(x-10, y);
    glVertex2f(x+10, y);
    
    glVertex2f(x, y+10);
    glVertex2f(x+5, y+15);
    glVertex2f(x, y+10);
    glVertex2f(x-5, y+15);
    
    glEnd();
    
    
}


void wallpaper(){
    //draw rectagles using a loop
    
    int i = 0;
        for (int cnt = 0; cnt < 5; cnt++) {
            glColor3ub(201, 193, 162);
            glRectf( -500+i, -180, -480+i, 300 ) ;
            glColor3ub(115, 121, 107);
            glRectf( -480+i, -180, -460+i, 300 ) ;
            i += 40;
            }
    
        for (int cnt = 0; cnt < 15; cnt++) {
            glColor3ub(201, 193, 162);
            glRectf( -500+i, 250, -480+i, 300 ) ;
            glColor3ub(115, 121, 107);
            glRectf( -480+i, 250, -460+i, 300 ) ;
            i += 40;
            }
    for (int cnt = 0; cnt < 5; cnt++) {
        glColor3ub(201, 193, 162);
        glRectf( -500+i, -180, -480+i, 300 ) ;
        glColor3ub(115, 121, 107);
        glRectf( -480+i, -180, -460+i, 300 ) ;
        i += 40;
    }
    //podokonnik
    glColor3ub(51, 25, 0);
    glRectf( -500, -300, 500, -180 ) ;
}

void frame(){
    
    //frame
    glColor3ub(63, 33, 4);
    glRectf( -300, -180, -270, 250 ) ;
    glRectf( 270, -180, 300, 250 ) ;
    glRectf( -10, -180, 10, 250 ) ;
    glRectf( -300, 220, 300, 250 ) ;
}

void snowx(){
    int dx=0;
    for(int k=0; k<10;k++){
        int size = rand() % 3 + 2; // random size between 2 and 6
        int x = -300 + dx + rand() % 50 - 25; // random x position within 25 pixels of original position
        int y = -150 + rand() % 50 - 25; // random y position within 25 pixels of original position
        glColor3ub(255, 255, 255);
        circle(x, y, size);
        dx+=70;
    }
}
void snow(){
    int dy=0;
    for(int k=0; k<10;k++){
        if(k%2==0){
            glTranslatef(0+35, dy, 0);
            snowx();
        }
        else{
            glTranslatef(0, dy, 0);
            snowx();
            
        }
        glLoadIdentity();
        dy+=50;
    }
}




void sleigh(){
    //add x so the sligh moves with time
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3ub(255, 255, 0);
    glVertex2f(-175+x, 50);
    glVertex2f(-180+x, 40);
    
    glVertex2f(-155+x, 50);
    glVertex2f(-150+x, 40);
    
    glVertex2f(-200+x, 40);
    glVertex2f(-130+x, 40);
    
    glVertex2f(-130+x, 40);
    glVertex2f(-125+x, 45);
    
    glVertex2f( -177+x, 72 ) ;
    glVertex2f( -142+x, 72 ) ;
    glEnd();
    
    
    
    
    
    glColor3f( 1, 0, 0 ) ;
    glBegin( GL_QUADS ) ;
    glVertex2f( -200+x, 90 ) ;
    glVertex2f( -180+x, 80 ) ;
    glVertex2f( -160+x, 50 ) ;
    glVertex2f( -190+x, 50 ) ;
    
    glEnd() ;
    
    
    glBegin( GL_QUADS ) ;
    glVertex2f( -175+x, 72 ) ;
    glVertex2f( -140+x, 72 ) ;
    glVertex2f( -140+x, 50 ) ;
    glVertex2f( -175+x, 50 ) ;
    glEnd() ;
    
    //stickman
    glColor3ub(255, 204, 153);
    circle(-160+x, 80, 9);
    glBegin(GL_LINES);
    glLineWidth(2.5f);
    glVertex2f(-160+x, 80);
    glVertex2f(-160+x, 65);
    glEnd();
    
    //beard
    glBegin(GL_TRIANGLES);
    glColor3ub(255, 255, 255);
    glVertex2f(-168+x, 77);
    glVertex2f(-151+x, 77);
    glVertex2f(-160+x, 65);
    //hat
    glColor3ub(255, 0, 0);
    glVertex2f(-167+x, 85);
    glVertex2f(-152+x, 85);
    glVertex2f(-160+x, 100);
    glEnd();
    glColor3ub(255, 255, 255);
    circle(-160+x, 100, 3);
    
    //eyes
    glColor3ub(0, 0, 0);
    circle(-163+x, 80, 1);
    circle(-156+x, 80, 1);
    
    
    //sleigh
    glBegin( GL_QUADS ) ;
    glColor3f( 1, 0, 0 ) ;
    glVertex2f( -140+x, 75 ) ;
    glVertex2f( -130+x, 80 ) ;
    glVertex2f( -140+x, 50 ) ;
    glVertex2f( -160+x, 50 ) ;
    
    glEnd() ;
    
    glLineWidth(2.5f); // set line width to 3 pixels
    glBegin(GL_LINE_STRIP);
    glColor3ub(255, 255, 0);
    glVertex2f(-200+x, 90);
    glVertex2f(-180+x, 80);
    glVertex2f( -175+x, 65 ) ;
    glVertex2f( -145+x, 65 ) ;
    glVertex2f( -140+x, 75 ) ;
    glVertex2f( -130+x, 80 ) ;
    glEnd();
}

int xsleigh=0;
    
void display()
{
   
   //
   // clear window to black
   //
   glClearColor( 0, 0 , 0 , 0 );
   glClear( GL_COLOR_BUFFER_BIT );
  
    
    
    //sky
    glColor3f( 0.0, 0.0, 1.0 ) ;
    glRectf( -300, -180, 300, 250 ) ;
    
    //moon
    moon( -230, 180, 100 ) ;
    
    //mountains
    glBegin( GL_TRIANGLES );
    glColor3f( 0, 0, 0 ) ;
      glVertex2f(194, 76 ) ;
      glVertex2f(-77, -200 ) ;
      glVertex2f(  395, -200 ) ;
    glEnd();
    
    glBegin( GL_TRIANGLES );
    glColor3f( 0.0, 0, 0.4 ) ;
      glVertex2f(-170, -11 ) ;
      glVertex2f(-390, -194 ) ;
      glVertex2f(  130, -187 ) ;
    glEnd();
    
    
    xsleigh=x;
    for(int f=0; f<giftnum; f++){
        if (isgift[f]) {
            gift(giftx[f],gifty[f]);
        }
        
    }
    
    sleigh();
    snow();
    wallpaper();
    frame();
    
    //name surname
    glColor3ub( 0, 0, 102 ) ;
    glRectf( -120, -250, 120, -200 ) ;
    glColor3f(1, 1, 1);
    vprint( -60, -230, GLUT_BITMAP_8_BY_13,"Saida Rustamova");
    vprint( 175, -230, GLUT_BITMAP_8_BY_13,"Press the 'S' key for a surprise");
    vprint( 175, -250, GLUT_BITMAP_8_BY_13,"Press the 'SPACE' key to stop the sleigh");
   
     
       glutSwapBuffers();

    }

      

    //
    void onKeyDown(unsigned char key, int x, int y )
    {
      //stop when space key is pressed
       if ( key == 32)
           activeTimer=!activeTimer;
        //drop the gift when the S/s key is pressed
        if( key== 's'|| key == 'S'){
            isgift[giftnum]=1;
            gifty[giftnum]=35;
            giftx[giftnum]=xsleigh-150;
            giftnum++;
            
        }
       // to refresh the window it calls display() function
       glutPostRedisplay() ;
    }

  

        
        void onResize( int w, int h )
        {
           glViewport( 0,0,w,h) ;
           glMatrixMode( GL_PROJECTION );
           glLoadIdentity();
           glOrtho( -w/2, w/2, -h/2, h/2, -1, 1);
           glMatrixMode( GL_MODELVIEW);
           glLoadIdentity();
        }

int main( int argc, char *argv[] )
{
    glutInit(&argc, argv );
   glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutCreateWindow( "Santa Claus is coming to town" ) ;

   glutDisplayFunc( display ) ;
   glutReshapeFunc( onResize );
   glutKeyboardFunc( onKeyDown ) ;
glutTimerFunc(1, onTimer, 0);
   
   glutMainLoop();
    return 0;
}
