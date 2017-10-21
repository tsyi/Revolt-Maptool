#pragma once

class cCamera;
class cScene;
class cGrid;
class cObject;
class cStuff;

#define MAINGAME cMainGame::GetInstance()

class cMainGame
{
public:
	SINGLETONE(cMainGame);
private:
	cCamera* m_camera;

	cGrid* m_grid0;
	cGrid* m_grid1;
	cGrid* m_grid2;

	cScene* m_pScene;
	
	// 임의 로만들어진 박스 오브젝트;
	cStuff* pBoxObj_0;
	cStuff* pBoxObj_1;
	cStuff* pBoxObj_2;
	cStuff* pBoxObj_3;
	cStuff* pBoxObj_4;
	cStuff* pBoxObj_5;
	cStuff* pBoxObj_6;
	cStuff* pBoxObj_7;



public:
	void Setup();
	void SetUI();
	void SetLight();

	void Destory();
	void Update();
	void Render();


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:	//EVENT
	void OnCreateObject(int eventID);
};

