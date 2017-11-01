#pragma once

#define MgrObject	cObjectManager::GetInstance()

class cObject;
class cScene;
class NxVec3;

class cObjectManager
{
	// >> : 
	//public:
	//	cObjectManager();
	//	~cObjectManager();
	const std::string OBJECTS_PACH = "Object/Stuffs/";
	const NxVec3 INITPOSITION = NxVec3(0, 0.5, 0);
	const NxVec3 INITSIZE = NxVec3(1, 1, 1);

	D3DXVECTOR3* m_vecCamTarget;

private:
	SINGLETONE(cObjectManager);

private:
	//	std::vector<cMesh*> m_vecCreatedObj;		// create

public:
	std::vector<std::string> m_vecObjectKey;	// setup
	void Setup();
	void AddObj(cScene* AddScene, int keyID);
	void DeleteObj();
	//	cMesh* LoadObj(eOBJ_TAG fileName);
		//void AddObject(cObject* pObject);
		//void RemoveObject(cObject* pObject);
	void Destroy();
	// << : 
	void Render();

	void LinkCamTarget(D3DXVECTOR3* camtarget) { m_vecCamTarget = camtarget; }
	
	cObject* CreateObject(int keyID);
	void CreateButton(std::string objectName);
};

