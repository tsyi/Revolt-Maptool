#pragma once

#define MgrD3D cDeviceManager::GetInstance()
#define g_pD3DDevice cDeviceManager::GetInstance()->GetD3DDevice()
#define MgrD3DDevice g_pD3DDevice

class cDeviceManager
{
public:
	//	cDeviceManager();
	//	~cDeviceManager();
	SINGLETONE(cDeviceManager);

private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

public:
	LPDIRECT3DDEVICE9	GetD3DDevice();
	void Destroy();
	void ex()
	{
		
	}


};

