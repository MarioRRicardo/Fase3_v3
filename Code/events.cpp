#include <Windows.h>

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
#include <cstring>

#include "events.h"
#include "camera.h"

#define ALPHAUNIT 0.05f
#define BETAUNIT 0.05f
#define DISTUNIT 0.07f


// ###########################################
// VARIAVEIS
extern int window;		// DESTROY WINDOW
extern Camera cam;		// Camera lx,ly,lz

bool mousePressed = false;

int xOrigin = -1;
int yOrigin = -1;
int tracking;


// ############################################################
// ####### KEYBOARD EVENTS
void keyboardEvents(unsigned char key, int x, int y) {

	switch (key) {

		// Deslocamento
	case 'W':
	case 'w':
		moveVert(BETAUNIT);
		break;
	case 'S':
	case 's':
		moveVert(-BETAUNIT);
		break;
	case 'A':
	case 'a':
		moveHorz(-BETAUNIT);
		break;
	case 'D':
	case 'd':
		moveHorz(BETAUNIT);
		break;
	case 'Q':
	case 'q':
		lookHorz(-BETAUNIT*10);
		break;
	case 'E':
	case 'e':
		lookHorz(BETAUNIT*10);
		break;
	case 'R':
	case 'r':
		lookVert(-BETAUNIT);
		break;
	case 'F':
	case 'f':
		lookVert(BETAUNIT);
		break;
	case 'Z':
	case 'z':
		moveNear(-DISTUNIT*10);
		break;
	case 'X':
	case 'x':
		moveNear(DISTUNIT*10);
		break;
	case 'C':
	case 'c':
		camInit();
		break;

		// ESQ - Sair
	case 27:
		glutDestroyWindow(window);
		exit(0);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

// Keyboard Event
void keyboardSpecialEvents(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		moveVert(BETAUNIT);
		break;
	case GLUT_KEY_DOWN:
		moveVert(-BETAUNIT);
		break;

	case GLUT_KEY_RIGHT:
		moveHorz(ALPHAUNIT);
		break;
	case GLUT_KEY_LEFT:
		moveHorz(-ALPHAUNIT);
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


// ############################################################
// ####### MOUSE EVENTS
void processMouseButtons(int button, int state, int xx, int yy) {

	if (state == GLUT_DOWN) {
		// Registar posição do CLICK
		xOrigin = xx;
		yOrigin = yy;

		switch (button) {
			case GLUT_LEFT_BUTTON:
				tracking = 1;
				break;

			default:
				tracking = 0;
		}
	}
	else {
		if (state == GLUT_UP) {
			/*switch (tracking) {
			case 1:
				break;

			default:
				*/tracking = 0;
			//}
		}

		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;

	if (!tracking)
		return;

	deltaX = xx - xOrigin;
	deltaY = yy - yOrigin;

	//if (tracking == 1) {
	cam.alpha -= deltaX * 0.005f;
	cam.beta += deltaY * 0.005f;

	if (cam.beta > 1.5)
		cam.beta = 1.5;
	else
		if (cam.beta < -1.5)
			cam.beta = -1.5;

	camPos();
}


// ############################################################
// ####### MENU EVENTS
void menuEvents(int option) {

	switch (option) {
	case 1:
		//glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT, GL_FILL);
		//glCullFace(GL_FRONT);
		break;
	case 2:
		//glEnable(GL_CULL_FACE); 
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	case 3:
		//glEnable(GL_CULL_FACE); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case 4:
		//glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case 5:
		//glDisable(GL_CULL_FACE); 
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	case 6:
		//glDisable(GL_CULL_FACE); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		break;

	default:
		break;
	}

	glutPostRedisplay();
}
