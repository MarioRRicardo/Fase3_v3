#include "objects.h"

// ###########################################
// ####### FUNÇÕES

void buildRotMatrix(float *x, float *y, float *z, float *m);
void cross(float *a, float *b, float *res);
void normalize(float *a);
float length(float *v);
void multMatrixVector(float *m, float *v, float *res);
float distance2points(float a[3], float b[3]);

void getCatmullRomPoint(float t, int* indices, float* pos, float* deriv, CatmullCurve * catmullPoints);
void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv, CatmullCurve * catmullPoints);
void renderCatmullRomCurve(CatmullCurve * catmullPoints);