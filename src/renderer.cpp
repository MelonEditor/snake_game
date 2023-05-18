
#include <glut.h>
#include <iostream>
#include "game.h"
using namespace std;

void timer_callback(int);
void display_callback();
void reshape_callback(int width, int height);
void init();

int main(int argc, char** argv){
    randomInit();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(300,100);
    glutInitWindowSize(700, 700);
    glutCreateWindow("snake");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutTimerFunc(0, timer_callback, 0);
    glutKeyboardFunc(processNormalKeys);
    init();
    glutMainLoop();
    return 0;
}

void init(){
    glClearColor(.2f, .2f, .2f, 1.0f);
    init_game();
}
void reshape_callback(int width, int height){
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}
void display_callback(){
    glClear(GL_COLOR_BUFFER_BIT);
    snake();
    draw_grid();
    glutSwapBuffers();
}
void timer_callback(int){
    glutPostRedisplay();
    glutTimerFunc(1000 / fps, timer_callback, 0);
}
