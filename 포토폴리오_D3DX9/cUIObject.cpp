#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_position(0, 0, 0)
	, m_pParent(NULL)
	, m_stSize(0, 0)
	, m_tag(E_UI_NONE)
	, m_isShow(true)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cUIObject::~cUIObject()
{
}

void cUIObject::SetSize(float x, float y)
{
	SetSize(D3DXVECTOR2(x, y));
}

void cUIObject::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void cUIObject::AddPosition(D3DXVECTOR3 addPos)
{
	m_position += addPos;
}

void cUIObject::AddPosition(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}

//void cUIObject::SetTexture(char * szFullPath)
//{
//	D3DXIMAGE_INFO stImageInfo;
//	m_pTexture = g_pTextureManager->GetTexture(szFullPath, &stImageInfo);
//
//	m_stSize.nWidth = stImageInfo.Width;
//	m_stSize.nHeight = stImageInfo.Height;
//}


void cUIObject::AddChild(cUIObject * pChild)
{
	pChild->SetParent(this);
	m_vecChild.push_back(pChild);
}

void cUIObject::Update()
{
	if (!m_isShow) return;
	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = m_position.x;
	m_matWorld._42 = m_position.y;
	m_matWorld._43 = m_position.z;

	if (m_pParent)
	{
		m_matWorld._41 += m_pParent->m_matWorld._41;
		m_matWorld._42 += m_pParent->m_matWorld._42;
		m_matWorld._43 += m_pParent->m_matWorld._43;
	}

	for each(auto c in m_vecChild)
	{
		c->Update();
	}
}

void cUIObject::Render()
{
	if (!m_isShow) { return; }
	for each(auto c in m_vecChild)
	{
		c->Render();
	}
	
}

void cUIObject::Destory()
{
	m_isShow = true;
	for each(auto c in m_vecChild)
	{
		c->Destory();
	}
	m_vecChild.clear();
//	this->Release();
}

cUIObject * cUIObject::FindChildByTag(eUITag tag)
{
	if (!m_isShow) return nullptr;
	if (m_tag == tag)
	{
		return this;
	}
	for each(auto c in m_vecChild)
	{
		cUIObject* p = c->FindChildByTag(tag);
		if (p) return p;
	}
	return nullptr;
}
