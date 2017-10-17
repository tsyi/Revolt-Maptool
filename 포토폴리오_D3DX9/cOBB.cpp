#include "stdafx.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"
#include <iostream>
#include "iCar.h"

cOBB::cOBB(void)
{
	DefaultColor = D3DCOLOR_XRGB(255, 255, 0);
	CollisionColor = D3DCOLOR_XRGB(255, 0, 0);

}


cOBB::~cOBB(void)
{
}

void cOBB::Setup(cTransform* objTr)
{
	cTransform::SetTransform(objTr);
	cTransform* pTr = cTransform::GetTransform();

	D3DXVECTOR3 vMin = pTr->GetPosition() - (pTr->GetSize() / 2.f);
	D3DXVECTOR3 vMax = vMin + pTr->GetSize();

	m_vOrgCenterPos = D3DXVECTOR3(0, 0, 0);

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(objTr->GetSize().x);
	m_fAxisLen[1] = fabs(objTr->GetSize().y);
	m_fAxisLen[2] = fabs(objTr->GetSize().z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	std::vector<D3DXVECTOR3> vecPos;
	vecPos.resize(8);
	vecPos[0] = D3DXVECTOR3(-0.5f,-0.5f,-0.5f);
	vecPos[1] = vecPos[0] + D3DXVECTOR3(0.5f, 0, 0);
	vecPos[2] = vecPos[0] + D3DXVECTOR3(0, 0.5f, 0);
	vecPos[3] = vecPos[0] + D3DXVECTOR3(0.5f, 0.5f, 0);

	vecPos[4] = vecPos[0] + D3DXVECTOR3(0, 0, 0.5f);
	vecPos[5] = vecPos[4] + D3DXVECTOR3(0.5f, 0, 0);
	vecPos[6] = vecPos[4] + D3DXVECTOR3(0, 0.5f, 0);
	vecPos[7] = vecPos[4] + D3DXVECTOR3(0.5f, 0.5f, 0);

	for (int i = 0; i < vecPos.size(); i++)
	{
		D3DXVec3TransformNormal(&vecPos[i],	&vecPos[i], &cTransform::GetMatrixWorld());
	}

	ST_PC_VERTEX vc;
	m_verVC.resize(24);
	//	vc.c = c;
	vc.p = vecPos[0];	m_verVC[0] = vc;
	vc.p = vecPos[1];	m_verVC[1] = vc;
	vc.p = vecPos[1];	m_verVC[2] = vc;
	vc.p = vecPos[3];	m_verVC[3] = vc;
	vc.p = vecPos[3];	m_verVC[4] = vc;
	vc.p = vecPos[2];	m_verVC[5] = vc;
	vc.p = vecPos[2];	m_verVC[6] = vc;
	vc.p = vecPos[0];	m_verVC[7] = vc;

	vc.p = vecPos[4];	m_verVC[8] = vc;
	vc.p = vecPos[5];	m_verVC[9] = vc;
	vc.p = vecPos[5];	m_verVC[10] = vc;
	vc.p = vecPos[7];	m_verVC[11] = vc;
	vc.p = vecPos[7];	m_verVC[12] = vc;
	vc.p = vecPos[6];	m_verVC[13] = vc;
	vc.p = vecPos[6];	m_verVC[14] = vc;
	vc.p = vecPos[4];	m_verVC[15] = vc;

	vc.p = vecPos[0];	m_verVC[16] = vc;
	vc.p = vecPos[4];	m_verVC[17] = vc;
	vc.p = vecPos[1];	m_verVC[18] = vc;
	vc.p = vecPos[5];	m_verVC[19] = vc;
	vc.p = vecPos[3];	m_verVC[20] = vc;
	vc.p = vecPos[7];	m_verVC[21] = vc;
	vc.p = vecPos[2];	m_verVC[22] = vc;
	vc.p = vecPos[6];	m_verVC[23] = vc;
}

void cOBB::Update(cTransform* pmatTr)
{
	D3DXMATRIXA16 matWorld = cTransform::GetMatrixWorld();
	if (pmatTr) matWorld = pmatTr->GetMatrixWorld();

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(
			&m_vAxisDir[i],
			&m_vOrgAxisDir[i],
			&matWorld);
	}

	D3DXVec3TransformCoord(
		&m_vCenterPos,
		&m_vOrgCenterPos,
		&matWorld);
}



bool cOBB::IsCollision(cOBB* pOBB1, cOBB* pOBB2)
{

	if (pOBB1 == NULL || pOBB2 == NULL) return false;
	float cos[3][3];
	float absCos[3][3];
	float dist[3];
	float r0, r1, r;

	const float cutOff = 0.999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 D = pOBB2->m_vCenterPos - pOBB1->m_vCenterPos;

	/// :  OBB1 의 3 축 (a) 을 기준으로 OBB2 의 3축(b) 을 검사
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			/// : OBB_A a 축과 OBB_B b 축의 cos 값
			cos[a][b] = D3DXVec3Dot(&pOBB1->m_vAxisDir[a], &pOBB2->m_vAxisDir[b]);

			/// : 위에서 구한 cos 값의 절대값
			absCos[a][b] = abs(cos[a][b]);

			/// : 한축이 직각이다 이러한경우 AABB 형태
			if (absCos[a][b] > cutOff)
				existsParallelPair = true;
		}


		/// : 사각형 끼리의 중심 거리 벡터를 OBB1 의 a 축으로 투영한 거리
		dist[a] = D3DXVec3Dot(&pOBB1->m_vAxisDir[a], &D);
		r = abs(dist[a]);

		/// : OBB1 의 a 축의 길이
		r0 = pOBB1->m_fAxisHalfLen[a];

		/// : OBB2 의 꼭지점이 OBB1 의a 축으로 투영된 길이
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[a][0] +
			pOBB2->m_fAxisHalfLen[1] * absCos[a][1] +
			pOBB2->m_fAxisHalfLen[2] * absCos[a][2];

		if (r > r0 + r1)
			return false;
	}

	/// OBB2 의 3축을 기준으로 OBB1 의 3축을 검사
	for (int b = 0; b < 3; b++)
	{
		r = abs(D3DXVec3Dot(&pOBB2->m_vAxisDir[b], &D));
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[0][b] +
			pOBB1->m_fAxisHalfLen[1] * absCos[1][b] +
			pOBB1->m_fAxisHalfLen[2] * absCos[2][b];

		r1 = pOBB2->m_fAxisHalfLen[b];

		if (r > r0 + r1)
			return false;
	}


	if (existsParallelPair) return true;

	/////////////////////////////////////////////////////////////////
	{	// : 3 x 3 개 
		r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][1];
		if (r > r0 + r1) return false;

		r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][0];
		if (r > r0 + r1) return false;

		r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][1] + pOBB2->m_fAxisHalfLen[1] * absCos[1][0];
		if (r > r0 + r1) return false;

		/////////////////////////////////////////////////////////////////	 

		r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][0] + pOBB1->m_fAxisHalfLen[1] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][1];
		if (r > r0 + r1) return false;

		r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][1] + pOBB1->m_fAxisHalfLen[1] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][0];
		if (r > r0 + r1) return false;

		r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][2] + pOBB1->m_fAxisHalfLen[1] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][1] + pOBB2->m_fAxisHalfLen[1] * absCos[2][0];
		if (r > r0 + r1) return false;

		/////////////////////////////////////////////////////////////////

		r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[1][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][1];
		if (r > r0 + r1) return false;

		r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[1][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][0];
		if (r > r0 + r1) return false;

		r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[1][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][1] + pOBB2->m_fAxisHalfLen[1] * absCos[0][0];
		if (r > r0 + r1) return false;
	} // << : 

	return true;
}





/// >> : 박스 그리기.
void cOBB::OBBBox_Render(D3DCOLOR c)
{

	for (int i = 0; i < m_verVC.size(); i++)
	{
		m_verVC[i].c = c;
	}

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &cTransform::GetMatrixWorld());
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	//	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PC_VERTEX));

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		m_verVC.size() / 2,
		&m_verVC[0],
		sizeof(ST_PC_VERTEX));
}

void cOBB::OBBBox_Render(bool isCollied)
{
	if (isCollied) OBBBox_Render(DefaultColor);
	else OBBBox_Render(CollisionColor);
}

void cOBB::PushBox(cOBB * pOBB1, float fObb1, cOBB * pOBB2, float fObb2, float deep)
{
	if (cOBB::IsCollision(pOBB1, pOBB2))
	{
		D3DXVECTOR3 dir = pOBB2->m_vCenterPos - pOBB1->m_vCenterPos;
		D3DXVec3Normalize(&dir, &dir);
	}
}