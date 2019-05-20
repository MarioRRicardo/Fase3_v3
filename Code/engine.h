#include <list>
#include "objects.h"

#define FPS 60

// ###########################################
// ####### ESTRUTURAS DE DADOS


// ###########################################
// ####### FUNÇÕES
void printInfo();
void initGL();
void timer(int);
void renderScene(void);
void drawScene(list<Group*> group);
