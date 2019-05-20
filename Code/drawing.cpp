#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#define ANG2RAD M_PI/180.0 
#include <math.h>
#include <stdio.h>

#include "drawing.h"

void axis(float width, float height){
	glBegin(GL_TRIANGLES);
	
	//Triângulo 1
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(width, 0, -height);
	glVertex3f(-width, 0, height);
	glVertex3f(-width, 0, -height);
	
	//Triângulo 1
	glColor3f(0.8, 0.8, 0.8);
	glVertex3f(width, 0, -height);
	glVertex3f(width, 0, height);
	glVertex3f(-width, 0, height);
	
	
	glEnd();
}

void box(float width, float height, float length){
	// Definir FRENTE/TRAS
	int side = 1;
	float x, y, z;

	glBegin(GL_TRIANGLES);
	
	// Criar completa lados paralelos
	for (int i = 1; i <= 2; i++) {
		x = width * side;
		y = height;
		z = length * side;

		//PLANO XY
		// //Triângulo 1
		glColor3f(0.3, 0.3, 0.3);
		glVertex3f(-x / 2, -y / 2, z / 2);
		glVertex3f(x / 2, -y / 2, z / 2);
		glVertex3f(-x / 2, y / 2, z / 2);

		// #### Triangulo 2 ####
		glVertex3f( -x / 2, y / 2, z / 2);
		glVertex3f( x / 2, -y / 2, z / 2);
		glVertex3f( x / 2, y / 2, z / 2);


		//PLANO YZ	
		// #### Triangulo 1 ####
		glVertex3f( x / 2, -y / 2, z / 2);
		glVertex3f( x / 2, -y / 2, -z / 2);
		glVertex3f( x / 2, y / 2, z / 2);

		// #### Triangulo 2 ####
		glVertex3f( x / 2, y / 2, z / 2);
		glVertex3f( x / 2, -y / 2, -z / 2);
		glVertex3f( x / 2, y / 2, -z / 2);

		//PLANO XZ	
		x = width * side;
		y = height * side;
		z = length * side;

		// #### Triangulo 1 ####
		glVertex3f( -x / 2, y / 2, z / 2);
		glVertex3f( x / 2, y / 2, z / 2);
		glVertex3f( -x / 2, y / 2, -z / 2);

		// #### Triangulo 2 ####
		glVertex3f(-x / 2, y / 2, -z / 2);
		glVertex3f(x / 2, y / 2, z / 2);
		glVertex3f(x / 2, y / 2, -z / 2);

		// Troca para o lado paralelo oposto
		side = -1;
	}

	glEnd();
}

void pyramid(float width, float height) {

	glBegin(GL_TRIANGLES);
	//LADOS
	glColor3f(1, 0, 0);
	glVertex3f(0, height, 0);
	glVertex3f(-width, 0, width);
	glVertex3f(width, 0, width);

	glColor3f(0, 1, 0);
	glVertex3f(0, height, 0);
	glVertex3f(width, 0, width);
	glVertex3f(width, 0, -width);

	glColor3f(0, 0, 1);
	glVertex3f(0, height, 0);
	glVertex3f(width, 0, -width);
	glVertex3f(-width, 0, -width);


	glColor3f(0, 1, 1);
	glVertex3f(0, height, 0);
	glVertex3f(-width, 0, -width);
	glVertex3f(-width, 0, width);

	//Base
	glColor3f(0.3, 0.3, 0.3);
	glVertex3f(width, 0, -width);
	glVertex3f(-width, 0, width);
	glVertex3f(-width, 0, -width);

	glVertex3f(width, 0, -width);
	glVertex3f(width, 0, width);
	glVertex3f(-width, 0, width);

	glEnd();
}

void cylinder(int radious, int height, int slices) {

	// Dividir angulo em fatias -- RADIANOS para se usar COS e SIN
	double alpha = 2*M_PI / slices;
	int slice = 0;
	double pt2_x, pt2_z, pt3_x, pt3_z;

	glBegin(GL_TRIANGLES);
	glColor3f(0.3, 0.3, 0.3);

	for (slice = 0; slice < slices; slice++) {
		pt2_x = sin(alpha*slice)*radious;
		pt2_z = cos(alpha*slice)*radious;
		pt3_x = sin(alpha*(slice + 1))*radious;
		pt3_z = cos(alpha*(slice + 1))*radious;

		// Topo
		glVertex3f(0, height, 0);
		glVertex3f(pt2_x, height, pt2_z);
		glVertex3f(pt3_x, height, pt3_z);

		// Lateral 1
		glVertex3f(pt3_x, height, pt3_z);
		glVertex3f(pt2_x, height, pt2_z);
		glVertex3f(pt2_x, 0, pt2_z);

		// Lateral 2
		glVertex3f(pt2_x, 0, pt2_z);
		glVertex3f(pt3_x, 0, pt3_z);
		glVertex3f(pt3_x, height, pt3_z);

		// Fundo
		glVertex3f(0, 0, 0);
		glVertex3f(pt3_x, 0, pt3_z);
		glVertex3f(pt2_x, 0, pt2_z);
	}
	glEnd();
}

void cylinderStripFan(float radius, float height, int sides) {

	int i;
	float step;

	step = 360.0/sides;

	glColor3f(1,0,0);
	glBegin(GL_TRIANGLE_FAN);

		glVertex3f(0,height*0.5,0);
		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_TRIANGLE_FAN);

		glVertex3f(0,-height*0.5,0);
		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();

	glColor3f(0,0,1);
	glBegin(GL_TRIANGLE_STRIP);

		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
			glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();
}

void cone(int radious, int height, int slices, int stacks) {

	// Dividir angulo em fatias -- RADIANOS para se usar COS e SIN
	double alphaStep = 2 * M_PI / slices;
	float radStep;
	int slice;
	int stack = 0;
	int triangle;
	double pt1_x, pt1_y, pt1_z, pt2_x, pt2_y, pt2_z, pt3_x, pt3_y, pt3_z;

	glBegin(GL_TRIANGLES);
	glColor3f(0.7, 0.7, 0.7);

	// Desenhar cada fatia
	for (slice = 0; slice < slices; slice++) {
		pt1_x = 0;
		pt1_y = 0;
		pt1_z = 0;

		pt2_x = sin(alphaStep * (slice + 1)) * radious * ((stacks - stack) / (float)stacks);
		pt2_y = height * (stack / (float)stacks);
		pt2_z = cos(alphaStep * (slice + 1)) * radious * ((stacks - stack) / (float)stacks);

		pt3_x = sin(alphaStep * slice) * radious * ((stacks - stack) / (float)stacks);
		pt3_y = pt2_y;
		pt3_z = cos(alphaStep * slice) * radious * ((stacks - stack) / (float)stacks);
		triangle = 0;

		// Fundo
		glVertex3f(pt1_x, pt1_y, pt1_z);
		glVertex3f(pt2_x, pt2_y, pt2_z);
		glVertex3f(pt3_x, pt3_y, pt3_z);
		
		// Desenhaar cada camada
		for (stack = 0; stack < stacks - 1; stack++) {
			
			// Triangulo 1
			pt1_x = pt3_x; pt1_y = pt3_y; pt1_z = pt3_z;
			pt3_x = sin(alphaStep * (slice + 1)) * radious * ((stacks - (stack + 1)) / (float)stacks);
			pt3_y = height * ((stack + 1) / (float)stacks);
			pt3_z = cos(alphaStep * (slice + 1)) * radious * ((stacks - (stack + 1)) / (float)stacks);
			triangle = 1;

			glVertex3f(pt1_x, pt1_y, pt1_z);
			glVertex3f(pt2_x, pt2_y, pt2_z);
			glVertex3f(pt3_x, pt3_y, pt3_z);

			// Triangulo 2
			pt2_x = pt3_x; pt2_y = pt3_y; pt2_z = pt3_z;
			pt3_x = sin(alphaStep * slice) * radious * ((stacks - (stack + 1)) / (float)stacks);
			pt3_y = height * ((stack + 1) / (float)stacks);
			pt3_z = cos(alphaStep * slice) * radious * ((stacks - (stack + 1)) / (float)stacks);
			triangle = 2;

			glVertex3f(pt1_x, pt1_y, pt1_z);
			glVertex3f(pt2_x, pt2_y, pt2_z);
			glVertex3f(pt3_x, pt3_y, pt3_z);
		}

		stack = 0;

		// TOPO
		pt1_x = pt3_x; pt1_y = pt3_y; pt1_z = pt3_z;
		pt3_x = 0;
		pt3_y = height;
		pt3_z = 0;

		glVertex3f(pt1_x, pt1_y, pt1_z);
		glVertex3f(pt2_x, pt2_y, pt2_z);
		glVertex3f(0, height, 0);

	}
	glEnd();
}

void sphere(float rad, float slices, float layers) {

	// Angulos
	float alphaAmp = 2 * M_PI / slices;
	float alpha1 = 0, alpha2 = 0;			// Horizontal
	float betaAmp = M_PI / layers;
	float beta1 = -(M_PI / 2), beta2 = 0;	// Vertical

	// Pontos
	float pt1X;	float pt1Y;	float pt1Z;
	float pt2X;	float pt2Y;	float pt2Z;
	float pt3X;	float pt3Y;	float pt3Z;

	
	glBegin(GL_TRIANGLES);

	// Começar desenho por baixo
	beta1 = -(M_PI / 2);

	for (int i = 0; i < layers; i++) {
		beta2 = beta1 + betaAmp;

		alpha1 = 0;
		for (int j = 0; j < slices; j++) {

			alpha2 = alpha1 + alphaAmp;

			// #### Triangulo 1 ####
			// Calcular coordenadas do ponto 1
			pt1X = rad * cos(beta1) * sin(alpha1);
			pt1Y = rad * sin(beta1);
			pt1Z = rad * cos(beta1) * cos(alpha1);

			// Calcular coordenadas do ponto 2
			pt2X = rad * cos(beta1) * sin(alpha2);
			pt2Y = rad * sin(beta1);
			pt2Z = rad * cos(beta1) * cos(alpha2);

			// Calcular cordenadas do ponto 3
			pt3X = rad * cos(beta2) * sin(alpha1);
			pt3Y = rad * sin(beta2);
			pt3Z = rad * cos(beta2) * cos(alpha1);

			// Desenhar pontos do triângulo 1
			glVertex3f(pt1X, pt1Y, pt1Z);
			glVertex3f(pt2X, pt2Y, pt2Z);
			glVertex3f(pt3X, pt3Y, pt3Z);

			// #### Triangulo 2 ####
			// Recalcular coordenadas do ponto 1
			pt1X = pt3X;
			pt1Y = pt3Y;
			pt1Z = pt3Z;

			// Recalcular coordenadas do ponto 2    | PONTO 2 do triângulo anteior é o PONTO 2 deste novo triângulo

			// Calcular coordenadas do ponto 3
			pt3X = rad * cos(beta2) * sin(alpha2);
			pt3Y = rad * sin(beta2);
			pt3Z = rad * cos(beta2) * cos(alpha2);

			// Desenhar pontos do triângulo 2
			glVertex3f(pt1X, pt1Y, pt1Z);
			glVertex3f(pt2X, pt2Y, pt2Z);
			glVertex3f(pt3X, pt3Y, pt3Z);

			// Próximo SLICE
			alpha1 = alpha2;
		}
		// Próximo CAMADA
		beta1 = beta2;
	}
	
		
	glEnd();
	
}