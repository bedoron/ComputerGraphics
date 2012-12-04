#pragma once
#include "Renderer.h"
#include "Scene.h"

class Renderable: public Model
{
	Renderer *_renderer;
public:
	Renderable(Renderer& renderer): _renderer(&renderer) { };
	Renderable(): _renderer(0) { };
	void setRenderer(Renderer& renderer) { _renderer = &renderer; };
	Renderer& getRenderer() { return *_renderer; };
	bool hasRenderer() { return _renderer!=0; };
	virtual ~Renderable(void);
};

