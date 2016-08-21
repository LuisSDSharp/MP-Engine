#ifndef ITASK_H_
#define ITASK_H_

class ITask
{
private:
	unsigned int	m_priority;
	bool			m_canKill;

public:
	explicit ITask(unsigned int priority)		{ m_priority = priority; m_canKill = false; }
	virtual ~ITask()								{}

	virtual bool	Start()						= 0;
	virtual void	OnSuspend()					= 0;
	virtual void	Update()					= 0;
	virtual void	OnResume()					= 0;
	virtual void	Stop()						= 0;

	void			SetCanKill(bool canKill)	{ m_canKill = canKill; }
	bool			CanKill()					{ return m_canKill; }
	unsigned int	Priority()					{ return m_priority; }

	static const unsigned int TIMER_PRIORITY	= 0;
	static const unsigned int PLATFORM_PRIORITY = 1000;
	static const unsigned int INPUT_PRIORITY	= 2000;
	static const unsigned int GAME_PRIORITY		= 3000;
	static const unsigned int RENDERER_PRIORITY = 4000;
	static const unsigned int FILE_PRIORITY		= 5000;
};

#endif