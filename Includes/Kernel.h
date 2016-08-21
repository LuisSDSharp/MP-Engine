#ifndef KERNEL_H_
#define KERNEL_H_

#include <list>

class ITask;

using namespace std;

class Kernel final
{
private:
	typedef list<ITask*>							TaskList;
	typedef list<ITask*>::iterator					TaskListIterator;

	TaskList										m_tasks;
	TaskList										m_pausedTasks;

	void											PriorityAdd ( ITask* );

public:
	Kernel ();
	virtual ~Kernel ();

	void											Execute ();

	bool											AddTask ( ITask* );
	void											SuspendTask ( ITask* );
	void											ResumeTask ( ITask* );
	void											RemoveTask ( ITask* );

	void											SuspendAllTasks ();
	void											ResumeAllTasks ();
	void											KillAllTasks ();

	bool											HasTask () { return ( m_tasks.size () > 0 ) ? true : false; }
};

#endif