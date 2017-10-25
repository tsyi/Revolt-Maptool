#pragma once

class cObject;
//#include "cObject.h"

#define CAMERA_MOVE_DETAIL 0.005
#define CAMERA_MAX_MIN_Y 0.98f

#define CAMERA_MIN_DISTANCE 2.f
#define CAMERA_MAX_DISTANCE 500.f

#define CAMERA_OPTION_MOVE_V 1;
#define CAMERA_OPTION_MOVE_H -1;

class cCamera : public cTransform
{
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3		m_target;
	D3DXVECTOR2		m_ptPrevMouse;
	bool			m_isLButtonDown;
	SYNTHESIZE(float, m_fCameraDistance, Distane);
	D3DXVECTOR3		m_vCamRotAngle;


	D3DXVECTOR3		m_freePos;
	bool m_posFree;

	cObject* m_pObjTarget;

	RECT rc;
public:
	cCamera(void);
	~cCamera(void);

	void Setup();
	void Update(D3DXVECTOR3 target = D3DXVECTOR3(0, 0, 0));

	void SetPosFree(bool isFree) {
		if ((m_posFree == false) && (isFree = true))
		{
			m_freePos = m_target + (cTransform::GetDirection()* -m_fCameraDistance);
		}
		m_posFree = isFree;
	}
	void Render();
};

