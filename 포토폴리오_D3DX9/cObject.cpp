#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_pMeshData(NULL)
	, m_PhysXData(NULL)
	, m_isActor(false)
	, m_objTag(E_OBJECT_NONE)
	, m_state(E_OBJECT_STATE_NONE)
{
	m_pMeshData = new cMesh;
	m_pMapData = new USERDATA;
	//	m_physxUserData = new USERDATA;
}


cObject::~cObject()
{
}

void cObject::Setup()
{

}

void cObject::Destory()
{
	if (GetPhysXData()) GetPhysXData()->Destory();
	if (GetMeshData()) GetMeshData()->Destory();
	//	MgrPhysXScene->releaseActor(*GetPhysXData()->m_pActor);
	m_PhysXData = NULL;
	m_pMeshData = NULL;

	m_state = E_OBJECT_STATE_NONE;
}

void cObject::Update()
{
	GetPhysXData()->m_pActor->putToSleep();
	SetMeshBox();
	if (!MgrInput->GetHooking())
	{
		switch (m_state)
		{
		case E_OBJECT_STATE_NONE:
		{
			if (GetMeshData())
			{
				D3DXCOLOR materColor = unSelectColor;
				GetMeshData()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
				GetMeshData()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
				GetMeshData()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
			}
			m_state = E_OBJECT_STATE_CANSLE;
		}
		break;
		case E_OBJECT_STATE_SELECT:
		{
			if (MgrInput->IsMouseDown(MOUSE_LEFT))
			{
				if (MgrPhysXData->RaycastAllShapeHitCount == 0)
				{
					SetState(E_OBJECT_STATE_CANSLE);
					if (GetMeshData())
					{
						D3DXCOLOR materColor = unSelectColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
					}
					break;
				}
				else if (GetPhysXData()->m_pUserData->RaycastClosestShape != NX_TRUE)
				{
					SetState(E_OBJECT_STATE_CANSLE);
					if (GetMeshData())
					{
						D3DXCOLOR materColor = unSelectColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
					}
					break;
				}
				SetMouseDistance(cTransform::GetPosition() - cTransform::NxVec3ToDxVec3(GetPhysXData()->m_pUserData->RayHitPos));
				SetHeigth(cTransform::GetPosition().y);
			}
		}
		if (MgrInput->IsMousePressDrag(MOUSE_LEFT))
		{
			NxVec3 pos(0, 0, 0);
			if (GetMapData()->RaycastAllShape == NX_TRUE)
			{
				if (MgrInput->IsKeyDown(VK_LCONTROL))
					SetHeigth(cTransform::GetPosition().y);

				pos = GetMapData()->RayHitPos;
				if (MgrInput->IsKeyPress(VK_LCONTROL))
					pos.y = GetHeigth();
				else
				{
					switch (GetPhysXData()->m_type)
					{
					case NxShapeType::NX_SHAPE_SPHERE: pos.y += (GetPhysXData()->m_sizeValue.x - GetPhysXData()->m_localPose.t.y); break;
					case NxShapeType::NX_SHAPE_BOX:pos.y += (GetPhysXData()->m_sizeValue.y - GetPhysXData()->m_localPose.t.y);	break;
					default:break;
					}
				}
			}
			GetPhysXData()->m_worldPose.t = pos;
		}
		break;
		case E_OBJECT_STATE_CANSLE:
		{
			if (MgrInput->IsMouseDown(MOUSE_LEFT))
			{
				if (MgrPhysXData->RaycastAllShapeHitCount == 0) break;
				if (GetPhysXData()->m_pUserData->RaycastClosestShape == NX_TRUE)
				{
					SetMouseDistance(cTransform::GetPosition() - cTransform::NxVec3ToDxVec3(GetPhysXData()->m_pUserData->RayHitPos));
					//	SetHeigth(cTransform::GetPosition().y);
					SetState(E_OBJECT_STATE_SELECT);

					if (GetMeshData())
					{
						D3DXCOLOR materColor = isSelectColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Ambient = materColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Diffuse = materColor;
						GetMeshData()->m_vecMtlTex[0]->GetMaterial().Specular = materColor;
					}
				}
			}
		} break;
		}
	}
}

void cObject::LastUpdate()
{
	if (!GetPhysXData()) return;
	NxVec3 pos = GetPhysXData()->m_worldPose.t;
	cTransform::SetNxVec3(pos);

	//	cTransform::GetDirection();

	NxF32 mtl[3 * 3];
	GetPhysXData()->m_worldPose.M.getColumnMajor(mtl);
	cTransform::SetNxF32(mtl);

	NxMat34 NxActorPose;
	NxActorPose.multiply(GetPhysXData()->m_worldPose, GetPhysXData()->m_localPose);

	GetPhysXData()->m_pActor->setGlobalPose(NxActorPose);
}

void cObject::Render()
{
	if (GetPhysXData()) GetPhysXData()->m_pUserData->Init();

	MgrD3DDevice->SetTransform(D3DTS_WORLD, &cTransform::GetMatrix());
	if (GetMeshData()) GetMeshData()->Render();
}