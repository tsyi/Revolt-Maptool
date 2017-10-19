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

public:
	void Destory();
	void Render();

	cMesh();
	cMesh(cMesh* pMesh)
	{
		cTransform::SetTransform(pMesh->GetTransform());

		D3DXCreateMeshFVF(
			pMesh->m_pMesh->GetNumFaces(),
			pMesh->m_pMesh->GetNumVertices(),
			pMesh->m_pMesh->GetOptions(),
			pMesh->m_pMesh->GetFVF(),
			MgrD3DDevice, &m_pMesh);

		m_vecMtlTex.resize(pMesh->m_vecMtlTex.size());
		for (size_t i = 0; i < m_vecMtlTex.size(); i++)
		{
			m_vecMtlTex[i] = pMesh->m_vecMtlTex[i];
		}
		m_eState = pMesh->m_eState;
	}
	~cMesh();

	void LoadMeshObjLoder(std::string folder, std::string name);

	void SetState(eMESH_STATE state) { m_eState = state; }
	eMESH_STATE GetState() { return m_eState; }
};

