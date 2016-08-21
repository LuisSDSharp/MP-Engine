#ifndef IMESH_H_
#define IMESH_H_

#ifndef NDEBUG
#include "Logger.h"
#endif

class IMesh
{
public:
	virtual void				RegisterMeshData ( const float (&) [3] [3], const unsigned short (&) [3], const unsigned short ) = 0;
	virtual void				RenderMesh () = 0;
	virtual void				DeallocateBuffers () = 0;
	
	virtual unsigned short		GetNumElements () = 0;
};

#endif
