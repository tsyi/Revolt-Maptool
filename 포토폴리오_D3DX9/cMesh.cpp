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

//void cMesh::LoadMeshXFile(std::string XFileForder, std::string XFileName)
//{
//	const std::string a;
//	ID3DXBuffer* adjBuffer = 0;
//	ID3DXBuffer* mtlBuffer = 0;
//	DWORD numMtl = 0;
//	std::string XFilePath = XFileForder + XFileName;
//
//	D3DXLoadMeshFromXA(XFilePath.c_str(), D3DXMESH_MANAGED, MgrD3DDevice, &adjBuffer, &mtlBuffer, 0, &numMtl, &m_pMesh);
//
//	if (mtlBuffer != 0 && numMtl != 0)
//	{
//		D3DXMATERIAL* mtl = (D3DXMATERIAL*)mtlBuffer->GetBufferPointer();
//		mtl->pTextureFilename;
//
//		m_vecMaterial.clear();
//		m_vecMaterial.resize(numMtl);
//		for (int i = 0; i < numMtl; i++)
//		{
//			m_vecMaterial[i] = mtl[i].MatD3D;
//			m_vecMaterial[i].Ambient = m_vecMaterial[i].Diffuse;
//
//			//m_vecMaterial.push_back(mtl[i].MatD3D);
//
//			if (mtl[i].pTextureFilename != 0)
//			{
//				IDirect3DTexture9* tex = 0;
//				D3DXCreateTextureFromFileA(MgrD3DDevice, "Zealot_Diffuse.bmp", &tex);
//				m_vecTexture.push_back(tex);
//			}
//			else
//			{
//				m_vecTexture.push_back(0);
//			}
//		}
//
//		//D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
//		//_pMeshMaterials = new D3DMATERIAL9[_dwNumMaterials];
//		//_pMeshTextures = new LPDIRECT3DTEXTURE9[_dwNumMaterials];
//		//char fullname[MAX_PATH];
//		//for (DWORD i = 0; i < _dwNumMaterials; i++)
//		//{
//		//	// ���� ���� ����.
//		//	_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
//		//	// �ֺ� ���� ������ Diffuse������.
//		//	_pMeshMaterials[i].Ambient = _pMeshMaterials[i].Diffuse;
//		//	// �ؽ�ó�� ������� �ؽ�ó ���� ����.
//		//	_pMeshTextures[i] = NULL;
//		//	if (d3dxMaterials[i].pTextureFilename != NULL && strlen(d3dxMaterials[i].pTextureFilename) > 0)
//		//	{
//		//		strcpy_s(fullname, MAX_PATH, "./CP/");
//		//		strcat_s(fullname, MAX_PATH, d3dxMaterials[i].pTextureFilename);
//		//		// �ؽ�ó�� ���Ͽ��� �ε��Ѵ�.
//		//		//D3DXCreateTextureFromFileA(ManageGraphic::GetInstance()->GetD3DDevice(),
//		//		//	fullname, &_pMeshTextures[i]))
//		//	}
//		//}
//
//
//
//	}
//	SAFE_RELEASE(adjBuffer);
//	SAFE_RELEASE(mtlBuffer);
//
//}

void cMesh::LoadMeshObjLoder(std::string folder, std::string name)
{
	cObjLoader::LoadMesh(this, folder, name);
}

void cMesh::Destory()
{
	//m_mapMtlTex.clear();
	for each (auto pMtlTex in m_mapMtlTex)
	{
		if(pMtlTex.second) pMtlTex.second->~cMtlTex();
	}
	m_mapMtlTex.clear();
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
//	else
//	{
//		MgrD3DDevice->SetMaterial(&m_material);
//		if (m_pTexture) MgrD3DDevice->SetTexture(0, m_pTexture);
//		m_pMesh->DrawSubset(0);
//		if (m_pTexture) MgrD3DDevice->SetTexture(0, NULL);
//	}

}
