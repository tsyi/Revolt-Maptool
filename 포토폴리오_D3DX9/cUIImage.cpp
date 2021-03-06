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

	if(m_pTexture) SetSize(stImageInfo.Width, stImageInfo.Height);
	else SetSize(0, 0);
}

void cUIImage::Destory()
{
	m_pTexture = NULL;
	cUIObject::Destory();
}

void cUIImage::Update()
{
	if (!GetShow()) return;
	if (m_pTexture)
	{

	}
	cUIObject::Update();
}

void cUIImage::Render()
{
	if (!m_isShow) return;
	if (m_pTexture)
	{
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

	//	MgrUI->GetSprite()->

		float a_x = m_anchor.x * GetSize().x;
		float a_y = m_anchor.y * GetSize().y;

		RECT rc;
		SetRect(&rc, 0, 0, GetSize().x, GetSize().y);
		MgrUI->GetSprite()->Draw(m_pTexture, &rc,
			&D3DXVECTOR3(a_x, a_y, 0),
			&D3DXVECTOR3(0, 0, 0),
			bassColor);


		MgrUI->GetSprite()->End();
	}
	cUIObject::Render();
}
