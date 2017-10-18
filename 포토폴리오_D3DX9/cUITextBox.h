#pragma once
#include "cUIText.h"
#include "cUIImage.h"
#include "cUIObject.h"

enum eTextBoxState
{
	E_TEXT_BOX_STATE_NONE = 0,
	E_TEXT_BOX_STATE_SELECT,
};

class cUITextBox : public cUIObject
{
protected:
	cUIText* m_pUiText;
	cUIImage* m_pUiImage;
	
	eTextBoxState m_state;
public:

	cUITextBox();
	~cUITextBox();

	void RegistTextBoxUI(cUIText* pUiText, cUIImage* pUiImage);
	void Destory();
	void Update();
	void Render();

	cUIText* GetText() { return m_pUiText; }
	cUIImage* GetImage() { return m_pUiImage; }

	std::string GetData_String();
	float GetData_Float();
	int GetData_Int();

	void SetData_ToString(std::string text);
	void SetData_ToString(float value);
	void SetData_ToString(int value);
};

