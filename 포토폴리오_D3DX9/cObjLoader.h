#pragma once

class cMtlTex;
class cMesh;

class cObjLoader
{
private:
	std::map<std::string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader(void);
	~cObjLoader(void);

	static void LoadMesh(OUT cMesh* pMesh, IN std::string szFolder, IN std::string szFile);
//	static void LoadMesh(OUT cMesh* pMesh, IN char* szFolder, IN char* szFile);

	static void LoadMtlLib(std::map<std::string, cMtlTex*>* mtlTex, std::string szFolder, std::string szFile);
	static void LoadSurface(OUT std::vector<D3DXVECTOR3>& vecSurface,
		IN std::string szFolder, IN std::string szFile,	IN D3DXMATRIXA16* pmat = NULL);
};

