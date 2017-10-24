#include "stdafx.h"
#include "cObjectManager.h"
#include "cObjLoader.h"
#include "cScene.h"
#include "cStuff.h"

cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{

}

void cObjectManager::Setup()
{
	m_vecObjectKey.push_back("check_box");
	m_vecObjectKey.push_back("follow_point");

	m_vecObjectKey.push_back("cheese");
	m_vecObjectKey.push_back("chicken");
	m_vecObjectKey.push_back("fabric");
	m_vecObjectKey.push_back("kidride");

	for (int i = 0; i < m_vecObjectKey.size(); i++)
	{
		CreateButton(m_vecObjectKey[i]);
	}
}

void cObjectManager::AddObj(cScene* addScene, int keyID)
{
	//cObject* pObject = CreateObject(m_vecObjectKey[keyID]);
	cObject* pObject = CreateObject(keyID);
	if (pObject)
	{
		addScene->PushObject(pObject);
	}
}

void cObjectManager::DeleteObj()
{

}
//
//cMesh * cObjectManager::LoadObj(eOBJ_TAG eObj)
//{
//
//	return NULL;
//}



void cObjectManager::Destroy()
{

}

void cObjectManager::Render()
{

}


//cObject* cObjectManager::CreateObject(std::string objectName)
//{
//	std::string strFolder;
//	std::string strFileName;
//
//	strFolder = "Object/Stuffs/" + objectName;
//	strFileName = objectName + ".obj";
//
//	cObject* pObject = new cObject;
//	pObject->SetObjName(objectName);
//	pObject->SetTag(E_OBJECT_STUFF);
//
//	cMesh*	mesh = new cMesh; // �Ž� ����
//	mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
//
//
//	cPhysX*	physX = new cPhysX;
//	if (objectName == "default_Sphere")
//	{
//		physX->m_sizeValue = NxVec3(0.5, 0, 0);
//		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_SPHERE, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
//			physX->m_pUserData, true, true, false);
//
//		//trigger / ���� / ���߷� ������ ��ü ����
//	}
//	else if (objectName == "default_Box")
//	{
//		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
//		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
//			physX->m_pUserData, true, true, false);
//		//trigger / ���� / ���߷� ������ ��ü ����
//	}
//	else
//	{
//		physX->LoadPhysX(objectName);
//		//������ ����Ǿ��ִ� ������ ����
//	}
//
//	if (physX)	//��������, ���� ������ ������Ʈ���� ���� ������ �ʼ�
//	{
//		pObject->SetMeshData(mesh);
//		pObject->SetPhysXData(physX);
//	}
//	else pObject = NULL;
//
//	return pObject;
//}

cObject * cObjectManager::CreateObject(int keyID)
{
	std::string strFolder;
	std::string strFileName;

	strFolder = "Object/Stuffs/" + m_vecObjectKey[keyID];
	strFileName = m_vecObjectKey[keyID] + ".obj";

	cObject* pObject = new cObject;
	pObject->SetObjName(m_vecObjectKey[keyID]);
	pObject->SetID((eOBJECT_ID)keyID);

	cMesh*	mesh;
	cPhysX*	physX = new cPhysX;

	switch (keyID)
	{
	case 0:
	{
		mesh = NULL;
		pObject->SetTag(E_OBJECT_CHECKBOX);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		//trigger / ���� / ���߷� ������ ��ü ����
	}
	break;
	case 1:
	{
		mesh = NULL;
		pObject->SetTag(E_OBJECT_FOLLOWPOINT);
		physX->m_sizeValue = NxVec3(0.5, 0, 0);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_SPHERE, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		//trigger / ���� / ���߷� ������ ��ü ����
	}
	break;
	case 2:
	{
		mesh = new cMesh; // �Ž� ����
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_STUFF);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, false, false, true);
	}
	break;
	case 3:
	{
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�
		pObject->SetTag(E_OBJECT_STUFF);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, false, false, true);
	}
	break;
	case 4:
	{
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_STUFF);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, false, false, true);
	}
	break;
	case 5:
	{
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_STUFF);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, false, false, true);
	}
	break;
	default:
	{
		mesh = NULL;
		pObject->SetTag(E_OBJECT_NONE);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, false, false, true);
	}
	break;
	}

	if (physX)	//��������, ���� ������ ������Ʈ���� ���� ������ �ʼ�
	{
		pObject->SetMeshData(mesh);
		pObject->SetPhysXData(physX);
	}
	else
	{
		MessageBoxA(g_hWnd, "physx �� null", "", MB_OK);
	}

	return pObject;
}

void cObjectManager::CreateButton(std::string objectName)
{
	// �������� ��ư ����
	cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
	if (ui)
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(200, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, objectName, "Image/UI_BUTTON.png");
		ui->AddButton(pButton);
	}
}
