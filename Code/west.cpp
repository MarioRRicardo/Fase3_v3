#include<stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#define ANG2RAD M_PI/180.0 

#include <math.h>
//#include "drawing.h"
#include "west.h"

#define COWBOYS 8
#define RAIO_COWBOYS 5
#define INDIOS 16
#define RAIO_INDIOS 25
#define ARVORES 1000
#define STEP_COWBOY 1.0f
#define STEP_INDIO 0.5f

// ###########################################
// ####### VARIAVEIS
extern float step;


// ###########################################
// ####### FUNCOES
void drawLand() {
	glColor3f(0.2f,0.8f,0.2f);
	
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-100.0, 0, 100.0);
	glVertex3f( 100.0, 0, 100.0);
	glVertex3f( 100.0, 0,-100.0);
	glVertex3f(-100.0, 0,-100.0);
	glEnd();
}

void drawTree() {
	glPushMatrix();
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 0.5);
	glutSolidCone(0.25f, 4, 5, 1);

	glColor3f(0.0f, 0.5f + rand() * 0.5f/RAND_MAX,0.0f);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glutSolidCone(2.0f, 5.0f, 5.0f, 1.0f);
	glPopMatrix();
}


void placeTrees() {
	float r = 35.0;
	float alpha;
	float rr;
	float x,z;

	srand(31457);
	int arvores = 0;

	while (arvores < ARVORES) {

		rr = rand() * 150.0/ RAND_MAX;
		alpha = rand() * 6.28 / RAND_MAX;

		x = cos(alpha) * (rr + r);
		z = sin(alpha) * (rr + r);

		if (fabs(x) < 100 && fabs(z) < 100) {

			glPushMatrix();
			glTranslatef(x,0.0,z);
			drawTree();
			glPopMatrix();
			arvores++;
		}
	}
}


void drawDonut() {
	glPushMatrix();
	glTranslatef(0.0,0.5,0.0);
	glColor3f(1.0f,0.0f,1.0f);
	glutSolidTorus(0.5,1.25,8,16);
	glPopMatrix();
}


void drawIndios() {
	float angulo;
	
	glColor3f(1.0f,0.0f,0.0f);
	
	for (int i = 0; i < INDIOS; i++) {
		angulo = i * 360.0/INDIOS + step * STEP_INDIO;
		
		glPushMatrix();
		glRotatef(angulo,0.0,1.0,0.0);
		glTranslatef(0.0,1,RAIO_INDIOS);
		glutSolidTeapot(1);
		glPopMatrix();
	}
}


void drawCowboys() {
	float angulo;
	
	glColor3f(0.0f,0.0f,1.0f);
	
	for (int i = 0; i < COWBOYS; i++) {
		angulo = i * 360.0/COWBOYS + step * STEP_COWBOY;
		
		glPushMatrix();
		glRotatef(-angulo,0.0,1.0,0.0);
		glTranslatef(RAIO_COWBOYS,1,0.0);
		glutSolidTeapot(1);
		glPopMatrix();
	}
}