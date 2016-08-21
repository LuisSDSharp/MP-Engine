#ifndef OPEN_GL_H_
#define OPEN_GL_H_

#include "Renderer.h"

#if RENDERER_OPENGL

#pragma comment(lib, "OpenGL32.lib")

#include <gl\GL.h>

class OpenGL final : public Renderer
{
protected:
	virtual void BeginFrame() {}
	virtual void EndFrame() {}

public:
	OpenGL(unsigned int priority);
	virtual ~OpenGL();

	virtual void Init() {}
	virtual void Destroy() {}
};
#endif

#endif