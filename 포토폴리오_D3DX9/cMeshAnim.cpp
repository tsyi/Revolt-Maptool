#include "StdAfx.h"
#include "cMeshAnim.h"
#include "cSkinnedMesh.h"
#include "cOBB.h"
#include "cCharacter.h"

cMeshAnim::cMeshAnim(void)
	: m_pSkinnedMesh(NULL)
	, m_pOBB(NULL)	/// : OBB
{
	m_isController = false;
}


cMeshAnim::~cMeshAnim(void)
{
	cCharacter::Release();
	SAFE_DELETE(m_pSkinnedMesh);
}

void cMeshAnim::Setup(bool isController)
{
	m_isController = isController;
	m_pSkinnedMesh = new cSkinnedMesh("Zealot/", "zealot.X");
	m_pSkinnedMesh->SetRandomTrackPosition();

	/// >> : OBB
	{
		m_pOBB = new cOBB;
		//m_pOBB->Setup(m_pSkinnedMesh);
	}
}

void cMeshAnim::Update(iMap* pMap)
{
	if(m_isController)cCharacter::Update();

	/// >> : OBB
	{
		if (m_pOBB)
		{
		//	m_pOBB->Update((m_isController ?cCharacter::GetWorldMatrix():NULL));
		}
	}
}

void cMeshAnim::Render(D3DCOLOR c)
{
	cTransform::Render();
	m_pSkinnedMesh->SetTransform(cCharacter::GetWorldMatrix());
	m_pSkinnedMesh->UpdateAndRender();
	m_pSkinnedMesh->SetAnimationIndex(3);
	/// >> : OBB
	{
		if (m_pOBB)	m_pOBB->OBBBox_Render(c);
	}
}
