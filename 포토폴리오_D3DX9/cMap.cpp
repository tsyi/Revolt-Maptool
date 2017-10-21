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
//	for each(cMesh* mesh in m_vecObj)
//	{
//		mesh->Destory();
//	}
//	m_vecObj.clear();
//
//	m_mapCamera.clear();
//	m_mapCheckBox.clear();
//	m_mapFollowPoint.clear();
}

HRESULT cMap::MapLoad(std::string rvlName)
{

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
		} // << : while ÆÄÀÏ
	}

	Load.close();
	
	return E_NOTIMPL;
}



