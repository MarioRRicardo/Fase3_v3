#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <iostream>

#include "camera.h"
#include "catmull-rom.h"
#include "drawing.h"
#include "events.h"
#include "fps.h"

#define FPS 60

// ###########################################
// ####### VARIAVIES
// GLUT
const int WINDOW_W = 800;
const int WINDOW_H = 600;

// Câmera
int window;
Camera cam;

// FPS
unsigned int time, timebase, frame = 0;
char fpsStr[15];


float step;
float y[3] = { -1,-1,1 };


// ###########################################
// ####### FUNÇÕES
void printInfo() {
	printf("\nUse WSAD to move the camera up/down and left/right\n");
	printf("Q and Z control the distance from the camera to the origin\n");
	printf("Press C to reset the camera\n");
}


void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0);
}

void renderScene(void) {

	float M[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
					{ 1.0f, -2.5f,  2.0f, -0.5f},
					{-0.5f,  0.0f,  0.5f,  0.0f},
					{ 0.0f,  1.0f,  0.0f,  0.0f} };

	// FPS
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	printFPS();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(cam.px, cam.py, cam.pz,
		cam.lx, cam.ly, cam.lz,
		cam.vx, cam.vy, cam.vz);

	//Drawings
	renderCatmullRomCurve();

	float pos[4];
	float deriv[4];

	// Tranf
	glPushMatrix();
	getGlobalCatmullRomPoint(step, pos, deriv);
	normalize(deriv);

	glTranslatef(pos[0], pos[1], pos[2]);

	float *x = deriv;
	float z[3];

	cross(x, y, z);
	cross(z, x, y);
	buildRotMatrix(x, y, z, *M);
	glMultMatrixf(*M);

	glutWireTeapot(0.1);

	glPopMatrix();

	step += 0.005;

	//glutTimerFunc(1000 / FPS, timer, 0);
	
	// End of frame
	glutSwapBuffers();

}




int main(int argc, char **argv) {
	
	//timebase = glutGet(GLUT_ELAPSED_TIME);
	//frame = 0;

	camInit();
	printInfo();

	// ############### Inicialização do GLUT e JANELAS
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("Guiao8@CompGrafica");

	// ############### Registar os CALLBACK das funções dos eventos
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(1000/ FPS, timer,0);
	//glutIdleFunc(renderScene);
	//glewInit();
	
	// ############### Definições do KEYBOARD e MOUSE
	glutKeyboardFunc(keyboardEvents);
	glutSpecialFunc(keyboardSpecialEvents);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	// ############### Definições do MENU
	int fillMenu = glutCreateMenu(menuEvents);
	glutAddMenuEntry("Front", 1);
	glutAddMenuEntry("Back", 2);
	glutAddMenuEntry("Front & Back", 3);
	int lineMenu = glutCreateMenu(menuEvents);
	glutAddMenuEntry("Front", 4);
	glutAddMenuEntry("Back", 5);
	glutAddMenuEntry("Front & Back", 6);
	int mainMenu = glutCreateMenu(menuEvents);
	glutAddSubMenu("Fill", fillMenu);
	glutAddSubMenu("Line", lineMenu);

	// Associar CLICK ao menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Callback do GLEW - Tem de estar depois de todos os callbacks do GLUT
	glewInit();

	// ############### Definições do OPENGL
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);	// DESATIVADO: Mostra triângulos de tras
	glPolygonMode(GL_FRONT, GL_LINE);


	// ############### Entrar no ciclo do GLUT
	glutMainLoop();
	
	return 1;
}
