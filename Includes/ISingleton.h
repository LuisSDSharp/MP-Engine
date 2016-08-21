#ifndef ISINGLETON_H_
#define ISINGLETON_H_

#include <cassert>

#ifndef NDEBUG
	#include "Logger.h"
#endif

template <typename T>
class ISingleton
{
private:
	static T* ms_instance;

public:
	ISingleton()
	{
		assert(!ms_instance);
		ms_instance = static_cast<T*>(this);
	}

	~ISingleton()
	{
		assert(ms_instance);
		ms_instance = 0;
	}

	static T& GetInstance()		{ return *ms_instance; }
	static T* GetInstancePtr()	{ return ms_instance; }
};

template <typename T> T* ISingleton<T>::ms_instance = NULL;

#endif