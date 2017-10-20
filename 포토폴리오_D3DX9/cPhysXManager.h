#pragma once
#include <NxPhysics.h>
#include <NxCooking.h>
#include "Stream.h"


struct USERDATA
{
	NxU32 ContactPairFlag;
	NX_BOOL RatcastHit;
};

class ContactCallBack : public NxUserContactReport
{
	//충돌확인
	void onContactNotify(NxContactPair& pair, NxU32 _event)
	{
		USERDATA* pUserData0 = NULL;
		USERDATA* pUserData1 = NULL;
		switch (_event)
		{
		case NX_NOTIFY_ON_START_TOUCH:
		{
			pUserData0 = (USERDATA*)pair.actors[0]->userData;
			pUserData1 = (USERDATA*)pair.actors[1]->userData;

			pUserData0->ContactPairFlag = NX_NOTIFY_ON_START_TOUCH;
			pUserData1->ContactPairFlag = NX_NOTIFY_ON_START_TOUCH;

			std::cout << "NX_NOTIFY_ON_START_TOUCH" << std::endl;

		}break;
		case NX_NOTIFY_ON_END_TOUCH:
		{
			pUserData0 = (USERDATA*)pair.actors[0]->userData;
			pUserData1 = (USERDATA*)pair.actors[1]->userData;

			pUserData0->ContactPairFlag = 0;
			pUserData1->ContactPairFlag = 0;

			std::cout << "NX_NOTIFY_ON_END_TOUCH" << std::endl;

		}break;
		}
	}
};

class RaycastCallBack : public NxUserRaycastReport
{
	virtual bool onHit(const NxRaycastHit& hit)
	{
		NxActor& actor = hit.shape->getActor();
		if (actor.userData)
		{
			USERDATA* userData = (USERDATA*)actor.userData;
			userData->RatcastHit = NX_TRUE;
		}
		return true;
	}
};

enum E_MATERIAL
{
	E_MATERIAL_DEFAULT = 0,
	E_MATERIAL_1,
	E_MATERIAL_2,
	E_MATERIAL_3,
	E_MATERIAL_4,
	E_MATERIAL_5,
};

class DEBUG_RENDERER;

#define MgrPhysX cPhysXManager::GetInstance()
#define MgrPhysXScene cPhysXManager::GetInstance()->GetPhysXScene()
#define MgrPhysXSDK   cPhysXManager::GetInstance()->GetPhysXSDK()


class cPhysXManager
{
public:
	SINGLETONE(cPhysXManager);

private:
	NxPhysicsSDK*	m_pNxPhysicsSDK;
	NxScene*		m_pNxScene;

	std::map<E_MATERIAL, NxMaterial*> m_mapMaterial;
public:
	NxPhysicsSDK* GetPhysXSDK() { return m_pNxPhysicsSDK; }
	NxScene* GetPhysXScene() { return m_pNxScene; }
	BOOL InitNxPhysX(DEBUG_RENDERER** pDebugRenderer);
	void Destory();

	NxTriangleMeshShapeDesc CreateTringleMesh(ID3DXMesh* pMesh, D3DXMATRIXA16* matS = NULL);


	void Update();

	void D3DVecToNxVec(NxVec3& vec, D3DXVECTOR3& Dvec3)
	{
		vec.x = Dvec3.x;
		vec.y = Dvec3.y;
		vec.z = Dvec3.z;
	}
	void D3DMatToNxMat(NxF32* mtl, D3DMATRIX* Dmat)
	{
		mtl[0] = Dmat->_11;
		mtl[1] = Dmat->_12;
		mtl[2] = Dmat->_13;
		mtl[3] = Dmat->_21;
		mtl[4] = Dmat->_22;
		mtl[5] = Dmat->_23;
		mtl[6] = Dmat->_31;
		mtl[7] = Dmat->_32;
		mtl[8] = Dmat->_33;
	}
};