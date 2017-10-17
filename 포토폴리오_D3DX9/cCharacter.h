#pragma once

class iMap;
//class cMeshAnim;
//�� ���� Ÿ��

//�̵� ����
enum E_Unit_Pos { E_U_GROUND, E_U_FLY };

//����
enum E_Unit_Type { E_T_BIONICS, E_T_MACHINE };

//ũ��
enum E_Unit_Size { E_S_SMALL, E_S_MEDIUM, E_S_BIG, E_S_BUILDING };

//��������
enum E_Unit_AttackType { E_L_����, E_L_���Ÿ� };

//���ݴ��
enum E_Unit_AttackTarget { E_ATg_GROUND, E_T_����, E_T_���, E_T_���� };
//enum UnitType_AttType	{ �Ϲ�, ����, };

//���ְ� �浹 Ÿ��
enum E_UnitCollider { E_C_����, E_C_����, E_C_���, E_C_���� };


enum E_UnitAnimTag
{
	E_ANIM_TESTUNIT_NONE = 0,
	E_ANIM_TESTUNIT_ATTACK = 1,
	E_ANIM_TESTUNIT_ATTACK1,
	E_ANIM_TESTUNIT_ATTACK2,
	E_ANIM_TESTUNIT_ATTACK3,
	E_ANIM_TESTUNIT_END,
};


//class cWeapon;
//class cAmour;

//enum E_TestUnitAnim;

class cCharacter : public cObject
{
protected:
	struct sCharacterBassDate
	{
	public:
		bool m_isDie;

		int m_hp, m_maxHp;
		int m_sh, m_maxSh;
		int m_mp, m_maxMp;

		float m_moveSpeed;
		float m_colliderSize;

		E_Unit_Pos		me_Pos;
		E_Unit_Size		me_Size;
		E_Unit_Type		me_Type;

		E_Unit_AttackType		me_Atype;
		E_Unit_AttackTarget		me_Atarget;

		E_UnitCollider	me_Collider;

		sCharacterBassDate(
			E_Unit_Pos e_Pos,
			E_Unit_Type e_Type,
			E_Unit_Size e_Size,
			E_Unit_AttackType e_AType,
			E_Unit_AttackTarget e_ATarget,
			int maxHp, int maxSh, int maxMp,
			float moveSpeed,
			float colliderSize,
			E_UnitCollider e_collider)
		{
			m_isDie = false;
			m_hp = m_maxHp = maxHp;
			m_sh = m_maxSh = maxSh;
			m_mp = m_maxMp = maxMp;
			m_moveSpeed = moveSpeed;
			m_colliderSize = colliderSize;

			me_Pos = e_Pos;
			me_Size = e_Size;
			me_Type = e_Type;
			me_Atype = e_AType;
			me_Atarget = e_ATarget;
			me_Collider = e_collider;
		}

		void HitDamage(int damage)
		{
			if (m_isDie) return;
			this->m_hp -= damage;
			if (this->m_hp <= 0)
			{
				m_isDie = true;
				this->m_hp = 0;
			}
		}
	};
	int				m_PlayerID;
	std::string		m_unitName;

	iMap*				m_pMap;
	sCharacterBassDate*	m_bassData;

public:
	cCharacter(void);
	virtual ~cCharacter(void);

	virtual void Setup(int player, std::string unitName);
	virtual void SetAnim(E_UnitAnimTag animTag) {};
	virtual void Update();
	virtual void MapUpload(iMap* pMap)
	{
		m_pMap = pMap;
	}
	virtual void Render();
	virtual D3DXVECTOR3& GetPosition();
	virtual D3DXMATRIXA16* GetWorldMatrix()
	{
		return &m_matWorld;
	}
	virtual sCharacterBassDate* GetCharacterData() { return m_bassData; }
//	virtual cWeapon* GetWeapon() { return nullptr; }
//	virtual cAmour* GetAmour() { return nullptr; }

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

