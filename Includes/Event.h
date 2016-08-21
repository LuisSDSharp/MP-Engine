#ifndef EVENT_H_
#define EVENT_H_

class IEventHandler;

#include <vector>

typedef unsigned int EventID;

class Event
{
private:
	typedef std::vector<IEventHandler*> EventHandlerList;
	typedef EventHandlerList::iterator EventHandlerListIterator;

	EventHandlerList	m_listeners;
	EventID				m_id;

public:
	explicit Event(EventID eventId);
	~Event();

	void Send();
	void SendToHandler(IEventHandler& eventHandler);
	void AttachListener(IEventHandler& eventHandler);
	void DetachListener(IEventHandler& eventHandler);

	EventID GetID() const { return m_id; }
};

#endif