#ifndef EGL_H_
#define EGL_H_

#include "Renderer.h"

#if RENDERER_EGL

#include <EGL\egl.h>

class EGL : public Renderer
{
private:
	android_app*	m_pState;
	EGLDisplay		m_display;
	EGLContext		m_context;
	EGLSurface		m_surface;
	int32_t			m_width;
	int32_t			m_height;

public:
	EGL ( android_app*, const unsigned int );
	virtual ~EGL ();

	virtual void	BeginFrame () override;
	virtual void	EndFrame () override;
	virtual void	Init () override;
	virtual void	Destroy () override;

	virtual bool	Start () override;
	virtual void	OnSuspend () override;
	virtual void	OnResume () override;
	virtual void	Stop () override;
};

#endif

#endif
