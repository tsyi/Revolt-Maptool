#include "stdafx.h"
#include "cUITextBox.h"


cUITextBox::cUITextBox()
	: m_pUiText(NULL)
	, m_pUiImage(NULL)
{

}


cUITextBox::~cUITextBox()
{
}

void cUITextBox::RegistTextBoxUI(cUIText * pUiText, cUIImage * pUiImage)
{
	this->AddChild(pUiImage);
	this->AddChild(pUiText);
	m_pUiText = pUiText;
	m_pUiImage = pUiImage;
}

void cUITextBox::Destory()
{
	cUIObject::Destory();
}

void cUITextBox::Update()
{
	switch (m_state)
	{
	case eTextBoxState::E_TEXT_BOX_STATE_NONE:
	{
		if (MgrInput->IsMouseDown(MOUSE_BUTTON::MOUSE_LEFT))
		{
			//선택 
			if (m_pUiImage->IsMouseOver())
			{
				m_state = eTextBoxState::E_TEXT_BOX_STATE_SELECT;
				m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255 / 2, 255 / 2, 255 / 2));
			}
		}
	} break;
	case eTextBoxState::E_TEXT_BOX_STATE_SELECT:
	{
		if (MgrInput->IsKeyDown(VK_DELETE))
		{
			//삭제
		}
		if (MgrInput->IsKeyDown(VK_RETURN))
		{
			//확인
			m_state = eTextBoxState::E_TEXT_BOX_STATE_NONE;
			m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if (MgrInput->IsMouseDown(MOUSE_BUTTON::MOUSE_LEFT))
		{
			//선택 취소
			if (!m_pUiImage->IsMouseOver())
			{
				m_state = eTextBoxState::E_TEXT_BOX_STATE_NONE;
				m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}break;
	}
	cUIObject::Update();
}

void cUITextBox::Render()
{
	cUIObject::Render();
}
