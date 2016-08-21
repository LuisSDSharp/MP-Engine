#include "..\Includes\SDLInput.h"
#include "..\Includes\Platform.h"

#if _SDL_OPEN_GL
SDLInput::SDLInput(unsigned int priority) : Input(priority)
{
}

SDLInput::~SDLInput()
{
}

void SDLInput::Update()
{
	m_bWasKeyDown = m_bWasKeyUp = false;

	while (SDL_PollEvent(&m_Event) != 0)
	{
		if (m_Event.type == SDL_KEYDOWN)
		{
			m_bWasKeyDown = true;
		}
		else if (m_Event.type == SDL_KEYUP)
		{
			m_bWasKeyUp = true;
		}

		if (m_bWasKeyDown || m_bWasKeyUp)
		{
			m_KeyCode = m_Event.key.keysym.sym;
		}

		if (m_Event.type == SDL_QUIT)
		{
			Platform::SetClosing();
		}
	}
}

void SDLInput::RemapKeyCodes()
{
	m_Keys.ESCAPE		= SDLK_ESCAPE;
	m_Keys.ARROW_UP		= SDLK_UP;
	m_Keys.ARROW_DOWN	= SDLK_DOWN;
	m_Keys.ARROW_LEFT	= SDLK_LEFT;
	m_Keys.ARROW_RIGHT	= SDLK_RIGHT;
}
#endif
