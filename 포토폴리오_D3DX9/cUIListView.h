#include "cUIObject.h"
#include "cUIImage.h"
#include "cUIButton.h"
#include "cEvent.h"

#pragma once
class cUIListView : public cUIObject
{
	typedef std::function<void(int)> Event;

protected:
	cUIImage* m_pImage;
	std::vector<cUIButton*> m_vecButton;

	float m_Vvalue;


	SYNTHESIZE(Event, onClick_List, OnClick_List);
	SYNTHESIZE(int, m_eventId, EventID);

public:
	cUIListView();
	~cUIListView();

	void SetBackgroundUI(cUIImage* pImage);
	void AddButton(cUIButton * pButton);
	void Destory();
	void Update();
	void Render();

	void OnCheckButtonClick(int buttonIndex);
	void SetEvent_OnCilck_List(Event function);
};

