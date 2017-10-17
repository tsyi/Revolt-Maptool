#pragma once

#define MgrTime cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETONE(cTimeManager);
	
private:
	DWORD m_dwLastUpdateTime;
	float m_fEllapsedTime;
	DWORD m_dwFrameCount;
	float m_fFrameTime;

	SYNTHESIZE(DWORD, m_dwFPS, FPS);

public:
	void Update();
	float GetElapsedTime();
	float GetLastUpdateTime();
};

