#include "stdafx.h"
#include "cMap.h"
#include "cObjLoader.h"
#include <fstream>

cMap::cMap()
{
}

cMap::~cMap()
{
}

void cMap::Destroy()
{
	for each(cMesh* mesh in m_vecObj)
	{
		mesh->Destory();
	}
	m_vecObj.clear();

	m_mapCamera.clear();
	m_mapCheckBox.clear();
	m_mapFollowPoint.clear();
}

HRESULT cMap::MapLoad(std::string rvlName)
{
	//std::fstream Load;
	//Load.open(rvlName);
	//
	//char szTemp[1024];
	//
	//if (Load.is_open())
	//{
	//	while (1)
	//	{
	//		if (Load.eof()) break;
	//
	//		Load.getline(szTemp, 1024);
	//		if (szTemp[0] == ' ' || szTemp[0] == '\t') continue;
	//		if (szTemp[0] == 'M') // Map Load 
	//		{
	//			char szMapFile[1024];
	//			sscanf_s(szTemp, "%*s %s", szMapFile, 1024);
	//			m_meshMap = new cMesh;
	//			std::string folder = BaseFolder + "/" + std::string(szMapFile);
	//			std::string fileName = szMapFile + std::string(".obj");
	//
	//			m_meshMap->LoadMesh(folder, fileName);
	//		}
	//		else if (szTemp[0] == 'O') //Object Load
	//		{
	//			cMesh* mesh = new cMesh;
	//			int nObj = 0;
	//			while (1)
	//			{
	//				Load.getline(szTemp, 1024);
	//				if (szTemp[0] == 'A') //Attribute
	//				{
	//					sscanf_s(szTemp, "%*s %d", &nObj);
	//					
	//					mesh = MgrObject->LoadObj((eOBJ_TAG)nObj);
	//				}
	//				else if (szTemp[0] == 'P') //Postion
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					mesh->SetPosition(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == 'S') //Scale
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					mesh->SetSize(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == 'R') //Rotation
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					mesh->SetQuaternionToVector(D3DXVECTOR3(x, y, z), true, true);
	//				}
	//				else if (szTemp[0] == 'N')
	//				{
	//					int nPhx =0;
	//					sscanf_s(szTemp, "%*s %d", nPhx, 1024);
	//					if (nPhx)
	//					{
	//						mesh->m_isActor = true;
	//						std::string fullPath = "Object/Objects/" + strObjName[nObj] + "/" + strObjName[nObj] + ".phx";
	//						PhxLoad(fullPath, mesh);
	//					}
	//					else
	//					{
	//						mesh->m_isActor = false;
	//					}
	//				}
	//				else if (szTemp[0] == '#') //End
	//				{
	//					m_vecObj.push_back(mesh);
	//					break;
	//				}
	//			} // << : while Object
	//		}
	//		else if (szTemp[0] == 'C') // Camera Load
	//		{
	//			cTransform* cam = new cTransform;
	//			int nNum;
	//			while (1)
	//			{
	//				Load.getline(szTemp, 1024);
	//				if (szTemp[0] == 'N') //Number
	//				{
	//					sscanf_s(szTemp, "%*s %d", &nNum); 
	//				}
	//				else if (szTemp[0] == 'P') //Position
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					cam->SetPosition(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == 'D') //Direction(LookAt)
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					cam->SetDirection(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == '#') // End
	//				{
	//					m_mapCamera.insert(std::make_pair(nNum, cam));
	//					break;
	//				}
	//			} // << : Camera position
	//		}
	//		else if (szTemp[0] == 'B') //CheckBox
	//		{
	//			cTransform* box = new cTransform;
	//			int nNum;
	//			while (1)
	//			{
	//				Load.getline(szTemp, 1024);
	//				if (szTemp[0] == 'N') //Number
	//				{
	//					sscanf_s(szTemp, "%*s %d", &nNum);
	//				}
	//				else if (szTemp[0] == 'P') //Position
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					box->SetPosition(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == 'S') //Scale
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					box->SetSize(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == 'R') //Rotation
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					box->SetQuaternionToVector(D3DXVECTOR3(x, y, z),true,true);
	//				}
	//				else if (szTemp[0] == '#') //End
	//				{
	//					m_mapCheckBox.insert(std::make_pair(nNum, box));
	//					break;
	//				}
	//			} // << : while CheckBox
	//		}
	//		else if (szTemp[0] == 'F') //FollowPoint
	//		{
	//			cTransform* follow = new cTransform;
	//			int nNum;
	//			while (1)
	//			{
	//				Load.getline(szTemp, 1024);
	//				if (szTemp[0] == 'N') //Number
	//				{
	//					sscanf_s(szTemp, "%*s %d", &nNum);
	//				}
	//				else if (szTemp[0] == 'P') // Position
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					follow->SetPosition(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == 'S') //Scale
	//				{
	//					float x, y, z;
	//					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
	//					follow->SetDirection(D3DXVECTOR3(x, y, z));
	//				}
	//				else if (szTemp[0] == '#') //End
	//				{
	//					m_mapFollowPoint.insert(std::make_pair(nNum, follow));
	//					break;
	//				}
	//			} // << : while FollowPoint
	//		}
	//	} // << : while 파일
	//}
	//else //파일 열기 실패
	//{
	//	MessageBoxA(g_hWnd, "파일을 찾을 수 없습니다.", "오류", MB_OK);
	//}
	//
	//Load.close();
	//
	return E_NOTIMPL;
}

HRESULT cMap::MapSave(std::string rvlName)
{
	return E_NOTIMPL;
}

HRESULT cMap::PhxLoad(std::string phxName, cMesh * dest)
{
	std::fstream Load;
	Load.open(phxName);

	char szTemp[1024];

	if (Load.is_open())
	{
		while (1)
		{
			if (Load.eof()) break;

			Load.getline(szTemp, 1024);
			if (szTemp[0] == ' ' || szTemp[0] == '\t') continue;
			if (szTemp[0] == 'A')
			{
				
			}
			else if (szTemp[0] == 'M')
			{

			}
			else if (szTemp[0] == 'B')
			{

			}
			else if (szTemp[0] == 'S')
			{

			}
			else if (szTemp[0] == 'U')
			{

			}
		} // << : while 파일
	}

	Load.close();
	
	return E_NOTIMPL;
}



