// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
//#pragma comment (linker)
//�ܼ�
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// Windows ��� ����:
#define NOMINMAX	//<windows.h>�� PhysX������ <nxmath.h> ���� max �� min�� �ߺ����� ������̱⿡ ������ �߻��Ѵ�.
#ifdef max			//�̸� ���� {#define NOMINMAX ~ } �� �̿��� �ߺ������ �����ش�. 
#undef max		//
#endif min			//
#undef min		//
#include <windows.h>


// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <assert.h>
#include <iostream>

#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "PhysXLoader.lib")

extern HWND g_hWnd;


//DefineValue
#define APIWidth 1300
#define APIHeight 800



#define BACKCOLOR D3DCOLOR_XRGB(47, 121, 112)




















//DefineFunction
#define SAFE_RELEASE(p) { if(p) {p->Release() ; p = NULL;}}
#define SAFE_DELETE(p) { if(p) {delete p ; p = NULL; }}
#define SAFE_DELETE_ARRAY(p)  { if(p) {delete[] p ; p = NULL; }}
#define SAFE_RELEASE_VECTOR(data){\
	if (!data.empty())	{\
		for each(auto p in data) p->Release();\
		data.clear();\
	}\
}

#define SINGLETONE(class_name) \
	private : \
		class_name(void); \
		~class_name(void); \
	public: \
		static class_name* GetInstance() \
		{ \
			static class_name instance; \
			return &instance; \
		}

//
#define Lerp(q0, q1 ,dt)  ((dt)*((q1) - (q0)) + (q0))


struct ST_P_VERTEX
{
	D3DXVECTOR3  p;
	enum { FVF = D3DFVF_XYZ };
};
struct ST_PC_VERTEX
{
	D3DXVECTOR3  p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ST_PN_VERTEX
{
	D3DXVECTOR3  p;
	D3DXVECTOR3  n;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3  p;
	D3DXVECTOR3  n;
	D3DXVECTOR2  t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3  p;
	D3DXVECTOR2  t;
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
};

struct ST_RHWC_VERTEX
{
	D3DXVECTOR4  p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
};

struct ST_POS_SAMPLE
{
	int n;
	D3DXVECTOR3 v;
	ST_POS_SAMPLE() :n(0), v(0, 0, 0) {}
};

struct ST_ROT_SAMPLE
{
	int n;
	D3DXQUATERNION q;
	ST_ROT_SAMPLE() :n(0) {
		D3DXQuaternionIdentity(&q);
	}
};

struct ST_SIZE
{
	int nWidth;
	int nHeight;
	ST_SIZE() :nWidth(0), nHeight(0) {}
	ST_SIZE(D3DXVECTOR2 vec2) { nWidth = vec2.x; nHeight = vec2.y; }
	ST_SIZE(int width, int height) :nWidth(width), nHeight(height) {}

};


//Mesh
struct ST_SPHERE
{
	D3DXVECTOR3 vCenter;
	float		fRaidus;
	bool isSelect;
	ST_SPHERE::ST_SPHERE() {}
	ST_SPHERE::ST_SPHERE(D3DXVECTOR3 center, float raidus)
	{
		vCenter = center;
		fRaidus = raidus;
		isSelect = false;
	}
};
struct ST_RAYCAST
{
	D3DXVECTOR3  p;
	D3DCOLOR	c;
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

#define SYNTHESIZE( varType , varName , funName) \
protected: varType varName ; \
public : inline varType Get##funName(void) const { return varName ; } \
public : inline void Set##funName(varType var) { varName = var ; }

#define SYNTHESIZE_PASS_BY_REF( varType , varName , funName) \
protected: varType varName ; \
public : inline varType& Get##funName(void)  { return varName ; } \
public : inline void Set##funName(varType& var) { varName = var ; }

#define SAFE_ADD_REF(p) { if(p) p->AddRef() ; }

#define SYNTHESIZE_ADD_REF( varType , varName, funName) \
	protected : varType  varName ; \
	public : virtual varType Get##funName(void) const { \
									return varName ; } \
	public : virtual void Set##funName(varType var) { \
				if( varName != var ) { \
					SAFE_ADD_REF(var) ; \
					SAFE_RELEASE(varName) ; \
					varName = var ; \
				} \
		}


enum ButtonState { None, Overlap, Down, Up };




//
//GameHeader
#include "cMainGame.h"
#include "cDeviceManager.h"

#include "cObject.h"
#include "cObjectManager.h"
#include "cTextureManager.h"
#include "cFontManager.h"
#include "cUIManager.h"

#include "cSoundManager.h"
#include "cInputManager.h"
#include "cTimeManager.h"

#include "cPhysXManager.h"

//Util
#include "cStringUtil.h"




