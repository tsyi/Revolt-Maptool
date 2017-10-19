#pragma once
#include "cMesh.h"
#include "cPhysXManager.h"
#include "cTransform.h"
#include "cEvent.h"
#include "cMap.h"

class cMeshAnim;
class cTransform;
class cCamera;
class cGrid;
class cMesh;

class cExUnit;
class cCharacter;
//class cUIImage;
class NxActor;
struct _USERDATA;

#define MAINGAME cMainGame::GetInstance()


class cMainGame : public cEvent
{
public:
	SINGLETONE(cMainGame);
private:
	cMeshAnim* meshAnim;
	cCamera* m_camera;

	cGrid* m_grid0;
	cGrid* m_grid1;
	cGrid* m_grid2;

	cMesh* mesh;
	cTransform* tr;

	cCharacter* m_Character;
	cExUnit* m_ExUnit;

//	cUIImage* m_pUIImage;


	//평면
	NxActor*	pPlaneActor;
	USERDATA	pPlaneUserData;
	std::string planeName;

	//박스
	ID3DXMesh* pMeshTest;
	D3DXMATRIXA16 matMeshTest;
	std::string TestName;

	//주전자
	ID3DXMesh* pMeshTeapot;
	cTransform TeapotTr;
	std::string TeapotName;
	NxActor* pTeapotActor;
	USERDATA pTeapotUserData;

//	D3DLIGHT9 stLight;


	cMesh* pMeshMapTest;
	std::string strMapName;

	cMesh* pMeshCarTest;
	std::string strCarName;
	cTransform carTr;
	NxActor* pCarActor;

public:
	void Setup();
	void SetupUI();
	
	void Destory();
	void Update();
	void Render();

	


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:	//EVENT
	virtual void OnClick(cUIButton* pSender, eEventTag eventTag);
	virtual void OnCilck(void* pvoid);

	void OnCreateObject(int eventID);
};

