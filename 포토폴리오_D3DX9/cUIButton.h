#pragma once

#include "cUIText.h"
#include "cUIImage.h"
#include "cUIObject.h"
#include "cEvent.h"


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
	typedef std::function<void(int)> Event;
protected:
	cUIText* m_pUiText;
	cUIImage* m_pUiImage;

	eButtonState m_state;
	bool m_isOn;


	SYNTHESIZE(Event, onClick_Up, OnClick_Up);
	SYNTHESIZE(int, m_eventId, EventID);

	float pushTime;
public:
	cUIButton();
	~cUIButton();

	void RegistButtonUI(cUIText* pUiText, cUIImage* pUiImage, std::string text, std::string InagePach);
	void Destory();
	void Update();
	void Render();

//	void EventSet()

	void SetEvent_OnCilck_Up(Event function);
};