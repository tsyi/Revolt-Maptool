#pragma once
#include "cMtlTex.h"
//class cMtlTex;

class cMesh
{
public:
	LPD3DXMESH m_pMesh;
	std::vector<cMtlTex*> m_vecMtlTex;

public:
	void Destory();
	void Render();

	cMesh();
	cMesh(cMesh* pMesh)
	{}
	~cMesh();

	void LoadMesh(std::string folder, std::string name);
	void LoadSphere();
	void LoadBox();
};

