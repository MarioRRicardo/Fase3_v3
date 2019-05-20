#pragma once
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GLUT/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <list>
#include <vector>
#include <string>
using namespace std;

#define TRANSLATION 1
#define ROTATION 2
#define SCALE 3
#define CATMULL_CURVE 4

#define CATMULL_CURVE_STEPS 100

// ###########################################
// ####### ESTRUTURAS DE DADOS

class Vertice {
	float x;
	float y;
	float z;

public:
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	void setVertice(float vx, float vy, float vz) { x = vx; y = vy; z = vz; }
};

class CatmullCurve {
	float perimeter = 0;
	float nextPoint = 0;
	vector< Vertice > catmullPoints;
	GLuint vertexCounter;
	GLuint vboBuffer[3];

	public:
		float getPerimeter() { return nextPoint; }
		float getNextPoint() { return perimeter; }
		vector< Vertice > getCatmullPoints() { return catmullPoints; }

		void setPerimeter(float perm) { perimeter = perm; }
		void setNextPoint(float point) { nextPoint = point; }
		void insertPoints(Vertice point) { catmullPoints.push_back( point ); }

};

class GeometricTransformation {
	int id;
	float tx, ty, tz;
	float time;
	float alpha;
	CatmullCurve * catmullCurve;

	public:
		void setTranslation(float x, float y, float z) { id = TRANSLATION; tx = x, ty = y, tz = z; time = 0; }
		void setRotation(float ang, float x, float y, float z) { id = ROTATION; alpha = ang, tx = x, ty = y, tz = z; time = 0; }
		void setTimedRotation(float tim, float x, float y, float z) { id = ROTATION; time = tim, tx = x, ty = y, tz = z; alpha = 0; }
		void setScale(float ang, float x, float y, float z) { id = SCALE; tx = x, ty = y, tz = z; }
		void setCatmull(float tim, CatmullCurve *points) { id = CATMULL_CURVE; time = tim, catmullCurve = points; }
		void setAlpha(float ang) { alpha = ang; }

		int getType() { return id; }
		float getX() { return tx; }
		float getY() { return ty; }
		float getZ() { return tz; }
		float getTime() { return time; }
		float getAlpha() { return alpha; }
		CatmullCurve * getGeomTransCatmull() { return catmullCurve; }
};

class Model {
		string name;
		list< Vertice > points;
		//GLuint vboBuffer[3];

	public:
		GLuint vertexCounter;
		GLuint vboBuffer[3];

		void initModel() { name = ""; points.clear(); }
		void setName(string s) { name = s; }
		string getName() { return name; }
		void insertPoint(Vertice v) { points.push_back(v); }
		list< Vertice > getPoints() { return points; }
		void setVertexCounter(GLuint count) { vertexCounter = count; }
		GLuint getVertexCounter() { return vertexCounter; }
		//void setVertexCounter(GLuint *buffer) { vboBuffer = buffer; }
		//GLuint * getVertexBuffer() { return &vboBuffer; }
};

class Group {
	list<GeometricTransformation *> transformations;
	list<Model *> models;
	list<Group *> subgroups;

public:
	void initGroup() { transformations.clear(); models.clear(); subgroups.clear(); }

	void addTransformations(GeometricTransformation *t) { transformations.push_back(t); }
	list<GeometricTransformation *> getTransformations() { return transformations; }
	void insertModel(Model *m) { models.push_back(m); }
	list<Model *> getModels() { return models; }
	void insertSubGroup(Group *sg) { subgroups.push_back(sg); }
	list<Group *> getSubGroups() { return subgroups; }
};

// ###########################################
// ####### FUNÇÕES
