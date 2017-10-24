#include "stdafx.h"
#include "cScene.h"
#include "cUIText.h"
#include "cMap.h"
#include "cLight.h"
#include "cStuff.h"
#include "cCheckBox.h"
#include <fstream>
#include <time.h>

cScene::cScene()
	: m_pMap(NULL)
{
	m_sceenName = "";
}

cScene::~cScene()
{
}

void cScene::LoadScene(std::string FileName)
{
	if (m_sceenName == FileName)
	{
		//동일한 씬을 로드했을 경우 로드를 무시.
		MessageBoxA(g_hWnd, "현재 열려있는 파일입니다.", "알림", MB_OK);
		return;
	}
	else
	{
		Destory();// 기존에 가지고 있던 정보 버리기
	}

	m_sceenName = FileName;
	std::string fullpath = "Object/Scene/" + FileName + ".scn";

	std::fstream Load;
	Load.open(fullpath);

	char szTemp[1024];

	if (Load.is_open())
	{
		while (1)
		{
			if (Load.eof()) break;

			Load.getline(szTemp, 1024);
			if (szTemp[0] == ' ' || szTemp[0] == '\t') continue;
			else if (szTemp[0] == '/') continue;
			else if (szTemp[0] == 'M') // Map Load 
			{
				char szMapFile[1024];
				sscanf_s(szTemp, "%*s %s", szMapFile, 1024);
				if (m_pMap != NULL)
				{
					m_pMap->Destory();	//기존 지형 지우기
					m_pMap = NULL;
				}
				cMap* newMap = new cMap;
				newMap->SetObjName(szMapFile);
				std::string Folder = "Object/Maps/" + FileName;
				std::string Name = FileName + ".obj";
				newMap->GetMeshData()->LoadMesh(Folder, Name);
				newMap->SetupPhyX();
				m_pMap = newMap;
			}
			else if (szTemp[0] == 'O') //Object Load
			{
				cObject* Obj = NULL;

				int tag;
				Load.getline(szTemp, 1024);
				sscanf_s(szTemp, "%*s %d", &tag);
				
				switch (tag)
				{
				case E_OBJECT_NONE:		break;
				case E_OBJECT_CAR:		break;
				case E_OBJECT_MAP:		break;
				case E_OBJECT_LIGHT:	Obj = new cLight; break;
				case E_OBJECT_STUFF:	Obj = new cStuff; break;
				case E_OBJECT_CAMERA:	break;
				case E_OBJECT_CHECKBOX: Obj = new cCheckBox; break;
				case E_OBJECT_END:		break;
				default: break;
				}

				Obj->SetTag((eOBJECT_TAG)tag);

				cPhysX* physx = new cPhysX;
				Obj->SetPhysXData(physx);

				std::string strName;
				Load >> strName;
				Load >> strName;
				Obj->SetObjName(strName);

				while (1)
				{
					Load.getline(szTemp, 1024);

					//if (szTemp[0] == 'N') // Name
					//{
					//	//char szName[1024];
					//	std::string strName;
					//	Load >> strName;
					//	//sscanf_s(szTemp, "%*s %s", &strName);
					//	Obj->SetObjName(strName);
					//}
					if (szTemp[0] == 'I') // Attribute
					{
						int nID = 0;
						sscanf_s(szTemp, "%*s %d", &nID);
						Obj->SetID((eOBJECT_ID)nID);
						cMesh* mesh = new cMesh;
						std::string folder = "Object/Stuffs/" + MgrObject->m_vecObjectKey[nID];
						std::string fileName = MgrObject->m_vecObjectKey[nID] + ".obj";
						if(mesh) mesh->LoadMesh(folder, fileName);
						Obj->SetMeshData(mesh);
					}
					else if (szTemp[0] == 'P') //Position
					{
						float x, y, z;
						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
						Obj->SetPosition(D3DXVECTOR3(x, y, z));
						Obj->GetPhysXData()->m_worldPose.t.x = x;
						Obj->GetPhysXData()->m_worldPose.t.y = y;
						Obj->GetPhysXData()->m_worldPose.t.z = z;
					}
					else if (szTemp[0] == 'S') //Scale
					{
						float x, y, z;
						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
						Obj->SetSize(D3DXVECTOR3(x, y, z));
					}
					else if (szTemp[0] == 'R') //Rotation
					{
						float x, y, z;
						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
						Obj->SetQuaternion(D3DXVECTOR3(x, y, z));
					}
					else if (szTemp[0] == 'X') //Physics
					{
						//물리정보입력
						if (szTemp[2] == 'A')
						{
							int isActor = 0;
							sscanf_s(szTemp, "%*s %d", &isActor);
							Obj->SetIsActor(isActor);
						}
						else if (szTemp[2] == 'T')
						{
							int nType = 0;
							sscanf_s(szTemp, "%*s %d", &nType);
							Obj->GetPhysXData()->m_type = (NxShapeType)nType;
						}
						else if (szTemp[2] == 'O')
						{
							int t, s, g;
							sscanf_s(szTemp, "%*s %d %d %d", &t, &s, &g);
							Obj->GetPhysXData()->m_isTrigger = t;
							Obj->GetPhysXData()->m_isStatic_ = s;
							Obj->GetPhysXData()->m_isGravity = g;
						}
						else if (szTemp[2] == 'P')
						{
							float x, y, z;
							sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
							Obj->GetPhysXData()->m_position.x = x;
							Obj->GetPhysXData()->m_position.y = y;
							Obj->GetPhysXData()->m_position.z = z;

							Obj->GetPhysXData()->m_localPose.t.x = x;
							Obj->GetPhysXData()->m_localPose.t.y = y;
							Obj->GetPhysXData()->m_localPose.t.z = z;

						}
						else if (szTemp[2] == 'S')
						{
							float x, y, z;
							sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
							Obj->GetPhysXData()->m_sizeValue.x = x;
							Obj->GetPhysXData()->m_sizeValue.y = y;
							Obj->GetPhysXData()->m_sizeValue.z = z;
						}
						else if (szTemp[2] == 'F')
						{
							float F[9];
							sscanf_s(szTemp, "%*s %f %f %f %f %f %f %f %f %f",
								&F[0], &F[1], &F[2], &F[3], &F[4], &F[5], &F[6], &F[7], &F[8]);
							for (int i = 0; i < 9; i++)
							{
								Obj->GetPhysXData()->m_matR[i] = F[i];
							}
						}
					}
					else if (szTemp[0] == '#') //Push
					{
						NxActor* pActor = MgrPhysX->CreateActor(
							Obj->GetPhysXData()->m_type,
							Obj->GetPhysXData()->m_position,
							Obj->GetPhysXData()->m_matR,
							Obj->GetPhysXData()->m_sizeValue,
							Obj->GetPhysXData()->m_pUserData,
							Obj->GetPhysXData()->m_isTrigger,
							Obj->GetPhysXData()->m_isStatic_,
							Obj->GetPhysXData()->m_isGravity);
						if (pActor)
						{
							//새 pActor 생성에 성공하면 
							//기존에 있던 정보내용 초기화하고
							//새 pActor 로 등록 
							MgrPhysXScene->releaseActor(*Obj->GetPhysXData()->m_pActor);
							Obj->GetPhysXData()->m_pActor = NULL;

							Obj->GetPhysXData()->m_pActor = pActor;
						}
						m_vecObject.push_back(Obj);
						break;
					}
				} // << : while Object
			}
		} // << : while 파일
	}
	else //파일 열기 실패
	{
		std::string pritfOut(FileName + ": 파일을 찾을 수 없습니다");
		MessageBoxA(g_hWnd, pritfOut.c_str(), "오류", MB_OK);
	}

	Load.close();

	//
//	return E_NOTIMPL;
}

void cScene::SaveScene(std::string FileName)
{
	std::string fullpath = "Object/Scene/" + FileName + ".scn";
	std::ofstream Save(fullpath);

	std::string strTemp;
	if (Save.is_open())
	{
		// Infomation
		Save << "//Revolt Scene File" << std::endl;
		Save << "//" << FileName << std::endl;
		struct tm datetime;
		time_t t;
		t = time(NULL);
		localtime_s(&datetime, &t);
		Save << "//Date : " << datetime.tm_year + 1900
			<< "-" << datetime.tm_mon + 1
			<< "-" << datetime.tm_mday << std::endl;
		Save << std::endl;

		//Map
		Save << "Map " << FileName << std::endl;
		Save << std::endl;

		//Object
		for (int i = 0; i < m_vecObject.size(); i++)
		{
			Save << "Object" << std::endl;

			Save << "Tag" << SPACE << m_vecObject[i]->GetTag() << std::endl;

			Save << "Name" << SPACE << m_vecObject[i]->GetObjName() << std::endl;

			Save << "ID" << SPACE << m_vecObject[i]->GetID() << std::endl;

			Save << "Position" << SPACE
				<< m_vecObject[i]->GetPosition().x << " "
				<< m_vecObject[i]->GetPosition().y << " "
				<< m_vecObject[i]->GetPosition().z << std::endl;

			Save << "Scale" << SPACE
				<< m_vecObject[i]->GetSize().x << " "
				<< m_vecObject[i]->GetSize().y << " "
				<< m_vecObject[i]->GetSize().z << std::endl;

			Save << "Rotation" << SPACE
				<< m_vecObject[i]->GetDirection().x << " "
				<< m_vecObject[i]->GetDirection().y << " "
				<< m_vecObject[i]->GetDirection().z << std::endl;

			Save << "X_Actor" << SPACE << m_vecObject[i]->GetIsActor() << std::endl;

			Save << "X_Type" << SPACE << (int)m_vecObject[i]->GetPhysXData()->m_type << std::endl;

			Save << "X_Option" << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_isTrigger << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_isStatic_ << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_isGravity << std::endl;

			Save << "X_Pos" << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_position.x << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_position.y << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_position.z << std::endl;

			Save << "X_Size" << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_sizeValue.x << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_sizeValue.y << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_sizeValue.z << std::endl;

			Save << "X_FXU32_[9]" << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_matR[0] << SPACE 
				<< m_vecObject[i]->GetPhysXData()->m_matR[1] << SPACE 
				<< m_vecObject[i]->GetPhysXData()->m_matR[2] << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_matR[3] << SPACE 
				<< m_vecObject[i]->GetPhysXData()->m_matR[4] << SPACE 
				<< m_vecObject[i]->GetPhysXData()->m_matR[5] << SPACE
				<< m_vecObject[i]->GetPhysXData()->m_matR[6] << SPACE 
				<< m_vecObject[i]->GetPhysXData()->m_matR[7] << SPACE 
				<< m_vecObject[i]->GetPhysXData()->m_matR[8] << std::endl;

			Save << "#" << std::endl;

			Save << std::endl;
		}

		MessageBoxA(g_hWnd, "저장완료", "", MB_OK);
	}
	else
	{
		MessageBoxA(g_hWnd, "파일을 생성할 수 없습니다.", "오류", MB_OK);
	}
	Save.close();

	//fullpath = "Object/PhysXData/PhysXData.phx";
	//std::ofstream SAVE(fullpath);
	//if (SAVE.is_open())
	//{
	//	SAVE << "DATA_NAME" << TAB << "SHAPE_TYPE" << TAB
	//		<< "IsTrigger" << TAB << "isStatic_" << TAB << "isGravaty" << TAB
	//		<< "POS_X" << TAB << "POS_Y" << TAB << "POS_Z" << TAB
	//		<< "SIZE_X" << TAB << "SIZE_Y" << TAB << "SIZE_Z" << TAB
	//		<< "POS_X" << TAB << "POS_Y" << TAB << "POS_Z" << TAB
	//		<< "FXU32_[0]" << TAB << "FXU32_[1]" << TAB << "FXU32_[2]" << TAB
	//		<< "FXU32_[3]" << TAB << "FXU32_[4]" << TAB << "FXU32_[5]" << TAB
	//		<< "FXU32_[6]" << TAB << "FXU32_[7]" << TAB << "FXU32_[8]" << std::endl;
	//	for each (cObject* pObj in m_vecObject)
	//	{
	//		cPhysX* p = pObj->GetPhysXData();
	//		SAVE << p->m_pActor->getName() << TAB << p->m_type << TAB
	//			<< p->m_isTrigger << TAB << p->m_isStatic_ << TAB << p->m_isGravity << TAB
	//			<< p->m_position.x << TAB << p->m_position.y << TAB << p->m_position.z << TAB
	//			<< p->m_sizeValue.x << TAB << p->m_sizeValue.y << TAB << p->m_sizeValue.z << TAB
	//			<< p->m_matR[0] << TAB << p->m_matR[1] << TAB << p->m_matR[2] << TAB
	//			<< p->m_matR[3] << TAB << p->m_matR[4] << TAB << p->m_matR[5] << TAB
	//			<< p->m_matR[6] << TAB << p->m_matR[7] << TAB << p->m_matR[8] << std::endl;
	//	}
	//}

	//	return E_NOTIMPL;	
}

void cScene::OnChangePhysX(int eventID)
{
	if (eventID == 1)
	{
		if (m_selectobj)
		{
		//	m_selectobj->GetPhysXData()->SavePhysX(m_selectobj->GetPhysXName());
		}
		for (int i = 0; i < GetObjects().size(); i++)
		{
			if (GetObjects()[i]->GetPhysXData()->m_pActor)
			{
				MgrPhysXScene->releaseActor(*GetObjects()[i]->GetPhysXData()->m_pActor);
			}
			GetObjects()[i]->GetPhysXData()->LoadPhysX("");
		}
	}
}

void cScene::Setup()
{

}

void cScene::Destory()
{
	if (m_pMap) m_pMap->Destory();
	for each (cObject* pObj in m_vecObject)
	{
		pObj->Destory();
	}
	m_selectobj = NULL;
}

void cScene::Update()
{
	if (m_pMap) m_pMap->Update();
	//계산 시작
	for each (cObject* pObj in m_vecObject)
	{
		pObj->SetMapData(m_pMap->GetPhysXData()->m_pUserData);
		pObj->Update();
	}
}

void cScene::LastUpdate()
{
	//계산 종료
	//if (m_pMap) m_pMap->LastUpdate();
	for each (cObject* pObj in m_vecObject)
	{
		//물리정보 동기화
		pObj->LastUpdate();
		//선택한 오브젝트 정보를 UI 에 연동
		if (pObj->GetState() == eOBJECT_STATE::E_OBJECT_STATE_SELECT)
		{
			m_selectobj = pObj;
			cPhysX* pPhysX = m_selectobj->GetPhysXData();

			cUITextBox* pTextBox = NULL;
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_OBJNAME);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(pObj->GetObjName());

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetPosition().x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetPosition().y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetPosition().z));

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetSize().x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetSize().y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetSize().z));

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetDirection().x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetDirection().y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetDirection().z));

			//	pPhysX->m_worldPose.t = MgrPhysX->D3DVecToNxVec(m_selectobj->GetPosition());
			//	NxF32 nxF[9] = { 1,0,0,0,1,0,0,0,1 };
			//	MgrPhysX->D3DMatToNxMat(nxF, m_selectobj->GetMatrix(false, true, true));
			//	NxMat33 nxMat; nxMat.setColumnMajor(nxF);
			//	pPhysX->m_worldPose.M = nxMat;



			//PhysX

			NxVec3 NxPos = pObj->GetPhysXData()->m_localPose.t;
			NxVec3 NxDir = pObj->GetPhysXData()->m_dirValue;

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_POS_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(NxPos.x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_POS_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(NxPos.y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_POS_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(NxPos.z));

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_SIZE_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetPhysXData()->m_sizeValue.x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_SIZE_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetPhysXData()->m_sizeValue.y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_SIZE_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(pObj->GetPhysXData()->m_sizeValue.z));

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_ROT_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(NxDir.x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_ROT_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(NxDir.y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_ROT_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetUIText()->SetText(cStringUtil::ToString(NxDir.z));
		}
	}
}

void cScene::Render()
{
	if (m_pMap) m_pMap->Render();
	for each (cObject* pObj in m_vecObject)
	{
		pObj->Render();
	}
}

void cScene::OnChangeValue(int eventID, std::string eventKey)
{
	if (eventID != VK_RETURN) return;
	if (!m_selectobj) return;
	if (m_selectobj->GetState() != eOBJECT_STATE::E_OBJECT_STATE_SELECT) return;
	{
		//D3DX
		cUITextBox* pTextBox = NULL;

		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_OBJNAME);
		m_selectobj->SetObjName(pTextBox->GetUIText()->GetTextData()->text);


		D3DXVECTOR3 vec3;
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_X);		vec3.x = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Y);		vec3.y = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Z);		vec3.z = pTextBox->GetUIText()->GetTextData()->GetFloat();
		m_selectobj->SetPosition(vec3);

		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_X);		vec3.x = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Y);		vec3.y = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Z);		vec3.z = pTextBox->GetUIText()->GetTextData()->GetFloat();
		m_selectobj->SetSize(vec3);

		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_X);		vec3.x = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Y);		vec3.y = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Z);		vec3.z = pTextBox->GetUIText()->GetTextData()->GetFloat();
		m_selectobj->SetQuaternion(vec3);



		//PhysX
		cPhysX* pPhysX = m_selectobj->GetPhysXData();

		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_POS_X);		vec3.x = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_POS_Y);		vec3.y = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_POS_Z);		vec3.z = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pPhysX->SetLocalPosition(vec3);

		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_SIZE_X);		vec3.x = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_SIZE_Y);		vec3.y = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_SIZE_Z);		vec3.z = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pPhysX->m_sizeValue = NxVec3(vec3.x, vec3.y, vec3.z);

		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_ROT_X);		vec3.x = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_ROT_Y);		vec3.y = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_NX_ROT_Z);		vec3.z = pTextBox->GetUIText()->GetTextData()->GetFloat();
		pPhysX->m_dirValue = NxVec3(vec3.x, vec3.y, vec3.z);
		if (vec3.x == 0.f && vec3.y == 0.f && vec3.z == 0.f) vec3.z = 1;
		{
			cTransform tr; tr.SetQuaternion(vec3);	NxF32 nxF[9] = { 1,0,0,0,1,0,0,0,1 };
			MgrPhysX->D3DMatToNxMat(nxF, tr.GetMatrix(false, true, true));
			NxMat33 nxMat; nxMat.setColumnMajor(nxF);
			pPhysX->SetLocalRotation(nxMat);
		}

		{
			pPhysX->m_worldPose.t = MgrPhysX->D3DVecToNxVec(m_selectobj->GetPosition());
			NxF32 nxF[9] = { 1,0,0,0,1,0,0,0,1 };
			MgrPhysX->D3DMatToNxMat(nxF, m_selectobj->GetMatrix(false, true, false));
			NxMat33 nxMat; nxMat.setColumnMajor(nxF);
			pPhysX->m_worldPose.M = nxMat;
		}
		NxMat34 NxActorPose;
		NxActorPose.multiply(pPhysX->m_worldPose, pPhysX->m_localPose);

		pPhysX->m_pActor->setGlobalPose(NxActorPose);
	}
}