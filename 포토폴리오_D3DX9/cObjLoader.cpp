#include "stdafx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cMesh.h"
#include <fstream>

cObjLoader::cObjLoader()
{
}


cObjLoader::~cObjLoader()
{
}


void cObjLoader::LoadMesh(OUT cMesh * pMesh, IN std::string szFolder, IN std::string szFile)
{
	// : vertex, texture, normal
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<ST_PNT_VERTEX> vecVertex;
	std::vector<DWORD> vecAttribute;         //: <<
	std::map<std::string, cMtlTex*> mapMtlTex;

	std::string sFullPath = szFolder + std::string("/") + szFile;


	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	std::string sMtlName;

	if (fp)
	{
		while (true)
		{
			if (feof(fp)) break;

			char szTemp[1024];
			fgets(szTemp, 1024, fp);
			if (szTemp[0] == '#')
			{
				continue;
			}
			else if (szTemp[0] == 'm')
			{
				char szMtlFile[1024];
				sscanf_s(szTemp, "%*s %s", szMtlFile, 1024);
				LoadMtlLib(&(mapMtlTex), szFolder, szMtlFile);
			}
			else if (szTemp[0] == 'g')
			{
				//	if (!vecVertex.empty())
				//	{
				//		// 삭제
				//	}
			}
			else if (szTemp[0] == 'v')
			{
				if (szTemp[1] == ' ')
				{
					float x, y, z;
					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
					vecV.push_back(D3DXVECTOR3(-x, y, z));
				}
				else if (szTemp[1] == 't')
				{
					float u, v;
					sscanf_s(szTemp, "%*s %f %f %*f", &u, &v);
					vecVT.push_back(D3DXVECTOR2(u, 1.0f - v));
				}
				else if (szTemp[1] == 'n')
				{
					float x, y, z;
					sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
					vecVN.push_back(D3DXVECTOR3(x, y, z));
				}
			}
			else if (szTemp[0] == 'u')
			{
				char szMtlName[1024];
				sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
				sMtlName = std::string(szMtlName);

			}
			else if (szTemp[0] == 'f')
			{
				int nIndex[3][3];
				sscanf_s(szTemp, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
					&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
					&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
					&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]);

				for (int i = 2; i >= 0; i--)
				{
					ST_PNT_VERTEX v;
					v.p = vecV[nIndex[i][0] - 1];
					v.t = vecVT[nIndex[i][1] - 1];
					v.n = vecVN[nIndex[i][2] - 1];
					vecVertex.push_back(v);
				}

				vecAttribute.push_back((mapMtlTex)[sMtlName]->GetMtlTexID());
			}
		} // : << while 끝
	}
	else
	{
		MessageBoxA(g_hWnd, "Stuff 경로를 찾을 수 없습니다.", "", MB_OK);
	}
	if(fp) fclose(fp);


	pMesh->m_vecMtlTex.resize(mapMtlTex.size());
	for each(auto it in mapMtlTex)
	{
		pMesh->m_vecMtlTex[it.second->GetMtlTexID()] = it.second;
	}

	/*vecMtlTex.resize((pMesh->m_mapMtlTex).size());

	for each(auto it in (pMesh->m_mapMtlTex))
	{
		vecMtlTex[it.second->GetMtlTexID()] = it.second;
	}*/

	//	LPD3DXMESH newMesh = NULL;
	D3DXCreateMeshFVF(vecAttribute.size(),
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&(pMesh->m_pMesh));


	ST_PNT_VERTEX* pV = NULL;
	pMesh->m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], sizeof(ST_PNT_VERTEX) * vecVertex.size());
	pMesh->m_pMesh->UnlockVertexBuffer();

	WORD* pI = NULL;
	pMesh->m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);

	for (size_t i = 0; i < vecVertex.size(); i++)
	{
		pI[i] = i;
	}

	pMesh->m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	pMesh->m_pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttribute[0], sizeof(DWORD) * vecAttribute.size());
	pMesh->m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecVertex.size());
	pMesh->m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);
	pMesh->m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);
}


// >> : vertexbuffer
// >> : indexbuffer
// >> : attr buffer

void cObjLoader::LoadMtlLib(std::map<std::string, cMtlTex*>* mtlTex,
	std::string szFolder, std::string szFile)
{
	// material
	std::string sFullPath = szFolder + std::string("/") + szFile;

	FILE *fp;
	fopen_s(&fp, sFullPath.c_str(), "r");

	std::string sMtlName;

	int nCnt = 0;  // :<<

	while (true)
	{
		if (feof(fp)) break;
		int lineCol = 0;
		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		std::cout << szTemp[0] << std::endl;

		while (true)
		{
			if (szTemp[lineCol] == ' ' || szTemp[lineCol] == '\t')
			{
				lineCol++;
			}
			else break;
		}

		if (szTemp[lineCol] == '#')
		{
			continue;
		}
		else if (szTemp[lineCol] == 'n')
		{
			char szMtlName[1024];
			sscanf_s(szTemp, "%*s %s", szMtlName, 1024);
			sMtlName = std::string(szMtlName);

			if (mtlTex->find(sMtlName) == mtlTex->end())
			{
				(*mtlTex)[sMtlName] = new cMtlTex;

				//: >>
				(*mtlTex)[sMtlName]->SetMtlTexID(nCnt);
				nCnt++;
				// : <<
			}
		}
		else if (szTemp[lineCol] == 'K')
		{
			if (szTemp[lineCol + 1] == 'a')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				(*mtlTex)[sMtlName]->GetMaterial().Ambient.r = r;
				(*mtlTex)[sMtlName]->GetMaterial().Ambient.g = g;
				(*mtlTex)[sMtlName]->GetMaterial().Ambient.b = b;
				(*mtlTex)[sMtlName]->GetMaterial().Ambient.a = 1.0f;
			}
			if (szTemp[lineCol + 1] == 'd')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				(*mtlTex)[sMtlName]->GetMaterial().Diffuse.r = r;
				(*mtlTex)[sMtlName]->GetMaterial().Diffuse.g = g;
				(*mtlTex)[sMtlName]->GetMaterial().Diffuse.b = b;
				(*mtlTex)[sMtlName]->GetMaterial().Diffuse.a = 1.0f;
			}
			if (szTemp[lineCol + 1] == 's')
			{
				float r, g, b;
				sscanf_s(szTemp, "%*s %f %f %f", &r, &g, &b);
				(*mtlTex)[sMtlName]->GetMaterial().Specular.r = r;
				(*mtlTex)[sMtlName]->GetMaterial().Specular.g = g;
				(*mtlTex)[sMtlName]->GetMaterial().Specular.b = b;
				(*mtlTex)[sMtlName]->GetMaterial().Specular.a = 1.0f;
			}
		}
		else if (szTemp[lineCol] == 'd')
		{
			float d;
			sscanf_s(szTemp, "%*s %f", &d);
			(*mtlTex)[sMtlName]->GetMaterial().Power = d;
		}
		else if (szTemp[lineCol] == 'm')
		{
			char szTexFile[1024];
			sscanf_s(szTemp, "%*s %s", szTexFile, 1024);
			sFullPath = std::string(szFolder);
			sFullPath += (std::string("/") + std::string(szTexFile));

			LPDIRECT3DTEXTURE9 pTexture = MgrTexture->GetTexture(sFullPath);
			(*mtlTex)[sMtlName]->SetTexture(pTexture);
		}
	} // << while 끝
	fclose(fp);
}

void cObjLoader::LoadSurface(OUT std::vector<D3DXVECTOR3>& vecSurface,
	IN std::string szFolder, IN std::string szFile, IN D3DXMATRIXA16 * pmat)
{
	std::vector<D3DXVECTOR3> vecV;

	std::string sFullPath = szFolder + std::string("/") + szFile;

	FILE* fp;
	fopen_s(&fp, sFullPath.c_str(), "r");
	std::string sMtlName;
	while (true)
	{
		if (feof(fp)) break;

		char szTemp[1024];
		fgets(szTemp, 1024, fp);
		if (szTemp[0] == '#')
		{
			continue;
		}
		else if (szTemp[0] == 'm')
		{

		}
		else if (szTemp[0] == 'g')
		{

		}
		else if (szTemp[0] == 'v')
		{
			if (szTemp[1] == ' ')
			{
				float x, y, z;
				sscanf_s(szTemp, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
			else if (szTemp[1] == 't')
			{

			}
			else if (szTemp[1] == 't')
			{

			}
			else if (szTemp[1] == 'n')
			{

			}
		}
		else if (szTemp[0] == 'u')
		{

		}
		else if (szTemp[0] == 'f')
		{
			int nIndex[3];
			sscanf_s(szTemp, "%*s %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &nIndex[0], &nIndex[1], &nIndex[2]);
			for (int i = 0; i < 3; i++)
			{
				vecSurface.push_back(vecV[nIndex[i] - 1]);
			}
		}
	} // : << while 끝

	fclose(fp);

	if (pmat)
	{
		for (size_t i = 0; i < vecSurface.size(); i++)
		{
			D3DXVec3TransformCoord(&vecSurface[i], &vecSurface[i], pmat);
		}
	}
}