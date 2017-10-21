#pragma once
#include "cTransform.h"
#include "cPhysXManager.h"
#include "cMesh.h"
//#include "cEvent.h"

enum eOBJECT_TAG
{
	E_OBJECT_NONE,
	E_OBJECT_CAR,
	E_OBJECT_MAP,
	E_OBJECT_LIGHT,
	E_OBJECT_STUFF,
	E_OBJECT_CAMERA,
	E_OBJECT_END,
};

enum eOBJECT_STATE
{
	E_OBJECT_STATE_NONE,
	E_OBJECT_STATE_SELECT,
	E_OBJECT_STATE_CANSLE,
};

class NxActor;
class cMesh;

class cObject : public cTransform
{
	//오직 멥툴에서만 필요.
protected:
	SYNTHESIZE(eOBJECT_STATE, m_state, State);
	SYNTHESIZE(USERDATA*, m_pMapData, MapData);
	SYNTHESIZE(float, m_heigth, Heigth);
	SYNTHESIZE(D3DXVECTOR3, m_mouseDistance, MouseDistance);

protected:
	SYNTHESIZE_VIRTUAL(eOBJECT_TAG, m_objTag, Tag);
	SYNTHESIZE_VIRTUAL(cMesh*, m_pMesh, Mesh);

	SYNTHESIZE_VIRTUAL(NxActor*, m_pActor, Actor);
	SYNTHESIZE_VIRTUAL(USERDATA*, m_physxUserData, UserData);
	SYNTHESIZE_VIRTUAL(bool, m_isActor, IsActor);
public:
	cObject();
	~cObject();

	virtual void Setup();
	virtual void Destory();
	virtual void Update();
	virtual void LastUpdate();
	virtual void Render();
	virtual void SetMeshBox() {}

//	virtual USERDATA* GetUserData();
//	virtual void SetMapUuerData(USERDATA* pMapData);
//	virtual void SetActor(NxActor* pActor);

	//TextBoxEvent

};

