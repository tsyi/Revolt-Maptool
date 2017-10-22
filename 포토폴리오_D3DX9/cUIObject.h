#pragma once

enum eUITag
{
	E_UI_NONE = 0,
	E_UI_TEXT1,
	E_UI_TEXT2,

	E_UI_WINDOW_L,
	E_UI_WINDOW_R,
	
	E_UI_TEXTBOX_MAPNAME,
	E_UI_BUTTON_SAVE,


	E_UI_TEXTBOX_OBJNAME,
	E_UI_TEXTBOX_POS_X,
	E_UI_TEXTBOX_POS_Y,
	E_UI_TEXTBOX_POS_Z,
	E_UI_TEXTBOX_SIZE_X,
	E_UI_TEXTBOX_SIZE_Y,
	E_UI_TEXTBOX_SIZE_Z,
	E_UI_TEXTBOX_ROT_X,
	E_UI_TEXTBOX_ROT_Y,
	E_UI_TEXTBOX_ROT_Z,

	E_UI_OBJLIST_VIEW,
	E_UI_OBJLIST_TITLE,
	E_UI_OBJLIST_BUTTONS,

};

#include <functional>

class cUIButton;

class cUIObject
{
public:
	cUIObject();
	virtual	~cUIObject();

protected:
	SYNTHESIZE(eUITag, m_tag, Tag);
	SYNTHESIZE(bool, m_isShow, Show);

	SYNTHESIZE(D3DXVECTOR3, m_position, Position);
	SYNTHESIZE(D3DXVECTOR2, m_stSize, Size);
	SYNTHESIZE(cUIObject*, m_pParent, Parent);

	D3DXMATRIXA16 m_matWorld;
	SYNTHESIZE(std::vector<cUIObject*>, m_vecChild, Childs);

//	cUIObject* pParent;
//	std::vector<cUIObject*> vecChild;
public:
	bool IsHookingCheck;

	virtual void SetSize(float x, float y);
	virtual void SetPosition(float x, float y, float z = 0);
	virtual void AddPosition(D3DXVECTOR3 addPos);
	virtual void AddPosition(float x, float y, float z = 0);
	virtual void AddChild(cUIObject* pChild);
	virtual void Update();
	virtual void Render();
	virtual void Destory();

	virtual cUIObject* FindByTag(eUITag tag);
	virtual bool IsMouseOver()
	{
		RECT rc;
		SetRect(&rc,
			(int)m_matWorld._41 ,
			(int)m_matWorld._42 ,
			(int)m_matWorld._41 + (int)GetSize().x,
			(int)m_matWorld._42 + (int)GetSize().y);

		return PtInRect(&rc, MgrInput->GetMousePoint());
	}
	virtual void OverCheck()
	{
		if (IsHookingCheck)
		{
			if (IsMouseOver())
			{
				MgrInput->SetHooking(true);
				return;
			}
		}
		for each(auto c in m_vecChild)
		{
			c->OverCheck();
			if (MgrInput->GetHooking()) return;
		}
	}

	virtual void AddButton(cUIButton* pButton) {}
};

