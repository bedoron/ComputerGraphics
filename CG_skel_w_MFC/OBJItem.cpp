#include "stdafx.h"
#include "OBJItem.h"
#include "Scene.h"
OBJItem::OBJItem(void):vertices(),faces(),normals(),vTex(),minX(5),minY(5),minZ(5)
	,maxX(0),maxY(0),maxZ(0), drawBox(false), drawVertexNormal(false), drawNormal(false),_color(0),calcNormals(true),renderMode(Phong)
{
	verticesTree = new AvlTree<Vertex>();
}

OBJItem::~OBJItem(void)
{
}
OBJItem::OBJItem(const OBJItem& item):vertices(item.vertices),faces(item.faces),normals(item.normals),vTex(item.vTex)
	,minX(item.minX),minY(item.minY),minZ(item.minZ),maxX(item.maxX),maxY(item.maxY),maxZ(item.maxZ),
	drawBox(false), drawVertexNormal(false), drawNormal(false),_color(item._color),verticesTree(item.verticesTree),calcNormals(true),renderMode(Phong)
	,verticesArray(item.verticesArray),verticesArray4(item.verticesArray4),normalsArray(item.normalsArray),normalsArray4(item.normalsArray4)
	,_kAmbiantArray(item._kAmbiantArray),_kDifuseArray(item._kDifuseArray),_kSpecularArray(item._kSpecularArray),_shineArray(item._shineArray)
	,_vtArray(item._vtArray)
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
void OBJItem::addVT(vec2 vt)
{
	vTex.push_back(vt);
}
vec3& OBJItem::getVertexByNumber(int id)
{
	return vertices.at(id); // will throw out of bound
}
vec3& OBJItem::getNormalByNumber(int id)
{
	return normals.at(id); // will throw out of bound
}
vec2& OBJItem::getVTByNumber(int id)
{
	return vTex.at(id); // will throw out of bound
}

void OBJItem::draw(Renderer& renderer)
{
	//draw bounding box
	/*if(drawBox) {
		CubeModel stupidCube = CubeModel(minX,maxX,minY,maxY,minZ,maxZ);
		stupidCube.setObjectTransform(renderer.getObjectMatrices());
		stupidCube.draw(renderer);
	}*/

			

	//for(std::vector<Face>::iterator it = faces.begin() ;it != faces.end(); ++it) 
	//{
	//	try
	//	{
	//		Face curentface(*it);
	//		vec3 currentVerticies = curentface.getVertices();
	//		if(!calcNormals)
	//		{
	//			curentface.setVN1(getCalculatedNormal(currentVerticies.x));
	//			curentface.setVN2(getCalculatedNormal(currentVerticies.y));
	//			curentface.setVN3(getCalculatedNormal(currentVerticies.z));
	//		}
	//		//draw triangle
	//		if(!curentface.isMaterial())
	//		{
	//			renderer.setKAbmbiant(curentface.getKAmbiant());
	//			renderer.setKDiffuze(curentface.getKDiffuze());
	//			renderer.setKspecular(curentface.getKSpecular());
	//			renderer.setShine(curentface.getNS());
	//		}
	//		switch(renderMode)
	//		{
	//		case Flat:
	//			{
	//				renderer.DrawTriangle(curentface,_color,true);
	//				break;
	//			}
	//		case french:
	//			{
	//				renderer.DrawTriangleFrech(curentface,_color);
	//				break;
	//			}
	//		case Phong:
	//			{
	//				renderer.DrawTriangle(curentface,_color);
	//				break;
	//			}

	//		}
	//		
	//		renderer.DrawTriangle(curentface,_color);
	//		/*{
	//		renderer.drawLineByVectors((*it).getVecX() ,(*it).getVecY() ,(unsigned int) RED);
	//		renderer.drawLineByVectors((*it).getVecY() ,(*it).getVecZ() ,(unsigned int) RED);
	//		renderer.drawLineByVectors((*it).getVecZ() ,(*it).getVecX() ,(unsigned int) RED);
	//		}*/
	//		//draw normal
	//		if(drawNormal)
	//			renderer.drawLineByVectors((*it).getNormalLine()[0] ,(*it).getNormalLine()[1] ,(unsigned int)BLUE);
	//		if(drawVertexNormal)
	//		{
	//			renderer.drawLineByVectors(curentface.getVecX(),curentface.getVecX()+curentface.getVnX(),(unsigned int)GREEN);
	//			renderer.drawLineByVectors(curentface.getVecY(),curentface.getVecY()+curentface.getVnY(),(unsigned int)GREEN);
	//			renderer.drawLineByVectors(curentface.getVecX(),curentface.getVecZ()+curentface.getVnZ(),(unsigned int)GREEN);
	//		}
	//	}
	//	catch(exception& e)
	//	{
	//		e.what();
	//	}
	//	
	//}
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
void OBJItem::copyData()
{
	verticesArray = new vec3[faces.size()*3];
	verticesArray4 = new vec4[faces.size()*3];
	normalsArray = new vec3[faces.size()*3];
	normalsArray4 = new vec4[faces.size()*3];
	_kAmbiantArray = new vec4[faces.size()*3];
	_kDifuseArray = new vec4[faces.size()*3];
	_kSpecularArray = new vec4[faces.size()*3];
	_shineArray = new float[faces.size()*3];
	_vtArray = new vec2[faces.size()*3];
	int k=0;
	for(std::vector<Face>::iterator it = faces.begin() ;it != faces.end(); ++it) 
	{
		Face tmp = (*it);
		vec3 faceVertices = tmp.getVertices();
		_vtArray[k] = tmp.getVtX();
		_kAmbiantArray[k] = tmp.getKAmbiant();
		_kDifuseArray[k] = tmp.getKDiffuze();
		_kSpecularArray[k] = tmp.getKSpecular();
		vec3 tmpNormal=tmp.getVnX();
		if(tmpNormal.x != 0 && tmpNormal.y != 0 && tmpNormal.z != 0)
			normalsArray[k]=tmp.getVnX();
		else
			normalsArray[k] = getCalculatedNormal(faceVertices.x);
		_shineArray[k] = tmp.getNS();
		normalsArray4[k]=vec4(normalsArray[k]);
		verticesArray[k]=tmp.getVecX();
		verticesArray4[k++]=vec4(tmp.getVecX());
		
		_vtArray[k] = tmp.getVtY();
		_kAmbiantArray[k] = tmp.getKAmbiant();
		_kDifuseArray[k] = tmp.getKDiffuze();
		_kSpecularArray[k] = tmp.getKSpecular();

		tmpNormal=tmp.getVnY();
		if(tmpNormal.x != 0 && tmpNormal.y != 0 && tmpNormal.z != 0)
			normalsArray[k]=tmp.getVnY();
		else
			normalsArray[k] = getCalculatedNormal(faceVertices.y);
		_shineArray[k] = tmp.getNS();
		normalsArray4[k]=vec4(normalsArray[k]);
		verticesArray[k]=tmp.getVecY();
		verticesArray4[k++]=vec4(tmp.getVecY());
		
		_vtArray[k] = tmp.getVtZ();
		_kAmbiantArray[k] = tmp.getKAmbiant();
		_kDifuseArray[k] = tmp.getKDiffuze();
		_kSpecularArray[k] = tmp.getKSpecular();

		tmpNormal=tmp.getVnZ();
		if(tmpNormal.x != 0 && tmpNormal.y != 0 && tmpNormal.z != 0)
			normalsArray[k]=tmp.getVnZ();
		else
			normalsArray[k] = getCalculatedNormal(faceVertices.z);
		_shineArray[k] = tmp.getNS();
		normalsArray4[k]=vec4(normalsArray[k]);
		verticesArray[k]=tmp.getVecZ();
		verticesArray4[k++]=vec4(tmp.getVecZ());
	}
}

void OBJItem::draw(GLuint program)
{
	
	vec4* points = verticesArray4;
	vec4* normals = normalsArray4;

	int NumVertices = getFacesSize()*3;
	GLuint buffer[7];
	glGenBuffers(7, buffer);
	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), points, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), normals, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[2]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), _kAmbiantArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[3]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), _kDifuseArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[4]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), _kSpecularArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[5]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(float), _shineArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[6]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec2), _vtArray, GL_STATIC_DRAW);


	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	GLuint vPosition = glGetAttribLocation( program, "vPosition");
	glEnableVertexAttribArray(vPosition);


	glVertexAttribPointer( vPosition/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	GLuint vColor= glGetAttribLocation( program, "vNormal");
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer( vColor/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[2]);
	GLuint kambiant= glGetAttribLocation( program, "kambiant");
	glEnableVertexAttribArray(kambiant);

	glVertexAttribPointer( kambiant/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[3]);
	GLuint kdiffuse= glGetAttribLocation( program, "kdiffuse");
	glEnableVertexAttribArray(kdiffuse);

	glVertexAttribPointer( kdiffuse/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);


	glBindBuffer( GL_ARRAY_BUFFER, buffer[4]);
	GLuint kspecular= glGetAttribLocation( program, "kspecular");
	glEnableVertexAttribArray(kspecular);

	glVertexAttribPointer( kspecular/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[5]);
	GLuint shine= glGetAttribLocation( program, "shininess");
	glEnableVertexAttribArray(shine);

	glVertexAttribPointer( shine/*atrib*/, 1/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[6]);
	GLuint vtex= glGetAttribLocation( program, "tCoor");
	glEnableVertexAttribArray(vtex);

	glVertexAttribPointer( vtex/*atrib*/, 2/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);


}
void OBJItem::reDraw(GLuint program,int type)
{
	programType type1 = (programType)type;
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	GLuint model_view = glGetUniformLocation( program, "ModelView");
	glUniformMatrix4fv(model_view, 1, GL_TRUE, _world_transform);
	
	glDrawArrays( GL_TRIANGLES, 0, faces.size()*3);
	
	
}
void OBJItem::drawSilhoette()
{
	
}
void OBJItem::drawTexture(GLuint program,GLuint textureID,GLint textid)
{
	GLuint a= glGetError();

	glBindVertexArray(_vao);
	glUseProgram( program );


	int NumVertices = getFacesSize()*3;
	GLuint buffer[7];
	glGenBuffers(7, buffer);
	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), verticesArray4, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), normalsArray4, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[2]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), _kAmbiantArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[3]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), _kDifuseArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[4]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec4), _kSpecularArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[5]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(float), _shineArray, GL_STATIC_DRAW);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[6]);
	glBufferData( GL_ARRAY_BUFFER,NumVertices* sizeof(vec2), _vtArray, GL_STATIC_DRAW);



	
	glBindBuffer( GL_ARRAY_BUFFER, buffer[0]);
	GLuint vPosition = glGetAttribLocation( program, "vPosition");
	glEnableVertexAttribArray(vPosition);


	glVertexAttribPointer( vPosition/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[1]);
	GLuint vColor= glGetAttribLocation( program, "vNormal");
	glEnableVertexAttribArray(vColor);

	glVertexAttribPointer( vColor/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[2]);
	GLuint kambiant= glGetAttribLocation( program, "kambiant");
	glEnableVertexAttribArray(kambiant);

	glVertexAttribPointer( kambiant/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[3]);
	GLuint kdiffuse= glGetAttribLocation( program, "kdiffuse");
	glEnableVertexAttribArray(kdiffuse);

	glVertexAttribPointer( kdiffuse/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);


	glBindBuffer( GL_ARRAY_BUFFER, buffer[4]);
	GLuint kspecular= glGetAttribLocation( program, "kspecular");
	glEnableVertexAttribArray(kspecular);

	glVertexAttribPointer( kspecular/*atrib*/, 4/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[5]);
	GLuint shine= glGetAttribLocation( program, "shininess");
	glEnableVertexAttribArray(shine);
	glVertexAttribPointer( shine/*atrib*/, 1/*size*/, GL_FLOAT/*type*/,
	GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	glBindBuffer( GL_ARRAY_BUFFER, buffer[6]);
	GLuint tCoor = glGetAttribLocation( program, "tCoor");	
	glEnableVertexAttribArray(tCoor);
	glVertexAttribPointer( tCoor/*atrib*/, 2/*size*/, GL_FLOAT/*type*/,
		GL_FALSE/*normalized*/, 0/*stride*/, 0/*pointer*/);

	
	
	GLuint tex_loc = glGetUniformLocation(program,"texMap"); 
	glUniform1i(tex_loc,textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);

	glDrawArrays(GL_TRIANGLES,0,NumVertices);
}