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

void cUITextBox::SetEvent_OnEnter(Event function)
{
	SetOnEnter(std::move(function));
}

void cUITextBox::RegistTextBoxUI(cUIText * pUiText, cUIImage * pUiImage, std::string text, std::string InagePach)
{
	m_pUiText = pUiText;
	m_pUiImage = pUiImage;

	this->AddChild(m_pUiImage);
	this->AddChild(m_pUiText);

	m_pUiText->SetOption(eFontType::E_DEFAULT, DT_CENTER | DT_BOTTOM, D3DCOLOR_XRGB(0, 0, 0));
	m_pUiText->SetPosition(0, 0, 0);
	m_pUiText->SetSize(this->GetSize());
	m_pUiText->SetText(text);

	m_pUiImage->SetTexture((char*)InagePach.c_str());

	D3DXVECTOR2 imageSize = m_pUiImage->GetSize();
	imageSize = D3DXVECTOR2(this->GetSize().x / imageSize.x, this->GetSize().y / imageSize.y);
	m_pUiImage->SetScale(imageSize);
	m_pUiImage->SetAnchor(0, 0, 0);
}

void cUITextBox::Destory()
{
	m_pUiText = NULL;
	m_pUiImage = NULL;
	cUIObject::Destory();
}

void cUITextBox::Update()
{
	if (!m_isShow) return;

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
			if (onEnter) 
				GetOnEnter()(VK_RETURN , GetUIText()->GetTextData()->text);
		}
		if (MgrInput->IsMouseDown(MOUSE_BUTTON::MOUSE_LEFT))
		{
			//선택 취소
			if (!m_pUiImage->IsMouseOver())
			{
				m_state = eTextBoxState::E_TEXT_BOX_STATE_NONE;
				m_pUiImage->SetBassColor(D3DCOLOR_ARGB(255, 255, 255, 255));
				if (onEnter) GetOnEnter()(VK_ESCAPE, GetUIText()->GetTextData()->text);
			}
		}
		if (m_state == eTextBoxState::E_TEXT_BOX_STATE_SELECT)
		{
			if (MgrInput->IsKeyDown(VK_DELETE))
			{
				m_pUiText->GetTextData()->text = "";
			}
			if (MgrInput->IsKeyDown(VK_BACK))
			{
				if (m_pUiText->GetTextData()->text.length() > 0)
				{
					m_pUiText->GetTextData()->text.pop_back();
				}
			}
			if (MgrInput->IsKeyPress(VK_BACK))
			{
				if (pushTime > 0.5f && m_pUiText->GetTextData()->text.length() > 0)
				{
					m_pUiText->GetTextData()->text.pop_back();
					pushTime -= 0.05f;
				}
				else pushTime += MgrTime->GetElapsedTime();
			}
			if (MgrInput->IsKeyUp(VK_BACK))
			{
				pushTime = 0.0f;
			}
			else
			{
				char textBuffer = MgrInput->PopKeyBuffer();
				if ((textBuffer >= 'A' && textBuffer <= 'Z')) // -65== '/'
				{
					if (!MgrInput->IsKeyPress(VK_LSHIFT)) textBuffer += ('a' - 'A');
					m_pUiText->GetTextData()->text.push_back(textBuffer);
				}
				else if ((textBuffer >= '0' && textBuffer <= '9')) // -66 == .
				{
					m_pUiText->GetTextData()->text.push_back(textBuffer);
				}
				else
				{
					if (textBuffer == -66) {
						m_pUiText->GetTextData()->text.push_back('.');
					}
					if (textBuffer == -65) {
						m_pUiText->GetTextData()->text.push_back('/');
					}
					if (textBuffer == -67)
					{
						if (MgrInput->IsKeyPress(VK_LSHIFT)) {
							m_pUiText->GetTextData()->text.push_back('_');
						}
						else {
							m_pUiText->GetTextData()->text.push_back('-');
						}
					}
				}
				//	std::cout << (int)textBuffer << std::endl;

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
	return m_pUiText->GetTextData()->text;
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
	m_pUiText->GetTextData()->text = text;
}

void cUITextBox::SetData_ToString(float value)
{
	cStringUtil::ToString(value);
}

void cUITextBox::SetData_ToString(int value)
{
	cStringUtil::ToString(value);
}

