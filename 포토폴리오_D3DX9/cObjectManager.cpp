#include "stdafx.h"
#include "cObjectManager.h"
#include "cObjLoader.h"


cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{
}

void cObjectManager::Setup()
{
	//모든 오브젝트를 불러와서 벡터에 담는다.

	//오브젝트 번호에따라 순서대로 불러와서 매쉬리스트 만든다.

}

void cObjectManager::AddObj(eOBJ_TAG eObj)
{
//	std::string strFolder;
//	std::string strFileName;
//
//	strFolder = "Object/Objects/" + strObjName[eObj];
//	strFileName = strObjName[eObj] + ".obj";
//
//	cMesh* mesh = new cMesh; // 매쉬 생성
//	if (eObj == 0)
//	{
//		mesh->m_pMesh = NULL;
//		mesh->m_vecMtlTex.clear();
//	}
//	else
//	{
//		mesh->LoadMesh(strFolder, strFileName); // 오브젝트 불러와서 매쉬에 넣는다.
//	}
//
//	m_vecCreatedObj.push_back(mesh); // 매쉬벡터에 넣어준다.
//	
//	std::cout << m_vecCreatedObj.size() << std::endl; // 벡터 사이즈 출력
}

void cObjectManager::DeleteObj()
{
	//for (int i = 0; i < m_vecCreatedObj.size(); i++)
	//{
	//	if (m_vecCreatedObj[i]->GetState() == MESH_STATE_SELET)
	//	{
	//		m_vecCreatedObj[i]->Destory();
	//		SAFE_DELETE(m_vecCreatedObj[i]);
	//
	//		m_vecCreatedObj.erase(m_vecCreatedObj.begin() + i);
	//	}
	//}
}

cMesh * cObjectManager::LoadObj(eOBJ_TAG eObj)
{
//	std::string strFolder;
//	std::string strFileName;
//
//	strFolder = "Object/Objects/" + strObjName[eObj];
//	strFileName = strObjName[eObj] + ".obj";
//
//	cMesh* mesh = new cMesh; // 매쉬 생성
//	if (eObj == 0)
//	{
//		mesh->m_pMesh = NULL;
//		mesh->m_vecMtlTex.clear();
//	}
//	else
//	{
//		mesh->LoadMesh(strFolder, strFileName); // 오브젝트 불러와서 매쉬에 넣는다.
//	}
//	return mesh;;
	return NULL;
}

//void cObjectManager::AddObject(cObject * pObject)
//{
//	
//}
//
//void cObjectManager::RemoveObject(cObject * pObject)
//{
//
//}

void cObjectManager::Destroy()
{
//	for each(auto p in m_vecCreatedObj)
//	{
//		p->Destory();
//		SAFE_DELETE(p);
//	}
//	m_vecCreatedObj.clear();
//
//	for each(auto p in m_vecObjList)
//	{
//		p->Destory();
//		SAFE_DELETE(p);
//	}
//	m_vecObjList.clear();
}

void cObjectManager::Render()
{
//	for each(cMesh* pMesh in m_vecCreatedObj)
//	{
//	//	pMesh->SetPosition(D3DXVECTOR3(0, 1, 0));
//		pMesh->Render();
//	}
}
