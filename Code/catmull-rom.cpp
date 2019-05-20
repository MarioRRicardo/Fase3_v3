#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "catmull-rom.h"
#include "engine.h"

// ###########################################
// ####### VARIAVEIS
extern float step;

float M[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
				{ 1.0f, -2.5f,  2.0f, -0.5f},
				{-0.5f,  0.0f,  0.5f,  0.0f},
				{ 0.0f,  1.0f,  0.0f,  0.0f} };

// ###########################################
// ####### FUNÇÕES

void buildRotMatrix(float *x, float *y, float *z, float *m) {
	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {
	res[0] = a[1]*b[2] - a[2]*b[1];
	res[1] = a[2]*b[0] - a[0]*b[2];
	res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {
	float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0]/l;
	a[1] = a[1]/l;
	a[2] = a[2]/l;
}

float length(float *v) {
	float res = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	return res;

}

void multMatrixVector(float *m, float *v, float *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}

}

float distance2points(float a[3], float b[3]) {
	return sqrt(powf(a[0] - b[0], 2) + powf(a[1] - b[1], 2) + powf(a[2] - b[2], 2));
}

void getCatmullRomPoint(float t, int* indices, float* pos, float* deriv, CatmullCurve *catmullPoints){
	vector<Vertice> points = catmullPoints->getCatmullPoints();

	float A[4] = { 0,0,0,0 };
	int i, j;

	float T[] = { powf(t,3), powf(t,2), t, 1 };
	float Td[] = { 3 * powf(t,2), 2 * t, 1, 0 };

	pos[0] = pos[1] = pos[2] = 0.0;
	deriv[0] = deriv[1] = deriv[2] = 0.0;
	
	// Compute point res = T * M * P
	//for (i = 0; i < 3; ++i) {

		// Compute A = T * M
		//multMatrixVector(T, (float *)M, A);

		// Compute pos = A * P
		for (j = 0; j < 4; ++j) {
			pos[0] += A[j] * points[indices[j]].getX();
			pos[1] += A[j] * points[indices[j]].getY();
			pos[2] += A[j] * points[indices[j]].getZ();
		}

	//}

	// Repetir o processo para a derivada
	//for (i = 0; i < 3; ++i) {

		// Compute A = T * M
		//multMatrixVector(Td, (float *)M, A);
		for (j = 0; j < 4; ++j) {
			//deriv[i] += A[j] * catmullPoints[indices[j]][i];
			deriv[0] += A[j] * points[indices[j]].getX();
			deriv[1] += A[j] * points[indices[j]].getY();
			deriv[2] += A[j] * points[indices[j]].getZ();
		}
	//}
}


void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, CatmullCurve *catmullPoints) {
	int pointsCount = (int) catmullPoints->getCatmullPoints().size();

	float t = gt * pointsCount;
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4]; 
	indices[0] = (index + pointsCount - 1) % pointsCount;
	indices[1] = (indices[0]+1) % pointsCount;
	indices[2] = (indices[1]+1) % pointsCount;
	indices[3] = (indices[2]+1) % pointsCount;

	getCatmullRomPoint(t, indices, pos, deriv, catmullPoints);
}


void renderCatmullRomCurve(CatmullCurve * catmullPoints) {
	// desenhar a curva usando segmentos de reta - GL_LINE_LOOP
	float pos[3];
	float deriv[3];
	float step;
	
	//gera a linha a partir dos pontos gerados acima
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < CATMULL_CURVE_STEPS; i++) {
		step = i / CATMULL_CURVE_STEPS;
		getGlobalCatmullRomPoint(step, pos, deriv, catmullPoints);
		glVertex3f(pos[0], pos[1], pos[2]);
	}

	glEnd();
}
