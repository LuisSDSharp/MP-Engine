#include "../Includes/Kernel.h"
#include "../Includes/ITask.h"
#include "../Includes/Platform.h"

#include <algorithm>

Kernel::Kernel ()
{}

Kernel::~Kernel ()
{}

void Kernel::PriorityAdd ( ITask* pTask )
{
	TaskListIterator iter;
	for ( iter = m_tasks.begin (); iter != m_tasks.end (); ++iter )
	{
		if ( ( *iter )->Priority () > pTask->Priority () )
		{
			break;
		}
	}
	m_tasks.insert ( iter, pTask );
}

bool Kernel::AddTask ( ITask* pTask )
{
	bool started = pTask->Start ();
	if ( started )
	{
		PriorityAdd ( pTask );;
	}
	return started;
}

void Kernel::Execute ()
{
	while ( m_tasks.size () )
	{
		if ( Platform::IsClosing () )
		{
			KillAllTasks ();
		}

		if ( Platform::IsPausing () )
		{
			SuspendAllTasks ();
			Platform::ClearPausing ();
		}
		
		if ( Platform::IsResuming () )
		{
			ResumeAllTasks ();
			Platform::ClearResuming ();
		}

		for ( const auto& pTask : m_tasks )
		{
			if ( !pTask->CanKill () )
			{
				pTask->Update ();
			}
		}

		TaskListIterator iter;
		for ( iter = m_tasks.begin (); iter != m_tasks.end (); )
		{
			ITask* pTask = ( *iter );
			++iter;
			if ( pTask->CanKill () )
			{
				pTask->Stop ();
				m_tasks.remove ( pTask );
				pTask = nullptr;
			}
		}
	}

	Platform::ClearClosing ();
}

void Kernel::SuspendTask ( ITask* pTask )
{
	if ( std::find ( m_tasks.begin (), m_tasks.end (), pTask ) != m_tasks.end () )
	{
		pTask->OnSuspend ();
		m_tasks.remove ( pTask );
		m_pausedTasks.push_back ( pTask );
	}
}

void Kernel::ResumeTask ( ITask* pTask )
{
	if ( std::find ( m_pausedTasks.begin (), m_pausedTasks.end (), pTask ) != m_pausedTasks.end () )
	{
		pTask->OnResume ();
		m_pausedTasks.remove ( pTask );
		PriorityAdd ( pTask );
	}
}

void Kernel::RemoveTask ( ITask* pTask )
{
	if ( std::find ( m_tasks.begin (), m_tasks.end (), pTask ) != m_tasks.end () )
	{
		pTask->SetCanKill ( true );
	}
}

void Kernel::SuspendAllTasks ()
{
	for ( const auto& pTask : m_tasks )
	{
		SuspendTask ( pTask );
	}
}

void Kernel::ResumeAllTasks ()
{
	for ( const auto& pTask : m_pausedTasks )
	{
		ResumeTask ( pTask );
	}
}

void Kernel::KillAllTasks()
{
	for ( const auto& pTask : m_tasks )
	{
		pTask->SetCanKill ( true );
	}

	for ( const auto& pTask : m_pausedTasks )
	{
		pTask->SetCanKill ( true );
	}
}
