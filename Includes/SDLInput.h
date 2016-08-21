#ifndef SDLINPUT_H_
#define SDLINPUT_H_

#include "Input.h"

#if _SDL_OPEN_GL
#include <SDL_events.h>

class SDLInput : public Input
{
private:
	SDL_Event					m_Event;

public:
	SDLInput(unsigned int priority);
	virtual ~SDLInput();

	virtual void				Update();
	virtual void				Stop() { Log::Info("SDLInput task stoped"); }

	virtual void				RemapKeyCodes();
};
#endif

#endif