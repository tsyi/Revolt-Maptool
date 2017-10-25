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

#define DEFAULT_MAPNAME "Market2"



cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{
}

void cMainGame::Setup()
{
	MgrPhysX->InitNxPhysX(&m_pDebugRenderer);

//	MgrPhysX->CreateActor(NX_SHAPE_SPHERE, NxVec3(0, 0, 0), NULL, NxVec3(0.5, 0, 0),
//		NULL, true, true, false);
	m_pScene = new cScene; m_pScene->Setup();

	MgrUI->Setup();
	SetUI();	//OnLoadMap(VK_RETURN); 다음에 실행

	MgrObject->Setup();	//SetUI(); 다음에 실행
	MgrInput->Setup();
	MgrFont->Setup();

	OnLoadMap(VK_RETURN, DEFAULT_MAPNAME); // 최초 디폴트 값을 로드

	m_camera = new cCamera; m_camera->Setup();

	m_grid0 = new cGrid; m_grid0->Setup(20, 1);
	m_grid1 = new cGrid; m_grid1->Setup(10, 10);
	m_grid2 = new cGrid; m_grid2->Setup(10, 100);

	//	m_pScene = new cScene; m_pScene->Setup();
	//
	//	//=========================Test
	//	m_pScene->LoadScene("Market2");
	//	m_pScene->SaveScene("Test");
		//=========================

		//	MgrSound->Setup();
		//	MgrSound->LoadSound("sound", "sound01.mp3", true);
		//	MgrSound->LoadSound("sound", "sound02.wav", false);
		//	MgrSound->Play("sound01.mp3", 1.f);	//배경음 재생


	// 로드되어있는 재질정보
	SetLight();
	
	//trigger 충돌을 위해 필요
	MgrPhysXScene->setUserTriggerReport(new TriggerCallback());
}


#define VAR std::placeholders
void cMainGame::SetUI()
{
	cUIImage* UIWindowL = new cUIImage;
	UIWindowL->SetTag(eUITag::E_UI_WINDOW_L);
	UIWindowL->SetTexture("Image/UI_WINDOW_L.png");
	UIWindowL->SetScaleSize(203, 1000);
	UIWindowL->SetPosition(0, 0, 0);
	UIWindowL->SetAnchor(0, 0, 0);
	UIWindowL->IsHookingCheck = true;
	MgrUI->RegisteredUI(UIWindowL);

	cUIImage* UIWindowR = new cUIImage;
	UIWindowR->SetTag(eUITag::E_UI_WINDOW_R);
	UIWindowR->SetTexture("Image/UI_WINDOW_R.png");
	UIWindowR->SetScaleSize(203, 1000);
	UIWindowR->SetPosition(APIWidth - 15, 0, 0);
	UIWindowR->SetAnchor(1, 0, 0);
	UIWindowR->IsHookingCheck = true;

	MgrUI->RegisteredUI(UIWindowR);

	//
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;


		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		UITextBox->SetPosition(0, 0, 0);
		UITextBox->SetSize(200, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, DEFAULT_MAPNAME, "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cMainGame::OnLoadMap, this, VAR::_1, VAR::_2));

		MgrUI->RegisteredUI(UITextBox);

		cUIButton* UIButton = new cUIButton;
		cUIText* UIButton_TextV = new cUIText;
		cUIImage* UIButton_Image = new cUIImage;

		UIButton->SetTag(eUITag::E_UI_BUTTON_SAVE);
		UIButton->SetPosition(0, 20, 0);
		UIButton->SetSize(200, 20);
		UIButton->RegistButtonUI(UIButton_TextV, UIButton_Image, "SAVE", "Image/UI_BUTTON.png");
		UIButton_TextV->SetFont(eFontType::E_DEFAULT);
		UIButton->SetEvent_OnCilck_Up(std::bind(&cMainGame::OnSaveMap, this, std::placeholders::_1));
		UITextBox->AddChild(UIButton);
	}
	//
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;
		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_OBJNAME);
		UITextBox->SetPosition(0, 60, 0);
		UITextBox->SetSize(200, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ObjName", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);

	}
	{
		cUIText* UIText = new cUIText;
		UIText->SetTag(eUITag::E_UI_TEXT_PHYSX_TITLE);
		UIText->SetPosition(15, 80, 0);
		UIText->SetSize(100, 20);
		UIText->SetText("Transform Data");
		UIText->SetFont(eFontType::E_DEFAULT);
		UIText->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		MgrUI->RegisteredUI(UIText);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1, VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POS_X");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1, VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POS_Y");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1, VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POS_Z");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1, VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SizeX");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1, VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SizeY");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SizeZ");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("RotX");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("RotY");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("RotZ");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}

	//{
	//	cUITextBox* UITextBox = new cUITextBox;
	//	cUIText* UITextBox_TextV = new cUIText;
	//	cUIImage* UITextBox_Image = new cUIImage;
	//
	//	UITextBox->SetTag(eUITag::E_UI_TEXTBOX_ROT_Z);
	//	UITextBox->SetPosition(50, 300, 0);
	//	UITextBox->SetSize(150, 20);
	//	UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
	//	UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
	//	UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1, VAR::_2));
	//
	//	cUIText* UITextBox_TextT = new cUIText;
	//	UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
	//	UITextBox_TextT->SetSize(50, 20);
	//	UITextBox_TextT->SetText("OBECT_ID");
	//	UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
	//	UITextBox->AddChild(UITextBox_TextT);
	//
	//	cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
	//	ui->AddChild(UITextBox);
	//}

	{
	//	cUITextBox* UITextBox = new cUITextBox;
	//	cUIText* UITextBox_TextV = new cUIText;
	//	cUIImage* UITextBox_Image = new cUIImage;
	//
	//	UITextBox->SetTag(eUITag::E_UI_TEXTBOX_PHYSX_KEY);
	//	UITextBox->SetPosition(50, 340, 0);
	//	UITextBox->SetSize(150, 20);
	//	UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "PhsyXKey", "Image/UI_TEXTBOX.png");
	//	UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
	//	UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));
	//
	//	cUIText* UITextBox_TextT = new cUIText;
	//	UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
	//	UITextBox_TextT->SetSize(50, 20);
	//	UITextBox_TextT->SetText("PhsX");
	//	UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
	//	UITextBox->AddChild(UITextBox_TextT);
	//
	//	cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
	//	ui->AddChild(UITextBox);
	}
	//PhysX 관련 UI
	//PhysX 적용버튼
	{
	//	cUIButton* UIButton = new cUIButton;
	//	cUIText* UIButton_TextV = new cUIText;
	//	cUIImage* UIButton_Image = new cUIImage;
	//
	//	MgrUI->RegisteredUI(UIButton);
	//
	//	UIButton->SetTag(eUITag::E_UI_BUTTON_STATIC);
	//	UIButton->SetPosition(0, 360, 0);
	//	UIButton->SetSize(100, 20);
	//	UIButton->RegistButtonUI(UIButton_TextV, UIButton_Image, "적용", "Image/UI_BUTTON.png");
	//	UIButton_TextV->SetFont(eFontType::E_DEFAULT);
	//	UIButton->SetEvent_OnCilck_Up(std::bind(&cScene::OnChangePhysX, m_pScene, std::placeholders::_1));
	//	UIButton->SetEventID(1);
	//}
	//{
	//	cUIButton* UIButton = new cUIButton;
	//	cUIText* UIButton_TextV = new cUIText;
	//	cUIImage* UIButton_Image = new cUIImage;
	//
	//	MgrUI->RegisteredUI(UIButton);
	//
	//	UIButton->SetTag(eUITag::E_UI_BUTTON_STATIC);
	//	UIButton->SetPosition(100, 360, 0);
	//	UIButton->SetSize(100, 20);
	//	UIButton->RegistButtonUI(UIButton_TextV, UIButton_Image, "취소", "Image/UI_BUTTON.png");
	//	UIButton_TextV->SetFont(eFontType::E_DEFAULT);
	//	UIButton->SetEvent_OnCilck_Up(std::bind(&cScene::OnChangePhysX, m_pScene, std::placeholders::_1));
	//	UIButton->SetEventID(0);
		cUIText* UIText= new cUIText;
		UIText->SetTag(eUITag::E_UI_TEXT_PHYSX_TITLE);
		UIText->SetPosition(0, 420, 0);
		UIText->SetSize(100, 20);
		UIText->SetText("PhysX Data");
		UIText->SetFont(eFontType::E_DEFAULT);
		UIText->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		MgrUI->RegisteredUI(UIText);
	}
	
	// NxShapeType
	{ 
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;
		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_SHAPET_TYPE);
		UITextBox->SetPosition(0, 440, 0);
		UITextBox->SetSize(200, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ShapeType", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1, VAR::_2));
		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	//{
	//
	//}
	//// Trigger
	//{
	//	cUIButton* UIButton = new cUIButton;
	//	cUIText* UIButton_TextV = new cUIText;
	//	cUIImage* UIButton_Image = new cUIImage;
	//
	//	UIButton->SetTag(eUITag::E_UI_BUTTON_TRIGGER);
	//	UIButton->SetPosition(0, 400, 0);
	//	UIButton->SetSize(120, 20);
	//	UIButton->RegistButtonUI(UIButton_TextV, UIButton_Image, "TRIGGER", "Image/UI_BUTTON.png");
	//	UIButton_TextV->SetFont(eFontType::E_DEFAULT);
	////	UIButton->SetEvent_OnCilck_Up(std::bind(&cMainGame::OnSaveMap, this, std::placeholders::_1));
	//
	//	MgrUI->RegisteredUI(UIButton);
	//}
	//{ 
	//	cUITextBox* UITextBox = new cUITextBox;
	//	cUIText* UITextBox_TextV = new cUIText;
	//	cUIImage* UITextBox_Image = new cUIImage;
	//	
	//	UITextBox->SetTag(eUITag::E_UI_TEXTBOX_TRIGGER);
	//	UITextBox->SetPosition(130, 400, 0);
	//	UITextBox->SetSize(70, 20);
	//	UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ON", "Image/UI_TEXTBOX.png");
	//	UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
	//	UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));
	//
	//	cUIText* UITextBox_TextT = new cUIText;
	//	UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
	//	UITextBox_TextT->SetSize(50, 20);
	//	UITextBox_TextT->SetText("");
	//	UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
	//	UITextBox->AddChild(UITextBox_TextT);
	//
	//	cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
	//	ui->AddChild(UITextBox);
	//}
	//// Static
	//{
	//	cUIButton* UIButton = new cUIButton;
	//	cUIText* UIButton_TextV = new cUIText;
	//	cUIImage* UIButton_Image = new cUIImage;
	//
	//	MgrUI->RegisteredUI(UIButton);
	//
	//	UIButton->SetTag(eUITag::E_UI_BUTTON_STATIC);
	//	UIButton->SetPosition(0, 420, 0);
	//	UIButton->SetSize(120, 20);
	//	UIButton->RegistButtonUI(UIButton_TextV, UIButton_Image, "STATIC", "Image/UI_BUTTON.png");
	//	UIButton_TextV->SetFont(eFontType::E_DEFAULT);
	//	UIButton->SetEvent_OnCilck_Up(std::bind(&cMainGame::OnSaveMap, this, std::placeholders::_1));
	//}
	//{ 
	//	cUITextBox* UITextBox = new cUITextBox;
	//	cUIText* UITextBox_TextV = new cUIText;
	//	cUIImage* UITextBox_Image = new cUIImage;
	//
	//	UITextBox->SetTag(eUITag::E_UI_TEXTBOX_STATIC);
	//	UITextBox->SetPosition(130, 420, 0);
	//	UITextBox->SetSize(70, 20);
	//	UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ON", "Image/UI_TEXTBOX.png");
	//	UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
	//	UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));
	//
	//	cUIText* UITextBox_TextT = new cUIText;
	//	UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
	//	UITextBox_TextT->SetSize(50, 20);
	//	UITextBox_TextT->SetText("");
	//	UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
	//	UITextBox->AddChild(UITextBox_TextT);
	//
	//	cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
	//	ui->AddChild(UITextBox);
	//}
	//// Gravity
	//{
	//	cUIButton* UIButton = new cUIButton;
	//	cUIText* UIButton_TextV = new cUIText;
	//	cUIImage* UIButton_Image = new cUIImage;
	//
	//	MgrUI->RegisteredUI(UIButton);
	//
	//	UIButton->SetTag(eUITag::E_UI_BUTTON_GRAVITY);
	//	UIButton->SetPosition(0, 440, 0);
	//	UIButton->SetSize(120, 20);
	//	UIButton->RegistButtonUI(UIButton_TextV, UIButton_Image, "GRAVITY", "Image/UI_BUTTON.png");
	//	UIButton_TextV->SetFont(eFontType::E_DEFAULT);
	//	UIButton->SetEvent_OnCilck_Up(std::bind(&cMainGame::OnSaveMap, this, std::placeholders::_1));
	//}
	//{
	//	cUITextBox* UITextBox = new cUITextBox;
	//	cUIText* UITextBox_TextV = new cUIText;
	//	cUIImage* UITextBox_Image = new cUIImage;
	//
	//	UITextBox->SetTag(eUITag::E_UI_TEXTBOX_GRAVITY);
	//	UITextBox->SetPosition(130, 440, 0);
	//	UITextBox->SetSize(70, 20);
	//	UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ON", "Image/UI_TEXTBOX.png");
	//	UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
	//	UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValue, m_pScene, VAR::_1,VAR::_2));
	//
	//	cUIText* UITextBox_TextT = new cUIText;
	//	UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
	//	UITextBox_TextT->SetSize(50, 20);
	//	UITextBox_TextT->SetText("");
	//	UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
	//	UITextBox->AddChild(UITextBox_TextT);
	//
	//	cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
	//	ui->AddChild(UITextBox);
	//}
	// Position
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_POS_X);
		UITextBox->SetPosition(50, 480, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POSX");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_POS_Y);
		UITextBox->SetPosition(50, 500, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POSY");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_POS_Z);
		UITextBox->SetPosition(50, 520, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("POSZ");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}

	// Size
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_SIZE_X);
		UITextBox->SetPosition(50, 560, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SIZEX");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_SIZE_Y);
		UITextBox->SetPosition(50, 580, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SIZEY");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_SIZE_Z);
		UITextBox->SetPosition(50, 600, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("SIZEZ");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}

	// Rotation
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_ROT_X);
		UITextBox->SetPosition(50, 640, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("ROTX");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_ROT_Y);
		UITextBox->SetPosition(50, 660, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("ROTY");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_NX_ROT_Z);
		UITextBox->SetPosition(50, 680, 0);
		UITextBox->SetSize(150, 20);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");
		UITextBox->GetUIText()->SetFont(eFontType::E_TEXTBOX);
		UITextBox->SetEvent_OnEnter(std::bind(&cScene::OnChangeValueNx, m_pScene, VAR::_1,VAR::_2));

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 20);
		UITextBox_TextT->SetText("ROTZ");
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox_TextT->GetTextData()->color = D3DCOLOR_XRGB(0, 0, 0);

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
		UIListView->SetEvent_OnCilck_List(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_TITLE);
		ui->AddChild(UIListView);
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

	SAFE_DELETE(m_camera);
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
	MgrInput->SetHooking(false);	//마우스 
	MgrUI->OverCheck();				//마우스가 UI와 겹쳐졌는지. 체크
	//업데이트====================================================
	{
		//PhysxData Updata
		MgrPhysXData->Init();
		MgrPhysX->RaycastAllShapes(m_camera->GetPosition(), MgrInput->MousePosToViewDir(m_camera));
		MgrPhysX->RaycastClosestShape(m_camera->GetPosition(), MgrInput->MousePosToViewDir(m_camera));


		if (m_pScene) m_pScene->Update();

	}
	//후순위 업데이트=============================================
	MgrSound->Update();
	MgrUI->Update();

	MgrPhysXScene->simulate(MgrTime->GetElapsedTime());	//프레임 지정
	MgrPhysXScene->flushStream();
	MgrPhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	if (m_pScene) m_pScene->LastUpdate();
	if (m_camera) m_camera->Update();
}

void cMainGame::Render()
{
	//	return;
	//"g_pD3DDevice->" 는 "MgrD3DDevice->" 으로도 접근 가능, , ,
	MgrD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(47, 121, 112), 1.0F, 0);
	MgrD3DDevice->BeginScene();
	//===========Render=============
	//LigthOn
	MgrD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	if (m_pScene) m_pScene->Render();

	// Camera
	if (m_camera) m_camera->Render();

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
	MgrObject->AddObj(m_pScene, eventID);
}

void cMainGame::OnLoadMap(int eventID, std::string eventKey)
{
	if (eventID == VK_RETURN)
	{
		if (!m_pScene)
		{
			m_pScene = new cScene; m_pScene->Setup();
			m_pScene->LoadScene(eventKey);
		}
		else
		{
			cUITextBox* pUI = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
			m_pScene->LoadScene(pUI->GetUIText()->GetTextData()->text);
		}
	}
}

void cMainGame::OnSaveMap(int eventID)
{
	if (m_pScene)
	{
		cUITextBox* pUI = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		m_pScene->SaveScene(pUI->GetUIText()->GetTextData()->text);
	}
}


