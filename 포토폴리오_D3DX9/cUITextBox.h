#pragma once
#include "cUIText.h"
#include "cUIImage.h"
#include "cUIObject.h"
#include "cEvent.h"


enum eTextBoxState
{
	E_TEXT_BOX_STATE_NONE = 0,
	E_TEXT_BOX_STATE_SELECT,
};

class cUITextBox : public cUIObject
{
	typedef std::function<void(int)> Event;
protected:
	cUIText* m_pUiText;
	cUIImage* m_pUiImage;

	SYNTHESIZE(eTextBoxState, m_state, State);

	SYNTHESIZE(Event, onEnter, OnEnter);
	Event OnCancle;
	SYNTHESIZE(int, m_eventId, EventID);

	float pushTime;
public:

	cUITextBox();
	~cUITextBox();
	void SetEvent_OnEnter(Event function);

	void RegistTextBoxUI(cUIText* pUiText, cUIImage* pUiImage, std::string text, std::string InagePach);
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

