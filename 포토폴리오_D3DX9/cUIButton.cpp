#include "stdafx.h"
#include "cUIButton.h"

cUIButton::cUIButton()
	: m_pUiText(NULL)
	, m_pUiImage(NULL)
	, m_state(eButtonState::E_BUTTON_NONE)
	, m_isOn(false)
	, m_isEvent(nullptr)
{
}

cUIButton::~cUIButton()
{
}

void cUIButton::RegistButtonUI(cUIText * pUiText, cUIImage * pUiImage, std::string text, std::string InagePach)
{
	m_pUiText = pUiText;
	m_pUiImage = pUiImage;

	this->AddChild(m_pUiImage);
	this->AddChild(m_pUiText);

	m_pUiText->SetOption(eFontType::E_DEFAULT, DT_CENTER | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	m_pUiText->SetPosition(0, 0, 0);
	m_pUiText->SetSize(this->GetSize());
	m_pUiText->SetText(text);

	m_pUiImage->SetTexture((char*)InagePach.c_str());
	D3DXVECTOR2 imageSize = m_pUiImage->GetSize();
	imageSize = D3DXVECTOR2(this->GetSize().x / imageSize.x, this->GetSize().y / imageSize.y);
	m_pUiImage->SetScale(imageSize);
	m_pUiImage->SetAnchor(0, 0, 0);
}



void cUIButton::Destory()
{
	m_pUiText = NULL;
	m_pUiImage = NULL;
	cUIObject::Destory();
}

void cUIButton::Update()
{
	if (!GetShow()) return;
	if (IsMouseOver()) { MgrInput->SetHooking(true); }

	switch (m_state)
	{
	case E_BUTTON_NONE:
	{
		if (IsMouseOver())
		{
			m_state = E_BUTTON_OVER;
			m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255 / 2, 255 / 2, 255 / 2));
		}
	}break;
	case E_BUTTON_OVER:
	{
		if (!IsMouseOver())
		{
			m_state = E_BUTTON_NONE;
			m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else if (MgrInput->IsMouseDown(MOUSE_LEFT))
		{
			m_state = E_BUTTON_DOWN;
			m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255 / 4, 255 / 4, 255 / 4));
		}
	}break;
	case E_BUTTON_DOWN:
	{
		if (!IsMouseOver())
		{
			m_state = E_BUTTON_NONE;
		}
		else  if (MgrInput->IsMouseUp(MOUSE_LEFT))
		{
			m_state = E_BUTTON___UP;
			m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255 / 2, 255 / 2, 255 / 2));
		}
	}break;
	case E_BUTTON___UP:
	{
		if (m_isEvent) m_isEvent->OnClick(this);
		m_state = E_BUTTON_OVER;
	}break;
	}
	cUIObject::Update();
}

void cUIButton::Render()
{
	cUIObject::Render();
}