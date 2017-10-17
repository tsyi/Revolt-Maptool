#pragma once

#define g_pSkinnedMeshManager cSkinnedMeshManager::GetInstance()
#define MgrSkinnedMesh cSkinnedMeshManager::GetInstance()
class cSkinnedMesh;

class cSkinnedMeshManager
{
private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;

public:
	SINGLETONE(cSkinnedMeshManager);

	cSkinnedMesh* GetSkinnedMesh(std::string szFolder, std::string szFilename);

	void Destroy();
};

