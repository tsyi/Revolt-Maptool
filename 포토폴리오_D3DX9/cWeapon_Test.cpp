#include "stdafx.h"
#include "cWeapon_Test.h"
#include "cSkinnedMesh.h"
#include "cCharacter.h"
#include "cAmour.h"

cWeapon_Test::cWeapon_Test()
{
}


cWeapon_Test::~cWeapon_Test()
{
}

void cWeapon_Test::Setup(E_WeaponType type, int damage, int speed, int lengthMin, int lengthMax)
{
	cWeapon::Setup();

	m_pWeaponData = new sWeaponData(type, damage, speed, lengthMin, lengthMax);
	m_pSkinnedMesh = NULL;
	collide = false;

	addAmourDemage_Light = 0;
	addAmourDemage_Heavy = 0;
	addSizeDemage_Small = 0;
	addSizeDemage_Medium = 0;
	addSizeDemage_Big = 0;
	addSizeDemage_Building = 0;
	addTypeDemage_Bionics = 0;
	addTypeDemage_Machine = 0;
}

void cWeapon_Test::LoadMesh(std::string forder, std::string file)
{
	m_pSkinnedMesh = new cSkinnedMesh(forder, file);
	if (m_pSkinnedMesh) m_pSkinnedMesh->SetRandomTrackPosition();
}

void cWeapon_Test::Update()
{
	cWeapon_Test::Update();
	if (collide)
	{
		InputDamage();
	}
}

void cWeapon_Test::InputDamage()
{
	cWeapon::InputDamage();
	float damage = GetWeaponValue();
	switch (m_target->GetCharacterData()->me_Type)
	{
	case E_T_BIONICS: damage += addTypeDemage_Bionics; break;
	case E_T_MACHINE: damage += addTypeDemage_Machine; break;
	}

	switch (m_target->GetCharacterData()->me_Size)
	{
	case E_S_SMALL:		damage += addSizeDemage_Small; break;
	case E_S_MEDIUM:	damage += addSizeDemage_Medium; break;
	case E_S_BIG:		damage += addSizeDemage_Big; break;
	case E_S_BUILDING:	damage += addSizeDemage_Building; break;
	}

	switch (m_target->GetAmour()->GetAmourType())
	{
	case E_A_LIGHT:		damage += addAmourDemage_Light; break;
	case E_A_HEAVY:		damage += addAmourDemage_Heavy; break;
	}
	damage -= m_target->GetAmour()->GetAmourValue();

	m_target->GetCharacterData()->HitDamage(damage);
}

void cWeapon_Test::Render()
{
	if (m_pSkinnedMesh) m_pSkinnedMesh->UpdateAndRender();
}

void cWeapon_Test::Destory()
{
	
}