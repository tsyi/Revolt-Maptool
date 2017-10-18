#pragma once

//#include "cUIObject.h"
enum eUITag;
class cUIObject;
class cUIText;
class cUIManager;
class cUiTextImage;

#define MgrUI cUIManager::GetInstance()
class cUIManager
{
public:
	SINGLETONE(cUIManager);
	SYNTHESIZE(LPD3DXSPRITE, m_pSprite, Sprite);

private:
	std::vector<cUIObject*> m_vecUI;

public:
	void Setup();
	void Destory();
	void Update();
	void Render();

public:
	void RegisteredUI(cUIObject* pUI);
//	void CreateTextUI(OUT cUIText* textUI, int tag, std::string text, eFontType pFontType, DWORD style, D3DCOLOR color);
	
	cUIObject* FindByTag(eUITag tag);
	cUIObject* FindChieldByTag(cUIObject* parent, eUITag tag);

};
