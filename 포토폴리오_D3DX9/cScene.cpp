#include "stdafx.h"
#include "cScene.h"
#include <fstream>
#include "cUIText.h"
#include "cMap.h"
#include "cLight.h"
#include "cStuff.h"
#include <time.h>

cScene::cScene()
{
}

cScene::~cScene()
{
}

HRESULT cScene::LoadScene(std::string FileName)
{
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
				m_pMap = new cMap;
				std::string folder = "Object/Maps/" + std::string(szMapFile);
				std::string fileName = szMapFile + std::string(".obj");
	
				m_pMap->GetMesh()->LoadMesh(folder, fileName);
			}
			else if (szTemp[0] == 'O') //Object Load
			{
				cObject* Obj;

				eOBJECT_TAG tag;
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
				case E_OBJECT_END:		break;
				default: break;
				}
				
				while (1)
				{
					Load.getline(szTemp, 1024);

					if (szTemp[0] == 'A') // Attribute
					{
						int nAtt = 0;
						sscanf_s(szTemp, "%*s %d", &nAtt);
						Obj->SetAttribute(nAtt);
					}
					else if (szTemp[0] == 'P') //Position
					{
						float x, y, z;
						sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
						Obj->SetPosition(D3DXVECTOR3(x, y, z));
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
					else if (szTemp[0] == 'N') //Physics
					{
						//물리정보입력
					}
					else if (szTemp[0] == '#') //Push
					{
						m_vecObject.push_back(Obj);
						break;
					}
				} // << : while Object
			}
		} // << : while 파일
	}
	else //파일 열기 실패
	{
		MessageBoxA(g_hWnd, "파일을 찾을 수 없습니다.", "오류", MB_OK);
	}
	
	Load.close();
	//
	return E_NOTIMPL;
}

HRESULT cScene::SaveScene(std::string FileName)
{
	std::string fullpath = "Object/Scene/" + FileName + ".scn";
	std::ofstream Save(fullpath);

	std::string strTemp;
	if (Save.is_open())
	{
		// Infomation
		Save << "//Revolt Scene File" << std::endl;
		Save << "//Market2" << std::endl;
		struct tm datetime;
		time_t t;
		t = time(NULL);
		localtime_s(&datetime,&t);
		Save << "//Date : " << datetime.tm_year + 1900
			<< "-" << datetime.tm_mon + 1
			<< "-" << datetime.tm_mday << std::endl;
		Save << std::endl;
		Save << "Map " << FileName << std::endl;
		Save << std::endl;

		//Object
		for (int i = 0; i < m_vecObject.size(); i++)
		{
			Save << "Object" << std::endl;
			
			Save << "Tag " << m_vecObject[i]->GetTag() << std::endl;
			
			Save << "Attribute " << m_vecObject[i]->GetAttribute() << std::endl;
			
			Save << "Position"
				<< m_vecObject[i]->GetPosition().x << " "
				<< m_vecObject[i]->GetPosition().y << " "
				<< m_vecObject[i]->GetPosition().z << std::endl;

			Save << "Scale"
				<< m_vecObject[i]->GetSize().x << " "
				<< m_vecObject[i]->GetSize().y << " "
				<< m_vecObject[i]->GetSize().z << std::endl;

			Save << "Rotation"
				<< m_vecObject[i]->GetDirection().x << " "
				<< m_vecObject[i]->GetDirection().y << " "
				<< m_vecObject[i]->GetDirection().z << std::endl;
			
			Save << "N " << std::endl;

			Save << "#" << std::endl;

			Save << std::endl;
		}
	}
	else
	{
		MessageBoxA(g_hWnd, "파일을 생성할 수 없습니다.", "오류", MB_OK);
	}

	Save.close();
	return E_NOTIMPL;
	
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