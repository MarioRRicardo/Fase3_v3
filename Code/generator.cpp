#include <XML/tinyxml2.h>
using namespace tinyxml2;

// TinyXML CHECK MACROS
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#include<stdio.h>
#include<stdlib.h>

#include "generator.h"
#include "xmlgen.h"


// ###########################################
// ####### VARIAVIES
// XML
char * object;
char * fileName;
XMLDocument xmlDoc;
XMLError xmlError;

float objParm1;	// PLANO: n/a | BOX: dim x | SPHERE: radius | CYLINDER: radious | CONE: radius
float objParm2;	// PLANO: n/a | BOX: dim y | SPHERE: slices | CYLINDER: height	| CONE: height
float objParm3;	// PLANO: n/a | BOX: dim z | SPHERE: stacks | CYLINDER: slices	| CONE: slices
float objParm4;	// PLANO: n/a | BOX: n/a   | SPHERE: n/a    | CYLINDER: n/a		| CONE: stacks



// ###########################################
// ####### FUNÇÕES
// Verificar arumentos da BOX
void tryPlaneXML(int argc, char * argv[]) {

	// Não contem parâmetros
	// Pode receber futuramente: COR

	// Escrever o cone
	xmlError = planeXML();
}

// Verificar arumentos da BOX
void tryBoxXML(int argc, char * argv[]) {

	if (argc < 6) {
		if (argc == 5)
			printf("Falta argumento DIM Z.\n");
		if (argc == 4)
			printf("Faltam os argumentos DIM Y e DIM Z.\n");
		if (argc == 3)
			printf("Faltam os argumentos DIM X, DIM Y e DIM Z.\n");

		printf("A aplicacao vai sair.\n");
		exit(0);
	}

	// Registar argumentos da BOX
	objParm1 = atof(argv[3]);
	objParm2 = atof(argv[4]);
	objParm3 = atof(argv[5]);

	// Escrever o cone
	xmlError = boxXML(objParm1, objParm2, objParm3);
}

// Verificar argumentos do SPHERE
void trySphereXML(int argc, char *argv[]) {
	if (argc < 6) {
		if (argc == 5)
			printf("Falta argumento CAMADAS.\n");
		if (argc == 4)
			printf("Faltam os argumentos FATIAS e CAMADAS.\n");
		if (argc == 3)
			printf("Faltam os argumentos RAIO, FATIAS e CAMADAS.\n");

		printf("A aplicacao vai sair.\n");
		exit(0);
	}

	// Registar argumentos do CONE
	objParm1 = atof(argv[3]);
	objParm2 = atof(argv[4]);
	objParm3 = atof(argv[5]);

	// Escrever o cone
	xmlError = sphereXML(objParm1, objParm2, objParm3);
}

// Verificar argumentos do Cilindro
void tryCylinderXML(int argc, char *argv[]) {

	if (argc < 6) {
		if (argc == 5)
			printf("Falta argumento ALTURA, FATIAS e CAMADAS.\n");
		if (argc == 4)
			printf("Faltam os argumentos ALTURA, FATIAS e CAMADAS.\n");
		if (argc == 3)
			printf("Faltam os argumentos RAIO, ALTURA, FATIAS.\n");

		printf("A aplicacao vai sair.\n");
		exit(0);
	}

	// Registar argumentos do CILINDRO
	objParm1 = atof(argv[3]);
	objParm2 = atof(argv[4]);
	objParm3 = atof(argv[5]);

	// Escrever o cilindro
	xmlError = cylinderXML(objParm1, objParm2, objParm3);
}

// Verificar argumentos do Cone
void tryConeXML(int argc, char *argv[]) {

	//if (argc < 6) {
	if (argc < 7) {
		if (argc == 6)
			printf("Faltam os argumentos CAMADAS.\n");
		if (argc == 5)
			printf("Falta argumento ALTURA, FATIAS e CAMADAS.\n");
		if (argc == 4)
			printf("Faltam os argumentos ALTURA, FATIAS e CAMADAS.\n");
		if (argc == 3)
			printf("Faltam os argumentos RAIO, ALTURA, FATIAS e CAMADAS.\n");

		printf("A aplicacao vai sair.\n");
		exit(0);
	}

	// Registar argumentos do CONE
	objParm1 = atof(argv[3]);
	objParm2 = atof(argv[4]);
	objParm3 = atof(argv[5]);
	objParm4 = atof(argv[6]);

	// Escrever o cone
	xmlError = coneXML(objParm1, objParm2, objParm3, objParm4);
	//xmlError = coneXML(objParm1, objParm2, objParm3);
}



// ###########################################
//	MAIN
int main(int argc, char **argv) {

	// Verificar argumentos passados
	if (argc == 1) {
		printf("Falta argumento do objeto a desenhar.\n");
		printf("Objetos disponíveis: Plane, Box, Sphere, Cylinder or Cone.\n");
		printf("Exemplo: Generator.exe Sphere Sphere.3d 5 16 16.\n");
		printf("A aplicacao vai sair.\n");
		exit(0);
	}
	if (argc == 2) {
		printf("Falta argumento do ficheiro de gravacao.\n");
		printf("A aplicacao vai sair.\n");
		exit(0);
	}

	// NOTA: ORDEM DOS ARGUMENTOS TROCADA
	object = argv[1];
	fileName = argv[2];
	printf("OBJECT: %s\n", object);
	printf("FILE: %s\n", fileName);


	if (strcmp(object, "Plane") == 0) {
		tryPlaneXML(argc, argv);
	}
	else if (strcmp(object, "Box") == 0) {
		tryBoxXML(argc, argv);
	}
	else if (strcmp(object, "Sphere") == 0) {
		trySphereXML(argc, argv);
	}
	else if (strcmp(object, "Cylinder") == 0) {
		tryCylinderXML(argc, argv);
	}
	else if (strcmp(object, "Cone") == 0) {
		tryConeXML(argc, argv);
	}
	else {
		printf("Objeto especificado invalido!\n");
		printf("Objetos disponíveis: Plane, Box, Sphere, Cone or Cylinder.\n");
		printf("Exemplo: Generator.exe Objecto ficheiro [parametro1] [parametro2] ... .\n");
		printf("A aplicacao vai sair.\n");
		exit(0);
	}

	if (xmlError != 0) {
		printf("SAVEFILE STATUS: %d", xmlError);
	}
	
	return 1;
}
