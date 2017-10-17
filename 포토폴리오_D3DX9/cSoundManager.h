#pragma once
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"
#pragma comment (lib, "fmod/fmodex_vc.lib")

using namespace FMOD;
//using namespace std;

#define MgrSound cSoundManager::GetInstance()
#define SOUND_MAX_CHANNEL	10


#define ERRORCHECK {\
	if (result != FMOD_OK)	{\
		std::cout<<"FMOD error! \t"<<result<<":"<<FMOD_ErrorString(result)<<std::endl;\
	}\
}


class cSoundManager
{
	SINGLETONE(cSoundManager);
private:
//	FMOD::System*	m_pSystem;
//	FMOD::Sound*	m_pSound;
//	FMOD::Channel*	m_pChannel;
//	FMOD_RESULT		result;	//error°Ë»ç
//
//	int m_nCountDrives;
//	char* m_lpDriverName;

	//Old
	System* m_pSystem;
	std::map<std::string, Sound*> m_sounds;
	std::list<Channel*> m_channels;

	std::string m_bgmSound;
	int m_bgmVoiune;
	Channel* m_pBgmChnnel;

	FMOD_VECTOR m_fmVector_Listener;
	FMOD_VECTOR m_forward;
	FMOD_VECTOR m_up;

public:
	//void Setup();
	//void LoadSound(std::string name, bool isLoop);
	//void PlaySound(float volume);
	//void Destory();




	//Old
	void Setup();
	void Destory();
	void Update();

	void Setup3DCamera(D3DXVECTOR3 pos, D3DXVECTOR3 forward, D3DXVECTOR3 up = D3DXVECTOR3(0.f,1.f,0.f));

	Sound* FindSound(std::string key);
	Channel* FindChannel(std::string key);

	void LoadSound(std::string folderName, std::string fileName, bool loop);

	void Play(std::string fileName, float volume = 1.0f, D3DXVECTOR3 soundPos = D3DXVECTOR3(0, 0, 0));
	void Play_BGM(std::string fileName, float volume = 1.0f);
	void SetVolum(std::string key, float volume);

	void SetSoundPosition(std::string fileName, D3DXVECTOR3 position);
	void SetSoundPosition(Channel* pChannel, D3DXVECTOR3 position);
	void SetSoundPosition(Channel* pChannel, FMOD_VECTOR position);

	
	void Stop(std::string fileName);
	void Pause(std::string fileName);
	void Resume(std::string fileName);

	void AllSoundIsStop();
	void AllSoundIsPause();
	void AllSoundIsResume();

};