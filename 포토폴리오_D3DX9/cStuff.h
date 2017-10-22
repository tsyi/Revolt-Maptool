#pragma once
#include "cObject.h"

class cStuff : public cObject
{
public:
	cStuff();
	~cStuff();

	void Update()
	{
		cObject::Update();
	}
	void Render()
	{
		cObject::Render();
	}
	void SetMeshBox()
	{
		D3DXCreateBox(MgrD3DDevice, 1, 1, 1, &(GetMeshData()->m_pMesh), NULL);

		D3DMATERIAL9 material;
		ZeroMemory(&material, sizeof(D3DMATERIAL9));
		D3DXCOLOR materColor = D3DXCOLOR(0.3f, 1.0f, 0.8f, 1.0f);
		material.Ambient = materColor;
		material.Diffuse = materColor;
		material.Specular = materColor;

		cMtlTex* mtlText = new cMtlTex;

		mtlText->SetMaterial(material);
		mtlText->SetMtlTexID(0);

		GetMeshData()->m_vecMtlTex.push_back(mtlText);
	}
};