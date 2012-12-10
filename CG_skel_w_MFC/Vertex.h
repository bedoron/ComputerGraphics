#pragma once
#include <vector>
#include "Face.h"
class Vertex
{
private:
	int _id;
	std::vector<int> faces;
public:
	Vertex(int id);
	~Vertex(void);
	void addFace(int id);
	std::vector<int>& getFaces();
	friend bool operator<(const Vertex& v1, const Vertex& v2);
	friend bool operator>(const Vertex& v1, const Vertex& v2);
	bool operator==(Vertex& v) const;
	bool operator!=(Vertex& v) const;
	friend std::ostream& operator<<(std::ostream& os, const Vertex& p);
};

