#include "..\Includes\SDLGL.h"

#if _SDL_OPEN_GL
SDLGL::SDLGL(unsigned int priority) : Renderer(priority)
{
}

SDLGL::~SDLGL()
{
}

void SDLGL::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		m_bInitialized = false;
		Log::Warn("SDL could not be initialized: %s", SDL_GetError());
	}
	else
	{
		m_pWindow = SDL_CreateWindow("SDL on Windows", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_pWindow == NULL)
		{
			m_bInitialized = false;
			Log::Warn("SDL Window could not be created: %s", SDL_GetError());
		}
		else
		{
			m_pScreenSurface = SDL_GetWindowSurface(m_pWindow);
			m_bInitialized = true;
		}
	}

	Log::Info("SDL initialized");
}

void SDLGL::Destroy()
{
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = NULL;

	SDL_Quit();

	Log::Info("SDL destroyed");
}

void SDLGL::EndFrame()
{
	SDL_UpdateWindowSurface(m_pWindow);
}
#endif
