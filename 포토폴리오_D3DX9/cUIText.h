#pragma once
#include "cUIObject.h"

struct sUIText
{
	RECT rc;

	std::string text;
	eFontType fontType;
	DWORD style;
	D3DCOLOR color;

	sUIText()
	{
		SetRect(&rc, 0, 0, 10, 10);

		text = "";
		fontType = eFontType::E_DEFAULT;
		//pFont = NULL;
		style = DT_CENTER | DT_VCENTER | DT_NOCLIP; // DT_LEFT | DT_TOP | DT_NOCLIP;
		color = D3DCOLOR_XRGB(255, 255, 255);
	}
	void Setup(std::string text, eFontType pFontType, DWORD style, D3DCOLOR color)
	{
		this->text = text;
		this->fontType = pFontType,
		this->style = style;
		this->color = color;
	}
	void SetText(std::string _text) { text = _text; }
	void SetOption(eFontType pFontType, DWORD style, D3DCOLOR color) 
	{
		this->fontType = pFontType,
			this->style = style;
		this->color = color;
	}
	void SetUIRect(RECT rc)
	{
		this->rc = rc;
	}
	void SetUIRect(float left, float top, float right, float bottom)
	{
		SetRect(&rc, left, top, right, bottom);
	}
	void Draw()
	{
		MgrFont->GetFont(fontType)->DrawTextA(NULL, text.c_str(), strlen(text.c_str()),
			&rc, style, color);
	}
};

class cUIText : public cUIObject
{
public:
	cUIText();
	~cUIText();

private:
	sUIText* pText;
public:
	//	void Setup();
	void CreateTextUI(std::string text, eFontType pFontType, DWORD style, D3DCOLOR color);

	void SetText(std::string text);
	//style : DT_CENTER | DT_VCENTER | ....
	void SetOption(eFontType pFontType, DWORD style, D3DCOLOR color);
	void Destory();
	void Update();
	void Render();

	// int(-1) -> 해당 변수는 변경하지 않음 

	sUIText* GetUIText();
};

