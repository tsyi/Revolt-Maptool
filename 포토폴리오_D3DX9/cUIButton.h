#pragma once

#include "cUIText.h"
#include "cUIImage.h"
#include "cUIObject.h"
#include "cEvent.h"
#include "cObjectManager.h"


#include <functional>

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


	Event OnClick;
	SYNTHESIZE(int, m_eventId, EventID);

public:
	cUIButton();
	~cUIButton();
	void SetOnClick(Event function);

	void RegistButtonUI(cUIText* pUiText, cUIImage* pUiImage, std::string text, std::string InagePach);
	void Destory();
	void Update();
	void Render();

//	void EventSet()

};