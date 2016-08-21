#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "ITask.h"
#include "Logger.h"

class Platform : public ITask
{
private:
	static bool				g_closing;
	static bool				g_pausing;
	static bool				g_resuming;

public:
	Platform ( const unsigned int );
	virtual ~Platform ();

	static bool				IsClosing () { return g_closing; }
	static void				SetClosing () { g_closing = true; }
	static void				ClearClosing () { g_closing = false; }

	static bool				IsPausing () { return g_pausing; }
	static void				SetPausing () { g_pausing = true; }
	static void				ClearPausing () { g_pausing = false; }

	static bool				IsResuming () { return g_resuming; }
	static void				SetResuming () { g_resuming = true; }
	static void				ClearResuming () { g_resuming = false; }

	virtual bool			Start () { LOG_FUNCTION_NOT_IMPLEMENTED; return true; }
	virtual void			OnResume () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void			Update () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void			OnSuspend () { LOG_FUNCTION_NOT_IMPLEMENTED; }
	virtual void			Stop () { LOG_FUNCTION_NOT_IMPLEMENTED; }
};

#endif