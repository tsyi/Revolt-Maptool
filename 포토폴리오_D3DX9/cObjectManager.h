#pragma once

#define MgrObject	cObjectManager::GetInstance()

class cObject;

enum eOBJ
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
	void AddObj(eOBJ eObj);
	void DeleteObj();
	//void AddObject(cObject* pObject);
	//void RemoveObject(cObject* pObject);
	void Destroy();
	// << : 
};

