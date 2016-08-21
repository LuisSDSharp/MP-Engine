#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include "Event.h"
#include "IEventHandler.h"
#include <unordered_map>

class EventManager
{
	friend void SendEvent(EventID eventId);
	friend void SendEventToHandler(EventID eventId, IEventHandler& eventHandler);
	friend bool RegisterEvent(EventID eventId);
	friend void AttachEvent(EventID eventId, IEventHandler& eventHandler);
	friend void DetachEvent(EventID eventId, IEventHandler& eventHandler);

private:
	typedef std::tr1::unordered_map<EventID, Event*> EventMap;
	typedef EventMap::iterator EventMapIterator;

	EventMap	m_eventMap;

	void SendEvent(EventID eventId);
	void SendEventToHandler(EventID eventId, IEventHandler& eventHandler);
	bool RegisterEvent(EventID eventId);
	void AttachEvent(EventID eventId, IEventHandler& eventHandler);
	void DetachEvent(EventID eventId, IEventHandler& eventHandler);

public:
	EventManager();
	~EventManager();
};

#endif