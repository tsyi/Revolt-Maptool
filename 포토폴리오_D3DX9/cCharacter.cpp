#include "StdAfx.h"
#include "cCharacter.h"

cCharacter::cCharacter(void)
	:// m_fRotY(0.0f)
	m_pMap(NULL)
	//	, m_position(0, 0, 0)
	//	, m_vDirection(0, 0, -1)
{
	//	D3DXMatrixIdentity(&m_matWorld);
}


cCharacter::~cCharacter(void)
{
}

void cCharacter::Setup(int playerID, std::string unitName)
{
	m_PlayerID = playerID;
	m_unitName = unitName;
//	m_bassData = new sCharacterBassDate();
}

void cCharacter::Update()
{

}

void cCharacter::Render()
{

}

D3DXVECTOR3& cCharacter::GetPosition()
{
	return m_position;
}

void cCharacter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			break;
		}
		break;
	}
}
