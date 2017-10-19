#pragma once
#include "cTransform.h"
#include "cDeviceManager.h"

enum eMESH_STATE
{
	MESH_STATE_NONE,
	MESH_STATE_SELET
};

class cMtlTex;
class cMesh : public cTransform
{
public:

	LPD3DXMESH m_pMesh;
	std::vector<cMtlTex*> m_vecMtlTex;

	eMESH_STATE m_eState;

	bool m_isActor;

public:
	void Destory();
	void Render();

	cMesh();
	cMesh(cMesh* pMesh)
	{}
	~cMesh();

	void LoadMeshObjLoder(std::string folder, std::string name);

	void SetState(eMESH_STATE state) { m_eState = state; }
	eMESH_STATE GetState() { return m_eState; }
};

