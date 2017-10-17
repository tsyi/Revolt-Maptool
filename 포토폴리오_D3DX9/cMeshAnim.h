#pragma once

#include "cCharacter.h"

class cSkinnedMesh;
class cOBB;

class cMeshAnim :public cCharacter
{
protected:
//	SYNTHESIZE_ADD_REF(cCharacter*, m_pCharacterController, CharacterController);
	cSkinnedMesh*	m_pSkinnedMesh;
	cOBB*			m_pOBB;	/// >> : OBB

	bool m_isController;
public:
	cMeshAnim(void);
	virtual ~cMeshAnim(void);

	void Setup(bool isController);
	void Update(iMap* pMap);
	void Render(D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255));

	/// >> : OBB
	cOBB* GetOBB()
	{
		return m_pOBB;
	}
};

