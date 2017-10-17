#pragma once
#include "cCollided.h"

class cOBB : public cCollided
{
private:
	D3DXVECTOR3 m_vOrgCenterPos;		
	D3DXVECTOR3 m_vOrgAxisDir[3];		
	
	D3DXVECTOR3 m_vCenterPos;		/// :  �ڽ� �߽�r ��ǥ
	D3DXVECTOR3 m_vAxisDir[3];		/// :  �ڽ��� ������ �� ���� ��������
	float		m_fAxisLen[3];		/// :  �ڽ��� ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	float		m_fAxisHalfLen[3];

	std::vector<ST_PC_VERTEX> m_verVC;

	D3DCOLOR CollisionColor;
	D3DCOLOR DefaultColor;
public:
	cOBB(void);
	~cOBB(void);

	void Setup(cTransform* objTr);
	void Update(cTransform* pmatTr);
	static bool IsCollision(cOBB* pOBB1, cOBB* pOBB2);


	void SetCollisionColor(D3DCOLOR c) { CollisionColor = c; }
	void OBBBox_Render(D3DCOLOR c);
	void OBBBox_Render(bool isCollied);


	void PushBox(cOBB* pOBB1, float fObb1, cOBB* pOBB2, float fObb2, float deep);


};

