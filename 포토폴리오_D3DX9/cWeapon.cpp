#include "stdafx.h"
#include "cWeapon.h"
#include "cCharacter.h"

cWeapon::cWeapon()
{
}


cWeapon::~cWeapon()
{
}

void cWeapon::Setup()
{
	m_target = nullptr;
	m_attacker = nullptr;
}

void cWeapon::SetTarget(cCharacter * attacker, cCharacter * target)
{
	m_target = target;
	m_attacker = attacker;
}

void cWeapon::Destory()
{
}

void cWeapon::Update()
{
}

void cWeapon::InputDamage()
{	
	
}

void cWeapon::Render()
{
}

int cWeapon::GetWeaponValue()
{
	return m_pWeaponData->m_damage + m_weaponLevel;
}
