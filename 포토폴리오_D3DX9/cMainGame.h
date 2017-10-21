#pragma once
#include "cEvent.h"

class cCamera;
class cScene;
class cGrid;
class cObject;
class cStuff;

#define MAINGAME cMainGame::GetInstance()

class cMainGame : public cEvent
{
public:
	SINGLETONE(cMainGame);
private:
	cCamera* m_camera;

	cGrid* m_grid0;
	cGrid* m_grid1;
	cGrid* m_grid2;

	cScene* m_pScene;

//	//���
//	NxActor*	pPlaneActor;
//	USERDATA	pPlaneUserData;
//	std::string planeName;
//
//	//�ڽ�
//	ID3DXMesh* pMeshTest;
//	D3DXMATRIXA16 matMeshTest;
//	USERDATA pMeshTextUserData;
//	std::string TestName;
//
//	//������
//	ID3DXMesh* pMeshTeapot;
//	cTransform TeapotTr;
//	std::string TeapotName;
//	NxActor* pTeapotActor;
//	USERDATA pTeapotUserData;
//
//	D3DLIGHT9 stLight;
//
//
//	cMesh* pMeshMapTest;
//	std::string strMapName;
//
//	cMesh* pMeshCarTest;
//	std::string strCarName;
//	cTransform carTr;
//	NxActor* pCarActor;
//	
//	Ray
//	NxRay worldRay;


	// ���� �θ������ �ڽ� ������Ʈ;
	cStuff* pBoxObj_0;
	cStuff* pBoxObj_1;
	cStuff* pBoxObj_2;
	cStuff* pBoxObj_3;
	cStuff* pBoxObj_4;



public:
	void Setup();
	void SetUI();
	void SetLight();

	void Destory();
	void Update();
	void Render();


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:	//EVENT
	virtual void OnClick(cUIButton* pSender, eEventTag eventTag);
	virtual void OnCilck(void* pvoid);

	void OnCreateObject(int eventID);
};

