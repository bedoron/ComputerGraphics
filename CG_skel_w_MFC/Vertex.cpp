#include "stdafx.h"
#include "Vertex.h"

Vertex::~Vertex()
{
}
Vertex::Vertex(int id):_id(id),faces()
{
}
bool Vertex::operator==(Vertex& v) const
{
	return _id == v._id;
}
bool Vertex::operator!=(Vertex& v) const
{
	return _id != v._id;
}
bool operator<(const Vertex& v1, const Vertex& v2)
{
	return v1._id<v2._id;
}
bool operator>(const Vertex& v1, const Vertex& v2)
{
	return v1._id>v2._id;
}
void Vertex::addFace(int id)
{
	faces.push_back(id);
}
std::vector<int>& Vertex::getFaces()
{
	return faces;
}