#pragma once

class cUIButton;
class cEvent
{
public:
	virtual void OnClick(cUIButton* pClass) = 0;
};

