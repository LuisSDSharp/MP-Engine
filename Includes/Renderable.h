#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "..\Includes\IMesh.h"
#include "..\Includes\IShader.h"

#ifndef NDEBUG
#include "Logger.h"
#endif

class Renderable
{
private:
	IMesh*			m_mesh;
	IShader*		m_shader;

public:
	Renderable ();
	~Renderable ();

	void			CreateMesh ( const float (&) [3] [3], const unsigned short (&) [3], const unsigned short );
	void			CreateShader ( const char*, const char* );

	IMesh*			GetMesh () { return m_mesh; }
	IShader*		GetShader () { return m_shader; }
};

#endif
