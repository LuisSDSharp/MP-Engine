#ifndef CANVAS_H_
#define CANVAS_H_

#include "ITask.h"

class Canvas : public ITask
{
public:
	Canvas ( const unsigned int );
	virtual ~Canvas();
};

#endif