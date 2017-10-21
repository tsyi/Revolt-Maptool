#include "stdafx.h"
#include "cUIListView.h"


cUIListView::cUIListView()
{
	m_Vvalue = 0;
}


cUIListView::~cUIListView()
{
}


void cUIListView::SetBackgroundUI(cUIImage * pImage)
{
	m_pImage = pImage;
	AddChild(m_pImage);

	D3DXVECTOR2 imageSize = m_pImage->GetSize();
	imageSize = D3DXVECTOR2(this->GetSize().x / imageSize.x, this->GetSize().y / imageSize.y);
	m_pImage->SetScale(imageSize);
	m_pImage->SetAnchor(0, 0, 0);
}

void cUIListView::AddButton(cUIButton * pButton)
{
	m_vecButton.push_back(pButton);
	AddChild(pButton);
	if (m_vecButton.size() == 1)
	{
		pButton->SetPosition(0, 0, 0);
	}
	else
	{
		D3DXVECTOR3 lastButton = m_vecButton.back()->GetPosition();
		lastButton.y += m_vecButton.back()->GetSize().y;
		pButton->SetPosition(lastButton);
	}
}

void cUIListView::Destory()
{
	for each(cUIButton* p in m_vecButton)
	{
		p->Destory();
	}
	m_vecButton.clear();
	cUIObject::Destory();
}

void cUIListView::Update()
{
	if (!GetShow()) return;
//	if (IsMouseOver()) { MgrInput->SetHooking(true); }

	if (MgrInput->IsMouseWheel())
	{
		if (IsMouseOver()) m_Vvalue += MgrInput->GetMouseWheelDelta() * 10;
	}

	float heightsize =0;

	for each(cUIButton* pButton in m_vecButton)
	{
		pButton->SetPosition(0, heightsize + m_Vvalue, 0);
		heightsize += pButton->GetSize().y;

	//	Get
	}

	cUIObject::Update();
}

void cUIListView::Render()
{
	cUIObject::Render();
}
