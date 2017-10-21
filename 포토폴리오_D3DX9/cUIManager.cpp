#include "stdafx.h"
#include "cUIManager.h"

#include "cUIObject.h"
#include "cUIText.h"

cUIManager::cUIManager()
{
}


cUIManager::~cUIManager()
{
}

void cUIManager::Setup()
{
	m_vecUI = std::vector<cUIObject*>();
	m_vecUI.clear();

	D3DXCreateSprite(MgrD3DDevice, &m_pSprite);
}

void cUIManager::Destory()
{
	for each(auto p in m_vecUI)
	{
		p->Destory();
	}
	m_vecUI.clear();
	SAFE_RELEASE(m_pSprite);
}

void cUIManager::OverCheck()
{
	for each(auto p in m_vecUI)
	{
		p->OverCheck();
	}
//	MgrInput->SetHooking(false);
}

void cUIManager::Update()
{
	for each(auto p in m_vecUI)
	{
		p->Update();
	}
}

void cUIManager::Render()
{
	for each(auto p in m_vecUI)
	{
		p->Render();
	}
}

void cUIManager::RegisteredUI(cUIObject* pUIObject)
{
	if (pUIObject)
	{
		if (pUIObject->GetParent() == NULL)	//최상위 부모객체만 등록시킨다.
			m_vecUI.push_back(pUIObject);
		else
		{
			std::cout << "오류: 부모가 있는 UI를 메니저에 등록하려고 시도했습니다." << std::endl;
		}
	}
}

cUIObject * cUIManager::FindByTag(eUITag tag)
{
	for each(auto pUI in m_vecUI)
	{
		cUIObject* p = pUI->FindByTag(tag);
		if (p) return p;
	}
	return nullptr;
}

cUIObject * cUIManager::FindChieldByTag(cUIObject* parent, eUITag tag)
{
	for each(auto pUI in parent->GetChilds())
	{
		cUIObject* p = pUI->FindByTag(tag);
		if (p) return p;
	}
	return nullptr;
}

//void cUIManager::CreateTextUI(OUT cUIText* textUI, int tag, std::string text, eFontType pFontType, DWORD style, D3DCOLOR color)
//{
//	if (textUI == NULL)	textUI = new cUIText;
//	
//	textUI->CreateTextUI(tag, text, pFontType, style, color);
//	//	textUI = _textUI;
//}
