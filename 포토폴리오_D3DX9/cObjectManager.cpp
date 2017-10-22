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
	cObject* pObject = CreateObject(m_vecObjectKey[keyID]);
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


cObject* cObjectManager::CreateObject(std::string objectName)
{
	std::string strFolder;
	std::string strFileName;

	strFolder = "Object/Objects/" + objectName;
	strFileName = objectName + ".obj";

	cObject*	pObject = new cObject;
	cMesh*		mesh = new cMesh; // 매쉬 생성
	if (objectName == "default_Box" || objectName == "default_Sphere")
	{
		mesh->m_pMesh = NULL;
		mesh->m_vecMtlTex.clear();
		delete mesh;
		mesh = NULL;
	}
	else
	{
		mesh->LoadMesh(strFolder, strFileName); // 오브젝트 불러와서 매쉬에 넣는다.
	}
	cPhysX*		physX = new cPhysX;
	if (objectName == "default_Sphere")
	{
		physX->m_sizeValue = NxVec3(0.5, 0, 0);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_SPHERE, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData,	true, true, false);

		//trigger / 정적 / 무중력 상태의 객체 생성
	}
	else if (objectName == "default_Box")
	{
		physX->m_sizeValue = NxVec3(0.5, 0.5, 0.5);
		physX->m_pActor = MgrPhysX->CreateActor(NX_SHAPE_BOX, NxVec3(0, 0, 0), NULL, physX->m_sizeValue,
			physX->m_pUserData, true, true, false);
		//trigger / 정적 / 무중력 상태의 객체 생성
	}
	else
	{
		physX->LoadPhysX(strFileName);
		//기존에 저장되어있는 정보로 생성
	}

	pObject->SetMeshData(mesh);
	pObject->SetPhysXData(physX);

	return pObject;
}

void cObjectManager::CreateButton(std::string objectName)
{
	// 동적으로 버튼 생성
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
