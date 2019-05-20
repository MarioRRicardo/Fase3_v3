#include <XML/tinyxml2.h>
using namespace tinyxml2;


void addPoint(XMLElement *elem, float a, float b, float c);
XMLError planeXML();
XMLError boxXML(float dimX, float dimY, float dimZ);
XMLError sphereXML(float rad, float slices, float layers);
XMLError cylinderXML(float radious, float height, float slices);
//XMLError coneXML(float radious, float height, float slices);
XMLError coneXML(float radious, float height, float slices, float stacks);