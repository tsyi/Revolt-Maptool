#pragma once
#include "cCharacter.h"

class cOBB;
class cSkinnedMesh;
//class cWeapon;
//class cAmour;

class cExUnit :public cCharacter
{
private:
	cSkinnedMesh*		m_pSkinnedMesh;
//	cWeapon*			m_pWeapon;
//	cAmour*				m_pAmour;
	cOBB*				m_pOBB;

	E_UnitAnimTag		m_animTag;
public:
	cExUnit();
	~cExUnit();

	void Setup();
	void LoadMesh(std::string forder, std::string file);

	//	sCharacterBassDate* GetCharacterData() {		return m_bassData;	}
	void SetAnim(E_UnitAnimTag animTag);
	void Destory();
	void Update();
	void Render();
};

