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
	if (GetMeshData()->m_pMesh)	//맵은 특별한 인자값을 주고 받지 않는다. (굳이 있다면 재질값 정도)
	{
		cPhysX* physx = new cPhysX;
		physx->m_pUserData = new USERDATA;
		NxActorDesc actorDesc;	actorDesc.setToDefault();
		NxBodyDesc  bodyDesc;	bodyDesc.setToDefault();

		//bodyDesc.flags |= NX_BF_KINEMATIC;

		NxTriangleMeshShapeDesc shapeDesc = MgrPhysX->CreateTringleMesh(GetMeshData()->m_pMesh);
		shapeDesc.materialIndex = 0; // 재질 : 0 (default)값
		shapeDesc.localPose.t = NxVec3(0, 0, 0);

		actorDesc.shapes.pushBack(&shapeDesc);
		//actorDesc.body = &bodyDesc;

		actorDesc.globalPose.t = NxVec3(0, 0, 0);
		actorDesc.userData = (physx->m_pUserData);
		
		physx->m_pActor = MgrPhysXScene->createActor(actorDesc);
		
		cObject::SetPhysXData(physx);
		//맵의 물리 정보를 주고 받지 않기 때문에  SetActor(NULL) 이다.
	}
}

void cMap::Destroy()
{

}

void cMap::Update()
{
	//맵은 뭘업데이트 하나??
}

void cMap::Render()
{
	cObject::Render();
}