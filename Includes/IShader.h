#ifndef ISHADER_H_
#define ISHADER_H_

#ifndef NDEBUG
#include "Logger.h"
#endif

class IShader
{
public:
	virtual void		RegisterData ( const char*, const char*, const unsigned short ) = 0;
	virtual void		DeallocateBuffers () = 0;
	virtual bool		Setup () = 0;
	virtual void		RenderShader () = 0;
};

#endif