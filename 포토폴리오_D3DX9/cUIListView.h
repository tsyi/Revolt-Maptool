#include "cUIObject.h"
#include "cUIImage.h"
#include "cUIButton.h"
#pragma once
class cUIListView : public cUIObject
{
	cUIImage* m_pImage;
	std::vector<cUIButton*> m_vecButton;

	float m_Vvalue;

public:
	cUIListView();
	~cUIListView();

	void SetBackgroundUI(cUIImage* pImage);
	void AddButton(cUIButton * pButton);
	void Destory();
	void Update();
	void Render();
};

