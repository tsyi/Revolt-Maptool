#include "stdafx.h"
#include "cTransform.h"


cTransform::cTransform()
	: m_position(0, 0, 0)
	, m_size(1, 1, 1)
	, m_direction(0, 0, 1)
	//, m_top(0, 1, 0)
{
	D3DXQuaternionIdentity(&m_Quaternion);
	DebugTr = true;

	front = D3DXVECTOR3(0, 0, 1);
	right = D3DXVECTOR3(1, 0, 0);
	up = D3DXVECTOR3(0, 1, 0);

	SetQuaternion(front);
	//	ArrowVectorSetting();
}

cTransform::~cTransform()
{
}

void cTransform::Update()
{
	ArrowVectorSetting();
}

void cTransform::Render()
{
	if (DebugTr)
	{
		ST_PC_VERTEX v;

		std::vector<ST_PC_VERTEX>	vecVertex;

		v.c = D3DCOLOR_XRGB(0, 0, 255);
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

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &GetMatrix());
		g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));
		g_pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_nNumLine);

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		SAFE_RELEASE(m_pVB);
	}
}

D3DXVECTOR3 cTransform::GetDirection()
{
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

void cTransform::SetNxF32(NxF32 * mtl)
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	matWorld._11 = mtl[0];
	matWorld._12 = mtl[1];
	matWorld._13 = mtl[2];
	matWorld._21 = mtl[3];
	matWorld._22 = mtl[4];
	matWorld._23 = mtl[5];
	matWorld._31 = mtl[6];
	matWorld._32 = mtl[7];
	matWorld._33 = mtl[8];

	cTransform::SetQuaternion(matWorld);
}

void cTransform::SetNxVec3(NxVec3 NxPos)
{
	m_position.x = NxPos.x; m_position.y = NxPos.y; m_position.z = NxPos.z;
}

NxVec3 cTransform::GetNxVec3()
{
	NxVec3 vec;
	vec.x = GetPosition().x;
	vec.y = GetPosition().y;
	vec.z = GetPosition().z;
	return vec;
}

NxF32 * cTransform::GetNxF32()
{
	NxF32 mtl[] = { 1,0,0,0,1,0,0,0,1 };
	D3DXMATRIXA16 mat = GetMatrix(false, true, true);

	mtl[0] = mat._11;
	mtl[1] = mat._12;
	mtl[2] = mat._13;
	mtl[3] = mat._21;
	mtl[4] = mat._22;
	mtl[5] = mat._23;
	mtl[6] = mat._31;
	mtl[7] = mat._32;
	mtl[8] = mat._33;

	return mtl;
}

void cTransform::ChangeMatrix()
{
}

void cTransform::ArrowVectorSetting()
{
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationQuaternion(&matR, &m_Quaternion);
	D3DXVec3TransformNormal(&front, &D3DXVECTOR3(0, 0, 1), &matR);
	D3DXVec3TransformNormal(&right, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXVec3TransformNormal(&up, &D3DXVECTOR3(0, 1, 0), &matR);

	D3DXVec3Normalize(&front, &front);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);

	SetDirection(front);
}

