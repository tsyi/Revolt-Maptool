#pragma once

enum INPUT_STATE
{
	INPUT_STATE_UP,
	INPUT_STATE_NONE,
	INPUT_STATE_DOWN,
	INPUT_STATE_PRESS,
	INPUT_STATE_DRAG,
};
enum MOUSE_BUTTON
{
	MOUSE_LEFT,
	MOUSE_CENTER,
	MOUSE_RIGHT,
	MOUSE_WHEEL_UP,
	MOUSE_WHEEL_DOWN
};

#define KEY_COUNT 256
#define MOUSE_COUNT 3

#define MgrInput cInputManager::GetInstance()

#define MOUSE_EPSILON	1.f			//MoueMove가 true를 반환 하기 위한 최소 거리
#define WHEEL_DETAIL	50.f		//Wheel 값이 높을 수록 조금씩 움직.
#define WHEEL_SPEED_DECREASE 0.5f	//Wheel 속도 감소 정도,	
class cInputManager
{
	SINGLETONE(cInputManager);

	float wheelDistance;
public:
	//	cInputManager();
	//	~cInputManager();

	//HWND m_hWnd;
	INPUT_STATE m_keyState[KEY_COUNT];
	INPUT_STATE m_mouseState[MOUSE_COUNT];

	D3DXVECTOR2 m_mousePos;
	D3DXVECTOR2 m_prevMousePos;
	D3DXVECTOR2 m_deltaMousePos;

	int inputKeyBuffer;
private:
	void KeyUpdate()
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

	void MouseUpdate()
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

	bool CheckMouseMove()
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

public:
	void Setup()
	{
		//m_hWnd = hWnd;
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(g_hWnd, &point);

		m_mousePos.x = point.x;
		m_mousePos.y = point.y;
		m_prevMousePos = m_mousePos;

		SetKeyNone();

		wheelDistance = 0.f;
	}
	void Release()
	{

	}
	void Update()
	{
		KeyUpdate();
		MouseUpdate();
	}

	void SetKeyNone()
	{
		for (int i = 0; i < KEY_COUNT; i++) { m_keyState[i] = INPUT_STATE_NONE; }
		for (int i = 0; i < MOUSE_COUNT; i++) { m_mouseState[i] = INPUT_STATE_NONE; }
	}

public:
	INPUT_STATE GetKeyState(int key) { return m_keyState[key]; }
	INPUT_STATE GetMouseState(MOUSE_BUTTON btn) { return m_mouseState[btn]; }

	D3DXVECTOR2 GetMousePos() { return m_mousePos; }
	POINT GetMousePoint() { POINT pt; pt.x = m_mousePos.x; pt.y = m_mousePos.y; return pt; }
	D3DXVECTOR2 GetMouseDelta() { return m_deltaMousePos; }
	float GetMouseWheelDelta() { return wheelDistance; }

	char PopKeyBuffer() { char pop = inputKeyBuffer; inputKeyBuffer = -1; return pop; }

	bool IsKeyUp(int key) { return GetKeyState(key) == INPUT_STATE_UP; }
	bool IsKeyNone(int key) { return GetKeyState(key) == INPUT_STATE_NONE; }
	bool IsKeyDown(int key) { return GetKeyState(key) == INPUT_STATE_DOWN; }
	bool IsKeyPress(int key) { return GetKeyState(key) == INPUT_STATE_PRESS; }
	bool IsKeyOn(int key) { return !IsKeyOff(key); }
	bool IsKeyOff(int key) { return IsKeyNone(key) || IsKeyUp(key); }

	bool IsMouseUp(MOUSE_BUTTON btn) { return GetMouseState(btn) == INPUT_STATE_UP; }
	bool IsMouseNone(MOUSE_BUTTON btn) { return GetMouseState(btn) == INPUT_STATE_NONE; }
	bool IsMouseDown(MOUSE_BUTTON btn) { return GetMouseState(btn) == INPUT_STATE_DOWN; }
	bool IsMousePress(MOUSE_BUTTON btn) { return GetMouseState(btn) == INPUT_STATE_PRESS; }
	bool IsMouseDrag(MOUSE_BUTTON btn) { return GetMouseState(btn) == INPUT_STATE_DRAG; }
	bool IsMouseOn(MOUSE_BUTTON btn) { return  !IsMouseOff(btn); }
	bool IsMouseOff(MOUSE_BUTTON btn) { return IsMouseUp(btn) || IsMouseDown(btn); }

	bool IsMouseWheel() { return (wheelDistance != 0.f); }
	bool IsMouseWheelUp() { return (wheelDistance > 0.0f); }
	bool IsMouseWheelDown() { return (wheelDistance < 0.0f); }


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_MOUSEWHEEL:
			wheelDistance = (GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DETAIL);
			break;
		default:
			//	wheelDistance = 0.f;
			break;
		}
		return 0;
	}

};

