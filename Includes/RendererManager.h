#ifndef RENDERER_MANAGER_H_
#define RENDERER_MANAGER_H_

class Renderer;
class Renderable;

#include "ISingleton.h"

class RendererManager : public ISingleton<RendererManager>
{
	friend class Renderer;

private:
	RendererManager ( Renderer* const );
	~RendererManager ();

	Renderer* m_owner;

public:
	void			RegisterRenderable ( Renderable* );
};

#endif
