#ifndef RENDERER_COMPONENT_H_
#define RENDERER_COMPONENT_H_

class RendererManager;
class Renderable;
class GameObject;

#include "Component.h"

class RendererComponent : public Component
{
private:
	RendererManager*	m_manager;
	Renderable*			m_renderable;

public:
	RendererComponent ( GameObject* );
	~RendererComponent ();

	void				CreateCustomMesh ( float(&)[3][3], const unsigned short (&) [3], unsigned short );
	void				CreateShader ( const char*, const char* );

	// Inherited via IComponent
	virtual void		Initialize () override;
};

#endif
