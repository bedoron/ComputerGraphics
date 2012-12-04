#include "stdafx.h"
#include "OBJItem.h"
#include "CubeModel.h"

OBJItem::OBJItem(void):vertices(),faces(),normals(),minX(5),minY(5),minZ(5)
	,maxX(0),maxY(0),maxZ(0), drawBox(false), drawVertexNormal(false), drawNormal(false)
{
}

OBJItem::~OBJItem(void)
{
}
OBJItem::OBJItem(const OBJItem& item):vertices(item.vertices),faces(item.faces),normals(item.normals)
	,minX(item.minX),minY(item.minY),minZ(item.minZ),maxX(item.maxX),maxY(item.maxY),maxZ(item.maxZ),
	drawBox(false), drawVertexNormal(false), drawNormal(false)
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
}
void OBJItem::addFace(Face& face)
{
	faces.push_back(face);
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
	if(drawBox)
		CubeModel(minX,maxX,minY,maxY,minZ,maxZ).draw(renderer);
			
	for(std::vector<Face>::iterator it = faces.begin() ;it != faces.end(); ++it) 
	{
		try
		{
			Face curentface(*it);
			//draw triangle
			renderer.drawLineByVectors((*it).getVecX() ,(*it).getVecY() ,(unsigned int) RED);
			renderer.drawLineByVectors((*it).getVecY() ,(*it).getVecZ() ,(unsigned int) RED);
			renderer.drawLineByVectors((*it).getVecZ() ,(*it).getVecX() ,(unsigned int) RED);
			//draw normal
			if(drawNormal)
				renderer.drawLineByVectors((*it).getNormalLine()[0] ,(*it).getNormalLine()[1] ,(unsigned int)BLUE);
			if(drawVertexNormal)
			{
				renderer.drawLineByVectors((*it).getVecX(),(*it).getVecX()+(*it).getVnX(),(unsigned int)GREEN);
				renderer.drawLineByVectors((*it).getVecY(),(*it).getVecY()+(*it).getVnY(),(unsigned int)GREEN);
				renderer.drawLineByVectors((*it).getVecX(),(*it).getVecZ()+(*it).getVnZ(),(unsigned int)GREEN);
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
	return vec3((maxX+minX)/2,(maxY-minY)/2,(maxZ-minZ)/2);
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