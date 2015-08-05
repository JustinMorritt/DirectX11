#ifndef _GAMETIMER_H
#define _GAMETIMER_H

class GameTimer
{
public:
	static float TotalTime();  //in Seconds
	static float DeltaTime(); //in seconds

	void Reset(); // call before message loop
	void Start(); // Call when unpaused
	void Stop();  // call when paused
	void Tick();  // call every frame

	GameTimer();
	~GameTimer();

private:
	static double m_SecondsPerCount;
	static double m_DeltaTime;

	static __int64 m_BaseTime;
	static __int64 m_PausedTime;
	static __int64 m_StopTime;
	static __int64 m_PrevTime;
	static __int64 m_CurrTime;

	static bool m_Stopped;
};

#endif