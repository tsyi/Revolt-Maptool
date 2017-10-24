#include "stdafx.h"
#include "cObjectManager.h"
#include "cObjLoader.h"
#include "cScene.h"


cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{

}

void cObjectManager::Setup()
{
	m_vecObjectKey.push_back("default_Box");
	m_vecObjectKey.push_back("default_Sphere");

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
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;

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
		physX->m_type = NX_SHAPE_SPHERE;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;
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
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
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
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
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
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
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
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
	}
	break;
	default:
	{
		mesh = NULL;
		pObject->SetTag(E_OBJECT_NONE);
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, false, false, true);
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
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
