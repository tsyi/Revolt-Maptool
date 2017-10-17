#pragma once
#include <NxPhysics.h>
#include <NxCooking.h>
#include "Stream.h"


struct USERDATA
{
	NxU32 ContactPairFlag;
	//	USERDATA() { ContactPairFlag = 0; }
};

class ContactCallBack : public NxUserContactReport
{
	//충돌확인
	void onContactNotify(NxContactPair& pair, NxU32 _event);
};


class DEBUG_RENDERER;

enum E_MATERIAL
{
	E_MATERIAL_DEFAULT = 0,
	E_MATERIAL_1,
	E_MATERIAL_2,
	E_MATERIAL_3,
	E_MATERIAL_4,
	E_MATERIAL_5,
};

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
	//	void CreateMaterial();

//	void D3DVecToNxVec(NxVec3& Nvec3, D3DXVECTOR3& Dvec3)
//	{
//	//	Nvec3.x = Dvec3.x;
//	//	Nvec3.y = Dvec3.y;
//	//	Nvec3.z = Dvec3.z;
//	}
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