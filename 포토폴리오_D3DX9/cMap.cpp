#include "stdafx.h"
#include "cMap.h"
#include "cObjLoader.h"
#include <fstream>

cMap::cMap()
{
}

cMap::~cMap()
{
}

void cMap::SetupPhyX()
{
	if (GetMeshData()->m_pMesh)	//���� Ư���� ���ڰ��� �ְ� ���� �ʴ´�. (���� �ִٸ� ������ ����)
	{
		cPhysX* physx = new cPhysX;
		physx->m_pUserData = new USERDATA;
		NxActorDesc actorDesc;	actorDesc.setToDefault();
		NxBodyDesc  bodyDesc;	bodyDesc.setToDefault();

		//bodyDesc.flags |= NX_BF_KINEMATIC;

		NxTriangleMeshShapeDesc shapeDesc = MgrPhysX->CreateTringleMesh(GetMeshData()->m_pMesh);
		shapeDesc.materialIndex = 0; // ���� : 0 (default)��
		shapeDesc.localPose.t = NxVec3(0, 0, 0);

		actorDesc.shapes.pushBack(&shapeDesc);
		//actorDesc.body = &bodyDesc;

		actorDesc.globalPose.t = NxVec3(0, 0, 0);
		actorDesc.userData = (physx->m_pUserData);
		
		physx->m_pActor = MgrPhysXScene->createActor(actorDesc);
		
		cObject::SetPhysXData(physx);
		//���� ���� ������ �ְ� ���� �ʱ� ������  SetActor(NULL) �̴�.
	}
}

void cMap::Destroy()
{

}

void cMap::Update()
{
	//���� ��������Ʈ �ϳ�??
}

void cMap::Render()
{
	cObject::Render();
}