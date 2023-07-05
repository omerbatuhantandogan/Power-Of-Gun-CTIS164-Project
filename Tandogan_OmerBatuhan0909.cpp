/*
   CTIS164 - Template Source Program
----------
STUDENT : Omer Batuhan Tandogan
SECTION :3
HOMEWORK:2
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:

the object firing at the target rotates at an angle

*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


int difflag;
bool activeTimer = false;
bool game = false;
int diffangle;
float targetX, targetY;
float counter = 6000;
bool bulletFired = false;
int bulletX, bulletY;
int bulletDistance = 0;



typedef struct {
    int angle;
    float firex, firey;
}gun_t;



gun_t gunpos;



//
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
    vsprintf_s(str, string, ap);
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
    vsprintf_s(str, string, ap);
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
float sx(int x, int y, int ang) {
    return x * cos(ang * D2R) - y * sin(ang * D2R);
}
// rotation formula with given angle
float sy(int x, int y, int ang) {
    return x * sin(ang * D2R) + y * cos(ang * D2R);
}

float randnum() {

    int number1 = rand() % 255 + 100;

    int number2 = rand() % 2;

    if (number2 == 0)
        number1 *= -1;


    return  number1;




}

void drawTarget() {

    glColor3f(0.45, 0.60, 0.90);
    circle(targetX, targetY, 45);
}

void randomTarget() {

    targetX = randnum();
    targetY = randnum();
}




void gun(int angle) {

    glLineWidth(4);
    glColor3f(1, 1, 1);
    circle_wire(sx(10, -5, angle), sy(10, -5, angle), 15);
    glLineWidth(1);

    glColor3f(0, 0, 0);
    glBegin(GL_QUAD_STRIP);
    glVertex2d(sx(53.5, 36.5, angle), sy(53.5, 36.5, angle));
    glVertex2d(sx(60, 36.5, angle), sy(60, 36.5, angle));
    glVertex2d(sx(53.5, 24.5, angle), sy(53.5, 24.5, angle));
    glVertex2d(sx(60, 24.5, angle), sy(60, 24.5, angle));
    glVertex2d(sx(60, 24.5, angle), sy(60, 24.5, angle));
    glVertex2d(sx(60, 0.5, angle), sy(60, 0.5, angle));
    glVertex2d(sx(48.5, 24.5, angle), sy(48.5, 24.5, angle));
    glVertex2d(sx(48.5, -0.5, angle), sy(48.5, -0.5, angle));
    glVertex2d(sx(0, 24, angle), sy(0, 24, angle));
    glColor3f(1, 1, 1);
    glVertex2d(sx(-0.5, 0.5, angle), sy(-0.5, 0.5, angle));
    glVertex2d(sx(-24.5, 24, angle), sy(-24.5, 24, angle));
    glVertex2d(sx(-25.5, 1, angle), sy(-25.5, 1, angle));
    glVertex2d(sx(-25.5, 1, angle), sy(-25.5, 1, angle));
    glVertex2d(sx(-25.5, 1, angle), sy(-25.5, 1, angle));
    glVertex2d(sx(-25.5, 1, angle), sy(-25.5, 1, angle));
    glColor3f(0, 0, 0);
    glVertex2d(sx(-0.5, 0, angle), sy(-0.5, 0, angle));
    glVertex2d(sx(-24.5, -45, angle), sy(-24.5, -45, angle));
    glVertex2d(sx(-0.5, -45, angle), sy(-0.5, -45, angle));



    glEnd();

    glColor3f(1, 0, 0);

    vprint(sx(-0.5, -45, angle), sy(-0.5, -45, angle), GLUT_BITMAP_8_BY_13, "%d", abs(angle));



    gunpos.firex = sx(65, 10, angle);
    gunpos.firey = sy(65, 10, angle);








}




//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(255, 255, 255, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    vprint(-700, 380, GLUT_BITMAP_HELVETICA_18, "Omer Batuhan Tandogan");
    glColor3f(1, 0, 0);
    vprint(-700, 360, GLUT_BITMAP_HELVETICA_18, "21803754");
    glColor3f(1, 0, 0);
    vprint(-700, 340, GLUT_BITMAP_HELVETICA_18, "CTIS164-3");

    if(counter==6000)
    vprint(-100, -390, GLUT_BITMAP_HELVETICA_18, "Press <F1> to start the game");
    if (game) {
        drawTarget();
        gun(gunpos.angle);
    }


    if (counter > 0) {
        glColor3f(1, 0, 0);
        vprint(-650, -300, GLUT_BITMAP_HELVETICA_18, "%4.2f", counter / 100);
       
    }
    else {
        glColor3f(1, 1, 1);
        vprint(-100, -390, GLUT_BITMAP_HELVETICA_18, "Game Over");
      
    }

    glColor3f(0, 0, 0);
    if (bulletFired)
        circle(bulletX, bulletY, 10);

   
   
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);


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
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
        while (counter != 0) {

    case GLUT_KEY_F1: game = true;
        //pause restart

        activeTimer = true; //to stop or resume timer.

        break;
        }
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
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
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


int is_clockwise() {

    int control[3];

    control[0] = diffangle - gunpos.angle;
    control[1] = diffangle - gunpos.angle + 360;
    control[2] = diffangle - gunpos.angle - 360;

    int mindex = 0;

    for (int i = 0; i < 3; i++) {
        if (abs(control[i]) < abs(control[mindex]))
            mindex = i;
    }

    if (control[mindex] > 0)
        return 1;
    return -1;
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (activeTimer && counter > 0) {
        counter--;
        diffangle = (atan2(targetY, targetX) / D2R);
        diffangle = diffangle > 0 ? diffangle : diffangle + 360;

        if (diffangle != gunpos.angle) {
            gunpos.angle += is_clockwise();
            if (gunpos.angle > 360)
                gunpos.angle -= 360;
            else if (gunpos.angle < 0)
                gunpos.angle += 360;
            
            bulletFired = false;
        }
        else {
            bulletFired = true;
            bulletX = gunpos.firex + cos(diffangle * D2R) * bulletDistance;
            bulletY = gunpos.firey + sin(diffangle * D2R) * bulletDistance;
            bulletDistance += 2;
            
            if (powf((targetY - bulletY), 2) + powf((targetX - bulletX), 2) < 2000) {
                randomTarget();
                bulletX = gunpos.firex;
                bulletY = gunpos.firey;
                bulletFired = false;
                bulletDistance = 0;
                
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
    gunpos.angle = 0;
    randomTarget();


}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("CTIS 164 HOMEWORK2 by Omer Batuhan Tandogan -  ''power of Gun'' ");
    srand(time(NULL));
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
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}