#include "stdafx.h"
#include "cTransform.h"


cTransform::cTransform()
	: m_position(0, 0, 0)
	, m_size(1, 1, 1)
	//	, m_direction(0, 0, 1)
	, m_top(0, 1, 0)
{
	//	D3DXVec3Normalize(&m_direction, &m_direction);
	D3DXMatrixIdentity(&m_matWorld);
	DebugTr = true;

	front = D3DXVECTOR3(0, 0, 1);
	right = D3DXVECTOR3(1, 0, 0);
	up = D3DXVECTOR3(0, 1, 0);

	SetQuaternionToFloat(0, 0, 0);
	//	ArrowVector();
}

cTransform::~cTransform()
{
}

D3DXVECTOR3 cTransform::GetDirection()
{
	//	D3DXVec3Normalize(&m_direction, &m_direction);
	return front;
}

D3DXVECTOR3 cTransform::GetFrontVec()
{
	return front;
}

D3DXVECTOR3 cTransform::GetRightVec()
{
	return right;
}

D3DXVECTOR3 cTransform::GetUpVec()
{
	return up;
}

//void cTransform::SetTopVector()
//{
//	D3DXVECTOR2 top = D3DXVECTOR2(0, 1);
//
//	D3DXVECTOR2 dir = D3DXVECTOR2((m_direction.x > m_direction.z ? m_direction.x : m_direction.y), m_direction.y);
//	D3DXVec2Normalize(&dir, &dir);
//
//
//}

void cTransform::SetPosition(NxVec3 NxPos)
{
	m_position.x = NxPos.x; m_position.y = NxPos.y; m_position.z = NxPos.z;
}

void cTransform::ChangeMatrix()
{
}

void cTransform::ArrowVector()
{
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationQuaternion(&matR, &m_Quaternion);
	D3DXVec3TransformNormal(&front, &D3DXVECTOR3(0, 0, 1), &matR);
	D3DXVec3TransformNormal(&right, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXVec3TransformNormal(&up, &D3DXVECTOR3(0, 1, 0), &matR);

	D3DXVec3Normalize(&front, &front);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);

//	m_matR = matR;
}

void cTransform::Update()
{
	ArrowVector();
}



void cTransform::Render()
{
//	D3DXMatrixRotationQuaternion(&m_matWorld, &m_Quaternion);

	if (DebugTr)
	{
		ST_PC_VERTEX v;

		std::vector<ST_PC_VERTEX>	vecVertex;

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.c = D3DCOLOR_XRGB(255, 255, 255);
		v.p = m_position; vecVertex.push_back(v);
		v.p = m_position + GetFrontVec() * 5; vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(0, 255, 0);
		v.p = m_position; vecVertex.push_back(v);
		v.p = m_position + GetUpVec() * 5; vecVertex.push_back(v);

		v.c = D3DCOLOR_XRGB(255, 0, 0);
		v.p = m_position; vecVertex.push_back(v);
		v.p = m_position + GetRightVec() * 5; vecVertex.push_back(v);


		LPDIRECT3DVERTEXBUFFER9 m_pVB;

		int m_nNumLine = vecVertex.size() / 2;
		g_pD3DDevice->CreateVertexBuffer(vecVertex.size() * sizeof(ST_PC_VERTEX),
			0, ST_PC_VERTEX::FVF, D3DPOOL_MANAGED,
			&m_pVB, NULL);
		ST_PC_VERTEX *pV = NULL;
		m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
		memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PC_VERTEX));
		m_pVB->Unlock();



		g_pD3DDevice->SetTexture(0, NULL);
	//	D3DXMatrixRotationQuaternion(&m_matWorld, &m_Quaternion);
		//	m_matWorld ;

		D3DXMATRIXA16 matIdentity;
		D3DXMatrixIdentity(&matIdentity);

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

		{
			g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));
			g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nNumLine);
		}


		SAFE_RELEASE(m_pVB);
	}
}
