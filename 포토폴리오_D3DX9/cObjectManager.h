#pragma once

#define MgrObject	cObjectManager::GetInstance()

class cObject;
class cScene;

enum eOBJ_TAG
{
	OBJ_NONE,
	OBJ_CHEESE,
	OBJ_CHICKEN,
	OBJ_FABRIC,
	OBJ_KIDRIDE,
	OBJ_MAX
};

const std::string strObjName[OBJ_MAX]
{
	"",	//default -> mesh ��� ���� ����(cTransform ������ ������ �ִ� ���)
	"cheese",
	"chicken",
	"fabric",
	"kidride"
};

class cObjectManager
{
	// >> : 
	//public:
	//	cObjectManager();
	//	~cObjectManager();
	const std::string OBJECTS_PACH = "Object/Objects/";

private:
	SINGLETONE(cObjectManager);

private:
	std::vector<cMesh*> m_vecObjList;			// setup
	std::vector<cMesh*> m_vecCreatedObj;		// create

public:
	void Setup();
	void AddObj(cScene* AddScene, eOBJ_TAG eObj);
	void DeleteObj();
	cMesh* LoadObj(eOBJ_TAG fileName);
	//void AddObject(cObject* pObject);
	//void RemoveObject(cObject* pObject);
	void Destroy();
	// << : 


	void Render();
};

