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
	"",
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

private:
	SINGLETONE(cObjectManager);

private:
	std::set<cObject*>	m_setObject;			//-
	std::map<eOBJ, cObject*> m_mapObjList;		//-
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

