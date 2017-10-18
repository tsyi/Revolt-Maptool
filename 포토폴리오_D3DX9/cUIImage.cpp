#include "stdafx.h"
#include "cUIImage.h"


cUIImage::cUIImage()
	:m_anchor(0.5, 0.5, 0)
	, m_scale(1, 1)
{
	bassColor = D3DCOLOR_ARGB(255, 255, 255, 255);
}


cUIImage::~cUIImage()
{
}

void cUIImage::SetTexture(char * szFullPath)
{
	D3DXIMAGE_INFO stImageInfo;
	m_pTexture = MgrTexture->GetTexture(szFullPath, &stImageInfo);

	SetSize(stImageInfo.Width, stImageInfo.Height);
}

void cUIImage::Destory()
{
	SAFE_RELEASE(m_pTexture);
	cUIObject::Destory();
}

void cUIImage::Update()
{
	if (!m_isShow) { return; }
	cUIObject::Update();
}

void cUIImage::Render()
{
	if (!m_isShow) return;

	MgrUI->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DXMATRIXA16 matS;
	float _x, _y, _z;
	_x = m_matWorld._41;
	_y = m_matWorld._42;
	_z = m_matWorld._43;

	D3DXMatrixScaling(&matS, GetScale().x, GetScale().y, 1);
	m_matWorld *= matS;
	m_matWorld._41 = _x;
	m_matWorld._42 = _y;
	m_matWorld._43 = _z;
	MgrUI->GetSprite()->SetTransform(&m_matWorld);

	float a_x = m_anchor.x * GetSize().x;
	float a_y = m_anchor.y * GetSize().y;

	RECT rc;
	SetRect(&rc, 0, 0, GetSize().x, GetSize().y);
	MgrUI->GetSprite()->Draw(m_pTexture, &rc,
		&D3DXVECTOR3(a_x, a_y, 0),
		&D3DXVECTOR3(0, 0, 0),
		bassColor);


	MgrUI->GetSprite()->End();

	cUIObject::Render();
}
