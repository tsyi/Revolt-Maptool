#pragma once
#include "cTransform.h"
#include "cPhysXManager.h"
#include "cMesh.h"
#include "cPhysX.h"
//#include "cEvent.h"

enum eOBJECT_TAG
{
	E_OBJECT_CHECKBOX,
	E_OBJECT_FOLLOWPOINT,
	E_OBJECT_CAR,
	E_OBJECT_MAP,
	E_OBJECT_LIGHT,
	E_OBJECT_STUFF,
	E_OBJECT_CAMERA,
	E_OBJECT_PICKUP,
	E_OBJECT_END,
	E_OBJECT_NONE,
};

enum eOBJECT_STATE
{
	E_OBJECT_STATE_NONE,
	E_OBJECT_STATE_SELECT,
	E_OBJECT_STATE_CANSLE,
	E_OBJECT_STATE_DELETE,
};

enum eOBJECT_ID
{
	E_OBJ_ID_CEHCKBOX,
	E_OBJ_ID_FOLLOWPOINT,
	E_OBJ_ID_STUFF_CHEESE,
	E_OBJ_ID_STUFF_CHICKEN,
	E_OBJ_ID_STUFF_FABRIC,
	E_OBJ_ID_STUFF_KIDRIDE,
	E_OBJ_ID_PICKUP,
	E_OBJ_ID_STAR,
};

class cMesh;
class cPhysX;

class cObject : public cTransform
{
protected:
	//오직 멥툴에서만 필요한 protected
	//D3DXCOLOR isSelectColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	//D3DXCOLOR unSelectColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	SYNTHESIZE(D3DXCOLOR, isSelectColor, IsSelectColor);
	SYNTHESIZE(D3DXCOLOR, unSelectColor, UnSelectColor);
	SYNTHESIZE(eOBJECT_STATE, m_state, State);
	SYNTHESIZE(USERDATA*, m_pMapData, MapData);
	SYNTHESIZE(float, m_heigth, Heigth);
	SYNTHESIZE(D3DXVECTOR3, m_mouseDistance, MouseDistance);
	SYNTHESIZE(std::string, objName, ObjName);

protected:
	SYNTHESIZE_VIRTUAL(eOBJECT_TAG, m_objTag, Tag);

	SYNTHESIZE_VIRTUAL(cMesh*, m_pMeshData, MeshData);
	SYNTHESIZE_VIRTUAL(cPhysX*, m_PhysXData, PhysXData);

	SYNTHESIZE_VIRTUAL(bool, m_isActor, IsActor);
protected:

	SYNTHESIZE_VIRTUAL(eOBJECT_ID, m_eID, ID);
public:
	cObject();
	~cObject();

	virtual void Setup();
	virtual void Destory();
	virtual void Update();
	virtual void LastUpdate();
	virtual void Render();

	virtual void SetActor()
	{
		//		MgrPhysX->CreateActor()
	}

	D3DXCOLOR colorMesh = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	virtual void SetMeshBox()
	{
		if (!GetMeshData())
		{
			SetMeshData(new cMesh);
			if (GetTag() == E_OBJECT_CHECKBOX)
			{
				colorMesh = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				D3DXCreateBox(MgrD3DDevice, 1, 1, 1, &(GetMeshData()->m_pMesh), NULL);
			}
			else if (GetTag() == E_OBJECT_FOLLOWPOINT)
			{
				colorMesh = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				D3DXCreateSphere(MgrD3DDevice, 0.5, 20, 20, &(GetMeshData()->m_pMesh), NULL);
			}
			else if (GetTag() == E_OBJECT_CAMERA)
			{
				colorMesh = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				D3DXCreateSphere(MgrD3DDevice, 0.1, 20, 20, &(GetMeshData()->m_pMesh), NULL);
			}
			
			D3DMATERIAL9 material;
			ZeroMemory(&material, sizeof(D3DMATERIAL9));
			D3DXCOLOR materColor = colorMesh;
			material.Ambient = materColor;
			material.Diffuse = materColor;
			material.Specular = materColor;

			cMtlTex* mtlText = new cMtlTex;

			mtlText->SetMaterial(material);
			mtlText->SetMtlTexID(0);

			GetMeshData()->m_vecMtlTex.push_back(mtlText);
		}
	}
};

