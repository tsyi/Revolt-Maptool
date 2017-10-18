#include "stdafx.h"
#include "cUITextBox.h"
#include "cStringUtil.h"

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
		if (m_state == eTextBoxState::E_TEXT_BOX_STATE_SELECT)
		{
			if (MgrInput->IsKeyDown(VK_DELETE))
			{
				m_pUiText->GetUIText()->text = "";
			}
			if (MgrInput->IsKeyDown(VK_BACK))
			{
				if (m_pUiText->GetUIText()->text.length() > 0)
					m_pUiText->GetUIText()->text.pop_back();
			}
			else
			{
				char textBuffer = MgrInput->PopKeyBuffer();
				if ((textBuffer >= '0' && textBuffer <= '9') || textBuffer == -66)
				{
					if (textBuffer == -66) textBuffer = '.';
					m_pUiText->GetUIText()->text.push_back(textBuffer);
				}
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

std::string cUITextBox::GetData_String()
{
	return m_pUiText->GetUIText()->text;
}

int cUITextBox::GetData_Int()
{
	return cStringUtil::ToInt(GetData_String());
}

float cUITextBox::GetData_Float()
{
	return cStringUtil::ToFloat(GetData_String());
}

void cUITextBox::SetData_ToString(std::string text)
{
	m_pUiText->GetUIText()->text = text;
}

void cUITextBox::SetData_ToString(float value)
{
	cStringUtil::ToString(value);
}

void cUITextBox::SetData_ToString(int value)
{
	cStringUtil::ToString(value);
}

