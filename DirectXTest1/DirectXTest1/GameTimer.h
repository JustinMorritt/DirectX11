#ifndef _GAMETIMER_H
#define _GAMETIMER_H

class GameTimer
{
public:
	float TotalTime()const;  //in Seconds
	float DeltaTime()const; //in seconds

	void Reset(); // call before message loop
	void Start(); // Call when unpaused
	void Stop();  // call when paused
	void Tick();  // call every frame

	GameTimer();
	~GameTimer();

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_Stopped;
};

#endif