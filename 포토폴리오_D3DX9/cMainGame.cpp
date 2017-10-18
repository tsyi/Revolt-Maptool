#include "stdafx.h"
#include "cMainGame.h"

#include "cMeshAnim.h"

#include "cUIText.h"
#include "cUIImage.h"
#include "cUITextBox.h"

#include  "cGrid.h" 
#include "cTransform.h"
#include "cCamera.h"
#include "cMesh.h"

#include "cExUnit.h"
#include "cCharacter.h"

#include "DEBUG_RENDERER.h"


static DEBUG_RENDERER* m_pDebugRenderer = NULL;



cMainGame::cMainGame()
	:meshAnim(NULL)
{
}


cMainGame::~cMainGame()
{
}

void cMainGame::Setup()
{
	MgrPhysX->InitNxPhysX(&m_pDebugRenderer);
	{
		NxActorDesc actorDesc;

		//재질 설정
		NxMaterial* defaultMaterial = MgrPhysXScene->getMaterialFromIndex(0);
		defaultMaterial->setRestitution(0.98f);	  //반발계수
		defaultMaterial->setStaticFriction(0.5f); //정지마찰력
		defaultMaterial->setDynamicFriction(0.5f);//운동마찰력

		NxMaterialDesc materialDesc;
		materialDesc.setToDefault();
		materialDesc.restitution = 0.5f;		//-> 이유는 모르지만 반발계수 "1.0f"값은 실제이론값보다 더 높이 튄다.. 곤란... // -> "0.985f" 조금씩 낮아짐...
		materialDesc.staticFriction = 0.5;
		materialDesc.dynamicFriction = 0.5;

		MgrPhysXScene->createMaterial(materialDesc);//재질은 1번 부터 순차적으로 들어간다(0번은 디폴트 값)


		NxMaterialDesc materialDescWheel;
		materialDescWheel.setToDefault();
		materialDescWheel.restitution = 0.1f;		//-> 이유는 모르지만 반발계수 "1.0f"값은 실제이론값보다 더 높이 튄다.. 곤란... // -> "0.985f" 조금씩 낮아짐...
		materialDescWheel.staticFriction = 0.0f;
		materialDescWheel.dynamicFriction = 0.0f;

		MgrPhysXScene->createMaterial(materialDescWheel);//재질은 1번 부터 순차적으로 들어간다(0번은 디폴트 값)

		//평면 생성
		pPlaneActor = NULL;
		planeName = "바닥";
		{
			//액터 초기화
			actorDesc.setToDefault();

			//평면 정보
			NxPlaneShapeDesc planeDesc;
			planeDesc.setToDefault();
			planeDesc.d = -10.0f;
			planeDesc.materialIndex = 0;

			//액터에 평면을 추가
			actorDesc.shapes.pushBack(&planeDesc);
			actorDesc.name = planeName.c_str();

			//충돌 검사를 위한 userData 추가
			pPlaneUserData.ContactPairFlag = 0;
			actorDesc.userData = &pPlaneUserData;

			//씬에 엑터를 생성/추가
			pPlaneActor = MgrPhysXScene->createActor(actorDesc);
		}
		//공 생성
		{
			//액터 초기화
			actorDesc.setToDefault();

			//공 정보
			NxSphereShapeDesc sphereDesc;
			sphereDesc.setToDefault();
			sphereDesc.radius = 2.0f;
			sphereDesc.materialIndex = 0;	//(해당 번호의 재질이 없으면 디폴트값(0번) 사용 

			//바디 정보  -> 설정하지 않으면 정적인 객채가 된다.(다른 객체를 이용해 밀어낼 수 없음. 일반적으로, 지면의 경우 설정하지 않는다.)
			NxBodyDesc bodyDesc;
			bodyDesc.setToDefault();

			//액터에 평면을 추가
			actorDesc.shapes.pushBack(&sphereDesc);
			actorDesc.body = &bodyDesc;	//바디정보 등록
			actorDesc.density = 0.01f;	//질량정보

			actorDesc.globalPose.t = NxVec3(-0.0f, 20.f, 0.0f);

			//씬에 엑터를 생성/추가
			MgrPhysXScene->createActor(actorDesc);
		}
		//박스 생성
		D3DXCreateBox(g_pD3DDevice, 1, 1, 2, &pMeshTest, NULL);
		TestName = std::string("박스");
		{
			actorDesc.setToDefault();

			NxBoxShapeDesc boxDesc;
			boxDesc.setToDefault();
			boxDesc.dimensions = NxVec3(0.5f, 0.5f, 1.0f); //각 변의 절반만큼의 크기.
			boxDesc.materialIndex = 1;

			NxBodyDesc bodyDesc;
			bodyDesc.setToDefault();

			actorDesc.shapes.pushBack(&boxDesc);
			actorDesc.body = &bodyDesc;	//바디정보 등록
			actorDesc.density = 0.01f;	//밀도정보
			actorDesc.globalPose.t = NxVec3(0.0f, 10.f, 3.0f);

			//회전
			float r = 10.f;
			//	D3DXMATRIXA16
			NxF32 mtl[] = { cosf(D3DXToRadian(r)), sin(D3DXToRadian(r)),0.0f,
							-sinf(D3DXToRadian(r)), cos(D3DXToRadian(r)),0.0f,
							0.0f,0.0f,1.0f };
			actorDesc.globalPose.M.setColumnMajor(mtl);
			actorDesc.name = TestName.c_str();

			MgrPhysXScene->createActor(actorDesc);
		}
		//Mesh 정보를 이용해 주전자 생성 (TriangleMwsh) //지형이나 복잡한매쉬의 물리값을 알고 싶을 경우 -> 비용이 많이드니 남발 금지
		D3DXCreateTeapot(g_pD3DDevice, &pMeshTeapot, NULL);
		TeapotTr.SetPosition(D3DXVECTOR3(1, 2, 0));
		TeapotTr.SetSize(D3DXVECTOR3(1.0f, 0.5f, 0.5f));
		TeapotTr.SetQuaternionToVector(D3DXVECTOR3(1, 2, 0), true, true);
		pTeapotActor = NULL;
		TeapotName = std::string("티포트");
		{
			actorDesc.setToDefault();

			NxTriangleMeshShapeDesc shapeDesc = MgrPhysX->CreateTringleMesh(pMeshTeapot, &TeapotTr.GetMatrixScale());
			shapeDesc.localPose.t = NxVec3(0, 0, 0);
			shapeDesc.materialIndex = 2;
			NxBodyDesc bodyDesc;
			bodyDesc.setToDefault();
			bodyDesc.angularDamping = 0.5f;

			actorDesc.shapes.pushBack(&shapeDesc);
			actorDesc.body = &bodyDesc;
			actorDesc.density = 0.5f;


			actorDesc.globalPose.t = NxVec3(TeapotTr.GetPosition().x, TeapotTr.GetPosition().y, TeapotTr.GetPosition().z);
			NxF32 mtl[9];
			D3DXMATRIXA16 matMesh = TeapotTr.GetMatrixWorld();
			mtl[0] = matMesh._11;
			mtl[1] = matMesh._12;
			mtl[2] = matMesh._13;
			mtl[3] = matMesh._21;
			mtl[4] = matMesh._22;
			mtl[5] = matMesh._23;
			mtl[6] = matMesh._31;
			mtl[7] = matMesh._32;
			mtl[8] = matMesh._33;
			actorDesc.globalPose.M.setColumnMajor(mtl);

			actorDesc.name = TeapotName.c_str();

			pTeapotUserData.ContactPairFlag = 0;
			actorDesc.userData = &pTeapotUserData;

			pTeapotActor = MgrPhysXScene->createActor(actorDesc);
		}
		actorDesc.setToDefault();

		MgrPhysXScene->setUserContactReport(new ContactCallBack);
		MgrPhysXScene->setActorPairFlags(*pPlaneActor, *pTeapotActor, NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_END_TOUCH);

		//요약
		// 메쉬 준비, 재질 준비
		//{
		//	actorDesc 엑터 정보 초기화
		//	shapeDesc 메쉬 정보 초기화 및 세팅
		//		localPose
		//		materialIndex
		//		그외 기타등등
		//	bodyDesc 바디 정보 초기화 및 세팅
		//	userData 이벤트 정보 등록
		//	actorDesc 엑터 정보 세팅
		//		density (밀도)
		//		shapeDesc 등록 (필수)
		//		bodyDesc 등록	(미등록시(NULL) 정적인 객체)
		//		userData 등록	(이벤트 호출함수)
		//		그외 기타등등
		//	MgrPhysXScene 에 actorDesc 등록 및 컨트롤을 위한 pActor 반환
		//}

	}// PhysX_


	//SetManager
	MgrUI->Setup();
	SetupUI();

	MgrInput->Setup();
	MgrFont->Setup();

	m_grid0 = new cGrid;
	if (m_grid0) m_grid0->Setup(20, 1);
	m_grid1 = new cGrid;
	if (m_grid1) m_grid1->Setup(10, 10);
	m_grid2 = new cGrid;
	if (m_grid2) m_grid2->Setup(10, 100);

	//cUIText* bassUI = new cUIText;
	//cUIText* bass2UI = new cUIText;
	//bassUI->SetTag(eUITag::E_UI_TEXT1);
	//bassUI->SetSize(200, 50);
	//bassUI->SetPosition(10, 10, 0);
	//bassUI->CreateTextUI("123", eFontType::E_TITLE, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
	////	MgrUI->CreateTextUI(bassUI, D3DXVECTOR2(10, 10), D3DXVECTOR2(200, 50),0, "TEST_OUT", eFontType::E_TITLE, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
	//
	//MgrUI->RegisteredUI(bassUI);	//UI 생성후 등록하는 과정을 거침...
	//
	//bass2UI->SetSize(200, 50);
	//bass2UI->SetPosition(20, 20);
	//bass2UI->CreateTextUI("ABCD", eFontType::E_QUEST, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
	//
	////등록후 수정가능
	//bassUI->AddChild(bass2UI);
	//bassUI->SetText("ABC");

	MgrSound->Setup();
	MgrSound->LoadSound("sound", "sound01.mp3", true);
	MgrSound->LoadSound("sound", "sound02.wav", false);
	//	MgrSound->Play("sound01.mp3", 1.f);	//배경음 재생

//	cUIImage* pUIImage = new cUIImage();
//	pUIImage->SetTexture("Image/Font.png");
//	pUIImage->SetShow(true);
//	MgrUI->RegisteredUI(pUIImage);


	//xfile 불러오기 (택스쳐안됨)
//	mesh = new cMesh;
//	if (mesh) mesh->LoadMeshXFile("Object/Unit/질럿/", "zealot.X");


	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	//	NxTriangleMeshShapeDesc mapDesc;


	actorDesc.setToDefault();
	bodyDesc.setToDefault();

	//================================================
	pMeshMapTest = new cMesh;
	pMeshMapTest->LoadMeshObjLoder("Object/Maps/Market2", "Market2.obj");
	strMapName = "Market2";

	NxTriangleMeshShapeDesc mapDesc = MgrPhysX->CreateTringleMesh(pMeshMapTest->m_pMesh);
	mapDesc.materialIndex = 1;

	actorDesc.shapes.pushBack(&mapDesc);
	actorDesc.name = strMapName.c_str();

	MgrPhysXScene->createActor(actorDesc);

	actorDesc.setToDefault();
	bodyDesc.setToDefault();
	mapDesc.setToDefault();

	//===============================================
	pMeshCarTest = new cMesh;
	pMeshCarTest->LoadMeshObjLoder("Object/Cars/tc2", "tc2.obj");
	carTr.SetPosition(D3DXVECTOR3(0, 1, 0));
	strCarName = "tc2";

	NxBoxShapeDesc	carDesc;
	carDesc.dimensions = NxVec3(0.5, 0.5, 1);
	carDesc.materialIndex = 1;

	bodyDesc.mass = 1.f;

	actorDesc.shapes.pushBack(&carDesc);
	actorDesc.body = &bodyDesc;
	actorDesc.name = strCarName.c_str();

	MgrPhysX->D3DVecToNxVec(actorDesc.globalPose.t, carTr.GetPosition());
	NxF32 mtl[9];
	MgrPhysX->D3DMatToNxMat(mtl, &carTr.GetMatrixRotation());
	actorDesc.globalPose.M.setColumnMajor(mtl);


	pCarActor = MgrPhysXScene->createActor(actorDesc);

	actorDesc.setToDefault();
	bodyDesc.setToDefault();
	carDesc.setToDefault();

	//============================================
	m_camera = new cCamera();
	if (m_camera) m_camera->Setup();



	//	m_ExUnit = new cExUnit;
	//	if(m_ExUnit)m_ExUnit->Setup();
	//
	//	m_Character = new cCharacter;
	//	if (m_Character) m_Character = m_ExUnit;




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

void cMainGame::SetupUI()
{
	cUIImage* UIWindow = new cUIImage;
	UIWindow->SetTag(eUITag::E_UI_WINDOW);
	UIWindow->SetTexture("Image/UI_WINDOW.png");
	UIWindow->SetScale(1, 2);
	UIWindow->SetPosition(0, 0, 0);
	UIWindow->SetAnchor(0, 0, 0);

	MgrUI->RegisteredUI(UIWindow);



	//PosX
	{
		cUITextBox* UITextBox_PosX = new cUITextBox;
		UITextBox_PosX->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_PosX->SetPosition(100, 30, 0);

		cUIImage* UITextBox_PosX_Image = new cUIImage;
		UITextBox_PosX_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_PosX_Image->SetScale(0.5, 0.05);
		UITextBox_PosX_Image->SetAnchor(0, 0, 0);

		cUIText* UITextBox_PosX_TextV = new cUIText;
		UITextBox_PosX_TextV->SetText("0");
		UITextBox_PosX_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_PosX_TextV->SetSize(UITextBox_PosX_Image->GetScaleSize());
		UITextBox_PosX_TextV->SetPosition(UITextBox_PosX_Image->GetPosition());

		cUIText* UITextBox_PosX_TextT = new cUIText;
		UITextBox_PosX_TextT->SetText("POS_X");
		UITextBox_PosX_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_PosX_TextT->SetSize(UITextBox_PosX_Image->GetScaleSize());
		UITextBox_PosX_TextT->SetPosition(UITextBox_PosX_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));

		UITextBox_PosX->RegistTextBoxUI(UITextBox_PosX_TextV, UITextBox_PosX_Image);
		UITextBox_PosX->AddChild(UITextBox_PosX_TextT);

		MgrUI->RegisteredUI(UITextBox_PosX);
	}
	//POsY
	{
		cUITextBox*	UITextBox_PosY = new cUITextBox;
		cUIImage*	UITextBox_PosY_Image = new cUIImage;
		cUIText*	UITextBox_PosY_TextV = new cUIText;
		cUIText*	UITextBox_PosY_TextT = new cUIText;

		UITextBox_PosY->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_PosY->SetPosition(100, 60, 0);

		UITextBox_PosY_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_PosY_Image->SetScale(0.5, 0.05);
		UITextBox_PosY_Image->SetAnchor(0, 0, 0);

		UITextBox_PosY_TextV->SetText("0");
		UITextBox_PosY_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_PosY_TextV->SetSize(UITextBox_PosY_Image->GetScaleSize());
		UITextBox_PosY_TextV->SetPosition(UITextBox_PosY_Image->GetPosition());

		UITextBox_PosY_TextT->SetText("POS_Y");
		UITextBox_PosY_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_PosY_TextT->SetSize(UITextBox_PosY_Image->GetScaleSize());
		UITextBox_PosY_TextT->SetPosition(UITextBox_PosY_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));

		UITextBox_PosY->RegistTextBoxUI(UITextBox_PosY_TextV, UITextBox_PosY_Image);
		UITextBox_PosY->AddChild(UITextBox_PosY_TextT);

		MgrUI->RegisteredUI(UITextBox_PosY);
	}
	{
		//PosZ
		cUITextBox*	UITextBox_PosZ = new cUITextBox;
		cUIImage*	UITextBox_PosZ_Image = new cUIImage;
		cUIText*	UITextBox_PosZ_TextV = new cUIText;
		cUIText*	UITextBox_PosZ_TextT = new cUIText;

		UITextBox_PosZ->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_PosZ->SetPosition(100, 90, 0);
		UITextBox_PosZ_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_PosZ_Image->SetScale(0.5, 0.05);
		UITextBox_PosZ_Image->SetAnchor(0, 0, 0);
		UITextBox_PosZ_TextV->SetText("0");
		UITextBox_PosZ_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_PosZ_TextV->SetSize(UITextBox_PosZ_Image->GetScaleSize());
		UITextBox_PosZ_TextV->SetPosition(UITextBox_PosZ_Image->GetPosition());
		UITextBox_PosZ_TextT->SetText("POS_Z");
		UITextBox_PosZ_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_PosZ_TextT->SetSize(UITextBox_PosZ_Image->GetScaleSize());
		UITextBox_PosZ_TextT->SetPosition(UITextBox_PosZ_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_PosZ->RegistTextBoxUI(UITextBox_PosZ_TextV, UITextBox_PosZ_Image);
		UITextBox_PosZ->AddChild(UITextBox_PosZ_TextT);

		MgrUI->RegisteredUI(UITextBox_PosZ);
	}
	//Size
	{
		cUITextBox*	UITextBox_SizeX = new cUITextBox;
		cUIImage*	UITextBox_SizeX_Image = new cUIImage;
		cUIText*	UITextBox_SizeX_TextV = new cUIText;
		cUIText*	UITextBox_SizeX_TextT = new cUIText;

		UITextBox_SizeX->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_SizeX->SetPosition(100, 130, 0);
		UITextBox_SizeX_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_SizeX_Image->SetScale(0.5, 0.05);
		UITextBox_SizeX_Image->SetAnchor(0, 0, 0);
		UITextBox_SizeX_TextV->SetText("0");
		UITextBox_SizeX_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_SizeX_TextV->SetSize(UITextBox_SizeX_Image->GetScaleSize());
		UITextBox_SizeX_TextV->SetPosition(UITextBox_SizeX_Image->GetPosition());
		UITextBox_SizeX_TextT->SetText("SIZE_X");
		UITextBox_SizeX_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_SizeX_TextT->SetSize(UITextBox_SizeX_Image->GetScaleSize());
		UITextBox_SizeX_TextT->SetPosition(UITextBox_SizeX_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_SizeX->RegistTextBoxUI(UITextBox_SizeX_TextV, UITextBox_SizeX_Image);
		UITextBox_SizeX->AddChild(UITextBox_SizeX_TextT);

		MgrUI->RegisteredUI(UITextBox_SizeX);
	}
	{
		cUITextBox*	UITextBox_SizeZ = new cUITextBox;
		cUIImage*	UITextBox_SizeZ_Image = new cUIImage;
		cUIText*	UITextBox_SizeZ_TextV = new cUIText;
		cUIText*	UITextBox_SizeZ_TextT = new cUIText;

		UITextBox_SizeZ->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_SizeZ->SetPosition(100, 160, 0);
		UITextBox_SizeZ_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_SizeZ_Image->SetScale(0.5, 0.05);
		UITextBox_SizeZ_Image->SetAnchor(0, 0, 0);
		UITextBox_SizeZ_TextV->SetText("0");
		UITextBox_SizeZ_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_SizeZ_TextV->SetSize(UITextBox_SizeZ_Image->GetScaleSize());
		UITextBox_SizeZ_TextV->SetPosition(UITextBox_SizeZ_Image->GetPosition());
		UITextBox_SizeZ_TextT->SetText("SIZE_Y");
		UITextBox_SizeZ_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_SizeZ_TextT->SetSize(UITextBox_SizeZ_Image->GetScaleSize());
		UITextBox_SizeZ_TextT->SetPosition(UITextBox_SizeZ_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_SizeZ->RegistTextBoxUI(UITextBox_SizeZ_TextV, UITextBox_SizeZ_Image);
		UITextBox_SizeZ->AddChild(UITextBox_SizeZ_TextT);

		MgrUI->RegisteredUI(UITextBox_SizeZ);
	}
	{
		cUITextBox*	UITextBox_SizeZ = new cUITextBox;
		cUIImage*	UITextBox_SizeZ_Image = new cUIImage;
		cUIText*	UITextBox_SizeZ_TextV = new cUIText;
		cUIText*	UITextBox_SizeZ_TextT = new cUIText;

		UITextBox_SizeZ->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_SizeZ->SetPosition(100, 190, 0);
		UITextBox_SizeZ_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_SizeZ_Image->SetScale(0.5, 0.05);
		UITextBox_SizeZ_Image->SetAnchor(0, 0, 0);
		UITextBox_SizeZ_TextV->SetText("0");
		UITextBox_SizeZ_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_SizeZ_TextV->SetSize(UITextBox_SizeZ_Image->GetScaleSize());
		UITextBox_SizeZ_TextV->SetPosition(UITextBox_SizeZ_Image->GetPosition());
		UITextBox_SizeZ_TextT->SetText("SIZE_Z");
		UITextBox_SizeZ_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_SizeZ_TextT->SetSize(UITextBox_SizeZ_Image->GetScaleSize());
		UITextBox_SizeZ_TextT->SetPosition(UITextBox_SizeZ_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_SizeZ->RegistTextBoxUI(UITextBox_SizeZ_TextV, UITextBox_SizeZ_Image);
		UITextBox_SizeZ->AddChild(UITextBox_SizeZ_TextT);

		MgrUI->RegisteredUI(UITextBox_SizeZ);
	}
	{
		cUITextBox*	UITextBox_RotX = new cUITextBox;
		cUIImage*	UITextBox_RotX_Image = new cUIImage;
		cUIText*	UITextBox_RotX_TextV = new cUIText;
		cUIText*	UITextBox_RotX_TextT = new cUIText;

		UITextBox_RotX->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_RotX->SetPosition(100, 230, 0);
		UITextBox_RotX_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_RotX_Image->SetScale(0.5, 0.05);
		UITextBox_RotX_Image->SetAnchor(0, 0, 0);
		UITextBox_RotX_TextV->SetText("0");
		UITextBox_RotX_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_RotX_TextV->SetSize(UITextBox_RotX_Image->GetScaleSize());
		UITextBox_RotX_TextV->SetPosition(UITextBox_RotX_Image->GetPosition());
		UITextBox_RotX_TextT->SetText("ROT_X");
		UITextBox_RotX_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_RotX_TextT->SetSize(UITextBox_RotX_Image->GetScaleSize());
		UITextBox_RotX_TextT->SetPosition(UITextBox_RotX_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_RotX->RegistTextBoxUI(UITextBox_RotX_TextV, UITextBox_RotX_Image);
		UITextBox_RotX->AddChild(UITextBox_RotX_TextT);

		MgrUI->RegisteredUI(UITextBox_RotX);
	}
	{
		cUITextBox*	UITextBox_RotY = new cUITextBox;
		cUIImage*	UITextBox_RotY_Image = new cUIImage;
		cUIText*	UITextBox_RotY_TextV = new cUIText;
		cUIText*	UITextBox_RotY_TextT = new cUIText;

		UITextBox_RotY->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_RotY->SetPosition(100, 260, 0);
		UITextBox_RotY_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_RotY_Image->SetScale(0.5, 0.05);
		UITextBox_RotY_Image->SetAnchor(0, 0, 0);
		UITextBox_RotY_TextV->SetText("0");
		UITextBox_RotY_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_RotY_TextV->SetSize(UITextBox_RotY_Image->GetScaleSize());
		UITextBox_RotY_TextV->SetPosition(UITextBox_RotY_Image->GetPosition());
		UITextBox_RotY_TextT->SetText("ROT_Y");
		UITextBox_RotY_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_RotY_TextT->SetSize(UITextBox_RotY_Image->GetScaleSize());
		UITextBox_RotY_TextT->SetPosition(UITextBox_RotY_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_RotY->RegistTextBoxUI(UITextBox_RotY_TextV, UITextBox_RotY_Image);
		UITextBox_RotY->AddChild(UITextBox_RotY_TextT);

		MgrUI->RegisteredUI(UITextBox_RotY);
	}
	{
		cUITextBox*	UITextBox_RotZ = new cUITextBox;
		cUIImage*	UITextBox_RotZ_Image = new cUIImage;
		cUIText*	UITextBox_RotZ_TextV = new cUIText;
		cUIText*	UITextBox_RotZ_TextT = new cUIText;

		UITextBox_RotZ->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox_RotZ->SetPosition(100, 290, 0);
		UITextBox_RotZ_Image->SetTexture("Image/UI_TEXTBOX.png");
		UITextBox_RotZ_Image->SetScale(0.5, 0.05);
		UITextBox_RotZ_Image->SetAnchor(0, 0, 0);
		UITextBox_RotZ_TextV->SetText("0");
		UITextBox_RotZ_TextV->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_RotZ_TextV->SetSize(UITextBox_RotZ_Image->GetScaleSize());
		UITextBox_RotZ_TextV->SetPosition(UITextBox_RotZ_Image->GetPosition());
		UITextBox_RotZ_TextT->SetText("ROT_Z");
		UITextBox_RotZ_TextT->SetOption(eFontType::E_DEFAULT, DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
		UITextBox_RotZ_TextT->SetSize(UITextBox_RotZ_Image->GetScaleSize());
		UITextBox_RotZ_TextT->SetPosition(UITextBox_RotZ_Image->GetPosition() + D3DXVECTOR3(-60, 0, 0));
		UITextBox_RotZ->RegistTextBoxUI(UITextBox_RotZ_TextV, UITextBox_RotZ_Image);
		UITextBox_RotZ->AddChild(UITextBox_RotZ_TextT);

		MgrUI->RegisteredUI(UITextBox_RotZ);
	}



}

void cMainGame::Destory()
{
	//	Destory

	//	if(mesh)mesh->Destory();
	//	if (meshAnim) meshAnim->
		//DestoryManager
	//	MgrUI->Destory();
	//	MgrSound->Destory();
	//	MgrFont->Destory();
	//
	//	//Last(Device)
	//	MgrD3D->Destroy();
}

void cMainGame::Update()
{

	//선순위 업데이트=============================================
	MgrTime->Update();		//시간 계산
	MgrInput->Update();		//키보드 입력과 마우스 입력을 검사한다.

	//업데이트====================================================

	//태그를 이용해 메니저에서 호출하기
//	cUIObject* pText = MgrUI->FindChildByTag(eUITag::E_UI_TEXT1);
//	pText->AddPosition(D3DXVECTOR3(1, 0, 0));

	//
	//	MgrSound->Play_BGM("sound01.mp3", 1.f);//, pText->GetPosition());
	//	
	if (MgrInput->IsKeyDown('Q'))
	{
		MgrSound->Play("sound02.wav", 1.0f);
	}

	if (MgrInput->IsKeyOn('W'))
	{
		pCarActor->addLocalForce(NxVec3(0, 0, 100));
		std::cout << "W";
	}
	if (MgrInput->IsKeyOn('S'))
	{
		pCarActor->addLocalForce(NxVec3(0, 0, -100));
		std::cout << "S";
	}
	//지정된 프레임을 이용해서 계산을 진행하는 부분
	{//->> PhysSimul
		MgrPhysXScene->simulate(MgrTime->GetElapsedTime());	//프레임 지정
		MgrPhysXScene->flushStream();
		MgrPhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

		NxU32 ContactPairFlag = 0;
		NxU32 actorCount = MgrPhysXScene->getNbActors();
		for (NxU32 i = 0; i < actorCount; i++)
		{
			NxActor* pActor = MgrPhysXScene->getActors()[i];
			if (pActor == NULL) continue;
			if (!ContactPairFlag)
			{
				USERDATA* UserData = (USERDATA*)pActor->userData;

				if (UserData)
				{
					ContactPairFlag = UserData->ContactPairFlag;
					//	pActor->userData = 0;
				}
				//std::cout << ContactPairFlag << std::endl;
			}

			if (pActor->getName() != NULL)
			{
				if (pActor->getName() == TestName)
				{
					D3DXMatrixIdentity(&matMeshTest);

					matMeshTest._41 = pActor->getGlobalPose().t.x;
					matMeshTest._42 = pActor->getGlobalPose().t.y;
					matMeshTest._43 = pActor->getGlobalPose().t.z;

					NxF32 mtl[3 * 3];
					pActor->getGlobalPose().M.getColumnMajor(mtl);
					matMeshTest._11 = mtl[0];
					matMeshTest._12 = mtl[1];
					matMeshTest._13 = mtl[2];
					matMeshTest._21 = mtl[3];
					matMeshTest._22 = mtl[4];
					matMeshTest._23 = mtl[5];
					matMeshTest._31 = mtl[6];
					matMeshTest._32 = mtl[7];
					matMeshTest._33 = mtl[8];
				}
				if (pActor->getName() == TeapotName)
				{
					D3DXMATRIXA16 matWorld;
					D3DXMatrixIdentity(&matWorld);

					matWorld._41 = pActor->getGlobalPose().t.x;
					matWorld._42 = pActor->getGlobalPose().t.y;
					matWorld._43 = pActor->getGlobalPose().t.z;

					NxF32 mtl[3 * 3];
					pActor->getGlobalPose().M.getColumnMajor(mtl);
					matWorld._11 = mtl[0];
					matWorld._12 = mtl[1];
					matWorld._13 = mtl[2];
					matWorld._21 = mtl[3];
					matWorld._22 = mtl[4];
					matWorld._23 = mtl[5];
					matWorld._31 = mtl[6];
					matWorld._32 = mtl[7];
					matWorld._33 = mtl[8];

					TeapotTr.SetQuaternion(matWorld);
					TeapotTr.SetPosition(matWorld);
				}
				if (pActor->getName() == strCarName)
				{
					D3DXMATRIXA16 matWorld;
					D3DXMatrixIdentity(&matWorld);

					matWorld._41 = pActor->getGlobalPose().t.x;
					matWorld._42 = pActor->getGlobalPose().t.y;
					matWorld._43 = pActor->getGlobalPose().t.z;

					NxF32 mtl[3 * 3];
					pActor->getGlobalPose().M.getColumnMajor(mtl);
					matWorld._11 = mtl[0];
					matWorld._12 = mtl[1];
					matWorld._13 = mtl[2];
					matWorld._21 = mtl[3];
					matWorld._22 = mtl[4];
					matWorld._23 = mtl[5];
					matWorld._31 = mtl[6];
					matWorld._32 = mtl[7];
					matWorld._33 = mtl[8];

					carTr.SetQuaternion(matWorld);
					carTr.SetPosition(matWorld);
				}
			}
		}
		if (ContactPairFlag == NX_NOTIFY_ON_START_TOUCH)
		{
			ContactPairFlag = 0;
		}
	}//->> PhysSimulEnd



	//후순위 업데이트=============================================
	if (m_camera) m_camera->Update(carTr.GetPosition());

	MgrUI->Update();
	MgrSound->Update();
}

void cMainGame::Render()
{
	//"g_pD3DDevice->" 는 "MgrD3DDevice->" 으로도 접근 가능, , ,
	MgrD3DDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);
	MgrD3DDevice->BeginScene();
	//===========Render=============
	//LigthOn
	MgrD3DDevice->SetRenderState(D3DRS_LIGHTING, true);


	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	D3DXCOLOR materColor = D3DXCOLOR(0.3f, 1.0f, 0.8f, 1.0f);
	material.Ambient = materColor;
	material.Diffuse = materColor;
	material.Specular = materColor;

	MgrD3DDevice->SetTransform(D3DTS_WORLD, &matMeshTest);
	MgrD3DDevice->SetMaterial(&material);
	MgrD3DDevice->SetTexture(0, NULL);
	pMeshTest->DrawSubset(0);

	materColor = D3DXCOLOR(1.0f, 3.0f, 0.8f, 1.0f);
	material.Ambient = materColor;
	material.Diffuse = materColor;
	material.Specular = materColor;

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	MgrD3DDevice->SetTransform(D3DTS_WORLD, &TeapotTr.GetMatrixWorld());
	MgrD3DDevice->SetMaterial(&material);
	MgrD3DDevice->SetTexture(0, NULL);
	pMeshTeapot->DrawSubset(0);

	TeapotTr.Render();
	//	//LigthOff
	MgrD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	if (m_grid0) m_grid0->Render();
	if (m_grid1) m_grid1->Render();
	if (m_grid2) m_grid2->Render();

	if (m_pDebugRenderer)
		m_pDebugRenderer->RenderData(MgrPhysXScene->getDebugRenderable());


	pMeshMapTest->Render();

	MgrD3DDevice->SetTransform(D3DTS_WORLD, &carTr.GetMatrixWorld());
	pMeshCarTest->Render();
	//UI
	MgrUI->Render();
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