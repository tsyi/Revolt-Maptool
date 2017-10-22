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

	//�� �������� �ε�
	if (GetMesh()->m_pMesh)	//���� Ư���� ���ڰ��� �ְ� ���� �ʴ´�. (���� �ִٸ� ������ ����)
	{
		NxActorDesc actorDesc;	actorDesc.setToDefault();
		NxBodyDesc  bodyDesc;	bodyDesc.setToDefault();

		bodyDesc.flags |= NX_BF_KINEMATIC;

		NxTriangleMeshShapeDesc shapeDesc = MgrPhysX->CreateTringleMesh(GetMesh()->m_pMesh);
		shapeDesc.materialIndex = 0; // ���� : 0 (default)��
		shapeDesc.localPose.t = NxVec3(0, 0, 0);

		actorDesc.shapes.pushBack(&shapeDesc);
		actorDesc.body = &bodyDesc;
		actorDesc.name = loadName.c_str();

		actorDesc.globalPose.t = NxVec3(0, 0, 0);

		MgrPhysXScene->createActor(actorDesc);
		cObject::SetActor(NULL);
		//���� ���� ������ �ְ� ���� �ʱ� ������  SetActor(NULL) �̴�.
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
		} // << : while ����
	}

	Load.close();
	
	return E_NOTIMPL;
}



