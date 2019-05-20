#define _USE_MATH_DEFINES
#include <math.h>

// ###########################################
// ####### ESTRUTURAS DE DADOS

typedef struct sCamera {
	// Variaveis mutaveis
	float alpha, beta;
	float distance;

	// Posição da cmera
	float px, py, pz;
	// LookAt da camera
	float lx, ly, lz;

	// Vetor de orientação da câmera
	float vx, vy, vz;
} Camera, *pCamera;


// ###########################################
// ####### FUNÇÕES
void camInit();
void camPos();

void lookHorz(float val);
void lookVert(float val);
void moveHorz(float val);
void moveVert(float val);
void moveNear(float val);
void moveUp(float val);
void moveDown(float val);