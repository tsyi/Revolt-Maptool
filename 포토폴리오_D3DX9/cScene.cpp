#include "stdafx.h"
#include "cScene.h"
#include "cUIText.h"
#include <fstream>

cScene::cScene()
{
}

cScene::~cScene()
{
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
		pObj->Update();
	}
}

void cScene::LastUpdate()
{
	//계산 종료
	if (m_pMap) m_pMap->LastUpdate();
	for each (cObject* pObj in m_vecObject)
	{
		//물리정보 동기화
		pObj->LastUpdate();
		//선택한 오브젝트 정보를 UI 에 연동
		if (pObj->GetState() == eOBJECT_STATE::E_OBJECT_STATE_SELECT)
		{
			m_selectobj = pObj;

			cUITextBox* pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetPosition().x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetPosition().y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetPosition().z));

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetSize().x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetSize().y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetSize().z));

			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_X);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetDirection().x));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Y);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetDirection().y));
			pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Z);
			if (pTextBox->GetState() == eTextBoxState::E_TEXT_BOX_STATE_NONE)
				pTextBox->GetText()->SetText(cStringUtil::ToString(pObj->GetDirection().z));
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

void cScene::OnChangeValue(int eventID)
{
	if (!m_selectobj) return;
	if (m_selectobj->GetState() != eOBJECT_STATE::E_OBJECT_STATE_SELECT) return;

	D3DXVECTOR3 vec3;
	cUITextBox* pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_X);
	vec3.x = pTextBox->GetText()->GetText()->GetFloat();
	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Y);
	vec3.y = pTextBox->GetText()->GetText()->GetFloat();
	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_POS_Z);
	vec3.z = pTextBox->GetText()->GetText()->GetFloat();
	m_selectobj->SetPosition(vec3);
	NxVec3 nxPos = MgrPhysX->D3DVecToNxVec(vec3);
	m_selectobj->GetActor()->setGlobalPosition(nxPos);

	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_X);
	vec3.x = pTextBox->GetText()->GetText()->GetFloat();
	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Y);
	vec3.y = pTextBox->GetText()->GetText()->GetFloat();
	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_SIZE_Z);
	vec3.z = pTextBox->GetText()->GetText()->GetFloat();
	m_selectobj->SetSize(vec3);

	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_X);
	vec3.x = pTextBox->GetText()->GetText()->GetFloat();
	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Y);
	vec3.y = pTextBox->GetText()->GetText()->GetFloat();
	pTextBox = (cUITextBox*)MgrUI->FindByTag(eUITag::E_UI_TEXTBOX_ROT_Z);
	vec3.z = pTextBox->GetText()->GetText()->GetFloat();
	m_selectobj->SetQuaternion(vec3);
	m_selectobj->SetDirection(vec3);

	NxF32 nxF[9] = { 1,0,0,0,1,0,0,0,1 };
	MgrPhysX->D3DMatToNxMat(nxF, m_selectobj->GetMatrix(false, true, true));
	NxMat33 nxMat;
	nxMat.setColumnMajor(nxF);
	m_selectobj->GetActor()->setGlobalOrientation(nxMat);

}
HRESULT cScene::LoadScene(std::string FileName)
{
	//	std::fstream Load;
	//	Load.open(rvlName);
	//	
	//	char szTemp[1024];
	//	
	//	if (Load.is_open())
	//	{
	//		while (1)
	//		{
	//			if (Load.eof()) break;
	//	
	//			Load.getline(szTemp, 1024);
	//			if (szTemp[0] == ' ' || szTemp[0] == '\t') continue;
	//			if (szTemp[0] == 'M') // Map Load 
	//			{
	//				char szMapFile[1024];
	//				sscanf_s(szTemp, "%*s %s", szMapFile, 1024);
	//				m_meshMap = new cMesh;
	//				std::string folder = BaseFolder + "/" + std::string(szMapFile);
	//				std::string fileName = szMapFile + std::string(".obj");
	//	
	//				m_meshMap->LoadMesh(folder, fileName);
	//			}
	//			else if (szTemp[0] == 'O') //Object Load
	//			{
	//				cMesh* mesh = new cMesh;
	//				int nObj = 0;
	//				while (1)
	//				{
	//					Load.getline(szTemp, 1024);
	//					if (szTemp[0] == 'A') //Attribute
	//					{
	//						sscanf_s(szTemp, "%*s %d", &nObj);
	//						
	//						mesh = MgrObject->LoadObj((eOBJ_TAG)nObj);
	//					}
	//					else if (szTemp[0] == 'P') //Postion
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						mesh->SetPosition(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == 'S') //Scale
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						mesh->SetSize(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == 'R') //Rotation
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						mesh->SetQuaternionToVector(D3DXVECTOR3(x, y, z), true, true);
	//					}
	//					else if (szTemp[0] == 'N')
	//					{
	//						int nPhx =0;
	//						sscanf_s(szTemp, "%*s %d", nPhx, 1024);
	//						if (nPhx)
	//						{
	//							mesh->m_isActor = true;
	//							std::string fullPath = "Object/Objects/" + strObjName[nObj] + "/" + strObjName[nObj] + ".phx";
	//							PhxLoad(fullPath, mesh);
	//						}
	//						else
	//						{
	//							mesh->m_isActor = false;
	//						}
	//					}
	//					else if (szTemp[0] == '#') //End
	//					{
	//						m_vecObj.push_back(mesh);
	//						break;
	//					}
	//				} // << : while Object
	//			}
	//			else if (szTemp[0] == 'C') // Camera Load
	//			{
	//				cTransform* cam = new cTransform;
	//				int nNum;
	//				while (1)
	//				{
	//					Load.getline(szTemp, 1024);
	//					if (szTemp[0] == 'N') //Number
	//					{
	//						sscanf_s(szTemp, "%*s %d", &nNum); 
	//					}
	//					else if (szTemp[0] == 'P') //Position
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						cam->SetPosition(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == 'D') //Direction(LookAt)
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						cam->SetDirection(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == '#') // End
	//					{
	//						m_mapCamera.insert(std::make_pair(nNum, cam));
	//						break;
	//					}
	//				} // << : Camera position
	//			}
	//			else if (szTemp[0] == 'B') //CheckBox
	//			{
	//				cTransform* box = new cTransform;
	//				int nNum;
	//				while (1)
	//				{
	//					Load.getline(szTemp, 1024);
	//					if (szTemp[0] == 'N') //Number
	//					{
	//						sscanf_s(szTemp, "%*s %d", &nNum);
	//					}
	//					else if (szTemp[0] == 'P') //Position
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						box->SetPosition(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == 'S') //Scale
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						box->SetSize(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == 'R') //Rotation
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						box->SetQuaternionToVector(D3DXVECTOR3(x, y, z),true,true);
	//					}
	//					else if (szTemp[0] == '#') //End
	//					{
	//						m_mapCheckBox.insert(std::make_pair(nNum, box));
	//						break;
	//					}
	//				} // << : while CheckBox
	//			}
	//			else if (szTemp[0] == 'F') //FollowPoint
	//			{
	//				cTransform* follow = new cTransform;
	//				int nNum;
	//				while (1)
	//				{
	//					Load.getline(szTemp, 1024);
	//					if (szTemp[0] == 'N') //Number
	//					{
	//						sscanf_s(szTemp, "%*s %d", &nNum);
	//					}
	//					else if (szTemp[0] == 'P') // Position
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						follow->SetPosition(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == 'S') //Scale
	//					{
	//						float x, y, z;
	//						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//						follow->SetDirection(D3DXVECTOR3(x, y, z));
	//					}
	//					else if (szTemp[0] == '#') //End
	//					{
	//						m_mapFollowPoint.insert(std::make_pair(nNum, follow));
	//						break;
	//					}
	//				} // << : while FollowPoint
	//			}
	//		} // << : while 파일
	//	}
	//	else //파일 열기 실패
	//	{
	//		MessageBoxA(g_hWnd, "파일을 찾을 수 없습니다.", "오류", MB_OK);
	//	}
	//	
	//	Load.close();
	//	//
	return E_NOTIMPL;
}

HRESULT cScene::SaveScene(std::string FileName)
{
	//	std::string Folder = BaseFolder + "/" + rvlName;
	//	std::string FullPath = Folder + "/" + rvlName + ".rvl";
	//	std::ofstream Save(FullPath);
	//
	//	std::string strTemp;
	//	if (Save.is_open())
	//	{
	//		Save << "Map " << rvlName << std::endl;
	//		Save << std::endl;
	//		for (int i = 0; i < m_vecObj.size(); i++)
	//		{
	//			Save << "Object" << std::endl;
	//			//Save << "Attribute " << m_vecObj[i]->GetOBJ_TAG() << std::endl;
	//			Save << "Position"
	//				<< m_vecObj[i]->GetPosition().x << " "
	//				<< m_vecObj[i]->GetPosition().y << " "
	//				<< m_vecObj[i]->GetPosition().z << std::endl;
	//			Save << "Scale"
	//				<< m_vecObj[i]->GetSize().x << " "
	//				<< m_vecObj[i]->GetSize().y << " "
	//				<< m_vecObj[i]->GetSize().z << std::endl;
	//			Save << "Rotation"
	//				<< m_vecObj[i]->Get.x << " "
	//				<< m_vecObj[i]->GetSize().y << " "
	//				<< m_vecObj[i]->GetSize().z << std::endl;
	//		}
	//
	//	}
	//	else
	//	{
	//		MessageBoxA(g_hWnd, "파일을 생성할 수 없습니다.", "오류", MB_OK);
	//	}
	//
	//	Save.close();
	//	return E_NOTIMPL;
	return E_NOTIMPL;
}
