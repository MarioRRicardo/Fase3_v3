#include <XML/tinyxml2.h>
using namespace tinyxml2;

// TinyXML CHECK MACROS
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

#include "xmleng.h"
#include "engine.h"
using namespace std;


// ###########################################
// ####### VARIAVEIS
extern
list<Group*> scene;

// XML
extern char * object;
extern char * fileName;

// TinyXML
extern XMLDocument xmlDoc;
extern XMLError xmlError;



// ###########################################
// ####### FUNÇÕES


// PARSER XML para SCENE
void xmlLoader() {
	XMLElement* xmlScene = xmlDoc.FirstChildElement("scene");

	if (xmlScene != nullptr) {
		// Foçar CLEAN
		scene.clear();

		printf("SCENE FOUND: %s\n", fileName);
		// Procurar todos os GROUP dentro da SCENE
		for (XMLElement* group = xmlScene->FirstChildElement("group"); group != nullptr; group = group->NextSiblingElement("group")) {
			// Adicionar cada grupo à SCENE
			printf("GROUP FOUND\n"); 
			xmlGroupParser(group);
		}
	}
	else {
		printf("ERRO: Ficheiro XML pode estar mal construido,\n");
	}
}

// PARSER XML para GROUP
void xmlGroupParser(XMLElement *elem) {
	Group newGroup;
	XMLElement * child;

	bool hasTransl = false, hasRotat = false, hasScale = false, hasModels = false;
	const char *nodeName;

	// Verificar o 1º child: TRANSLATIONS | ROTATION | SCALES | CATMULL_CURVE | MODELS | SUBGROUP
	child = elem->FirstChildElement();

	if (child != nullptr) {
		// Preparar GROUP
		newGroup.initGroup();

		// Percorrer os seguintos nodos: models | subgroups
		for (; child != nullptr; child = child->NextSiblingElement()) {
			// Registar nome do FILHO
			nodeName = child->Name();
			printf("NODE FOUND: %s\n", nodeName);

			// Procurar 1 TRANSLAÇÃO apenas ==> hasTransl
			if (strcmp(nodeName, "translate") == 0 && hasTransl == false) {
				GeometricTransformation translation;
				float x, y, z = 0;

				// Verificar se tem TEMPO
				const char * timeStr = child->ToElement()->Attribute("time");

				// Verificar se é TIMED ==> CATUMLL
				if (timeStr != 0) {
					CatmullCurve points;
					float time;

					// Registar tempo
					time = atof(timeStr);

					// Registar PONTOS DA CURVA
					for (XMLElement* point = child->FirstChildElement(); point != 0; point = point->NextSiblingElement()) {
						Vertice v;

						x = atof(point->ToElement()->Attribute("X"));
						y = atof(point->ToElement()->Attribute("Y"));
						x = atof(point->ToElement()->Attribute("Z"));

						// Registar VERTICE
						v.setVertice(x, y, z);
						
						// Carregar VERTICE
						points.insertPoints(v);
					}

					translation.setCatmull(time, &points);

				}
				else {
					const char* ctx = child->Attribute("X");
					if (ctx != NULL) {
						x = atof(ctx);
					}
					const char* cty = child->Attribute("Y");
					if (cty != NULL) {
						y = atof(cty);
					}
					const char* ctz = child->Attribute("Z");
					if (ctz != NULL) {
						z = atof(ctz);
					}

					translation.setTranslation(x, y, z);
				}

				// Registar valores da translação
				newGroup.addTransformations(&translation);

				// Registar como TRANSLAÇÃO existente
				hasTransl = true;
			}

			// Procurar 1 TRANSLAÇÃO apenas ==> hasRotat
			if (strcmp(nodeName, "rotate") == 0 && hasRotat == false) {
				GeometricTransformation rotation; 
				float x, y, z;

				const char* crx = child->Attribute("X");
				if (crx != NULL) {
					x = atof(crx);
				}
				const char* cry = child->Attribute("Y");
				if (cry != NULL) {
					y = atof(cry);
				}
				const char* crz = child->Attribute("Z");
				if (crz != NULL) {
					z = atof(crz);
				}

				// Verificar se tem TEMPO
				const char * timeStr = child->ToElement()->Attribute("time");

				// Verificar se é TIMED ==> TIMED ROTATION
				if (timeStr != 0) {
					float time;

					time = atof(timeStr);

					// Criar rotação
					rotation.setTimedRotation(time, x, y, z);
				}
				else {
					float ang = 0;

					const char* cra = child->Attribute("Angle");
					if (cra != NULL) {
						ang = atof(cra);
					}

					// Criar rotação
					rotation.setRotation(ang, x, y, z);
				}

				// Registar valores da rotação
				newGroup.addTransformations(&rotation);

				// Registar como ROTAÇÃO existente
				hasRotat = true;
			}

			// Procurar 1 ESCALA apenas ==> hasScale
			if (strcmp(nodeName, "scale") == 0 && hasScale == false) {
				GeometricTransformation scale;
				float  x, y, z;

				const char * csx = child->Attribute("X");
				if (csx != NULL) {
					x = atof(csx);
				}
				const char * csy = child->Attribute("Y");
				if (csy != NULL) {
					y = atof(csy);
				}
				const char * csz = child->Attribute("Z");
				if (csz != NULL) {
					z = atof(csz);
				}

				// Criar escala
				scale.setScale(0, x, y, z);

				// Registar valores da rotação
				newGroup.addTransformations(&scale);

				// Registar como ESCALA existente
				hasScale = true;
			}

			// Verificar 1 conjunto de MODELOS
			if (strcmp(nodeName, "models") == 0 && hasModels == false) {

				// Registar como ESCALA existente
				hasModels = true;

				xmlModelParser(child, &newGroup);
			}

			// Verificar todos os SUBGRUPOS
			if (strcmp(nodeName, "group") == 0) {
				xmlGroupParser(child, &newGroup);
			}
		}

		// Adicionar à SCENE
		scene.push_back(&newGroup);
		printf("ADDED TO SCENE:%s | %d\n", nodeName, scene.size());
	}
}

// PARSER XML para GROUP
void xmlGroupParser(XMLElement *elem, Group * g) {
	Group newGroup;
	XMLElement * child;

	bool hasTransl = false, hasRotat = false, hasScale = false, hasModels = false;
	const char *nodeName;

	child = elem->FirstChildElement();

	if (child != nullptr) {
		// Preparar GROUP
		newGroup.initGroup();

		// Percorrer os seguintos nodos: MODELOS | SUBGRUPOS
		for (; child != nullptr; child = child->NextSiblingElement()) {
			// Registar nome do FILHO
			nodeName = child->Name();

			// Procurar 1 TRANSLAÇÃO apenas ==> hasTransl
			if (strcmp(nodeName, "translate") == 0 && hasTransl == false) {
				GeometricTransformation translation;
				float x, y, z = 0;

				// Verificar se tem TEMPO
				const char * timeStr = child->ToElement()->Attribute("time");

				// Verificar se é TIMED ==> CATUMLL
				if (timeStr != 0) {
					CatmullCurve points;
					float time;

					// Registar tempo
					time = atof(timeStr);

					// Registar PONTOS DA CURVA
					for (XMLElement* point = child->FirstChildElement(); point != 0; point = point->NextSiblingElement()) {
						Vertice v;

						x = atof(point->ToElement()->Attribute("X"));
						y = atof(point->ToElement()->Attribute("Y"));
						x = atof(point->ToElement()->Attribute("Z"));

						// Registar VERTICE
						v.setVertice(x, y, z);

						// Carregar VERTICE
						points.insertPoints(v);
					}

					translation.setCatmull(time, &points);

				}
				else {
					const char* ctx = child->Attribute("X");
					if (ctx != NULL) {
						x = atof(ctx);
					}
					const char* cty = child->Attribute("Y");
					if (cty != NULL) {
						y = atof(cty);
					}
					const char* ctz = child->Attribute("Z");
					if (ctz != NULL) {
						z = atof(ctz);
					}

					translation.setTranslation(x, y, z);
				}

				// Registar valores da translação
				newGroup.addTransformations(&translation);

				// Registar como TRANSLAÇÃO existente
				hasTransl = true;
			}

			// Procurar 1 TRANSLAÇÃO apenas ==> hasRotat
			if (strcmp(nodeName, "rotate") == 0 && hasRotat == false) {
				GeometricTransformation rotation;
				float x, y, z;

				const char* crx = child->Attribute("X");
				if (crx != NULL) {
					x = atof(crx);
				}
				const char* cry = child->Attribute("Y");
				if (cry != NULL) {
					y = atof(cry);
				}
				const char* crz = child->Attribute("Z");
				if (crz != NULL) {
					z = atof(crz);
				}

				// Verificar se tem TEMPO
				const char * timeStr = child->ToElement()->Attribute("time");

				// Verificar se é TIMED ==> TIMED ROTATION
				if (timeStr != 0) {
					float time;

					time = atof(timeStr);

					// Criar rotação
					rotation.setTimedRotation(time, x, y, z);
				}
				else {
					float ang = 0;

					const char* cra = child->Attribute("Angle");
					if (cra != NULL) {
						ang = atof(cra);
					}

					// Criar rotação
					rotation.setRotation(ang, x, y, z);
				}

				// Registar valores da rotação
				newGroup.addTransformations(&rotation);

				// Registar como ROTAÇÃO existente
				hasRotat = true;
			}

			// Procurar 1 ESCALA apenas ==> hasScale
			if (strcmp(nodeName, "scale") == 0 && hasScale == false) {
				GeometricTransformation scale;
				float  x, y, z;

				const char * csx = child->Attribute("X");
				if (csx != NULL) {
					x = atof(csx);
				}
				const char * csy = child->Attribute("Y");
				if (csy != NULL) {
					y = atof(csy);
				}
				const char * csz = child->Attribute("Z");
				if (csz != NULL) {
					z = atof(csz);
				}

				// Criar escala
				scale.setScale(0, x, y, z);

				// Registar valores da rotação
				newGroup.addTransformations(&scale);

				// Registar como ESCALA existente
				hasScale = true;
			}

			// Verificar 1 conjunto de MODELOS
			if (strcmp(nodeName, "models") == 0 && hasModels == false) {

				// Registar como ESCALA existente
				hasModels = true;

				xmlModelParser(child, &newGroup);
			}

			// Verificar todos os SUBGRUPOS
			if (strcmp(nodeName, "group") == 0) {
				xmlGroupParser(child, &newGroup);
			}
		}

		// Adicionar à SCENE
		g->insertSubGroup(&newGroup);
	}
}

// PARSER XML para MODELS
void xmlModelParser(XMLElement *models, Group *g) {
	const char * modelName;
	XMLElement * model;

	for (model = models->FirstChildElement(); model != nullptr; model = model->NextSiblingElement()) {

		// Registar nome do MODEL
		modelName = model->Attribute("file");

		// Abrir ficheiro 3D
		XMLDocument xml3DFile;
		XMLError error = xml3DFile.LoadFile(modelName);

		// Verificar ERROR de abertura
		if (error != XML_SUCCESS) {
			printf("Erro ao abrir o ficheiro %s\n", modelName);
			exit(0);
		}

		// Preparar MODEL
		Model m;
		m.initModel();

		// Registar NOME
		XMLElement *child = xml3DFile.FirstChildElement("object")->FirstChildElement("name");
		std::string s(child->GetText());
		m.setName(s);

		// Registar TRAINGULOS
		for (child = child->NextSiblingElement("triangle"); child != nullptr; child = child->NextSiblingElement("triangle")) {
			// Registar cada vertice do TRIANGLE
			float x, y, z; 
			Vertice point;
			XMLElement *vertice;

			vertice = child->FirstChildElement("vertice");
			x = atof(vertice->Attribute("x"));
			y = atof(vertice->Attribute("y"));
			z = atof(vertice->Attribute("z"));
			point.setVertice( x, y, z);
			m.insertPoint(point);

			vertice = vertice->NextSiblingElement();
			x = atof(vertice->Attribute("x"));
			y = atof(vertice->Attribute("y"));
			z = atof(vertice->Attribute("z"));
			point.setVertice(x, y, z);
			m.insertPoint(point);

			vertice = vertice->NextSiblingElement();
			x = atof(vertice->Attribute("x"));
			y = atof(vertice->Attribute("y"));
			z = atof(vertice->Attribute("z"));
			point.setVertice(x, y, z);
			m.insertPoint(point);
		}

		// Adicionar ao GRUPO atual
		g->insertModel(&m);
	}
}
