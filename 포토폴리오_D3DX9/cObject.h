#pragma once
#include "cTransform.h"
#include "cPhysXManager.h"
#include "cMesh.h"

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
	eOBJECT_STATE m_state;
	USERDATA* m_pMapData;
	float y;

protected:
	eOBJECT_TAG m_objTag;
	cMesh* m_pMesh;

	NxActor* m_pActor;
	USERDATA* m_physxUserData;
	bool m_isActor;
public:
	cObject();
	~cObject();

	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void SetMeshBox() {}

	virtual USERDATA* GetUserData();
	virtual void SetMapUuerData(USERDATA* pMapData);
	virtual void SetActor(NxActor* pActor);
};

