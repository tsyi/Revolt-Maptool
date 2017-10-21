#pragma once
#include "cObject.h"

const std::string BaseFolder("Object/Maps");

class cMap : public cObject
{
private:
	cMesh* m_meshMap;
	std::vector<cMesh*> m_vecObj;
	std::map<int, cTransform*> m_mapCamera;
	std::map<int, cTransform*> m_mapCheckBox;
	std::map<int, cTransform*> m_mapFollowPoint;

public:
	cMap();
	~cMap();

	void Destroy();
	HRESULT MapLoad(std::string rvlName);
	HRESULT MapSave(std::string rvlName);
	HRESULT PhxLoad(std::string phxName,cMesh* dest);
};