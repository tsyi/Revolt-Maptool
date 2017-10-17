#pragma once
#include "cUIObject.h"

struct sUIImage
{

};


class cUIImage : public cUIObject
{
public:
	cUIImage();
	~cUIImage();
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
	SYNTHESIZE(D3DXVECTOR3, m_anchor, Anchor);
	SYNTHESIZE(D3DXVECTOR2, m_scale, Scale);

	D3DCOLOR bassColor;
public:
	virtual void SetTexture(char* szFullPath);
	virtual void Destory();
	virtual void Update();
	virtual void Render();

	void SetScale(float x, float y) { m_scale.x = x; m_scale.y = y; }
	void SetBassColor(D3DCOLOR c) { bassColor = c; }
	virtual void cUIImage::SetAnchor(float x, float y, float z)
	{
		m_anchor.x = x;
		m_anchor.y = y;
		m_anchor.z = z;
	}

};
