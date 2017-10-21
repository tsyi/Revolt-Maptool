#pragma once
#include <NxPhysics.h>
#include <NxCooking.h>
#include "Stream.h"


struct USERDATA
{
	NxU32 ContactPairFlag;
	NX_BOOL RaycastClosestShape;
	NX_BOOL RaycastAllShape;
	NxVec3	RayHitPos;
	USERDATA() { Init(); }
	void Init() 
	{
		ContactPairFlag = 0;
		RaycastClosestShape = NX_FALSE;
		RaycastAllShape = NX_FALSE;
		RayHitPos = NxVec3(0, 0, 0);
	}
};

class ContactCallBack : public NxUserContactReport
{
	//�浹Ȯ��
	void onContactNotify(NxContactPair& pair, NxU32 _event)
	{
		USERDATA* pUserData0 = NULL;
		USERDATA* pUserData1 = NULL;
		switch (_event)
		{
		case NX_NOTIFY_ON_START_TOUCH:
		{
			pUserData0 = (USERDATA*)pair.actors[0]->userData;
			pUserData1 = (USERDATA*)pair.actors[1]->userData;

			pUserData0->ContactPairFlag = NX_NOTIFY_ON_START_TOUCH;
			pUserData1->ContactPairFlag = NX_NOTIFY_ON_START_TOUCH;

			std::cout << "NX_NOTIFY_ON_START_TOUCH" << std::endl;

		}break;
		case NX_NOTIFY_ON_END_TOUCH:
		{
			pUserData0 = (USERDATA*)pair.actors[0]->userData;
			pUserData1 = (USERDATA*)pair.actors[1]->userData;

			pUserData0->ContactPairFlag = 0;
			pUserData1->ContactPairFlag = 0;

			std::cout << "NX_NOTIFY_ON_END_TOUCH" << std::endl;

		}break;
		}
	}
};

class RaycastCallBack : public NxUserRaycastReport
{
	virtual bool onHit(const NxRaycastHit& hit)
	{
		NxActor& actor = hit.shape->getActor();
		if (actor.userData)
		{
			USERDATA* userData = (USERDATA*)actor.userData;
			userData->RaycastAllShape = NX_TRUE;
			userData->RayHitPos = hit.worldImpact;
		}
		return true;
	}
};

enum E_MATERIAL
{
	E_MATERIAL_DEFAULT = 0,
	E_MATERIAL_1,
	E_MATERIAL_2,
	E_MATERIAL_3,
	E_MATERIAL_4,
	E_MATERIAL_5,
};
enum eShapeTag
{
	E_SHAPE_NONE,
	E_SHAPE_BOX,
	E_SHAPE_SPHERE,
	E_SHAPE_PLANE,
	E_SHAPE_TRIANGLE,
};
class DEBUG_RENDERER;

#define MgrPhysX cPhysXManager::GetInstance()
#define MgrPhysXScene cPhysXManager::GetInstance()->GetPhysXScene()
#define MgrPhysXSDK   cPhysXManager::GetInstance()->GetPhysXSDK()


class cPhysXManager
{
public:
	SINGLETONE(cPhysXManager);

private:
	NxPhysicsSDK*	m_pNxPhysicsSDK;
	NxScene*		m_pNxScene;

	std::map<E_MATERIAL, NxMaterial*> m_mapMaterial;
public:
	NxPhysicsSDK* GetPhysXSDK() { return m_pNxPhysicsSDK; }
	NxScene* GetPhysXScene() { return m_pNxScene; }
	BOOL InitNxPhysX(DEBUG_RENDERER** pDebugRenderer);
	void Destory();


	NxTriangleMeshShapeDesc CreateTringleMesh(ID3DXMesh* pMesh, D3DXMATRIXA16* matS = NULL);
	NxBoxShapeDesc CreateBoxShape(int materialIndex, NxVec3 boxSize);




	void Update();


	NxVec3 D3DVecToNxVec(D3DXVECTOR3& d3d)
	{
		NxVec3 vec;
		vec.x = d3d.x;
		vec.y = d3d.y;
		vec.z = d3d.z;
		return vec;
	}
	NxF32* D3DMatToNxMat(D3DMATRIX d3d)
	{
		NxF32* mtl = new NxF32;

		mtl[0] = d3d._11;
		mtl[1] = d3d._12;
		mtl[2] = d3d._13;
		mtl[3] = d3d._21;
		mtl[4] = d3d._22;
		mtl[5] = d3d._23;
		mtl[6] = d3d._31;
		mtl[7] = d3d._32;
		mtl[8] = d3d._33;

		return mtl;
	}

	void RaycastClosestShape(D3DXVECTOR3 start, D3DXVECTOR3 dir);
	void RaycastAllShapes(D3DXVECTOR3 start, D3DXVECTOR3 dir);


	//CreateActor Actor �� ����� �Ŵ����� ����� ��ȯ�Ѵ�.
	//1
	//NxShapeType
	//NX_SHAPE_SPHERE		:	NxVec3(radius, 0, 0);
	//NX_SHAPE_BOX			:	NxVec3(width/2, height/2, deep/2);
	//NX_SHAPE_CAPSULE		:	NxVec3(radius, height, 0);
	//NX_SHAPE_WHEEL		:	NxVec3(radius, 0, 0);
	//2
	//NxU32 : NxBodyFlag
	//0, NULL							:	���� ��ü
	//NX_BF_DISABLE_GRAVITY				:	�߷¹���ȭ
	//(default) NX_BF_VISUALIZATION		:	�ð�ȭ
	//NX_BF_KINEMATIC					:	�ٸ� ��ü�� ������ �浹�� ���� ����.					
	//NX_BF_ENERGY_SLEEP_TEST			:	������ ��� ���� �˰����� Ȱ��ȭ (default)
	//NX_BF_FILTER_SLEEP_VEL			:	�������� ������ ����
	//3
	//USERDATA*							:	���� ������(�浹üũ)
	//4	
	//NxVec3 (x, y, z);					:	type �� �ٸ� ������
	//5	
	//materialIndex						:	��������(�̸� �޴����� ��Ͽܾ��־�� �Ѵ�)
	//density							:	�е� ���� (ũ�Ⱑ Ŀ���� �翬�� �� ���Դ� �����Ѵ�.)
	//6	
	//trMatrix							:	������ Matrix���� ( = matR * matT) 
	NxActor* CreateActor(NxShapeType type, NxU32 bodyFlag, USERDATA* pUserData, NxVec3 sizeValue, int materialIndex, float density
		, D3DXMATRIX trMatrix)
	{
		NxActorDesc actorDesc;	actorDesc.setToDefault();
		NxBodyDesc bodyDesc;	bodyDesc.setToDefault();
		NxShapeDesc* shapeDesc = NULL;

		switch (type)
		{
		case NX_SHAPE_PLANE: {
			break;
		}
		case NX_SHAPE_SPHERE: {
			NxSphereShapeDesc desc; desc.setToDefault();
			desc.materialIndex - materialIndex;
			desc.radius = sizeValue.x;
			break;
		}
		case NX_SHAPE_BOX: {
			NxBoxShapeDesc desc; desc.setToDefault();
			desc.materialIndex = materialIndex;
			desc.dimensions = sizeValue;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_CAPSULE: {
			NxCapsuleShapeDesc desc; desc.setToDefault();
			desc.materialIndex = materialIndex;
			desc.radius = sizeValue.x;
			desc.height = sizeValue.y;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_WHEEL: {
			NxWheelShapeDesc desc; desc.setToDefault();
			desc.materialIndex = materialIndex;
			desc.radius = 0;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_CONVEX: {
			break;
		}
		case NX_SHAPE_MESH: {
			break;
		}
		case NX_SHAPE_HEIGHTFIELD: {
			break;
		}
		case NX_SHAPE_RAW_MESH: {
			break;
		}
		case NX_SHAPE_COMPOUND: {
			break;
		}
		case NX_SHAPE_COUNT: {
			break;
		}
		case NX_SHAPE_FORCE_DWORD: {
			break;
		}
		default:
			break;
		}

		actorDesc.shapes.pushBack(shapeDesc);

		if (bodyFlag == 0)
		{
			actorDesc.body = NULL;
		}
		else
		{
			bodyDesc.flags = NX_BF_VISUALIZATION | bodyFlag;
			actorDesc.body = &bodyDesc;
			actorDesc.density = density;
		}

		actorDesc.globalPose.t = NxVec3(trMatrix._41, trMatrix._42, trMatrix._43);
		//NxF32* mtl = D3DMatToNxMat(trMatrix);
		actorDesc.globalPose.M.setColumnMajor(D3DMatToNxMat(trMatrix));

		if (pUserData)
		{
			pUserData->ContactPairFlag = 0;
			pUserData->RaycastClosestShape = NX_FALSE;
			actorDesc.userData = pUserData;
		}

		return MgrPhysXScene->createActor(actorDesc);
	}
};