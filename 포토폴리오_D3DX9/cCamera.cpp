#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
	: m_vUp(0, 1, 0)
	, m_target(0, 0, 0)
	, m_freePos(0, 0, 0)
	, m_fCameraDistance(20.0f)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
	m_posFree = true;
}


cCamera::~cCamera(void)
{
}

void cCamera::Setup()
{

	D3DXVECTOR3 vLookAtDir = D3DXVECTOR3(0, -1, 1);
	D3DXVec3Normalize(&vLookAtDir, &vLookAtDir);
	cTransform::SetQuaternionToVector(vLookAtDir, true, true);

	GetClientRect(g_hWnd, &rc);
	SetRect(&rc, 400, 0, APIWidth, APIHeight);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f,
		rc.right / (float)rc.bottom, 1.0f, 10000.0f);

	MgrD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Update(D3DXVECTOR3 target)
{
	if (MgrInput->GetHooking()) return;

	if (MgrInput->IsMouseDown(MOUSE_RIGHT))
	{
		m_ptPrevMouse = MgrInput->GetMousePos();
	}
	if (MgrInput->IsMouseDrag(MOUSE_RIGHT))
	{
		D3DXVECTOR3 cameraDir = cTransform::GetDirection();
		float fDeltaX = MgrInput->GetMouseDelta().x * CAMERA_MOVE_DETAIL;
		float fDeltaY = MgrInput->GetMouseDelta().y * CAMERA_MOVE_DETAIL;

		cameraDir += cTransform::GetRightVec() * fDeltaX		*CAMERA_OPTION_MOVE_V;
		cameraDir += cTransform::GetUpVec() * fDeltaY		* CAMERA_OPTION_MOVE_H;

		float y, p, r;

		while (true)
		{
			bool isbreak = true;
			if (cameraDir.y > CAMERA_MAX_MIN_Y)
			{
				cameraDir.y = CAMERA_MAX_MIN_Y;
				isbreak = false;
			}
			else if (cameraDir.y < -CAMERA_MAX_MIN_Y)
			{
				cameraDir.y = -CAMERA_MAX_MIN_Y;
				isbreak = false;
			}
			D3DXVec3Normalize(&cameraDir, &cameraDir);

			if (isbreak) break;
		}
		cTransform::SetQuaternionToVector(cameraDir, true, true);
	}
	if (MgrInput->IsKeyOn(VK_UP))
	{
		m_freePos += cTransform::GetDirection() * 1;
	}
	if (MgrInput->IsKeyOn(VK_DOWN))
	{
		m_freePos -= cTransform::GetDirection() * 1;
	}
	if (MgrInput->IsKeyOn(VK_LEFT))
	{
		m_freePos -= cTransform::GetRightVec() * 1;
	}
	if (MgrInput->IsKeyOn(VK_RIGHT))
	{
		m_freePos += cTransform::GetRightVec() * 1;
	}

	m_fCameraDistance -= MgrInput->GetMouseWheelDelta();
	if (m_fCameraDistance > CAMERA_MAX_DISTANCE) m_fCameraDistance = CAMERA_MAX_DISTANCE;
	else if (m_fCameraDistance < CAMERA_MIN_DISTANCE) m_fCameraDistance = CAMERA_MIN_DISTANCE;

	{
		D3DXMATRIXA16 matProj;
		MgrD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	//	matProj._41 = m_fCameraDistance / 4;
		MgrD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	m_target = target;
	if (m_posFree)
	{
		m_target = m_freePos;
	}
	cTransform::SetPosition(m_target + (cTransform::GetDirection()* -m_fCameraDistance));

	D3DXVECTOR3 vEye = cTransform::GetPosition();
	D3DXVECTOR3 m_vLookAt = cTransform::GetDirection();



	//	D3DXVECTOR3 movepos = D3DXVECTOR3(100, 0, 0);
	D3DXMATRIXA16 matCamera;
	D3DXMatrixIdentity(&matCamera);
	D3DXMatrixLookAtLH(&matCamera, &(cTransform::GetPosition()), &m_target, &(cTransform::GetUpVec()));

	MgrD3DDevice->SetTransform(D3DTS_VIEW, &matCamera);
}