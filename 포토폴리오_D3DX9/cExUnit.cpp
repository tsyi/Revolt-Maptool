#include "stdafx.h"
#include "cExUnit.h"
#include "cSkinnedMesh.h"
//#include "cWeapon.h"
//#include "cAmour.h"
#include "cOBB.h"


cExUnit::cExUnit()
	: m_pSkinnedMesh(NULL)
//	, m_pWeapon(NULL)
//	, m_pAmour(NULL)
	, m_animTag(E_ANIM_TESTUNIT_NONE)
	, m_pOBB(NULL)
{
}


cExUnit::~cExUnit()
{
}

void cExUnit::Setup()
{
	cCharacter::Setup(0, "TestUnit_Zealot");

	m_pSkinnedMesh = new cSkinnedMesh("Object/Unit/","zealot.X");
//	LoadMesh("Object/Unit/공성전차/", "Tank.X"); //-> Anim 가 없는 매쉬일 경우
	m_pSkinnedMesh->SetRandomTrackPosition();

	m_bassData = new sCharacterBassDate(
		E_Unit_Pos::E_U_GROUND,
		E_Unit_Type::E_T_BIONICS,
		E_Unit_Size::E_S_SMALL,
		E_Unit_AttackType::E_L_근접,
		E_Unit_AttackTarget::E_ATg_GROUND,
		100, 100, 0, 3, 1, E_UnitCollider::E_C_지상);

//	m_pWeapon = new cWeapon();
//	m_pAmour = new cAmour();
}

void cExUnit::LoadMesh(std::string forder, std::string file)
{
	m_pSkinnedMesh = new cSkinnedMesh(forder, file);
	m_pSkinnedMesh->SetRandomTrackPosition();
}

void cExUnit::SetAnim(E_UnitAnimTag animTag)
{
	m_animTag = animTag;
}

void cExUnit::Destory()
{
	cCharacter::Release();
	SAFE_DELETE(m_pSkinnedMesh);
}

void cExUnit::Update()
{
	cCharacter::Update();

	static int a = 0;
	
	if (MgrInput->IsKeyDown('A'))
	{
		m_animTag = (E_UnitAnimTag)(a % (int)E_ANIM_TESTUNIT_END);
		a++;
		m_pSkinnedMesh->SetAnimationIndexBlend((int)m_animTag);
	}

	D3DXMATRIXA16 matR, matT;
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);

	D3DXVECTOR3 vPosition = m_position;

	D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationQuaternion(&matR, &cTransform::GetQuaternion());
	m_matWorld = matR * matT;
}

void cExUnit::Render()
{
	cTransform::Render();
	m_pSkinnedMesh->SetTransform(cCharacter::GetWorldMatrix());
	m_pSkinnedMesh->UpdateAndRender();
}
