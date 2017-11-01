#include "stdafx.h"
#include "cObjectManager.h"
#include "cObjLoader.h"
#include "cScene.h"

#include "cMap.h"
#include "cLight.h"
#include "cStuff.h"
#include "cCheckBox.h"
#include "cFollowPoint.h"
#include "cPickUp.h"


cObjectManager::cObjectManager()
{
}

cObjectManager::~cObjectManager()
{

}

void cObjectManager::Setup()
{
	m_vecObjectKey.push_back("Check_Box");
	m_vecObjectKey.push_back("Follow_Point");

	m_vecObjectKey.push_back("cheese");
	m_vecObjectKey.push_back("chicken");
	m_vecObjectKey.push_back("fabric");
	m_vecObjectKey.push_back("kidride");
	m_vecObjectKey.push_back("pickup");
	m_vecObjectKey.push_back("star");
	m_vecObjectKey.push_back("cone");
	
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

	cObject* pObject = NULL;
	cMesh*	mesh = NULL;
	cPhysX*	physX = new cPhysX;

	switch (keyID)
	{
	case 0:
	{
		pObject = new cCheckBox;

		mesh = NULL;
		pObject->SetTag(E_OBJECT_CHECKBOX);
		pObject->SetPosition(*m_vecCamTarget);
		
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;
		
		//pObject->GetPhysXData()->SetLocalPosition(*m_vecCamTarget);
		//pObject->GetPhysXData()->SetPosition(*m_vecCamTarget);

		//trigger / ���� / ���߷� ������ ��ü ����
	}
	break;
	case 1:
	{
		pObject = new cFollowPoint;
		mesh = NULL;
		pObject->SetTag(E_OBJECT_FOLLOWPOINT);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		//trigger / ���� / ���߷� ������ ��ü ����
		physX->m_type = NX_SHAPE_SPHERE;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;
	}
	break;
	case 2:
	{
		pObject = new cStuff;
		mesh = new cMesh; // �Ž� ����
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_STUFF);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
	}
	break;
	case 3:
	{
		pObject = new cStuff;
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�
		pObject->SetTag(E_OBJECT_STUFF);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
	}
	break;
	case 4:
	{
		pObject = new cStuff;
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_STUFF);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
	}
	break;
	case 5:
	{
		pObject = new cStuff;
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_STUFF);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
	}
	break;
	case 6:
	{
		pObject = new cPickUp;
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_PICKUP);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		//trigger / ���� / ���߷� ������ ��ü ����
		physX->m_type = NX_SHAPE_SPHERE;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;
	}
	break;
	case 7:
	{
		pObject = new cPickUp;
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_PICKUP);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		//trigger / ���� / ���߷� ������ ��ü ����
		physX->m_type = NX_SHAPE_SPHERE;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;
	}
	break;
	case 8:
	{
		pObject = new cLight;
		mesh = new cMesh; // �Ž� ����	
		mesh->LoadMesh(strFolder, strFileName); // ������Ʈ �ҷ��ͼ� �Ž��� �ִ´�.
		pObject->SetTag(E_OBJECT_LIGHT);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		//trigger / ���� / ���߷� ������ ��ü ����
		physX->m_type = NX_SHAPE_SPHERE;
		physX->m_isTrigger = true;
		physX->m_isStatic_ = true;
		physX->m_isGravity = false;
	}
	break;
	default:
	{
		pObject = new cStuff;
		mesh = NULL;
		pObject->SetTag(E_OBJECT_NONE);
		pObject->SetPosition(*m_vecCamTarget);
		physX->m_sizeValue = INITSIZE;
		NxVec3 campos = { m_vecCamTarget->x,m_vecCamTarget->y,m_vecCamTarget->z };
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, campos, NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		physX->m_worldPose.t = campos;
		physX->m_type = NX_SHAPE_BOX;
		physX->m_isTrigger = false;
		physX->m_isStatic_ = false;
		physX->m_isGravity = true;
	}
	break;
	}

	if (pObject)
	{
		pObject->SetObjName(m_vecObjectKey[keyID]);
		pObject->SetID((eOBJECT_ID)keyID);


		if (physX)	//��������, ���� ������ ������Ʈ���� ���� ������ �ʼ�
		{
			pObject->SetMeshData(mesh);
			pObject->SetPhysXData(physX);
		}
		else
		{
			MessageBoxA(g_hWnd, "physx �� null", "�ɰ��� ����", MB_OK);
		}
	}
	else
	{
		MessageBoxA(g_hWnd, "object �� null", "�ɰ��� ����", MB_OK);
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
