#pragma once

class cMtlTex 
{
private:
	SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Material);
	SYNTHESIZE_ADD_REF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);
	SYNTHESIZE(int, m_nID, MtlTexID);
public:
	cMtlTex(void);
	~cMtlTex(void);
};

