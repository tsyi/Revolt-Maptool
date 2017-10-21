#include "stdafx.h"
#include "cUIText.h"


cUIText::cUIText()
	:pText(NULL)
{
	SetTag(eUITag::E_UI_NONE);
	SetShow(true);
	SetParent(NULL);

	
}


cUIText::~cUIText()
{
}

void cUIText::CreateTextUI(std::string text, eFontType pFontType, DWORD style, D3DCOLOR color)
{
	pText = new sUIText();
	pText->Setup(text, pFontType, style, color);
}

void cUIText::SetText(std::string text)
{
	if (!pText)
	{
		pText  = new sUIText();
	}
	pText->SetText(text);
}

void cUIText::SetOption(eFontType pFontType, DWORD style, D3DCOLOR color)
{
	if (!pText)
	{
		pText = new sUIText();
		pText->SetText("");
	}
	pText->SetOption(pFontType, style, color);
}

void cUIText::SetFont(eFontType pFontType)
{
	pText->SetFont(pFontType);
}

void cUIText::Destory()
{
	cUIObject::Destory();
}

void cUIText::Update()
{
	if (!GetShow()) return;
	if (IsMouseOver()) { MgrInput->SetHooking(true); }
	cUIObject::Update();
}

void cUIText::Render()
{
	if (!GetShow()) return;

	pText->SetUIRect((int)m_matWorld._41, (int)m_matWorld._42,
		(int)m_matWorld._41 + (int)GetSize().x,
		(int)m_matWorld._42 + (int)GetSize().y);
	pText->Draw();
	cUIObject::Render();
}



sUIText* cUIText::GetUIText()
{
	return pText;
}
