#ifndef TIMER_H_
#define TIMER_H_

#include "Platforms.h"
#include "Logger.h"
#include "ITask.h"

#if PLATFORM_ANDROID
#include <time.h>
#endif

class Timer final : public ITask
{
private:
	#if PLATFORM_ANDROID
	typedef long long			TimeUnits;
	#elif PLATFORM_WINDOWS
	typedef unsigned int		TimeUnits;
	#endif

	TimeUnits					nanoTime ();

	TimeUnits					m_timeLastFrame;
	float						m_frameDt;
	float						m_simDt;
	float						m_simMultiplier;

public:
	Timer ( const unsigned int priority );
	~Timer () { Log::Info ( "Timer task destroyed" ); }

	float						dt () { return m_frameDt; }

	float						GetTimeFrame () const;
	float						GetTimeSim () const;
	void						SetSimMultiplier ( const float simMultiplier );

	virtual bool				Start () override;
	virtual void				OnSuspend () override { Log::Info ( "Timer taks suspended" ); }
	virtual void				Update () override;
	virtual void				OnResume () override;
	virtual void				Stop () override { Log::Info ( "Timer task stoped" ); }
};

#endif