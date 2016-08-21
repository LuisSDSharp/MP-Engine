#ifndef BASE_H_
#define BASE_H_

#include "Platforms.h"

#if _DEBUG
#include "Logger.h"
#include <assert.h>
#endif

#include "ITask.h"
#include "Kernel.h"
#include "Timer.h"
#include "Platform.h"
#include "Renderer.h"

#if PLATFORM_ANDROID
	#include "Android.h"

	#if _EGL
		//TODO - Include EGL header
	#elif _SDL_OPEN_GL_ES_1
		// TODO - Include SDLGLES1 header
	#elif _SDL_OPEN_GL_ES_2
		// TODO - Include SDLGLES2 header
	#endif
#elif PLATFORM_WINDOWS
	#include "Windows.h"
	#include "WinInput.h"

	#if _OPEN_GL
		#include "OpenGL.h"
	#elif _SDL_OPEN_GL
		#include "SDLGL.h"
		#include "SDLInput.h"
	#elif _DIRECT_X
		#include "D3D.h"
	#endif
#elif PLATFORM_IOS
	// TODO - Include IOS header
#endif

#endif