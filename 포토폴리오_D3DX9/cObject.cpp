#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_pMesh(NULL)
	, m_pActor(NULL)
	, m_isActor(false)
	, m_objTag(E_OBJECT_NONE)
	, m_state(E_OBJECT_STATE_CANSLE)
{
	m_pMesh = new cMesh;
	m_physxUserData = new USERDATA;
	m_pMapData = new USERDATA;
}


cObject::~cObject()
{
}

void cObject::Setup()
{

}

void cObject::Update()
{
	NxVec3 pos = m_pActor->getGlobalPose().t;

	NxF32 mtl[3 * 3];
	m_pActor->getGlobalPose().M.getColumnMajor(mtl);

	cTransform::SetNxVec3(pos);
	cTransform::SetNxF32(mtl);





	if (m_physxUserData->RaycastClosestShape == NX_TRUE)
	{

		if (MgrInput->IsMouseDown(MOUSE_LEFT))
		{
			y = cTransform::GetPosition().y;
			m_state = E_OBJECT_STATE_SELECT;



			D3DXCOLOR materColor = D3DXCOLOR(1.0f, 0.3f, 0.8f, 1.0f);
			m_pMesh->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
			m_pMesh->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
			m_pMesh->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
		}
	}
	if (m_state == E_OBJECT_STATE_SELECT)
	{
		if (MgrInput->IsMouseOn(MOUSE_LEFT))
		{
			NxVec3 pos(0, 0, 0);
			if (m_pMapData->RaycastAllShape == NX_TRUE)
			{
				pos = m_pMapData->RayHitPos;
			}
			else
			{
				pos = m_physxUserData->RayHitPos;
				pos.y = y;
			}
			m_pActor->setGlobalPosition(pos);
		}
		if (MgrInput->IsMouseUp(MOUSE_LEFT))
		{
			m_state = E_OBJECT_STATE_CANSLE;

			D3DXCOLOR materColor = D3DXCOLOR(0.3f, 1.0f, 0.8f, 1.0f);
			m_pMesh->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
			m_pMesh->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
			m_pMesh->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
		}
	}
}

void cObject::Render()
{
	m_physxUserData->Init();

	MgrD3DDevice->SetTransform(D3DTS_WORLD, &cTransform::GetMatrix());
	m_pMesh->Render();
}

USERDATA * cObject::GetUserData()
{
	return m_physxUserData;
}

void cObject::SetMapUuerData(USERDATA * pMapData)
{
	m_pMapData = pMapData;
}

void cObject::SetActor(NxActor * pActor)
{
	m_pActor = pActor;
	m_pActor->putToSleep()
//	m_pActor->wakeUp()
}
