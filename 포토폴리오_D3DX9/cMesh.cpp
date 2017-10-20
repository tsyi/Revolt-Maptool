#include "stdafx.h"
#include "cMesh.h"
#include "cMtlTex.h"
#include "cObjLoader.h"

//������� å , �������� å
//���α׷��ָ���, �̷��� �غ��Ѵ�.

cMesh::cMesh()
	: m_pMesh(NULL)
{
}


cMesh::~cMesh()
{
}

void cMesh::LoadMesh(std::string folder, std::string name)
{
	cObjLoader::LoadMesh(this, folder, name);
}

void cMesh::Destory()
{
	//m_mapMtlTex.clear();
	for each (auto pMtlTex in m_vecMtlTex)
	{
		pMtlTex->~cMtlTex();
	}
	m_vecMtlTex.clear();
	SAFE_RELEASE(m_pMesh);
}

void cMesh::Render()
{
	if (!m_vecMtlTex.empty())
	{
		int index = 0;
		for each (auto pMtlTex in m_vecMtlTex)
		{
			MgrD3DDevice->SetMaterial(&(pMtlTex->GetMaterial()));
			if (pMtlTex->GetTexture())
				MgrD3DDevice->SetTexture(0, pMtlTex->GetTexture());

			m_pMesh->DrawSubset(index++);

			if (pMtlTex->GetTexture())
				MgrD3DDevice->SetTexture(0, NULL);
		}
	}
}
