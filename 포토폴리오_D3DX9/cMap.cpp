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

void cMap::Destroy()
{
//	for each(cMesh* mesh in m_vecObj)
//	{
//		mesh->Destory();
//	}
//	m_vecObj.clear();
//
//	m_mapCamera.clear();
//	m_mapCheckBox.clear();
//	m_mapFollowPoint.clear();
}

HRESULT cMap::MapLoad(std::string loadName)
{
	std::string folder = "Object/Maps/" + std::string(loadName);
	std::string objName = loadName + std::string(".obj");
	std::string rvlName = loadName + std::string(".rvl");
	GetMesh()->LoadMesh(folder, objName);

	//맵 물리정보 로드
	if (GetMesh()->m_pMesh)	//맵은 특별한 인자값을 주고 받지 않는다. (굳이 있다면 재질값 정도)
	{
		NxActorDesc actorDesc;	actorDesc.setToDefault();
		NxBodyDesc  bodyDesc;	bodyDesc.setToDefault();

		bodyDesc.flags |= NX_BF_KINEMATIC;

		NxTriangleMeshShapeDesc shapeDesc = MgrPhysX->CreateTringleMesh(GetMesh()->m_pMesh);
		shapeDesc.materialIndex = 0; // 재질 : 0 (default)값
		shapeDesc.localPose.t = NxVec3(0, 0, 0);

		actorDesc.shapes.pushBack(&shapeDesc);
		actorDesc.body = &bodyDesc;
		actorDesc.name = loadName.c_str();

		actorDesc.globalPose.t = NxVec3(0, 0, 0);

		MgrPhysXScene->createActor(actorDesc);
		cObject::SetActor(NULL);
		//맵의 물리 정보를 주고 받지 않기 때문에  SetActor(NULL) 이다.
	}
	return E_NOTIMPL;
}

HRESULT cMap::MapSave(std::string rvlName)
{
	return E_NOTIMPL;
}

HRESULT cMap::PhxLoad(std::string phxName, cMesh * dest)
{
	std::fstream Load;
	Load.open(phxName);

	char szTemp[1024];

	if (Load.is_open())
	{
		while (1)
		{
			if (Load.eof()) break;

			Load.getline(szTemp, 1024);
			if (szTemp[0] == ' ' || szTemp[0] == '\t') continue;
			if (szTemp[0] == 'A')
			{
				
			}
			else if (szTemp[0] == 'M')
			{

			}
			else if (szTemp[0] == 'B')
			{

			}
			else if (szTemp[0] == 'S')
			{

			}
			else if (szTemp[0] == 'U')
			{

			}
		} // << : while 파일
	}

	Load.close();
	
	return E_NOTIMPL;
}



