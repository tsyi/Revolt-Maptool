#include "stdafx.h"
#include "cMainGame.h"

#include "cGrid.h" 
#include "cCamera.h"
#include "cScene.h"

#include "cObject.h"
#include "cStuff.h"

//#include "cPhysXManager.h"


#include "DEBUG_RENDERER.h"
static DEBUG_RENDERER* m_pDebugRenderer = NULL;

cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{
}

void cMainGame::Setup()
{
	MgrPhysX->InitNxPhysX(&m_pDebugRenderer);

	//	MgrObject->Setup();
	MgrUI->Setup();
	MgrInput->Setup();
	MgrFont->Setup();

	m_camera = new cCamera; m_camera->Setup();

	m_grid0 = new cGrid; m_grid0->Setup(20, 1);
	m_grid1 = new cGrid; m_grid1->Setup(10, 10);
	m_grid2 = new cGrid; m_grid2->Setup(10, 100);

	m_pScene = new cScene; m_pScene->Setup();


	//	MgrSound->Setup();
	//	MgrSound->LoadSound("sound", "sound01.mp3", true);
	//	MgrSound->LoadSound("sound", "sound02.wav", false);
	//	MgrSound->Play("sound01.mp3", 1.f);	//배경음 재생


	// 로드되어있는 재질정보
	NxMaterialDesc defaultMaterial;
	defaultMaterial.setToDefault();
	defaultMaterial.restitution = 0.5f;
	defaultMaterial.staticFriction = 0.5f;
	defaultMaterial.dynamicFriction = 0.5f;
	MgrPhysXScene->createMaterial(defaultMaterial);


	std::cout << std::endl << "0";
	pBoxObj_0 = new cStuff;
	pBoxObj_0->SetPosition(0, 0, 0);
	pBoxObj_0->SetMeshBox();
	pBoxObj_0->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(-6, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_0->GetUserData(),
		false, true, true));	//->

	std::cout << std::endl << "1";
	pBoxObj_1 = new cStuff;
	pBoxObj_1->SetPosition(5, 0, 0);
	pBoxObj_1->SetMeshBox();
	pBoxObj_1->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(-4, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_1->GetUserData(),
		true, false, true));

	std::cout << std::endl << "2";
	pBoxObj_2 = new cStuff;
	pBoxObj_2->SetPosition(10, 0, 0);
	pBoxObj_2->SetMeshBox();
	pBoxObj_2->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(-2, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_2->GetUserData(),
		true, true, false));

	std::cout << std::endl << "3";
	pBoxObj_3 = new cStuff;
	pBoxObj_3->SetPosition(15, 0, 0);
	pBoxObj_3->SetMeshBox();
	pBoxObj_3->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(-0, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_3->GetUserData(),
		true, false, false));

	std::cout << std::endl << "4";
	pBoxObj_4 = new cStuff;
	pBoxObj_4->SetPosition(20, 0, 0);
	pBoxObj_4->SetMeshBox();
	pBoxObj_4->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(2, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_4->GetUserData(),
		false, true, false));	//->

	std::cout << std::endl << "5";
	pBoxObj_5 = new cStuff;
	pBoxObj_5->SetPosition(20, 0, 0);
	pBoxObj_5->SetMeshBox();
	pBoxObj_5->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(4, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_5->GetUserData(),
		false, false, true));

	std::cout << std::endl << "6";
	pBoxObj_6 = new cStuff;
	pBoxObj_6->SetPosition(20, 0, 0);
	pBoxObj_6->SetMeshBox();
	pBoxObj_6->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(6, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_6->GetUserData(),
		false, false, false, false));

	std::cout << std::endl << "7";
	pBoxObj_7 = new cStuff;
	pBoxObj_7->SetPosition(20, 0, 0);
	pBoxObj_7->SetMeshBox();
	pBoxObj_7->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX,
		NxVec3(8, 0, 0), NxVec3(0.5, 0.5, 0.5), pBoxObj_7->GetUserData(),
		true, true, true));	//	->

	m_pScene->PushObject(pBoxObj_0);
	m_pScene->PushObject(pBoxObj_1);
	m_pScene->PushObject(pBoxObj_2);
	m_pScene->PushObject(pBoxObj_3);
	m_pScene->PushObject(pBoxObj_4);
	m_pScene->PushObject(pBoxObj_5);
	m_pScene->PushObject(pBoxObj_6);
	m_pScene->PushObject(pBoxObj_7);


	SetUI();
	SetLight();

	//MgrPhysXScene->setUserTriggerReport(new TriggerCallback());



	//NxShape* nx1 = *m_pScene->GetObjcets()[0]->GetActor()->getShapes();
	//NxShape* nx2 = *m_pScene->GetObjcets()[2]->GetActor()->getShapes();
	//MgrPhysXScene->setShapePairFlags(
	//	*nx1, *nx2,
	//	NX_TRIGGER_ON_ENTER);
}

void cMainGame::SetUI()
{
	cUIImage* UIWindowL = new cUIImage;
	UIWindowL->SetTag(eUITag::E_UI_WINDOW_L);
	UIWindowL->SetTexture("Image/UI_WINDOW_L.png");
	UIWindowL->SetScaleSize(203, 1000);
	UIWindowL->SetPosition(0, 0, 0);
	UIWindowL->SetAnchor(0, 0, 0);
	MgrUI->RegisteredUI(UIWindowL);

	cUIImage* UIWindowR = new cUIImage;
	UIWindowR->SetTag(eUITag::E_UI_WINDOW_R);
	UIWindowR->SetTexture("Image/UI_WINDOW_R.png");
	UIWindowR->SetScaleSize(203, 1000);
	UIWindowR->SetPosition(APIWidth - 15, 0, 0);
	UIWindowR->SetAnchor(1, 0, 0);

	MgrUI->RegisteredUI(UIWindowR);
	//
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;
		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		UITextBox->SetPosition(0, 0, 0);
		UITextBox->SetSize(200, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "MapName", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->IsHookingCheck = false;
		MgrUI->RegisteredUI(UITextBox);
	}
	//
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;
		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_OBJNAME);
		UITextBox->SetPosition(0, 30, 0);
		UITextBox->SetSize(200, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ObjName", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		UITextBox->IsHookingCheck = false;
		ui->AddChild(UITextBox);

	}
	//POS
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox->SetPosition(50, 100, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POS_X");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_POS_Y);
		UITextBox->SetPosition(50, 120, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POS_Y");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);


		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_POS_Z);
		UITextBox->SetPosition(50, 140, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POS_Z");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);


		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	//
	//size
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_SIZE_X);
		UITextBox->SetPosition(50, 180, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SizeX");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_SIZE_Y);
		UITextBox->SetPosition(50, 200, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SizeY");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_SIZE_Z);
		UITextBox->SetPosition(50, 220, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SizeZ");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	//Rot
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_ROT_X);
		UITextBox->SetPosition(50, 260, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("RotX");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);


		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_ROT_Y);
		UITextBox->SetPosition(50, 280, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("RotY");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_ROT_Z);
		UITextBox->SetPosition(50, 300, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, std::placeholders::_1));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("RotZ");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}

	//List
	{
		cUIText* UITextBox_TextT = new cUIText;

		UITextBox_TextT->SetTag(eUITag::E_UI_OBJLIST_TITLE);
		UITextBox_TextT->SetPosition(APIWidth - 215, 0, 0);
		UITextBox_TextT->SetSize(200, 20);
		UITextBox_TextT->SetText("오브젝트 리스트");
		UITextBox_TextT->GetText()->color = D3DCOLOR_XRGB(0, 0, 0);

		MgrUI->RegisteredUI(UITextBox_TextT);
	}
	{
		cUIListView* UIListView = new cUIListView;
		cUIImage* pBackgroundImage = new cUIImage;
		pBackgroundImage->SetTexture("Image/UI_LIST.png");
		UIListView->SetTag(eUITag::E_UI_OBJLIST_VIEW);
		UIListView->SetPosition(0, 30, 0);
		UIListView->SetSize(200, 1000);
		UIListView->SetBackgroundUI(pBackgroundImage);
		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_TITLE);
		ui->AddChild(UIListView);
	}
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(200, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, "button1", "Image/UI_TEXTBOX.png");
		pButton->SetEvent_OnCilck(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));
		pButton->SetEventID(eOBJ_TAG::OBJ_NONE);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
		ui->AddButton(pButton);
	}
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(200, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, "button2", "Image/UI_TEXTBOX.png");
		pButton->SetEvent_OnCilck(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));
		pButton->SetEventID(eOBJ_TAG::OBJ_CHEESE);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
		ui->AddButton(pButton);
	}
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(200, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, "button3", "Image/UI_TEXTBOX.png");
		pButton->SetEvent_OnCilck(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));
		pButton->SetEventID(eOBJ_TAG::OBJ_CHICKEN);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
		ui->AddButton(pButton);
	}
}
void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::Destory()
{
	m_pScene->Destory();
	SAFE_DELETE(m_grid0);
	SAFE_DELETE(m_grid1);
	SAFE_DELETE(m_grid2);

	//	MgrObject->Destroy();
	MgrFont->Destory();
	MgrTexture->Destroy();
	MgrUI->Destory();
	//
	//
	//
	//
	//	//엔진 종료
	SAFE_DELETE(m_pDebugRenderer);
	MgrPhysX->Destory();
	MgrD3D->Destroy();
}


void cMainGame::Update()
{
	//	return;
		//선순위 업데이트=============================================
	MgrTime->Update();		//시간 계산
	MgrInput->Update();		//키보드 입력과 마우스 입력을 검사한다.
	MgrInput->SetHooking(false);
	MgrUI->OverCheck();
	//업데이트====================================================
	{
		//PhysxData Updata
		MgrPhysXData->Init();
		MgrPhysX->RaycastAllShapes(m_camera->GetPosition(), MgrInput->MousePosToViewDir(m_camera));
		MgrPhysX->RaycastClosestShape(m_camera->GetPosition(), MgrInput->MousePosToViewDir(m_camera));

		//		TriggerCallback triggerCallBack;

				//		MgrPhysXScene->setActorPairFlags()

		m_pScene->Update();

	}
	//후순위 업데이트=============================================
	MgrSound->Update();
	MgrUI->Update();


	MgrPhysXScene->simulate(MgrTime->GetElapsedTime());	//프레임 지정
	MgrPhysXScene->flushStream();
	MgrPhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	m_pScene->LastUpdate();
	if (m_camera) m_camera->Update();
}

void cMainGame::Render()
{
	//	return;
		//"g_pD3DDevice->" 는 "MgrD3DDevice->" 으로도 접근 가능, , ,
	MgrD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);
	MgrD3DDevice->BeginScene();
	//===========Render=============
	//LigthOn
	MgrD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	m_pScene->Render();


	//	MgrObject->Render();


	//LigthOff
	MgrD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	if (m_grid0) m_grid0->Render();
	if (m_grid1) m_grid1->Render();
	if (m_grid2) m_grid2->Render();

	//UI
	MgrUI->Render();
	if (m_pDebugRenderer) m_pDebugRenderer->RenderData(MgrPhysXScene->getDebugRenderable());
	//===========EndRender==========
	MgrD3DDevice->EndScene();
	MgrD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#define WND_PROC WndProc(hWnd, message, wParam, lParam)
LRESULT cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MgrInput->WND_PROC;	//휠처리를 위한 호출

	return 0;
}

void cMainGame::OnCreateObject(int eventID)
{
	cStuff* pBoxObject = new cStuff;
	pBoxObject->SetPosition(0, 4, 0);
	pBoxObject->SetMeshBox();
	//pBoxObject->SetActor(MgrPhysX->CreateActor(NX_SHAPE_BOX, NX_SF_VISUALIZATION, NX_BF_ENERGY_SLEEP_TEST | NX_BF_DISABLE_GRAVITY, pBoxObject->GetUserData(),
	//	NxVec3(1, 1, 1), 0, 1, pBoxObject->GetMatrix(false, true, true)));

	m_pScene->PushObject(pBoxObject);

	//	if (eOBJ_TAG::OBJ_MAX <= eventID) eventID = 0;
	//	MgrObject->AddObj((eOBJ_TAG)eventID);
}
