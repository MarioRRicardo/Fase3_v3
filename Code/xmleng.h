#include <XML/tinyxml2.h>
using namespace tinyxml2;

#include "objects.h"

// ###########################################
// ####### ESTRUTURAS DE DADOS


// ###########################################
// ####### FUNÇÕES
void xmlLoader();
void xmlGroupParser(XMLElement *elem);
void xmlGroupParser(XMLElement *elem, Group * g);
void xmlModelParser(XMLElement *models, Group *g);