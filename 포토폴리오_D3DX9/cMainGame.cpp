#include "stdafx.h"
#include "cMainGame.h"

#include "cMeshAnim.h"

#include "cUIText.h"
#include "cUIImage.h"
#include "cUITextBox.h"
#include "cUIListView.h"


#include  "cGrid.h" 
#include "cTransform.h"
#include "cCamera.h"
#include "cMesh.h"

#include "cExUnit.h"
#include "cCharacter.h"

#include "DEBUG_RENDERER.h"


//#include "cPhysXManager.h"


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

		//���� ����
		NxMaterial* defaultMaterial = MgrPhysXScene->getMaterialFromIndex(0);
		defaultMaterial->setRestitution(0.98f);	  //�ݹ߰��
		defaultMaterial->setStaticFriction(0.5f); //����������
		defaultMaterial->setDynamicFriction(0.5f);//�������

		NxMaterialDesc materialDesc;
		materialDesc.setToDefault();
		materialDesc.restitution = 0.5f;		//-> ������ ������ �ݹ߰�� "1.0f"���� �����̷а����� �� ���� Ƥ��.. ���... // -> "0.985f" ���ݾ� ������...
		materialDesc.staticFriction = 0.5;
		materialDesc.dynamicFriction = 0.5;

		MgrPhysXScene->createMaterial(materialDesc);//������ 1�� ���� ���������� ����(0���� ����Ʈ ��)


		NxMaterialDesc materialDescWheel;
		materialDescWheel.setToDefault();
		materialDescWheel.restitution = 0.1f;		//-> ������ ������ �ݹ߰�� "1.0f"���� �����̷а����� �� ���� Ƥ��.. ���... // -> "0.985f" ���ݾ� ������...
		materialDescWheel.staticFriction = 0.0f;
		materialDescWheel.dynamicFriction = 0.0f;

		MgrPhysXScene->createMaterial(materialDescWheel);//������ 1�� ���� ���������� ����(0���� ����Ʈ ��)

		//��� ����
		pPlaneActor = NULL;
		planeName = "�ٴ�";
		{
			//���� �ʱ�ȭ
			actorDesc.setToDefault();

			//��� ����
			NxPlaneShapeDesc planeDesc;
			planeDesc.setToDefault();
			planeDesc.d = -10.0f;
			planeDesc.materialIndex = 0;

			//���Ϳ� ����� �߰�
			actorDesc.shapes.pushBack(&planeDesc);
			actorDesc.name = planeName.c_str();

			//�浹 �˻縦 ���� userData �߰�
			pPlaneUserData.ContactPairFlag = 0;
			pPlaneUserData.RatcastHit = NX_FALSE;
			actorDesc.userData = &pPlaneUserData;

			//���� ���͸� ����/�߰�
			pPlaneActor = MgrPhysXScene->createActor(actorDesc);
		}
		//�� ����
		{
			//���� �ʱ�ȭ
			actorDesc.setToDefault();

			//�� ����
			NxSphereShapeDesc sphereDesc;
			sphereDesc.setToDefault();
			sphereDesc.radius = 2.0f;
			sphereDesc.materialIndex = 0;	//(�ش� ��ȣ�� ������ ������ ����Ʈ��(0��) ��� 

			//�ٵ� ����  -> �������� ������ ������ ��ä�� �ȴ�.(�ٸ� ��ü�� �̿��� �о �� ����. �Ϲ�������, ������ ��� �������� �ʴ´�.)
			NxBodyDesc bodyDesc;
			bodyDesc.setToDefault();

			//���Ϳ� ����� �߰�
			actorDesc.shapes.pushBack(&sphereDesc);
			actorDesc.body = &bodyDesc;	//�ٵ����� ���
			actorDesc.density = 0.01f;	//��������

			actorDesc.globalPose.t = NxVec3(-0.0f, 20.f, 0.0f);

			//���� ���͸� ����/�߰�
			MgrPhysXScene->createActor(actorDesc);
		}
		//�ڽ� ����
		D3DXCreateBox(g_pD3DDevice, 1, 1, 2, &pMeshTest, NULL);
		TestName = std::string("�ڽ�");
		{
			actorDesc.setToDefault();

			NxBoxShapeDesc boxDesc;
			boxDesc.setToDefault();
			boxDesc.dimensions = NxVec3(0.5f, 0.5f, 1.0f); //�� ���� ���ݸ�ŭ�� ũ��.
			boxDesc.materialIndex = 1;

			NxBodyDesc bodyDesc;
			bodyDesc.setToDefault();

			actorDesc.shapes.pushBack(&boxDesc);
			actorDesc.body = &bodyDesc;	//�ٵ����� ���
			actorDesc.density = 0.01f;	//�е�����
			actorDesc.globalPose.t = NxVec3(0.0f, 10.f, 3.0f);

			//ȸ��
			float r = 10.f;
			//	D3DXMATRIXA16
			NxF32 mtl[] = { cosf(D3DXToRadian(r)), sin(D3DXToRadian(r)),0.0f,
							-sinf(D3DXToRadian(r)), cos(D3DXToRadian(r)),0.0f,
							0.0f,0.0f,1.0f };
			actorDesc.globalPose.M.setColumnMajor(mtl);
			actorDesc.name = TestName.c_str();

			pMeshTextUserData.RatcastHit = NX_FALSE;
			actorDesc.userData = &pMeshTextUserData;

			MgrPhysXScene->createActor(actorDesc);
		}
		//Mesh ������ �̿��� ������ ���� (TriangleMwsh) //�����̳� �����ѸŽ��� �������� �˰� ���� ��� -> ����� ���̵�� ���� ����
		D3DXCreateTeapot(g_pD3DDevice, &pMeshTeapot, NULL);
		TeapotTr.SetPosition(D3DXVECTOR3(1, 2, 0));
		TeapotTr.SetSize(D3DXVECTOR3(10.0f, 10.f, 10.f));
		TeapotTr.SetQuaternionToVector(D3DXVECTOR3(1, 2, 0), true, true);
		pTeapotActor = NULL;
		TeapotName = std::string("Ƽ��Ʈ");
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
			pTeapotUserData.RatcastHit = NX_FALSE;
			actorDesc.userData = &pTeapotUserData;

			pTeapotActor = MgrPhysXScene->createActor(actorDesc);
		}
		actorDesc.setToDefault();

		MgrPhysXScene->setUserContactReport(new ContactCallBack);
		MgrPhysXScene->setActorPairFlags(*pPlaneActor, *pTeapotActor, NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_END_TOUCH);

		//���
		// �޽� �غ�, ���� �غ�
		//{
		//	actorDesc ���� ���� �ʱ�ȭ
		//	shapeDesc �޽� ���� �ʱ�ȭ �� ����
		//		localPose
		//		materialIndex
		//		�׿� ��Ÿ���
		//	bodyDesc �ٵ� ���� �ʱ�ȭ �� ����
		//	userData �̺�Ʈ ���� ���
		//	actorDesc ���� ���� ����
		//		density (�е�)
		//		shapeDesc ��� (�ʼ�)
		//		bodyDesc ���	(�̵�Ͻ�(NULL) ������ ��ü)
		//		userData ���	(�̺�Ʈ ȣ���Լ�)
		//		�׿� ��Ÿ���
		//	MgrPhysXScene �� actorDesc ��� �� ��Ʈ���� ���� pActor ��ȯ
		//}

	}// PhysX_

	MgrObject->Setup();
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


	MgrSound->Setup();
	MgrSound->LoadSound("sound", "sound01.mp3", true);
	MgrSound->LoadSound("sound", "sound02.wav", false);
	//	MgrSound->Play("sound01.mp3", 1.f);	//����� ���


	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
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


	//
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		UITextBox->SetPosition(0, 0, 0);
		UITextBox->SetSize(400, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "MapName", "Image/UI_TEXTBOX.png");

		MgrUI->RegisteredUI(UITextBox);
	}
	//
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_OBJNAME);
		UITextBox->SetPosition(0, 30, 0);
		UITextBox->SetSize(400, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "ObjName", "Image/UI_TEXTBOX.png");

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	//POS
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_POS_X);
		UITextBox->SetPosition(50, 100, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("POS_X");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_POS_Y);
		UITextBox->SetPosition(50, 130, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");


		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("POS_Y");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);


		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_POS_Z);
		UITextBox->SetPosition(50, 160, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");


		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("POS_Z");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->SetPosition(50, 200, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("SizeX");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_SIZE_Y);
		UITextBox->SetPosition(50, 230, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");

		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("SizeY");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_SIZE_Z);
		UITextBox->SetPosition(50, 260, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");


		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("SizeZ");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
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
		UITextBox->SetPosition(50, 300, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");


		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("RotX");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);


		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_ROT_Y);
		UITextBox->SetPosition(50, 330, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");


		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("RotY");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}
	{
		cUITextBox* UITextBox = new cUITextBox;
		cUIText* UITextBox_TextV = new cUIText;
		cUIImage* UITextBox_Image = new cUIImage;

		UITextBox->SetTag(eUITag::E_UI_TEXTBOX_ROT_Z);
		UITextBox->SetPosition(50, 360, 0);
		UITextBox->SetSize(350, 30);
		UITextBox->RegistTextBoxUI(UITextBox_TextV, UITextBox_Image, "0", "Image/UI_TEXTBOX.png");


		cUIText* UITextBox_TextT = new cUIText;
		UITextBox_TextT->SetPosition(D3DXVECTOR3(-50, 0, 0));
		UITextBox_TextT->SetSize(50, 30);
		UITextBox_TextT->SetText("RotZ");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);
		UITextBox->AddChild(UITextBox_TextT);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_MAPNAME);
		ui->AddChild(UITextBox);
	}

	//List
	{
		cUIText* UITextBox_TextT = new cUIText;

		UITextBox_TextT->SetTag(eUITag::E_UI_OBJLIST_TITLE);
		UITextBox_TextT->SetPosition(0, 500, 0);
		UITextBox_TextT->SetSize(400, 30);
		UITextBox_TextT->SetText("������Ʈ ����Ʈ");
		UITextBox_TextT->GetUIText()->color = D3DCOLOR_XRGB(0, 0, 0);

		MgrUI->RegisteredUI(UITextBox_TextT);
	}
	{
		cUIListView* UIListView = new cUIListView;
		cUIImage* pBackgroundImage = new cUIImage;
		pBackgroundImage->SetTexture("Image/UI_TEXTBOX.png");
		UIListView->SetTag(eUITag::E_UI_OBJLIST_VIEW);
		UIListView->SetPosition(0, 30, 0);
		UIListView->SetSize(400, 400);
		UIListView->SetBackgroundUI(pBackgroundImage);
		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_TITLE);
		ui->AddChild(UIListView);
	}
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(400, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, "button1", "Image/UI_TEXTBOX.png");
		pButton->SetOnClick(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));
		pButton->SetEventID(eOBJ_TAG::OBJ_NONE);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
		ui->AddButton(pButton);
	}
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(400, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, "button2", "Image/UI_TEXTBOX.png");
		pButton->SetOnClick(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));
		pButton->SetEventID(eOBJ_TAG::OBJ_CHEESE);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
		ui->AddButton(pButton);
	}
	{
		cUIButton* pButton = new cUIButton;
		cUIImage* pButtonImage = new cUIImage;
		cUIText* pbuttonText = new cUIText;

		pButton->SetTag(eUITag::E_UI_OBJLIST_BUTTONS);
		pButton->SetSize(400, 40);
		pButton->RegistButtonUI(pbuttonText, pButtonImage, "button3", "Image/UI_TEXTBOX.png");
		pButton->SetOnClick(std::bind(&cMainGame::OnCreateObject, this, std::placeholders::_1));
		pButton->SetEventID(eOBJ_TAG::OBJ_CHICKEN);

		cUIObject* ui = MgrUI->FindByTag(eUITag::E_UI_OBJLIST_VIEW);
		ui->AddButton(pButton);
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

	//������ ������Ʈ=============================================
	MgrTime->Update();		//�ð� ���
	MgrInput->Update();		//Ű���� �Է°� ���콺 �Է��� �˻��Ѵ�.
	MgrInput->SetHooking(false);

	//������Ʈ====================================================

	//�±׸� �̿��� �޴������� ȣ���ϱ�
//	cUIObject* pText = MgrUI->FindByTag(eUITag::E_UI_TEXT1);
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
	//������ �������� �̿��ؼ� ����� �����ϴ� �κ�
	{//->> PhysSimul



		//	ContactCallBack raadfg;

//		RaycastCallBack raycastCallback;
//		MgrPhysXScene->raycastAllShapes(worldRay, raycastCallback, NX_ALL_SHAPES);


		if (MgrInput->IsKeyPress('O'))
		{
			MgrPhysX->D3DVecToNxVec(worldRay.orig, m_camera->GetPosition());
			MgrPhysX->D3DVecToNxVec(worldRay.dir, MgrInput->MousePosToViewDir(m_camera));

			NxRaycastHit raycastHit;
			MgrPhysXScene->raycastClosestShape(worldRay, NX_ALL_SHAPES, raycastHit, 0xffffffff, NX_MAX_F32, 0xffffffff, NULL, NULL);

			NxVec3 ratcastHitPosition = raycastHit.worldImpact;
			TeapotTr.SetPosition(ratcastHitPosition);

			std::cout << worldRay.dir.x << "\t";
			std::cout << worldRay.dir.y << "\t";
			std::cout << worldRay.dir.z << std::endl;
		}

		//ratcastHitPosition = NxVec3(2, 2, 0);
		//NxMat34 pose = pCarActor->getGlobalPose();
		//std::cout << pose.t.x << std::endl;
		//std::cout << pose.t.y << std::endl;
		//std::cout << pose.t.z << std::endl;
		//
		//pose.t.x = 2;
		//pose.t.y = 2;
		//pose.t.z = 2;
		//pCarActor->setGlobalPose(pose);

		//NxVec3 ratcastHitPosition = raycastHit.worldImpact;
		//ratcastHitPosition = NxVec3(2, 2, 0);
		//
		//pCarActor->setGlobalPosition(ratcastHitPosition);

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

					//		TeapotTr.SetQuaternion(matWorld);
					//		TeapotTr.SetPosition(matWorld);
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



	//�ļ��� ������Ʈ=============================================
	MgrUI->Update();
	MgrSound->Update();

	{
		NxVec3 ratcastHitPosition = NxVec3(2, 2, 0);
		NxMat34 pose = pCarActor->getGlobalPose();
		//std::cout << pose.t.x << std::endl;
		//std::cout << pose.t.y << std::endl;
		//std::cout << pose.t.z << std::endl;

		pose.t.x = 2;
		pose.t.y = 2;
		pose.t.z = 2;
		pCarActor->setGlobalPose(pose);
	}

	//	MgrPhysXScene->simulate(MgrTime->GetElapsedTime());	//������ ����
	//	MgrPhysXScene->flushStream();
	//	MgrPhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	//	{
	//		NxVec3 ratcastHitPosition = NxVec3(2, 2, 0);
	//		NxMat34 pose = pCarActor->getGlobalPose();
	//		std::cout << pose.t.x << std::endl;
	//		std::cout << pose.t.y << std::endl;
	//		std::cout << pose.t.z << std::endl;
	//
	//		pose.t.x = 2;
	//		pose.t.y = 2;
	//		pose.t.z = 2;
	//		pCarActor->setGlobalPose(pose);
	//	}

		//	raycastClosestShape
	if (m_camera) m_camera->Update(carTr.GetPosition());
}

void cMainGame::Render()
{
	//"g_pD3DDevice->" �� "MgrD3DDevice->" ���ε� ���� ����, , ,
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


	//TeapotTr.Render();

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







	MgrObject->Render();

	//UI
	MgrUI->Render();
	//===========EndRender==========
	MgrD3DDevice->EndScene();
	MgrD3DDevice->Present(NULL, NULL, NULL, NULL);
}

#define WND_PROC WndProc(hWnd, message, wParam, lParam)
LRESULT cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MgrInput->WND_PROC;	//��ó���� ���� ȣ��

	return 0;
}

void cMainGame::OnClick(cUIButton * pSender, eEventTag eventTag)
{
	//CreateCar

//	eOBJ_TAG tag = pSender->GetObjTag();
//	MgrObject->AddObj(tag);
}

void cMainGame::OnCilck(void * pvoid)
{

}

void cMainGame::OnCreateObject(int eventID)
{
	if (eOBJ_TAG::OBJ_MAX <= eventID) eventID = 0;
	MgrObject->AddObj((eOBJ_TAG)eventID);
}
