#pragma once

class cUIButton;

enum eEventTag
{
	E_NONE = 0,
	E_CREATE_OBJECT,
};
class cEvent
{
//	void* pvoid;
public:
	virtual void OnClick(cUIButton* pClass, eEventTag eventTag) = 0;
//	virtual void OnClick(void* vpoint) = 0;
};

