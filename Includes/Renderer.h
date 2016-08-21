#ifndef RENDERER_H_
#define RENDERER_H_

class RendererManager;
class Renderable;

#include "ITask.h"
#include "Logger.h"
#include <vector>

class Renderer : public ITask
{
	friend class RendererManager;

private:
	RendererManager*			m_manager;

	static bool					g_initializing;
	static bool					g_destroying;

protected:
	std::vector<Renderable*>	m_renderables;

	bool						m_bInitialized;
	bool						m_vsyncEnabled;
	bool						m_fullscreenEnabled;

	unsigned int				WINDOW_WIDTH = 800;
	unsigned int				WINDOW_HEIGHT = 600;

	const float					SCREEN_DEPTH = 1000.0f;
	const float					SCREEN_NEAR = 0.1f;

	virtual void				RenderFrame ();
	virtual void				BeginFrame () = 0;
	virtual void				EndFrame () = 0;

public:
	Renderer ( const unsigned int priority );
	virtual ~Renderer ();

	virtual void				Init () = 0;
	virtual void				Destroy () = 0;

	virtual bool				Start () = 0;
	virtual void				OnSuspend () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void				Update ();
	virtual void				OnResume () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void				Stop () = 0;

	virtual void				RegisterRenderable ( Renderable* pRenderable ) = 0;

	static void					SetInitializing () { g_initializing = true; }
	static void					SetDestroying () { g_destroying = true; }
};

#endif