#pragma once

#include "cUIText.h"
#include "cUIImage.h"
#include "cUIObject.h"
#include "cEvent.h"
#include "cObjectManager.h"

enum eButtonState
{
	E_BUTTON_NONE,
	E_BUTTON_OVER,
	E_BUTTON_DOWN,
	E_BUTTON___UP,
//	E_BUTTON_NONE,
};

class cUIButton : public cUIObject
{
protected:
	cUIText* m_pUiText;
	cUIImage* m_pUiImage;

	eButtonState m_state;
	bool m_isOn;

	SYNTHESIZE(cEvent*, m_isEvent, Event);

	SYNTHESIZE(eOBJ, m_createObjEnum, ObjTag);

public:
	cUIButton();
	~cUIButton();

	void RegistButtonUI(cUIText* pUiText, cUIImage* pUiImage, std::string text, std::string InagePach);
	void Destory();
	void Update();
	void Render();


};