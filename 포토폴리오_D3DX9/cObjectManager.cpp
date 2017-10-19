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
	//��� ������Ʈ�� �ҷ��ͼ� ���Ϳ� ��´�.

	//������Ʈ ��ȣ������ ������� �ҷ��ͼ� �Ž�����Ʈ �����.
	std::string strFolder;
	std::string strFileName;

	for (int i = 0; i < OBJ_MAX; i++)
	{
		strFolder = "Object/Objects/" + strObjName[i];
		strFileName = strObjName[i] + ".obj";

		cMesh* mesh = new cMesh; // �Ž� ����
		if (i == 0)
		{
			mesh->m_pMesh = NULL;
		}
		else
		{
			mesh->LoadMeshObjLoder(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		}

		m_vecObjList.push_back(mesh); // �Ž����Ϳ� �־��ش�.
	}
}

void cObjectManager::AddObj(eOBJ_TAG eObj)
{

	std::string strFolder;
	std::string strFileName;


	strFolder = "Object/Objects/" + strObjName[eObj];
	strFileName = strObjName[eObj] + ".obj";

	cMesh* mesh = new cMesh; // �Ž� ����
	if (eObj == 0)
	{
		mesh->m_pMesh = NULL;
	}
	else
	{
		mesh->LoadMeshObjLoder(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
	}

	m_vecCreatedObj.push_back(mesh); // �Ž����Ϳ� �־��ش�.
	
	//// �Ž� ����

	//cMesh* mesh = new cMesh(m_vecObjList[eObj]);

	//m_vecCreatedObj.push_back(mesh);
	//std::cout << m_vecCreatedObj.size() << std::endl;
}

void cObjectManager::DeleteObj()
{
	for (int i = 0; i < m_vecCreatedObj.size(); i++)
	{
		if (m_vecCreatedObj[i]->GetState() == MESH_STATE_SELET)
		{
			m_vecCreatedObj[i]->Destory();
			SAFE_DELETE(m_vecCreatedObj[i]);

			m_vecCreatedObj.erase(m_vecCreatedObj.begin() + i);
		}
	}
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
	for each(auto p in m_vecCreatedObj)
	{
		p->Destory();
		SAFE_DELETE(p);
	}
	m_vecCreatedObj.clear();

	for each(auto p in m_vecObjList)
	{
		p->Destory();
		SAFE_DELETE(p);
	}
	m_vecObjList.clear();
}

void cObjectManager::Render()
{
	for each(cMesh* pMesh in m_vecCreatedObj)
	{
	//	pMesh->SetPosition(D3DXVECTOR3(0, 1, 0));
		pMesh->Render();
	}

}
