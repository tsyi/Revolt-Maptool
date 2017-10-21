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

void cObject::Destory()
{
	MgrPhysXScene->releaseActor(*m_pActor);
	m_pActor = NULL;
	GetMesh()->Destory();
}

void cObject::Update()
{
	//GetActor()->putToSleep();

	if (!MgrInput->GetHooking())
	{
		switch (m_state)
		{
		case E_OBJECT_STATE_NONE:
			break;
		case E_OBJECT_STATE_SELECT:
		{
			if (MgrInput->IsKeyDown('Q'))
			{
				GetActor()->putToSleep();
			}
			if (MgrInput->IsKeyUp('Q'))
			{
				GetActor()->wakeUp();
			}
			if (MgrInput->IsMouseDown(MOUSE_LEFT))
			{
				if (MgrPhysXData->RaycastAllShapeHitCount == 0)
				{
					SetState(E_OBJECT_STATE_CANSLE);
					D3DXCOLOR materColor = D3DXCOLOR(0.3f, 1.0f, 0.8f, 1.0f);
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
					break;
				}
				else if (GetUserData()->RaycastClosestShape != NX_TRUE)
				{
					SetState(E_OBJECT_STATE_CANSLE);
					D3DXCOLOR materColor = D3DXCOLOR(0.3f, 1.0f, 0.8f, 1.0f);
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
					break;
				}
				SetMouseDistance(cTransform::GetPosition() - cTransform::NxVec3ToDxVec3(GetUserData()->RayHitPos));
				SetHeigth(cTransform::GetPosition().y);
			}
			if (MgrInput->IsMousePressDrag(MOUSE_LEFT))
			{
				NxVec3 pos(0, 0, 0);
				if (GetMapData()->RaycastAllShape == NX_TRUE)
				{
					pos = GetMapData()->RayHitPos;
					pos += cTransform::DxVec3ToNxVec3(GetMouseDistance());
					pos.y = GetHeigth();
				}
				else
				{
					pos = GetUserData()->RayHitPos;
					pos += cTransform::DxVec3ToNxVec3(GetMouseDistance());
					pos.y = GetHeigth();
				}
				GetActor()->setGlobalPosition(pos);
			}

		}break;
		case E_OBJECT_STATE_CANSLE:
		{
			if (MgrInput->IsMouseDown(MOUSE_LEFT))
			{
				if (MgrPhysXData->RaycastAllShapeHitCount == 0) break;
				if (GetUserData()->RaycastClosestShape == NX_TRUE)
				{
					SetMouseDistance(cTransform::GetPosition() - cTransform::NxVec3ToDxVec3(GetUserData()->RayHitPos));
					SetHeigth(cTransform::GetPosition().y);
					SetState(E_OBJECT_STATE_SELECT);

					D3DXCOLOR materColor = D3DXCOLOR(1.0f, 0.3f, 0.8f, 1.0f);
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
					GetMesh()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
				}
			}
		} break;
		}
	}
}

void cObject::LastUpdate()
{
	NxVec3 pos = GetActor()->getGlobalPose().t;
	cTransform::SetNxVec3(pos);

//	cTransform::GetDirection();
	
	NxF32 mtl[3 * 3];
	GetActor()->getGlobalPose().M.getColumnMajor(mtl);
	cTransform::SetNxF32(mtl);
}

void cObject::Render()
{
	m_physxUserData->Init();

	MgrD3DDevice->SetTransform(D3DTS_WORLD, &cTransform::GetMatrix());
	m_pMesh->Render();
}