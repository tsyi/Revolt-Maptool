#pragma once

#define MgrObject	cObjectManager::GetInstance()

class cObject;
class cScene;


class cObjectManager
{
	// >> : 
	//public:
	//	cObjectManager();
	//	~cObjectManager();
	const std::string OBJECTS_PACH = "Object/Stuffs/";

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


	cObject* CreateObject(std::string objectName);
	void CreateButton(std::string objectName);
};

