#pragma once
#include "renderable.h"
#include <vector>

using std::vector;
class CompositeElement :
	public Renderable
{
	vector<Renderable*> elements;
public:
	void add(Renderable& element) {
		element.setRenderer(getRenderer());
		elements.push_back(&element);
	};

	CompositeElement(Renderer &renderer): Renderable(renderer) { };
	CompositeElement() { };

	void draw() {
		vector<Renderable*>::const_iterator iter = elements.begin();
		for(;iter!=elements.end();++iter)
			(*iter)->draw;
	}

	~CompositeElement(void) {
		vector<Renderable*>::const_iterator iter = elements.begin();
		for(;iter!=elements.end();++iter)
			delete (*iter);
	}
};

