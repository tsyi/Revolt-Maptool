#pragma once

class cMtlTex;
class cMesh 
{
public:
//	ID3DXMesh*
	LPD3DXMESH m_pMesh;
	std::map<std::string, cMtlTex*> m_mapMtlTex;
	std::vector<cMtlTex*> m_vecMtlTex;

//	std::vector<D3DMATERIAL9> m_vecMaterial;
//	std::vector<LPDIRECT3DTEXTURE9> m_vecTexture;

//	std::string m_pXFileMesh;
//	std::vector<cMtlTex*> m_vecMtlTex;

public:
	void Destory();
	void Render();

	cMesh();
	~cMesh();

//	void LoadMeshXFile(std::string folder, std::string name);
	void LoadMeshObjLoder(std::string folder, std::string name);
};

