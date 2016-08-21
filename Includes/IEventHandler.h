#ifndef IEVENT_HANDLER_H_
#define IEVENT_HANDLER_H_

#include "Event.h"

class Event;

class IEventHandler
{
public:
	virtual ~IEventHandler() {}

	virtual void HandleEvent ( Event* pEvent ) = 0;
};

#endif