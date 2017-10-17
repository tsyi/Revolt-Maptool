#include "stdafx.h"
#include "cSoundManager.h"

cSoundManager::cSoundManager()
{
}


cSoundManager::~cSoundManager()
{
}

//void cSoundManager::Setup()
//{
//	FMOD::System_Create(&m_pSystem);
//	m_pSystem->init(SOUND_MAX_CHANNEL, FMOD_INIT_NORMAL, 0);
//}
//
//void cSoundManager::LoadSound(std::string name, bool isLoop)
//{
//	FMOD::System_Create(&m_pSystem);
//	m_pSystem->init(SOUND_MAX_CHANNEL, FMOD_INIT_NORMAL, 0);
//	if (isLoop)m_pSystem->createStream(name.c_str(), FMOD_LOOP_NORMAL, NULL, &m_pSound);
//	else  m_pSystem->createSound(name.c_str(), FMOD_DEFAULT, 0, &m_pSound);
//}
//
//void cSoundManager::PlaySound(float volume)
//{
//	FMOD_MODE mode;
//	m_pSound->getMode(&mode);
//	m_pSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, FALSE, &m_pChannel);
//	m_pChannel->setVolume(volume);
//}
//
//void cSoundManager::Destory()
//{
//	if (m_pSound) { m_pSound->release(); m_pSound = NULL; }
//	if (m_pSystem) { m_pSystem->release(); m_pSystem = NULL; }
//}


//=======================================================
//=======================================================
//=======================================================
//=======================================================
//=======================================================
//=======================================================


//Bass
void cSoundManager::Setup()
{
	System_Create(&m_pSystem);

	//	m_pSystem->init(SOUND_MAX_CHANNEL, FMOD_INIT_NORMAL, 0);
	Setup3DCamera(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1));



	unsigned int uiFMODVersion;
	m_pSystem->getVersion(&uiFMODVersion);
	if (uiFMODVersion < FMOD_VERSION)
	{
		std::cout << "���� ����" << std::endl;
		return;
	}

	FMOD_SPEAKERMODE speakerMode;
	FMOD_CAPS	caps;

	m_pSystem->getDriverCaps(0, &caps, 0, &speakerMode);
	m_pSystem->setSpeakerMode(speakerMode);

	m_pSystem->setSoftwareChannels(SOUND_MAX_CHANNEL);

	m_pSystem->init(SOUND_MAX_CHANNEL, FMOD_INIT_NORMAL, 0);


	m_channels.push_back(m_pBgmChnnel);
}



void cSoundManager::Destory()
{
	m_pSystem->release();
	m_pSystem->close();
}

void cSoundManager::Update()
{
	m_pSystem->update();

	std::list<Channel*>::iterator it;
	for (it = m_channels.begin(); it != m_channels.end(); it++)
	{
		bool isPlaying;
		(*it)->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			m_channels.erase(it);
			break;
		}
	}
}

void cSoundManager::Setup3DCamera(D3DXVECTOR3 pos, D3DXVECTOR3 forward, D3DXVECTOR3 up)
{
	m_fmVector_Listener.x = pos.x;
	m_fmVector_Listener.y = pos.y;
	m_fmVector_Listener.z = pos.z;

	m_forward.x = forward.x;
	m_forward.y = forward.y;
	m_forward.z = forward.z;

	m_up.x = up.x;
	m_up.y = up.y;
	m_up.z = up.z;

	m_pSystem->set3DListenerAttributes(0, &m_fmVector_Listener, nullptr, &m_forward, &m_up);
}

//Find
Sound* cSoundManager::FindSound(std::string fileName)
{
	if (m_sounds.find(fileName) == m_sounds.end()) return NULL;
	return m_sounds[fileName];
}

Channel * cSoundManager::FindChannel(std::string fileName)
{
	Sound* pSound = FindSound(fileName);
	if (pSound == NULL) return NULL;

	std::list<Channel*>::iterator it;
	for (it = m_channels.begin(); it != m_channels.end(); it++)
	{
		Sound* pCurrentSound;
		(*it)->getCurrentSound(&pCurrentSound);
		if (pCurrentSound == pSound) return (*it);
	}
	return NULL;
}

//Load
void cSoundManager::LoadSound(std::string folderName, std::string fileName, bool loop)
{
	Sound* pSound = FindSound(fileName);

	if (pSound != NULL) return;

	std::string fullName = folderName + "/" + fileName;

	if (loop)
	{
		m_pSystem->createStream(fullName.c_str(), FMOD_LOOP_NORMAL, NULL, &pSound);
		if (pSound == NULL)
		{
			std::cout << "�ش� ������ ��θ� ã�� �� �����ϴ� : " << fullName << std::endl;
			return;
		}
	}
	else
	{
		m_pSystem->createSound(fullName.c_str(), FMOD_3D, NULL, &pSound);
		//FMOD_DEFAULT | 
		if (pSound == NULL)
		{
			std::cout << "�ش� ������ ��θ� ã�� �� �����ϴ� : " << fullName << std::endl;
			return;
		}
		//	pSound->setMode(FMOD_LOOP_NORMAL);	//test;
		//pSound->set3DMinMaxDistance(10, 11);
		//	)
		//	pSound->Set3dSpread(1);
		//	pSound->SetMaxDistance(10);
		//	pSound->SetMinDistance(1000);
		//	pSound->SetDoplerEffect(1);


	}

	m_sounds[fileName] = pSound;
}


//Play
void cSoundManager::Play(std::string fileName, float volume, D3DXVECTOR3 soundPos)
{
	Sound* pSound = FindSound(fileName);
	if (pSound == NULL)
	{
		std::cout << "�ش� Ű ���� �޴����� ��ϵǾ����� �ʽ��ϴ� : " << fileName << std::endl;
		return;
	}


	Channel* pChannel;
	m_pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
	if (pChannel == NULL)
	{
		std::cout << "ä�� �ҷ����� ����! : " << fileName << std::endl;
		return;

	}
	pChannel->set3DMinMaxDistance(20, 100);
	//pChannel->set3DSpread(1);
	//pChannel->set3DPanLevel(0.5);
	pChannel->set3DDistanceFilter(false,0,1500);
	SetSoundPosition(pChannel, soundPos);

	pChannel->setVolume(volume);
	m_channels.push_back(pChannel);
}

void cSoundManager::Play_BGM(std::string fileName, float volume)
{
	if (m_bgmSound == fileName)	//bgm ���� ����
	{
		if (m_bgmVoiune == volume)
		{
			m_bgmVoiune = volume;
			m_pBgmChnnel->setVolume(m_bgmVoiune);
		}
		return;
	}
	m_bgmSound = fileName;

	Sound* pSound = FindSound(fileName);
	if (pSound == NULL)
	{
		std::cout << "�ش� Ű ���� �޴����� ��ϵǾ����� �ʽ��ϴ� : " << fileName << std::endl;
		return;
	}

	//	Channel* pChannel;
	m_pBgmChnnel = m_channels.front();
	m_pSystem->playSound(FMOD_CHANNEL_REUSE, pSound, false, &m_pBgmChnnel);
	if (m_pBgmChnnel == NULL)
	{
		std::cout << "ä�� �ҷ����� ����! : " << fileName << std::endl;
		return;
	}
	m_pBgmChnnel->setVolume(volume);

}


void cSoundManager::SetVolum(std::string fileName, float volume)
{
	Channel* pChannel = FindChannel(fileName);
	if (pChannel == NULL)
	{
		std::cout << "�ش� Ű ���� �޴����� ��ϵǾ����� �ʽ��ϴ� : " << fileName << std::endl;
	}
	pChannel->setVolume(volume);
}

void cSoundManager::SetSoundPosition(std::string fileName, D3DXVECTOR3 position)
{
	Channel* pChannel = FindChannel(fileName);
	SetSoundPosition(pChannel, position);
}

void cSoundManager::SetSoundPosition(Channel* pChannel, D3DXVECTOR3 position)
{
	FMOD_VECTOR fmodVector;
	fmodVector.x = position.x;
	fmodVector.y = position.y;
	fmodVector.z = position.z;
	SetSoundPosition(pChannel, fmodVector);
}

void cSoundManager::SetSoundPosition(Channel * pChannel, FMOD_VECTOR position)
{
	pChannel->set3DAttributes(&position, NULL);
}


//Contorl
void cSoundManager::Stop(std::string fileName)
{
	Channel* pChannel = FindChannel(fileName);
	if (pChannel != NULL) { pChannel->stop(); }//wcout << "SOUND-> " << key << " ->����" << endl; }
}

void cSoundManager::Pause(std::string fileName)
{
	Channel* pChannel = FindChannel(fileName);
	if (pChannel != NULL) { pChannel->setPaused(true); }//wcout << "SOUND-> " << key << " ->��������" << endl;
}
void cSoundManager::Resume(std::string fileName)
{
	Channel* pChannel = FindChannel(fileName);
	if (pChannel != NULL) { pChannel->setPaused(false); }//wcout << "SOUND-> " << key << " ->�ٽ����" << endl; }
}


void cSoundManager::AllSoundIsStop()
{
	std::list<Channel*>::iterator it;
	for (it = m_channels.begin(); it != m_channels.end(); it++)
	{
		(*it)->stop();
	}
}
void cSoundManager::AllSoundIsPause()
{
	std::list<Channel*>::iterator it;
	for (it = m_channels.begin(); it != m_channels.end(); it++)
	{
		(*it)->setPaused(true);
	}
	//	FindChannel(currentSound)->setPaused(false);
}
void cSoundManager::AllSoundIsResume()
{
	std::list<Channel*>::iterator it;
	for (it = m_channels.begin(); it != m_channels.end(); it++)
	{
		(*it)->setPaused(false);
	}
}

