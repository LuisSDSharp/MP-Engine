#include "..\Includes\IEventHandler.h"
#include "..\Includes\Event.h"
#include <cassert>

Event::Event(EventID eventId)
{
}

Event::~Event()
{
}

void Event::Send()
{
	for (EventHandlerListIterator iter = m_listeners.begin();
		iter != m_listeners.end();
		++iter)
	{
		IEventHandler* pEventHandler = *iter;
		assert(pEventHandler);
		if (pEventHandler)
		{
			pEventHandler->HandleEvent(this);
		}
	}
}

void Event::SendToHandler(IEventHandler& eventHandler)
{
	for (EventHandlerListIterator iter = m_listeners.begin();
		iter != m_listeners.end();
		++iter)
	{
		if (&eventHandler == *iter)
		{
			eventHandler.HandleEvent(this);
		}
	}
}

void Event::AttachListener(IEventHandler& eventHandler)
{
	m_listeners.push_back(&eventHandler);
}

void Event::DetachListener(IEventHandler& eventHandler)
{
	for (EventHandlerListIterator iter = m_listeners.begin();
		iter != m_listeners.end();
		++iter)
	{
		if (&eventHandler == *iter)
		{
			m_listeners.erase(iter);
			break;
		}
	}
}
