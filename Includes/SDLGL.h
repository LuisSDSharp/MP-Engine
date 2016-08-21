#ifndef SDLGL_H_
#define SDLGL_H_

#include "Renderer.h"

#if _SDL_OPEN_GL

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

#include <SDL.h>

class SDLGL : public Renderer
{
private:
	SDL_Window*					m_pWindow			= NULL;
	SDL_Surface*				m_pScreenSurface	= NULL;

	virtual void				BeginFrame() {}
	virtual void				EndFrame();

public:
	SDLGL(unsigned int priority);
	virtual ~SDLGL();

	virtual void				Init();
	virtual void				Destroy();
};
#endif

#endif