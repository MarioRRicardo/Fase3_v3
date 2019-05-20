#include "camera.h"

// ###########################################
// ####### VARIAVEIS
extern Camera cam;


// ###########################################
// ####### FUNÇÕES
void camInit() {

	cam.distance = 150.0f;
	cam.alpha = 0.0f;
	cam.beta = 0.0f;

	cam.px = 0.0f;
	cam.py = 0.0f;
	cam.pz = 5.0f;

	cam.lx = 0.0f;
	cam.ly = 0.0f;
	cam.lz = 0.0f;

	cam.vx = 0.0f;
	cam.vy = 1.0f;
	cam.vz = 0.0f;

	camPos();
}

void camPos() {
	cam.px = cam.distance * cos(cam.beta) * sin(cam.alpha);
	cam.py = cam.distance * sin(cam.beta);
	cam.pz = cam.distance * cos(cam.beta) * cos(cam.alpha);
}

void moveHorz(float val) {
	cam.alpha += val;
	camPos();
}

void moveVert(float val) {
	if ((fabs(cam.beta + val) < 1.5) && (-fabs(cam.beta + val) > -1.5)) {
		cam.beta += val;
		camPos();
	}
}

void lookHorz(float val) {
	cam.lx += val;
	camPos();
}

void lookVert(float val) {
	cam.ly += val;
	camPos();
}

void moveUp(float val) {
	if (fabs(cam.beta + val) < M_PI * 0.5) {
		cam.beta += val;
		camPos();
	}
}
void moveDown(float val) {
	if (-fabs(cam.beta - val) > -M_PI * 0.5) {
		cam.beta -= val;
		camPos();
	}
}

void moveNear(float val) {
	cam.distance += val;
	camPos();
}
