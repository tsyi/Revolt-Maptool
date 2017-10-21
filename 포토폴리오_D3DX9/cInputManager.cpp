#include "stdafx.h"
#include "cInputManager.h"
#include "cCamera.h"

cInputManager::cInputManager()
{
}


cInputManager::~cInputManager()
{
}

void cInputManager::KeyUpdate()
{
	for (int i = 0; i < KEY_COUNT; i++)
	{
		bool isPressed = GetAsyncKeyState(i) & 0x8000;
		if (isPressed)
		{
			switch (m_keyState[i])
			{
			case INPUT_STATE_UP: m_keyState[i] = INPUT_STATE_DOWN; break;
			case INPUT_STATE_NONE:
				m_keyState[i] = INPUT_STATE_DOWN;
				inputKeyBuffer = i; break;
			case INPUT_STATE_DOWN: m_keyState[i] = INPUT_STATE_PRESS; break;
			case INPUT_STATE_PRESS: m_keyState[i] = INPUT_STATE_PRESS; break;
			case INPUT_STATE_DRAG: m_keyState[i] = INPUT_STATE_PRESS; break;
			}
		}
		else
		{
			switch (m_keyState[i])
			{
			case INPUT_STATE_UP: m_keyState[i] = INPUT_STATE_NONE; break;
			case INPUT_STATE_NONE: m_keyState[i] = INPUT_STATE_NONE; break;
			case INPUT_STATE_DOWN: m_keyState[i] = INPUT_STATE_UP; break;
			case INPUT_STATE_PRESS: m_keyState[i] = INPUT_STATE_UP; break;
			case INPUT_STATE_DRAG: m_keyState[i] = INPUT_STATE_UP; break;
			}
		}
	}
}

void cInputManager::MouseUpdate()
{
	bool checkMouseMove = CheckMouseMove();

	for (int i = 0; i < MOUSE_COUNT; i++)
	{
		int vKey = 0;
		switch (i)
		{
		case MOUSE_LEFT: vKey = VK_LBUTTON; break;
		case MOUSE_RIGHT: vKey = VK_RBUTTON; break;
		case MOUSE_CENTER: vKey = VK_MBUTTON; break;
		}
		bool isPressed = GetAsyncKeyState(vKey) & 0x8000;
		if (isPressed)
		{
			switch (m_mouseState[i])
			{
			case INPUT_STATE_UP: m_mouseState[i] = INPUT_STATE_DOWN; break;
			case INPUT_STATE_NONE: m_mouseState[i] = INPUT_STATE_DOWN; break;
			case INPUT_STATE_DOWN: m_mouseState[i] = (checkMouseMove ? INPUT_STATE_DRAG : INPUT_STATE_PRESS); break;
			case INPUT_STATE_PRESS: m_mouseState[i] = (checkMouseMove ? INPUT_STATE_DRAG : INPUT_STATE_PRESS); break;
			case INPUT_STATE_DRAG: m_mouseState[i] = (checkMouseMove ? INPUT_STATE_DRAG : INPUT_STATE_PRESS); break;
			}
		}
		else
		{
			switch (m_mouseState[i])
			{
			case INPUT_STATE_UP: m_mouseState[i] = INPUT_STATE_NONE; break;
			case INPUT_STATE_NONE: m_mouseState[i] = INPUT_STATE_NONE; break;
			case INPUT_STATE_DOWN: m_mouseState[i] = INPUT_STATE_UP; break;
			case INPUT_STATE_PRESS: m_mouseState[i] = INPUT_STATE_UP; break;
			case INPUT_STATE_DRAG: m_mouseState[i] = INPUT_STATE_UP; break;
			}
		}
	}

	if (IsMouseWheelUp())
	{
		wheelDistance -= (WHEEL_SPEED_DECREASE);
		if (wheelDistance <= 0) wheelDistance = 0;
	}
	else
	{
		wheelDistance += (WHEEL_SPEED_DECREASE);
		if (wheelDistance >= 0) wheelDistance = 0;
	}
}

bool cInputManager::CheckMouseMove()
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(g_hWnd, &point);
	m_mousePos.x = point.x;
	m_mousePos.y = point.y;
	m_deltaMousePos = m_mousePos - m_prevMousePos;
	float length = D3DXVec2Length(&m_deltaMousePos);

	if (length < MOUSE_EPSILON && length > -MOUSE_EPSILON)
	{
		return false;
	}
	else
	{
		m_prevMousePos = m_mousePos;
		return true;
	}
}

D3DXVECTOR3 cInputManager::MousePosToViewDir(cCamera * cCamera)
{
	D3DXVECTOR3 vDir(0, 0, 0);
	D3DVIEWPORT9 vp;
	MgrD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	vDir.x = (((+2.0f*GetMousePos().x) / vp.Width) - 1.0f) / matProj._11;
	vDir.y = (((-2.0f*GetMousePos().y) / vp.Height) + 1.0f) / matProj._22;
	vDir.z = 1.0f;

	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matInvView, 0, &matView);
	D3DXVec3TransformNormal(&vDir, &vDir, &matInvView);
	D3DXVec3Normalize(&vDir, &vDir);

	return vDir;
}
