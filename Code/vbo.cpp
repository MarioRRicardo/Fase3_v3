#include "catmull-rom.h"
#include "vbo.h"


#define TRANSLATION 1
#define ROTATION 2
#define SCALE 3
#define CATMULL_CURVE 4

#define CATMULL_CURVE_STEPS 100
#define CATMULL_STEPS = 100;

// ###########################################
// VARIAVEIS

// ###########################################
// ####### FUNÇÕES

void generateVBO(Group* g) {

	// Gerar VBO para CATMULL, caso exista
	for each (GeometricTransformation * t in g->getTransformations()) {
		if (t->getType() == CATMULL_CURVE) {
			generateCatmullCurveVBO(t);
		}
	}

	// Gerar VBO para cada modelo dentro do grupo
	for each (Model * m in g->getModels()) {
		generateModelVBO(m);
	}

	// Gerar VBO para cada subgrupo
	if (g->getSubGroups().size() > 0) {
		for each (Group * sg in g->getSubGroups()) {
			generateVBO(g);
		}
	}
}


void generateCatmullCurveVBO(GeometricTransformation* t) {
	float perimeter = 0;
	float point[3];
	float previusPoint[3];
	float d[3];

	CatmullCurve * catmullcurve = t->getGeomTransCatmull();

	int i = 0;

	getGlobalCatmullRomPoint(i / CATMULL_CURVE_STEPS, previusPoint, d, catmullcurve);

	for (i = 1; i < CATMULL_CURVE_STEPS; i++) {
		getGlobalCatmullRomPoint(i / CATMULL_CURVE_STEPS, point, d, catmullcurve);

		//Ter cuidado para ver se é para subtrair o anterior com o atual
		if (previusPoint[0] < point[0]) {
			perimeter += distance2points(previusPoint, point);
		}
		else {
			if (previusPoint[0] == point[0] && (previusPoint[1] <= point[1] || previusPoint[2] <= point[2])) {
				perimeter += distance2points(previusPoint, point);
			}
			else {
				perimeter += distance2points(point, previusPoint);
			}
			previusPoint[0] = point[0];
			previusPoint[1] = point[1];
			previusPoint[2] = point[2];
		}

		// Registar
		catmullcurve->setPerimeter(perimeter);
	}
}

void generateModelVBO(Model* m) {
	float vertCount;

	// Contar vertices
	vertCount = m->getPoints().size();

	// Gerar VBO com pontos da lista
	glGenBuffers(3, m->vboBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m->vboBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->getPoints().size(), &(m->getPoints()), GL_STATIC_DRAW);
}