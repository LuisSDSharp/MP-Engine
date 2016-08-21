#ifndef APPLICATION_H_
#define APPLICATION_H_

/* FOWARD DECLARATIONS */
class ITask;
class Platform;
class Input;
class Renderer;
class Canvas;
/* ------------------- */

#include "Kernel.h"
#include "Timer.h"

class Application final
{
private:
	Platform*			m_platform;
	Input*				m_input;
	Renderer*			m_renderer;
	Kernel				m_kernel;
	Timer				m_timer;
	Canvas*				m_canvas;

public:
	Application ();

#if PLATFORM_ANDROID
	Application ( android_app* );
#endif

	virtual ~Application ();

	bool				Initialize ();
	void				Run ();

	bool				AddCanvasKernelTask ( Canvas* );
	bool				AddCustomKernelTask ( ITask* );
};

#endif