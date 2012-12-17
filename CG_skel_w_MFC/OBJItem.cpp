#include "stdafx.h"
#include "OBJItem.h"
#include "CubeModel.h"

OBJItem::OBJItem(void):vertices(),faces(),normals(),minX(5),minY(5),minZ(5)
	,maxX(0),maxY(0),maxZ(0), drawBox(false), drawVertexNormal(false), drawNormal(false),_color(WHITE),calcNormals(true)
{
	verticesTree = new AvlTree<Vertex>();
}

OBJItem::~OBJItem(void)
{
}
OBJItem::OBJItem(const OBJItem& item):vertices(item.vertices),faces(item.faces),normals(item.normals)
	,minX(item.minX),minY(item.minY),minZ(item.minZ),maxX(item.maxX),maxY(item.maxY),maxZ(item.maxZ),
	drawBox(false), drawVertexNormal(false), drawNormal(false),_color(item._color),verticesTree(item.verticesTree),calcNormals(true)
{
}
void OBJItem::addVertex(vec3& vertex)
{
	if(vertex[0] < minX)
		minX= vertex[0];
	if(vertex[1] < minY)
		minY= vertex[1];
	if(vertex[2] < minZ)
		minZ= vertex[2];
	if(vertex[0] > maxX)
		maxX= vertex[0];
	if(vertex[1] > maxY)
		maxY = vertex[1];
	if(vertex[2] > maxZ)
		maxZ= vertex[2];
	
	vertices.push_back(vertex);
	Vertex v(vertices.size());
	verticesTree->insert(v);
}
void OBJItem::addFace(Face& face)
{
	faces.push_back(face);
	vec3 vertices = face.getVertices();
	int v1 = vertices.x;
	int v2 = vertices.y;
	int v3 = vertices.z;
	Vertex ver1(v1);
	Vertex ver2(v2);
	Vertex ver3(v3);
	int currentFace = faces.size();
	verticesTree->find(&ver1)->addFace(currentFace);
	verticesTree->find(&ver2)->addFace(currentFace);
	verticesTree->find(&ver3)->addFace(currentFace);
}
void OBJItem::addNormal(vec3& normal)
{
	normals.push_back(normal);
}
vec3& OBJItem::getVertexByNumber(int id)
{
	return vertices.at(id); // will throw out of bound
}
vec3& OBJItem::getNormalByNumber(int id)
{
	return normals.at(id); // will throw out of bound
}

void OBJItem::draw(Renderer& renderer)
{
	//draw bounding box
	if(drawBox) {
		CubeModel stupidCube = CubeModel(minX,maxX,minY,maxY,minZ,maxZ);
		stupidCube.setObjectTransform(renderer.getObjectMatrices());
		stupidCube.draw(renderer);
	}

			
	for(std::vector<Face>::iterator it = faces.begin() ;it != faces.end(); ++it) 
	{
		try
		{
			Face curentface(*it);
			vec3 currentVerticies = curentface.getVertices();
			if(calcNormals)
			{
				curentface.setVN1(getCalculatedNormal(currentVerticies.x));
				curentface.setVN2(getCalculatedNormal(currentVerticies.y));
				curentface.setVN3(getCalculatedNormal(currentVerticies.z));
			}
			//draw triangle
			if(renderer.DrawTriangle(curentface,_color) && false)
			{
				renderer.drawLineByVectors((*it).getVecX() ,(*it).getVecY() ,(unsigned int) RED);
				renderer.drawLineByVectors((*it).getVecY() ,(*it).getVecZ() ,(unsigned int) RED);
				renderer.drawLineByVectors((*it).getVecZ() ,(*it).getVecX() ,(unsigned int) RED);
			}
			//draw normal
			if(drawNormal)
				renderer.drawLineByVectors((*it).getNormalLine()[0] ,(*it).getNormalLine()[1] ,(unsigned int)BLUE);
			if(drawVertexNormal)
			{
				renderer.drawLineByVectors(curentface.getVecX(),curentface.getVecX()+curentface.getVnX(),(unsigned int)GREEN);
				renderer.drawLineByVectors(curentface.getVecY(),curentface.getVecY()+curentface.getVnY(),(unsigned int)GREEN);
				renderer.drawLineByVectors(curentface.getVecX(),curentface.getVecZ()+curentface.getVnZ(),(unsigned int)GREEN);
			}
		}
		catch(exception& e)
		{
			e.what();
		}
		
	}
}
vec3 OBJItem::getMidPoint()
{
	return vec3((maxX+minX)/2,(maxY+minY)/2,(maxZ+minZ)/2);
}
GLfloat OBJItem::getMinX()
{
	return minX;
}
GLfloat OBJItem::getMinY()
{
	return minY;
}
GLfloat OBJItem::getMinZ()
{
	return minZ;
}
GLfloat OBJItem::getWidth()
{
	return maxX - minX;
}
GLfloat OBJItem::getHeight()
{
	return maxY- minY;
}
vec3 OBJItem::getCalculatedNormal(int id)
{
	Vertex v (id);
	GLfloat totalArea=0.0;
	vec3 out(0,0,0);
	vector<int>& vfaces = verticesTree->find(&v)->getFaces();
	for(vector<int>::iterator it = vfaces.begin() ; it != vfaces.end();++it)
	{
		totalArea +=  faces[*it-1].getFaceArea();
	}
	for(vector<int>::iterator it = vfaces.begin() ; it != vfaces.end();++it)
	{
		mat3 normal= faces[*it-1].getNormalLine();
		vec3 normalVecor= normal[1]-normal[0];
		out += normalVecor * faces[*it-1].getFaceArea()/totalArea;
	}
	return out;
}