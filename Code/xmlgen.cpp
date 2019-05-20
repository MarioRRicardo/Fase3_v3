#include "xmlgen.h"
//#include <XML/tinyxml2.h>

#define _USE_MATH_DEFINES
#include <math.h>

// TinyXML CHECK MACROS
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#define SIZE 1


// ###########################################
// ####### VARIAVEIS

// XML
extern char * object;
extern char * fileName;

// TinyXML
extern XMLDocument xmlDoc;



// ###########################################
// ####### FUNÇÕES

void addPoint(XMLElement *elem, float a, float b, float c) {
	XMLElement * point = xmlDoc.NewElement("vertice");
	point->SetAttribute("x", a);
	point->SetAttribute("y", b);
	point->SetAttribute("z", c);

	elem->InsertEndChild(point);
}

// PLANO
XMLError planeXML() {
	// Iniciar Objecto XML
	XMLNode * obj = xmlDoc.NewElement("object");
	xmlDoc.InsertFirstChild(obj);

	// Indicar o LABEL
	XMLElement * lab = xmlDoc.NewElement("label");
	lab->SetText(object);
	obj->InsertEndChild(lab);

	// Indicar o NOME
	XMLElement * name = xmlDoc.NewElement("name");
	name->SetText(fileName);
	obj->InsertEndChild(name);
		
	// Indicar COORDENADAS 1
	XMLElement * tri1 = xmlDoc.NewElement("triangle");
	tri1->SetAttribute("pntCount ", 3);
	addPoint(tri1, SIZE, 0, -SIZE);
	addPoint(tri1, -SIZE, 0, -SIZE);
	addPoint(tri1, -SIZE, 0, SIZE);
	obj->InsertEndChild(tri1);

	// Indicar COORDENADAS 2
	XMLElement * tri2 = xmlDoc.NewElement("triangle");
	tri2->SetAttribute("pntCount", 3);
	addPoint(tri2, -SIZE, 0, SIZE);
	addPoint(tri2, SIZE, 0, SIZE);
	addPoint(tri2, SIZE, 0, -SIZE);
	obj->InsertEndChild(tri2);
	
	// Gravar em ficheiro
	XMLError eResult = xmlDoc.SaveFile(fileName);
	XMLCheckResult(eResult);
}


XMLError boxXML(float dimX, float dimY, float dimZ) {
	float x, y, z;

	// Definir FRENTE/TRAS
	int side = 1;

	// Iniciar Objecto XML
	XMLNode * obj = xmlDoc.NewElement("object");
	xmlDoc.InsertFirstChild(obj);

	// Indicar o LABEL
	XMLElement * lab = xmlDoc.NewElement("label");
	lab->SetText(object);
	obj->InsertEndChild(lab);

	// Indicar o NOME
	XMLElement * name = xmlDoc.NewElement("name");
	name->SetText(fileName);
	obj->InsertEndChild(name);
	
	// Criar completa lados paralelos
	for (int i = 1; i <= 2; i++) {
		x = dimX * side;
		y = dimY;
		z = dimZ * side;

		//PLANO XY
		// #### Triangulo 1 ####
		XMLElement * coordXY1 = xmlDoc.NewElement("triangle");
		coordXY1->SetAttribute("pntCount", 3);
		addPoint(coordXY1, -x / 2, -y / 2, z / 2);
		addPoint(coordXY1, x / 2, -y / 2, z / 2);
		addPoint(coordXY1, -x / 2, y / 2, z / 2);
		obj->InsertEndChild(coordXY1);

		// #### Triangulo 2 ####
		XMLElement * coordXY2 = xmlDoc.NewElement("triangle");
		coordXY2->SetAttribute("pntCount", 3);
		addPoint(coordXY2, -x / 2, y / 2, z / 2);
		addPoint(coordXY2, x / 2, -y / 2, z / 2);
		addPoint(coordXY2, x / 2, y / 2, z / 2);
		obj->InsertEndChild(coordXY2);


		//PLANO YZ	
		// #### Triangulo 1 ####
		XMLElement * coordYZ1 = xmlDoc.NewElement("triangle");
		coordYZ1->SetAttribute("pntCount", 3);
		addPoint(coordYZ1, x / 2, -y / 2, z / 2);
		addPoint(coordYZ1, x / 2, -y / 2, -z / 2);
		addPoint(coordYZ1, x / 2, y / 2, z / 2);
		obj->InsertEndChild(coordYZ1);

		// #### Triangulo 2 ####
		XMLElement * coordYZ2 = xmlDoc.NewElement("triangle");
		coordYZ2->SetAttribute("pntCount", 3);
		addPoint(coordYZ2, x / 2, y / 2, z / 2);
		addPoint(coordYZ2, x / 2, -y / 2, -z / 2);
		addPoint(coordYZ2, x / 2, y / 2, -z / 2);
		obj->InsertEndChild(coordYZ2);

		//PLANO XZ	
		x = dimX * side;
		y = dimY * side;
		z = dimZ * side;

		// #### Triangulo 1 ####
		XMLElement * coordXZ1 = xmlDoc.NewElement("triangle");
		coordXZ1->SetAttribute("pntCount", 3);
		addPoint(coordXZ1, -x / 2, y / 2, z / 2);
		addPoint(coordXZ1, x / 2, y / 2, z / 2);
		addPoint(coordXZ1, -x / 2, y / 2, -z / 2);
		obj->InsertEndChild(coordXZ1);

		// #### Triangulo 2 ####
		XMLElement * coordXZ2 = xmlDoc.NewElement("triangle");
		coordXZ2->SetAttribute("pntCount", 3);
		addPoint(coordXZ2, -x / 2, y / 2, -z / 2);
		addPoint(coordXZ2, x / 2, y / 2, z / 2);
		addPoint(coordXZ2, x / 2, y / 2, -z / 2);
		obj->InsertEndChild(coordXZ2);

		// Troca para o lado paralelo oposto
		side = -1;
	}

	// Gravar em ficheiro
	XMLError eResult = xmlDoc.SaveFile(fileName);
	XMLCheckResult(eResult);
}


XMLError sphereXML(float rad, float slices, float layers) {
	// Iniciar Objecto XML
	XMLNode * obj = xmlDoc.NewElement("object");
	xmlDoc.InsertFirstChild(obj);

	// Indicar o LABEL
	XMLElement * lab = xmlDoc.NewElement("label");
	lab->SetText(object);
	obj->InsertEndChild(lab);

	// Indicar o NOME
	XMLElement * name = xmlDoc.NewElement("name");
	name->SetText(fileName);
	obj->InsertEndChild(name);


	// Angulos
	float alphaAmp = 2 * M_PI / slices;
	float alpha1 = 0, alpha2 = 0;			// Horizontal
	float betaAmp = M_PI / layers;
	float beta1 = -(M_PI / 2), beta2 = 0;	// Vertical

	// Pontos
	float pt1X;	float pt1Y;	float pt1Z;
	float pt2X;	float pt2Y;	float pt2Z;
	float pt3X;	float pt3Y;	float pt3Z;


	// Começar desenho por baixo
	beta1 = -(M_PI / 2);

	for (int i = 0; i < layers; i++) {
		beta2 = beta1 + betaAmp;

		alpha1 = 0;
		for (int j = 0; j < slices; j++) {

			alpha2 = alpha1 + alphaAmp;

			// #### Triangulo 1 ####
			XMLElement * tri1 = xmlDoc.NewElement("triangle");
			tri1->SetAttribute("pntCount", 3);

			// Calcular coordenadas do ponto 1
			pt1X = rad * cos(beta1) * sin(alpha1);
			pt1Y = rad * sin(beta1);
			pt1Z = rad * cos(beta1) * cos(alpha1);

			// Calcular coordenadas do ponto 2
			pt2X = rad * cos(beta1) * sin(alpha2);
			pt2Y = rad * sin(beta1);
			pt2Z = rad * cos(beta1) * cos(alpha2);

			// Calcular cordenadas do ponto 3
			pt3X = rad * cos(beta2) * sin(alpha1);
			pt3Y = rad * sin(beta2);
			pt3Z = rad * cos(beta2) * cos(alpha1);

			// Desenhar pontos do triângulo 1
			addPoint(tri1, pt1X, pt1Y, pt1Z);
			addPoint(tri1, pt2X, pt2Y, pt2Z);
			addPoint(tri1, pt3X, pt3Y, pt3Z);
			obj->InsertEndChild(tri1);

			// #### Triangulo 2 ####
			XMLElement * tri2 = xmlDoc.NewElement("triangle");
			tri2->SetAttribute("pntCount", 3);

			// Recalcular coordenadas do ponto 1
			pt1X = pt3X;
			pt1Y = pt3Y;
			pt1Z = pt3Z;

			// Recalcular coordenadas do ponto 2    | PONTO 2 do triângulo anteior é o PONTO 2 deste novo triângulo

			// Calcular coordenadas do ponto 3
			pt3X = rad * cos(beta2) * sin(alpha2);
			pt3Y = rad * sin(beta2);
			pt3Z = rad * cos(beta2) * cos(alpha2);

			// Desenhar pontos do triângulo 2
			addPoint(tri2, pt1X, pt1Y, pt1Z);
			addPoint(tri2, pt2X, pt2Y, pt2Z);
			addPoint(tri2, pt3X, pt3Y, pt3Z);
			obj->InsertEndChild(tri2);

			// Próximo SLICE
			alpha1 = alpha2;
		}
		// Próximo CAMADA
		beta1 = beta2;
	}
	
	// Gravar em ficheiro
	XMLError eResult = xmlDoc.SaveFile(fileName);
	XMLCheckResult(eResult);
}


XMLError cylinderXML(float radious, float height, float slices) {
	// Iniciar Objecto XML
	XMLNode * obj = xmlDoc.NewElement("object");
	xmlDoc.InsertFirstChild(obj);

	// Indicar o LABEL
	XMLElement * lab = xmlDoc.NewElement("label");
	lab->SetText(object);
	obj->InsertEndChild(lab);

	// Indicar o NOME
	XMLElement * name = xmlDoc.NewElement("name");
	name->SetText(fileName);
	obj->InsertEndChild(name);


	// Calcular amplitute do angulo
	float alphaAmp = 2 * M_PI / slices;
	float alphaStep;
	int slice;
	double pt2_x, pt2_z, pt3_x, pt3_z;

	for (slice = 0; slice < slices; slice++) {
		alphaStep = alphaAmp * slice;
		pt2_x = sin(alphaStep)*radious;
		pt2_z = cos(alphaStep)*radious;
		alphaStep = alphaAmp * (slice + 1);
		pt3_x = sin(alphaStep)*radious;
		pt3_z = cos(alphaStep)*radious;

		// Topo
		XMLElement * tri1 = xmlDoc.NewElement("triangle");
		tri1->SetAttribute("pntCount", 3);
		addPoint(tri1, 0, height, 0);
		addPoint(tri1, pt2_x, height, pt2_z);
		addPoint(tri1, pt3_x, height, pt3_z);
		obj->InsertEndChild(tri1);

		// Lateral 1
		XMLElement * tri2 = xmlDoc.NewElement("triangle");
		tri2->SetAttribute("pntCount", 3);
		addPoint(tri2, pt3_x, height, pt3_z);
		addPoint(tri2, pt2_x, height, pt2_z);
		addPoint(tri2, pt2_x, 0, pt2_z);
		obj->InsertEndChild(tri2);

		// Lateral 2
		XMLElement * tri3 = xmlDoc.NewElement("triangle");
		tri3->SetAttribute("pntCount", 3);
		addPoint(tri3, pt2_x, 0, pt2_z);
		addPoint(tri3, pt3_x, 0, pt3_z);
		addPoint(tri3, pt3_x, height, pt3_z);
		obj->InsertEndChild(tri3);

		// Fundo
		XMLElement * tri4 = xmlDoc.NewElement("triangle");
		tri4->SetAttribute("pntCount", 3);
		addPoint(tri4, 0, 0, 0);
		addPoint(tri4, pt3_x, 0, pt3_z);
		addPoint(tri4, pt2_x, 0, pt2_z);
		obj->InsertEndChild(tri4);
	}

	XMLError eResult = xmlDoc.SaveFile(fileName);
	XMLCheckResult(eResult);
}


XMLError coneXML(float radious, float height, float slices, float stacks) {
	// Iniciar Objecto XML
	XMLNode * obj = xmlDoc.NewElement("object");
	xmlDoc.InsertFirstChild(obj);

	// Indicar o LABEL
	XMLElement * lab = xmlDoc.NewElement("label");
	lab->SetText(object);
	obj->InsertEndChild(lab);

	// Indicar o NOME
	XMLElement * name = xmlDoc.NewElement("name");
	name->SetText(fileName);
	obj->InsertEndChild(name);


	// Calcular amplitute do angulo
	double alphaStep = 2 * M_PI / slices;
	float radStep;
	int slice;
	int stack = 0;
	double pt1_x, pt1_y, pt1_z, pt2_x, pt2_y, pt2_z, pt3_x, pt3_y, pt3_z;


	// Desenhar cada fatia
	for (slice = 0; slice < slices; slice++) {
		pt1_x = 0;
		pt1_y = 0;
		pt1_z = 0;

		pt2_x = sin(alphaStep * (slice + 1)) * radious * ((stacks - stack) / (float)stacks);
		pt2_y = height * (stack / (float)stacks);
		pt2_z = cos(alphaStep * (slice + 1)) * radious * ((stacks - stack) / (float)stacks);

		pt3_x = sin(alphaStep * slice) * radious * ((stacks - stack) / (float)stacks);
		pt3_y = pt2_y;
		pt3_z = cos(alphaStep * slice) * radious * ((stacks - stack) / (float)stacks);

		// Fundo
		XMLElement * tri0 = xmlDoc.NewElement("triangle");
		tri0->SetAttribute("pntCount", 3);
		addPoint(tri0, pt1_x, pt1_y, pt1_z);
		addPoint(tri0, pt2_x, pt2_y, pt2_z);
		addPoint(tri0, pt3_x, pt3_y, pt3_z);
		obj->InsertEndChild(tri0);

		// Desenhaar cada camada
		for (stack = 0; stack < stacks - 1; stack++) {

			// Triangulo 1
			pt1_x = pt3_x; pt1_y = pt3_y; pt1_z = pt3_z;
			pt3_x = sin(alphaStep * (slice + 1)) * radious * ((stacks - (stack + 1)) / (float)stacks);
			pt3_y = height * ((stack + 1) / (float)stacks);
			pt3_z = cos(alphaStep * (slice + 1)) * radious * ((stacks - (stack + 1)) / (float)stacks);

			XMLElement * tri1 = xmlDoc.NewElement("triangle");
			tri1->SetAttribute("pntCount", 3);
			addPoint(tri1, pt1_x, pt1_y, pt1_z);
			addPoint(tri1, pt2_x, pt2_y, pt2_z);
			addPoint(tri1, pt3_x, pt3_y, pt3_z);
			obj->InsertEndChild(tri1);

			// Triangulo 2
			pt2_x = pt3_x; pt2_y = pt3_y; pt2_z = pt3_z;
			pt3_x = sin(alphaStep * slice) * radious * ((stacks - (stack + 1)) / (float)stacks);
			pt3_y = height * ((stack + 1) / (float)stacks);
			pt3_z = cos(alphaStep * slice) * radious * ((stacks - (stack + 1)) / (float)stacks);

			XMLElement * tri2 = xmlDoc.NewElement("triangle");
			tri2->SetAttribute("pntCount", 3);
			addPoint(tri2, pt1_x, pt1_y, pt1_z);
			addPoint(tri2, pt2_x, pt2_y, pt2_z);
			addPoint(tri2, pt3_x, pt3_y, pt3_z);
			obj->InsertEndChild(tri2);
		}

		stack = 0;

		// Ultima camada
		pt1_x = pt3_x; pt1_y = pt3_y; pt1_z = pt3_z;
		pt3_x = 0;
		pt3_y = height;
		pt3_z = 0;

		XMLElement * tri3 = xmlDoc.NewElement("triangle");
		tri3->SetAttribute("pntCount", 3);
		addPoint(tri3, pt1_x, pt1_y, pt1_z);
		addPoint(tri3, pt2_x, pt2_y, pt2_z);
		addPoint(tri3, pt3_x, pt3_y, pt3_z);
		obj->InsertEndChild(tri3);
	}

	XMLError eResult = xmlDoc.SaveFile(fileName);
	XMLCheckResult(eResult);
}