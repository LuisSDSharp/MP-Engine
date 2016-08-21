#include "..\Includes\Timer.h"

Timer::Timer ( const unsigned int priority ) : ITask ( priority ), m_timeLastFrame ( 0 ), m_frameDt ( 0.0f )
{}

float Timer::GetTimeFrame () const
{
	return 0.0f;
}

float Timer::GetTimeSim () const
{
	return 0.0f;
}

void Timer::SetSimMultiplier ( const float simMultiplier )
{}

Timer::TimeUnits Timer::nanoTime ()
{
	#if PLATFORM_ANDROID
		timespec now;
		int err = clock_gettime ( CLOCK_MONOTONIC, &now );
		return now.tv_sec * 1000000000LL + now.tv_nsec;
	#elif PLATFORM_WINDOWS
		return GetTickCount ();
	#endif
}

bool Timer::Start ()
{
	Log::Info ( "Timer taks started" );

	m_timeLastFrame = nanoTime ();
	return true;
}

void Timer::OnResume ()
{
	Log::Info ( "Timer taks resumed" );

	m_timeLastFrame = nanoTime ();
}

void Timer::Update ()
{
	TimeUnits currentTime = nanoTime ();
	#if PLATFORM_ANDROID
	const float MULTIPLIER = 0.000000001f;
	#elif PLATFORM_WINDOWS
	const float MULTIPLIER = 0.001f;
	#endif
	m_frameDt = ( currentTime - m_timeLastFrame ) * MULTIPLIER;
	m_timeLastFrame = currentTime;
	m_simDt = m_frameDt * m_simMultiplier;
}
