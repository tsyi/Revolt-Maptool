#include "StdAfx.h"
#include "cSkinnedMeshManager.h"
#include "cSkinnedMesh.h"

cSkinnedMeshManager::cSkinnedMeshManager(void)
{
}


cSkinnedMeshManager::~cSkinnedMeshManager(void)
{
}

void cSkinnedMeshManager::Destroy()
{
	for each(auto it in m_mapSkinnedMesh)
	{
		it.second->Destroy();
		SAFE_DELETE(it.second);
	}
}

cSkinnedMesh* cSkinnedMeshManager::GetSkinnedMesh(std::string szFolder, std::string szFilename)
{
	std::string sFullPath = szFolder  + szFilename;

	if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh();
		pSkinnedMesh->Load(szFolder, szFilename);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
	}
	return m_mapSkinnedMesh[sFullPath];
}
