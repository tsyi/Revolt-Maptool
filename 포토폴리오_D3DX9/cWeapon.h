#pragma once
#include "cTransform.h"

enum E_WeaponType { E_WT_즉시, E_WT_투사체 };
struct sWeaponData
{
	int m_damage;
	int m_speed;
	int m_lengthMin;
	int m_lengthMax;
	E_WeaponType m_Type;
	sWeaponData(E_WeaponType type, int damage, int speed, int lengthMin,int lengthMax)
	{
		m_Type		 = type;
		m_damage	 = damage;
		m_speed		 = speed;
		m_lengthMin	 = lengthMin;
		m_lengthMax	 = lengthMax;
	}
};
class cCharacter;

class cWeapon :public cTransform
{
public:
	cWeapon();
	~cWeapon();
protected:
	sWeaponData* m_pWeaponData;

	cCharacter* m_attacker;
	cCharacter* m_target;

	float m_damage;
	int m_weaponLevel;

public:
	virtual void Setup();
	virtual void SetTarget(cCharacter* attacker, cCharacter* target);
	virtual void Destory();

	virtual void Update();
	virtual void InputDamage();

	virtual void Render();

	virtual int GetWeaponValue();
};

