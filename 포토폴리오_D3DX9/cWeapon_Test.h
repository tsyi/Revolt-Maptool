#pragma once
#include "cWeapon.h"
class cSkinnedMesh;

class cWeapon_Test : public cWeapon
{
	//cCharacter* m_attacker;
	cSkinnedMesh* m_pSkinnedMesh;
	bool collide;


	//추가데미지값
	int addAmourDemage_Light;
	int addAmourDemage_Heavy;

	int addSizeDemage_Small;
	int addSizeDemage_Medium;
	int addSizeDemage_Big;
	int addSizeDemage_Building;

	int addTypeDemage_Bionics;
	int addTypeDemage_Machine;


public:
	cWeapon_Test();
	~cWeapon_Test();

	void Setup(E_WeaponType type, int damage, int speed, int lengthMin, int lengthMax);
	void LoadMesh(std::string forder, std::string file);
	void Update();
	void InputDamage();
	void Render();
	void Destory();

};

