#pragma once
#include "cTransform.h"
#include "cPhysXManager.h"
#include "cMesh.h"
#include "cPhysX.h"
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
class cPhysX;

class cObject : public cTransform
{
protected:
	//오직 멥툴에서만 필요한 protected
	D3DXCOLOR isSelectColor = D3DXCOLOR(0.3f, 1.0f, 0.8f, 1.0f);
	D3DXCOLOR unSelectColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	SYNTHESIZE(eOBJECT_STATE, m_state, State);
	SYNTHESIZE(USERDATA*, m_pMapData, MapData);
	SYNTHESIZE(float, m_heigth, Heigth);
	SYNTHESIZE(D3DXVECTOR3, m_mouseDistance, MouseDistance);
	SYNTHESIZE(std::string, objName, Name)

protected:
	SYNTHESIZE_VIRTUAL(eOBJECT_TAG, m_objTag, Tag);

	SYNTHESIZE_VIRTUAL(cMesh*, m_pMeshData, MeshData);
	SYNTHESIZE_VIRTUAL(cPhysX*, m_PhysXData, PhysXData);

	SYNTHESIZE_VIRTUAL(bool, m_isActor, IsActor);
protected:

	SYNTHESIZE_VIRTUAL(int, m_nAttribute, Attribute);
public:
	cObject();
	~cObject();

	virtual void Setup();
	virtual void Destory();
	virtual void Update();
	virtual void LastUpdate();
	virtual void Render();
	virtual void SetMeshBox() {}


	virtual void SetActor()
	{
//		MgrPhysX->CreateActor()
	}
};

