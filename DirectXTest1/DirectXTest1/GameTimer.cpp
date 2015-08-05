#include "GameTimer.h"
#include  <windows.h>


double GameTimer::m_SecondsPerCount;
double GameTimer::m_DeltaTime;
__int64 GameTimer::m_BaseTime;
__int64 GameTimer::m_CurrTime;
__int64 GameTimer::m_PrevTime;
__int64 GameTimer::m_PausedTime;
__int64 GameTimer::m_StopTime;
bool GameTimer::m_Stopped;


GameTimer::GameTimer() 
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}


GameTimer::~GameTimer()
{
}



float GameTimer::TotalTime() 
{
	return (float)m_CurrTime;
	if (m_Stopped)
	{
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
	else
	{
		return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
}

float GameTimer::DeltaTime() 
{
	return (float)m_DeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_Stopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	if (m_Stopped) // if it is stopped accumulate the paused time 
	{
		m_PausedTime += (startTime - m_StopTime);
		m_PrevTime = startTime; // old previous time is the new time
		m_StopTime = 0;
		m_Stopped = false;
	}
}

void GameTimer::Stop()
{
	if (!m_Stopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_StopTime = currTime;
		m_Stopped = true;
	}
}

void GameTimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;

	//time difference between this frame and the previous
	m_DeltaTime = (m_CurrTime - m_PrevTime)* m_SecondsPerCount;

	// re-assign prev time 
	m_PrevTime = m_CurrTime;

	// check if delta time is > 0 ... can jumble if hopps into power save mode
	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}

}

