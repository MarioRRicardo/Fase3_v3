#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

// TinyXML CHECK MACROS
#include <XML/tinyxml2.h>
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#include <stdio.h>
#include <string>
#include <tuple>
using namespace std;

#include "catmull-rom.h"
#include "camera.h"
#include "events.h"
#include "fps.h"
#include "vbo.h"
#include "xmleng.h"
#include "engine.h"
using namespace tinyxml2;


// ###########################################
// ####### VARIAVIES
// GLUT
const int WINDOW_W = 800;
const int WINDOW_H = 600;
int window;
Camera cam;

std::string text2Draw;

// FPS
unsigned int time, timebase, frame = 0;
char fpsStr[15];

// XML
char * object;
char * fileName;
XMLDocument xmlDoc;
XMLNode * xmlRoot;
XMLError xmlError;

// Cena
list<Group *> scene;

// Tempo
int baseTime, endTime, lapsedTime;


// ###########################################
// ####### FUNÇÕES

void printInfo() {
	const char * str1 = "\nUse W S A D to move the camera lookup (up/down and left/right)\nZ and X to control the distance from the camera to the origin\n";
	const char * str2 = "Q and E to change horizontal position  of the camera\n";
	const char * str3 = "R and F to change vertical position of the camera\n";
	const char * str4 = "Press C to reset the camera\n";
	std::string text2Draws = str1; text2Draw += str2; text2Draw += str3; text2Draw += str4;
	printf("%s", text2Draw);

	/*
	printf("\nUse W S A D to move the camera lookup (up/down and left/right)\n");
	printf("Z and X to control the distance from the camera to the origin\n");
	printf("Q and E to change horizontal position  of the camera\n");
	printf("R and F to change vertical position of the camera\n");
	printf("Press C to reset the camera\n");*/
}

void initGL() {
	glewInit();

	// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);

	// Inicialziar o VERTEX
	glEnableClientState(GL_VERTEX_ARRAY);

	// Gerar VBOs
	for each (Group * g in scene) {
		generateVBO(g);
	}
}


//	GLUT EVENTS
void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawScene(list<Group *> group) {
	for each (Group * g in group) {
		int type;
		float time, ang, x, y, z;

		list<GeometricTransformation *> lTransf = g->getTransformations();
		list<Model *> lModels = g->getModels();
		list<Group *> lGroups = g->getSubGroups();
		list<Vertice> lPontos;

		// Aplicar transformações
		glPushMatrix();

		for each(GeometricTransformation * t in lTransf) {
			type = t->getType();

			// Translação
			if (type == TRANSLATION) {
				x = t->getX(); y = t->getY(); z = t->getZ();
				if (x != 0 || y != 0 || z != 0) {
					glTranslatef(x, y, z);
				}
			}

			// Rotação
			if (type == ROTATION) {
				x = t->getX(); y = t->getY(); z = t->getZ();
				ang = t->getAlpha();
				time = t->getTime();

				if (time == 0) {
					if (ang != 0 || x != 0 || y != 0 || z != 0) {
						glRotatef(ang, x, y, z);
					}
				}
				else {
					if (ang != 0 || x != 0 || y != 0 || z != 0) {
						ang += (360.0 / t->getAlpha()) / 1000 * lapsedTime;
						t->setAlpha(ang);
						glRotatef(ang, x, y, z);
					}
				}
			}

			// Escala
			if (type == SCALE) {
				x = t->getX(); y = t->getY(); z = t->getZ();
				if (x != 0 || y != 0 || z != 0) {
					glScalef(x, y, z);
				}
			}

			// CATMULL
			if (type == CATMULL_CURVE) {
				CatmullCurve * points = t->getGeomTransCatmull();
				float pos[3], deriv[3];

				renderCatmullRomCurve(points);

				points->setNextPoint( points->getNextPoint() + (points->getPerimeter() / t->getTime()) / 1000 * lapsedTime );
				getGlobalCatmullRomPoint( points->getNextPoint() / CATMULL_CURVE_STEPS, pos, deriv, points);
				glTranslatef(pos[0], pos[1], pos[2]);

			}
		}

		// Processar modelos
		for each(Model * m in lModels) {
			// Triangulos
			lPontos = m->getPoints();

			for each(Vertice v in lPontos) {

				glColor3d(1.0, 1.0, 1.0);
				glBegin(GL_TRIANGLES);
				glVertex3f(v.getX(), v.getY(), v.getZ());
				glEnd();
			}


			// Nome
			glRasterPos2i(0, 5);
			string s = m->getName();
			void * font = GLUT_BITMAP_8_BY_13;
			for (string::iterator i = s.begin(); i != s.end(); ++i) {
				char c = *i;
				glutBitmapCharacter(font, c);
			}
		}

		// Processar subgrupos
		drawScene(lGroups);

		glPopMatrix();
	}

}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0);
}

void renderScene(void) {
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
	
	endTime = glutGet(GLUT_ELAPSED_TIME);
	lapsedTime = endTime - baseTime;
	baseTime = endTime;

	drawScene(scene);

	//glutTimerFunc(1000 / FPS, timer, 0);

	// End of frame
	glutSwapBuffers();

}

// ###########################################
//	MAIN
int main(int argc, char **argv) {

	// Verificar argumentos
	if (argc == 1) {
		printf("Falta argumento do XML a carregar.\n");
		printf("Exemplo: Engine.exe scenetest.xml.\n");
		printf("A aplicacao vai sair.\n");
		exit(1);
	}

	// Registar ficheiro XML
	fileName = argv[1];
	printf("Filename: %s\n", fileName);

	// Registar tempo do glut
	timebase = glutGet(GLUT_ELAPSED_TIME);
	frame = 0;

	// Inicializar camera
	camInit();
	printf("CAMERA INIT OK\n");

	// Carregar ficheiro XML
	xmlError = xmlDoc.LoadFile(fileName);
	if (xmlError != XML_SUCCESS) {
		printf("Erro ao abrir o ficheiro %s\n", fileName);
		printf("ERROR: %i\n", xmlError);
		xmlDoc.PrintError();
		exit(0);
	}
	printf("File loaded: %s\n", fileName);


	// Fazer o RENDER do ficheiro
	xmlLoader();

	// ###############
	//	0 - Inicialização do GLUT e JANELAS
	// ###############
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	window = glutCreateWindow("TrabPratico_Fase3@CompGrafica");


	// ###############
	// 1 - Registar os CALLBACK das funções dos eventos
	// ###############
	// Função responsavel pelo conteudo da janela
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(1000 / FPS, timer, 0);
	//glutIdleFunc(renderScene);
	//glewInit();


	// ###############
	//	2 - Definições do KEYBOARD
	// ###############
	glutKeyboardFunc(keyboardEvents);
	glutSpecialFunc(keyboardSpecialEvents);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	//glutMouseWheelFunc(mouseWheel);


	// ###############
	//	3 - Definições do MENU
	// ###############
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

	// Associar CLICK ao menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Callback do GLEW - Tem de estar depois de todos os callbacks do GLUT
	//glewInit();
	initGL();


	// ###############
	// 4 - Definições do OPENGL
	// ###############
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	// DESATIVADO: Mostra triângulos de tras

	glutMainLoop();

	return 1;
}
